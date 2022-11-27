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
  headers << " " + tr("Payed") + " ";
  headers << " " + tr("Delivery Note") + " ";
  headers << " " + tr("Invoice") + " ";
  headers << " " + tr("Article") + " ";
  headers << " " + tr("Title") + " ";
  headers << " " + tr("Provider") + " ";
  headers << " " + tr("Provider Id") + " ";
  headers << " " + tr("Created") + " ";
  headers << " " + tr("Delivered") + " ";
  setColumnCount(headers.count());
  setRowCount(0);
  setHorizontalHeaderLabels(headers);
  horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

QTableWidgetItem *CustomersOrders::iconItem(const QVariant &data) {
  QString link = data.toBool() ? "ok" : "cancel";
  QString title = data.toBool() ? tr("Yes") : tr("No");
  QIcon icon = QIcon("://icons/action_" + link + ".png");
  QTableWidgetItem *i = new QTableWidgetItem(title, Qt::DisplayRole);
  i->setData(Qt::DecorationRole, icon);
  i->setFlags(default_flags);
  return i;
}

QTableWidgetItem *CustomersOrders::numidItem(const QVariant &data) {
  QString str = data.toString().trimmed();
  QString num = str.rightJustified(7, '0');
  QTableWidgetItem *i = new QTableWidgetItem(num, Qt::DisplayRole);
  i->setFlags(default_flags);
  return i;
}

QTableWidgetItem *CustomersOrders::createItem(const QVariant &data) {
  QTableWidgetItem *i = new QTableWidgetItem(data.toString(), Qt::DisplayRole);
  i->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
  return i;
}

QTableWidgetItem *CustomersOrders::createDate(const QVariant &data) {
  QString str = data.toString();
  QDateTime dt = QDateTime::fromString(str, ANTIQUACRM_TIMESTAMP_IMPORT);
  if (dt.isNull())
    dt = QDateTime::fromString(str, Qt::ISODate);

  QTableWidgetItem *i = new QTableWidgetItem(
      dt.toString(ANTIQUACRM_DATETIME_DISPLAY), Qt::DisplayRole);
  i->setFlags(default_flags);
  return i;
}
