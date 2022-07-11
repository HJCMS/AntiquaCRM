// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "purchasedebugtable.h"

#include <QStringList>
#include <QHeaderView>

namespace Antiqua {

PurchaseDebugTable::PurchaseDebugTable(QWidget *parent) : QTableWidget{parent} {
  setColumnCount(2);
  QStringList headers({tr("Parameter"), tr("Value")});
  setHorizontalHeaderLabels(headers);
  QHeaderView *header = horizontalHeader();
  header->setDefaultAlignment(Qt::AlignCenter);
  header->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  header->setSectionResizeMode(1, QHeaderView::Stretch);
}

QTableWidgetItem *PurchaseDebugTable::createItem(const QVariant &value) const {
  QString txt = value.toString();
  QTableWidgetItem *item = new QTableWidgetItem(txt);
  item->setFlags(Qt::ItemIsEnabled);
  return item;
}

}; // namespace Antiqua
