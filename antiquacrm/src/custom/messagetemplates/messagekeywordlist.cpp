// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "messagekeywordlist.h"

#include <QHeaderView>

MessageKeywordList::MessageKeywordList(QWidget *parent) : QTreeWidget{parent} {
  setObjectName("message_keyword_list");
  setColumnCount(1);
  QStringList titles(tr("Category"));
  setHeaderLabels(titles);
  setSelectionMode(QAbstractItemView::SingleSelection);
  setAlternatingRowColors(true);
  setExpandsOnDoubleClick(true);
  setAcceptDrops(false);
  setDragEnabled(true);
  setDropIndicatorShown(true);
  setDragDropMode(QAbstractItemView::DragOnly);
}

QTreeWidgetItem *MessageKeywordList::getSection(const QString &name) {
  for (int i = 0; i < topLevelItemCount(); i++) {
    QTreeWidgetItem *p = topLevelItem(i);
    if (p->text(0) == name)
      return p;
  }

  QTreeWidgetItem *item = new QTreeWidgetItem(this);
  item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
  item->setFlags(Qt::ItemIsEnabled);
  item->setText(0, name);
  item->setData(1, Qt::DisplayRole, tr("Section"));
  item->setData(1, Qt::UserRole, false);
  return item;
}

bool MessageKeywordList::insertTplKey(QTreeWidgetItem *parent,
                                      const QString &key,
                                      const QJsonObject &value) {
  if (parent == nullptr)
    return false;

  QTreeWidgetItem *item = new QTreeWidgetItem(parent);
  item->setText(0, key);
  item->setData(0, Qt::UserRole, value);
  item->setData(1, Qt::DisplayRole, tr("Template"));
  item->setData(1, Qt::UserRole, true);
  return true;
}

void MessageKeywordList::addKey(const QString &section, const QString &key,
                                const QJsonObject &value) {
  QTreeWidgetItem *parent = getSection(section);
  if (parent != nullptr)
    insertTplKey(parent, key, value);
}
