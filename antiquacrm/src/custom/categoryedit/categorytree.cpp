// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "categorytree.h"
#include "myicontheme.h"

#include <QAction>
#include <QDebug>
#include <QDrag>
#include <QHeaderView>
#include <QMenu>
#include <QMimeData>

CategoryTree::CategoryTree(QWidget *parent) : QTreeWidget{parent} {
  setObjectName("category_tree");
  setAcceptDrops(true);
  setColumnCount(2);
  QStringList titles(tr("Provider Categories"));
  titles << tr("Display");
  setHeaderLabels(titles);

  setAlternatingRowColors(true);
  setExpandsOnDoubleClick(true);

  QHeaderView *m_header = header();
  m_header->setSectionResizeMode(QHeaderView::ResizeToContents);
  setHeader(m_header);
}

QTreeWidgetItem *CategoryTree::findParent(int id) {
  for (int i = 0; i < topLevelItemCount(); i++) {
    QTreeWidgetItem *twi = topLevelItem(i);
    if (twi->data(0, Qt::UserRole).toInt() == id)
      return twi;
  }
  return nullptr;
}

const QIcon CategoryTree::setIcon(bool b) const {
  return (b) ? myIcon("button_ok") : myIcon("button_cancel");
}

void CategoryTree::toggleSubTree(bool) {
  QTreeWidgetItem *parent = currentItem()->parent();
  QStringList list;
  if (parent != nullptr) {
    for (int i = 0; i < parent->childCount(); i++) {
      QTreeWidgetItem *item = parent->child(i);
      item->setData(0, Qt::DecorationRole, setIcon(false));
      item->setData(1, Qt::DisplayRole, tr("No"));
      item->setData(1, Qt::UserRole, false);
      qint64 id = item->data(0, Qt::UserRole).toInt();
      list.append(QString::number(id));
    }
  }
  update();
  if (list.count() > 0)
    emit sendDisableUsageList(list);
}

void CategoryTree::toggleActivation(bool) {
  QTreeWidgetItem *item = currentItem();
  bool current = item->data(1, Qt::UserRole).toBool();
  bool active = (current) ? false : true;
  item->setData(0, Qt::DecorationRole, setIcon(active));
  item->setData(1, Qt::DisplayRole, (active) ? tr("Yes") : tr("No"));
  item->setData(1, Qt::UserRole, active);
  update();
  qint64 ce_id = item->data(0, Qt::UserRole).toInt();
  if (ce_id > 0)
    emit sendCompanyUsage(ce_id, active);
}

void CategoryTree::removeKeyword(bool) {
  QTreeWidgetItem *item = currentItem();
  QTreeWidgetItem *parent = item->parent();
  if (item != nullptr && parent != nullptr) {
    emit sendListItemVisible(item->text(0));
    parent->removeChild(item);
  }
}

bool CategoryTree::addKeywordItem(QTreeWidgetItem *parent,
                                  const QString &name) {
  if (parent == nullptr)
    return false;

  QTreeWidgetItem *item =
      new QTreeWidgetItem(parent, QTreeWidgetItem::UserType);
  item->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
  item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
  item->setText(0, name);
  item->setData(0, Qt::DecorationRole, myIcon("group"));
  item->setData(1, Qt::DisplayRole, tr("Keyword"));
  item->setData(1, Qt::UserRole, "keyword");
  return true;
}

void CategoryTree::contextMenuEvent(QContextMenuEvent *event) {
  QModelIndex index = indexAt(event->pos());
  if (index.isValid() && (index.flags() & Qt::ItemIsSelectable)) {
    QMenu *m = new QMenu(this);
    bool keyword = (index.parent().flags() & Qt::ItemIsDropEnabled);
    QAction *ac_toggle = m->addAction(tr("Toggle Display"));
    ac_toggle->setEnabled((!keyword));
    ac_toggle->setIcon(myIcon("db_update"));
    connect(ac_toggle, SIGNAL(triggered(bool)), this,
            SLOT(toggleActivation(bool)));
    QAction *ac_remove = m->addAction(tr("Remove keyword"));
    ac_remove->setEnabled(keyword);
    ac_remove->setIcon(myIcon("db_remove"));
    connect(ac_remove, SIGNAL(triggered(bool)), this,
            SLOT(removeKeyword(bool)));

    QAction *ac_toggleAll = m->addAction(tr("Disable all in this Section"));
    ac_toggleAll->setIcon(myIcon("db_comit"));
    connect(ac_toggleAll, SIGNAL(triggered(bool)), this,
            SLOT(toggleSubTree(bool)));

    m->exec(event->globalPos());
    delete m;
  }
}

void CategoryTree::dragEnterEvent(QDragEnterEvent *event) {
  if (event->mimeData()->hasText()) {
    event->setAccepted(true);
  } else {
    event->setAccepted(false);
  }
}

void CategoryTree::dragMoveEvent(QDragMoveEvent *event) {
  if (event->mimeData()->hasText()) {
    event->setAccepted(true);
  } else {
    event->setAccepted(false);
  }
}

void CategoryTree::dropEvent(QDropEvent *event) {
  if (event->mimeData()->hasText()) {
    const QMimeData *mime = event->mimeData();
    QPoint position = event->pos();
    QModelIndex index = indexAt(position);
    if (index.isValid() && (index.flags() & Qt::ItemIsDropEnabled)) {
      QTreeWidgetItem *item = itemAt(position);
      QString text = mime->text();
      if (addKeywordItem(item, text)) {
        item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
        expandItem(item);
        update();
        emit sendListItemHidden(text);
      }
    } else {
      event->setAccepted(false);
    }
  } else {
    event->setAccepted(false);
  }
}

void CategoryTree::toggleTreeView() {
  if (p_expandet) {
    collapseAll();
    p_expandet = false;
  } else {
    expandAll();
    p_expandet = true;
  }
}

QTreeWidgetItem *CategoryTree::addTopLevel(const QString &name) {
  QTreeWidgetItem *item = new QTreeWidgetItem(this);
  item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
  item->setFlags(Qt::ItemIsEnabled);
  item->setText(0, name);
  item->setData(1, Qt::DisplayRole, tr("Main category"));
  item->setData(1, Qt::UserRole, true);
  return item;
}

QTreeWidgetItem *CategoryTree::addSubLevel(const QString &name, int bind,
                                           bool active) {
  QTreeWidgetItem *parent = findParent(bind);
  if (parent != nullptr) {
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    item->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
    item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsDropEnabled |
                   Qt::ItemIsSelectable);
    item->setText(0, name);
    item->setData(0, Qt::DecorationRole, setIcon(active));
    item->setData(1, Qt::DisplayRole, (active) ? tr("Yes") : tr("No"));
    item->setData(1, Qt::UserRole, active);
    return item;
  }
  return nullptr;
}
