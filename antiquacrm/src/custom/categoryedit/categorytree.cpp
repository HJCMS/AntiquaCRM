// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "categorytree.h"
#include "draglabel.h"
#include "myicontheme.h"
#include "treekeyworditem.h"

#include <QAction>
#include <QDebug>
#include <QDrag>
#include <QHeaderView>
#include <QMenu>
#include <QMimeData>
#include <QMimeType>
#include <QThread>
#include <QTimer>

CategoryTree::CategoryTree(QWidget *parent) : QTreeWidget{parent} {
  setObjectName("category_tree");
  setColumnCount(2);
  QStringList titles(tr("Provider Categories"));
  titles << tr("Display");
  setHeaderLabels(titles);
  setSelectionMode(QAbstractItemView::SingleSelection);
  setAlternatingRowColors(true);
  setExpandsOnDoubleClick(true);
  setAcceptDrops(true);
  setDragEnabled(true);
  setDropIndicatorShown(true);
  setDragDropMode(QAbstractItemView::InternalMove);
  QHeaderView *m_header = header();
  m_header->setSectionResizeMode(QHeaderView::ResizeToContents);
  setHeader(m_header);
}

bool CategoryTree::isExpandable(QTreeWidgetItem *item) const {
  if (item->childIndicatorPolicy() == QTreeWidgetItem::ShowIndicator)
    return (item->childCount() > 0);

  return false;
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

void CategoryTree::expandOnDragHover() {
  QTreeWidgetItem *parent = currentItem();
  if (isExpandable(parent))
    expandItem(parent);
}

void CategoryTree::toggleParentsTree(bool) {
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

  TreeKeywordItem *item = new TreeKeywordItem(parent);
  item->setText(0, name);
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
            SLOT(toggleParentsTree(bool)));

    m->exec(event->globalPos());
    delete m;
  }
}

void CategoryTree::dragEnterEvent(QDragEnterEvent *event) {
  if (event->mimeData()->hasFormat(itemMime)) {
    event->setAccepted(true);
  } else if (event->mimeData()->hasText()) {
    event->setAccepted(true);
  } else {
    event->setAccepted(false);
  }
}

void CategoryTree::dragMoveEvent(QDragMoveEvent *event) {
  if (event->mimeData()->hasFormat(itemMime)) {
    event->setAccepted(true);
  } else if (event->mimeData()->hasText()) {
    event->setAccepted(true);
  } else {
    event->setAccepted(false);
    return;
  }

  /**
   * Wenn beim verschieben über ein Elternelement gefahren wird.
   * Eine Prüfung durchführen ...
   * Dann currentItem setzen und einen Timer auf expandOnDragHover starten.
   */
  QModelIndex index = indexAt(event->pos());
  if (!index.isValid())
    return;

  QTreeWidgetItem *item = itemAt(event->pos());
  if (item == nullptr)
    return;

  setCurrentItem(item);
  if (index.flags() ^ Qt::ItemIsDropEnabled && isExpandable(item)) {
    if (!item->isExpanded()) {
      p_expandTimerId = startTimer(p_timerWait);
      return;
    }
  }
}

void CategoryTree::timerEvent(QTimerEvent *event) {
  if (p_expandTimerId == event->timerId()) {
    killTimer(p_expandTimerId);
    p_expandTimerId = -1;
    expandOnDragHover();
  }
  QTreeWidget::timerEvent(event);
}

void CategoryTree::dropEvent(QDropEvent *event) {
  const QMimeData *mime = event->mimeData();
  if (event->source() == this && mime->hasFormat(itemMime)) {
    QModelIndex index = indexAt(event->pos());
    if (index.isValid() && (index.flags() & Qt::ItemIsDropEnabled)) {
      if (p_sDrag.first == nullptr || p_sDrag.second == nullptr) {
        event->setAccepted(false);
        return;
      }

      setDropIndicatorShown(true);
      QTreeWidgetItem *target = itemAt(event->pos());
      QString text = mime->text();
      if (addKeywordItem(target, text)) {
        target->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
        expandItem(target);
        p_sDrag.first->removeChild(p_sDrag.second);
        update();
      }
      event->setDropAction(Qt::MoveAction);
      event->setAccepted(true);

      p_sDrag.first = nullptr;
      p_sDrag.second = nullptr;
    } else {
      event->setAccepted(false);
    }
  } else if (event->mimeData()->hasText()) {
    QModelIndex index = indexAt(event->pos());
    if (index.isValid() && (index.flags() & Qt::ItemIsDropEnabled)) {
      setDropIndicatorShown(true);
      QTreeWidgetItem *target = itemAt(event->pos());
      QString text = mime->text();
      if (addKeywordItem(target, text)) {
        target->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
        expandItem(target);
        update();
        emit sendListItemHidden(text);
      }
      event->setAccepted(true);
    } else {
      event->setAccepted(false);
    }
  } else {
    event->setAccepted(false);
  }
}

void CategoryTree::mousePressEvent(QMouseEvent *event) {
  QTreeWidget::mousePressEvent(event);
  if (event->button() != Qt::LeftButton)
    return;

  QModelIndex index = indexAt(event->pos());
  if (!index.isValid()) {
    event->setAccepted(false);
    return;
  }

  if (index.flags() & Qt::ItemIsDragEnabled) {
    TreeKeywordItem *item =
        reinterpret_cast<TreeKeywordItem *>(itemAt(event->pos()));
    if (item == nullptr)
      return;

    QTreeWidgetItem *parent = item->parent();
    if (parent == nullptr)
      return;

    p_sDrag.first = parent;
    p_sDrag.second = item;

    event->setAccepted(true);

    QPoint itemView = event->pos() - visualItemRect(item).topLeft();
    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << item;

    QMimeData *m_mimeData = new QMimeData;
    m_mimeData->setData(itemMime, itemData);
    m_mimeData->setText(item->text(0));

    DragLabel *m_label = new DragLabel(m_mimeData->text(), this);

    QDrag *m_drag = new QDrag(this);
    m_drag->setMimeData(m_mimeData);
    m_drag->setPixmap(m_label->pixmap(Qt::ReturnByValue));
    m_drag->setHotSpot(itemView);
    m_label->hide();

    Qt::DropActions da = m_drag->exec(Qt::MoveAction, Qt::MoveAction);
    if (da == Qt::MoveAction) {
      m_label->hide();
      m_label->deleteLater();
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
