// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "treekeyworditem.h"
#include "myicontheme.h"

#include <QObject>

TreeKeywordItem::TreeKeywordItem(QTreeWidgetItem *parent)
    : QTreeWidgetItem{parent} {
  setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
  setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled);
  setData(0, Qt::DecorationRole, myIcon("group"));
  setData(1, Qt::DisplayRole, QObject::tr("Keyword"));
  setData(1, Qt::UserRole, QString("keyword"));
}
