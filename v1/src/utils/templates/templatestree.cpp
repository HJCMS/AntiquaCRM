// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "templatestree.h"

#include <QBrush>
#include <QFont>

TemplatesTree::TemplatesTree(QWidget *parent) : QTreeWidget{parent} {
  setItemsExpandable(true);
  setSortingEnabled(false);
  setWordWrap(false);
  setAlternatingRowColors(true);
  setAcceptDrops(false);
  setDragEnabled(false);
  setColumnCount(2);
  setHeaderLabels(QStringList({tr("Title"), tr("Template")}));

  createCategories();
  connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)),
          SLOT(templateClicked(QTreeWidgetItem *, int)));
}

QTreeWidgetItem *TemplatesTree::getParent(const QString &id) {
  for (int i = 0; i < topLevelItemCount(); i++) {
    QTreeWidgetItem *parent = topLevelItem(i);
    if (parent != nullptr && parent->type() == QTreeWidgetItem::Type) {
      QString section = parent->data(0, Qt::UserRole).toString();
      if (section == id)
        return parent;
    }
  }
  return nullptr;
}

void TemplatesTree::createCategories() {
  QTreeWidgetItem *i = new QTreeWidgetItem(this, QTreeWidgetItem::Type);
  i->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
  i->setFlags(Qt::ItemIsEnabled);
  i->setText(0, tr("eMail"));
  i->setData(0, Qt::DecorationRole, QIcon("://icons/groups.png"));
  i->setData(0, Qt::UserRole, QString("email"));
  i->setExpanded(true);
  addTopLevelItem(i);

  i = new QTreeWidgetItem(this, QTreeWidgetItem::Type);
  i->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
  i->setFlags(Qt::ItemIsEnabled);
  i->setText(0, tr("Printing"));
  i->setData(0, Qt::DecorationRole, QIcon("://icons/view_text.png"));
  i->setData(0, Qt::UserRole, QString("printing"));
  i->setExpanded(true);
  addTopLevelItem(i);

  resizeColumnToContents(0);
}

void TemplatesTree::insertTemplate(const QJsonObject &data) {
  QTreeWidgetItem *parent = getParent(data.value("tb_category").toString());
  if (parent == nullptr)
    return;

  QTreeWidgetItem *i = new QTreeWidgetItem(parent, QTreeWidgetItem::UserType);
  i->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled |
              Qt::ItemNeverHasChildren);

  i->setText(0, data.value("tb_title").toString());
  i->setData(0, Qt::DecorationRole, QIcon("://icons/edit.png"));
  QFont f = font();
  f.setStyleHint(QFont::Serif, QFont::PreferDefault);
  f.setStyle(QFont::StyleItalic);
  i->setFont(1, f);
  i->setForeground(1, QBrush(Qt::gray));
  i->setText(1, data.value("tb_caller").toString());
  i->setData(1, Qt::UserRole, data);
  parent->addChild(i);
}

void TemplatesTree::templateClicked(QTreeWidgetItem *item, int column) {
  Q_UNUSED(column);
  if (item->type() != QTreeWidgetItem::UserType)
    return;

  if (item->flags() == Qt::NoItemFlags)
    return;

  QJsonObject obj = item->data(1, Qt::UserRole).toJsonObject();
  if (obj.isEmpty())
    return;

  emit templateSelected(obj);
}

bool TemplatesTree::clearTree() {
  for (int i = 0; i < topLevelItemCount(); i++) {
    QTreeWidgetItem *p = topLevelItem(i);
    if (p == nullptr)
      continue;

    QListIterator<QTreeWidgetItem *> it(p->takeChildren());
    while(it.hasNext()) {
      p->removeChild(it.next());
    }
  }
  return true;
}

void TemplatesTree::addTemplates(const QJsonObject &data) {
  QJsonArray array = data.value("tree").toArray();
  for (int i = 0; i < array.size(); i++) {
    QJsonObject obj = array[i].toObject();
    if (!obj.isEmpty())
      insertTemplate(obj);
  }
  resizeColumnToContents(0);
}
