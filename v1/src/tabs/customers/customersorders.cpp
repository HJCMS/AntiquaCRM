// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customersorders.h"

#include <AGlobal>
#include <QDateTime>
#include <QHeaderView>

CustomersOrders::CustomersOrders(QWidget *parent) : QTableWidget{parent} {
  restore();
}

void CustomersOrders::restore() {
  QStringList headers;
  headers << " " + tr("Delivery Note") + " ";
  headers << " " + tr("Invoice") + " ";
  headers << " " + tr("Article") + " ";
  headers << " " + tr("Provider") + " ";
  headers << " " + tr("Provider Id") + " ";
  headers << " " + tr("Created") + " ";
  headers << " " + tr("Delivered") + " ";
  setColumnCount(headers.count());
  setRowCount(0);
  setHorizontalHeaderLabels(headers);
  horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

QTableWidgetItem *CustomersOrders::numidItem(const QVariant &data) {
  QString str = data.toString().trimmed();
  QString num = str.rightJustified(7, '0');
  return new QTableWidgetItem(num, Qt::DisplayRole);
}

QTableWidgetItem *CustomersOrders::createItem(const QVariant &data) {
  return new QTableWidgetItem(data.toString(), Qt::DisplayRole);
}

QTableWidgetItem *CustomersOrders::createDate(const QVariant &data) {
  QString str = data.toString();
  QDateTime dt = QDateTime::fromString(str, ANTIQUACRM_TIMESTAMP_IMPORT);
  if (dt.isNull())
    dt = QDateTime::fromString(str, Qt::ISODate);

  return new QTableWidgetItem(dt.toString(ANTIQUACRM_DATETIME_DISPLAY),
                              Qt::DisplayRole);
}
