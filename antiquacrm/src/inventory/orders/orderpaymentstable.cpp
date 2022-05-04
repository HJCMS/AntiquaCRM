// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderpaymentstable.h"
#include "myicontheme.h"

#include <QHash>
#include <QHeaderView>
#include <QString>
#include <QTableWidgetItem>

OrderPaymentsTable::OrderPaymentsTable(QWidget *parent) : QTableWidget{parent} {
  setObjectName("order_payments_table");
  setColumnCount(5);
  addHeaderItem(0, tr("Article"), tr("article id"));
  addHeaderItem(1, tr("Price"), tr("selling price"));
  addHeaderItem(2, tr("Retail Price"), tr("Retail Price"));
  addHeaderItem(3, tr("Count"), tr("Article count"));
  addHeaderItem(4, tr("Summary"), tr("Article Summary"));
  horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
  horizontalHeader()->setStretchLastSection(true);
}

void OrderPaymentsTable::addHeaderItem(int i, const QString &name,
                                       const QString &tip) {
  QTableWidgetItem *item = new QTableWidgetItem(myIcon("autostart"), name,
                                                QTableWidgetItem::UserType);
  item->setToolTip(tip);
  setHorizontalHeaderItem(i, item);
}

int OrderPaymentsTable::getArticleId(int row) const {
  QTableWidgetItem *item = QTableWidget::item(row, 0);
  if (item != nullptr)
    return item->text().toInt();

  return -1;
}
