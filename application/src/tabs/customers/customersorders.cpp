// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customersorders.h"

#include <AGlobal>
#include <AntiquaCRM>
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

QTableWidgetItem *CustomersOrders::paymentItem(const QVariant &data) {
  QString title;
  QIcon icon;
  switch (static_cast<AntiquaCRM::OrderPayment>(data.toInt())) {
  case AntiquaCRM::OrderPayment::NOTPAID:
    title = tr("Not paid");
    icon = QIcon("://icons/action_warning.png");
    break;

  case AntiquaCRM::OrderPayment::PAYED:
    title = tr("Payed");
    icon = QIcon("://icons/action_ok.png");
    break;

  // Erinnert
  case AntiquaCRM::OrderPayment::REMIND:
    title = tr("Remindet");
    icon = QIcon("://icons/flag-yellow.png");
    break;

  // Mahnung
  case AntiquaCRM::OrderPayment::ADMONISH:
    title = tr("Admonished");
    icon = QIcon("://icons/flag-yellow.png");
    break;

  // Retour
  case AntiquaCRM::OrderPayment::RETURN:
    title = tr("Returning");
    icon = QIcon("://icons/action_redo.png");
    break;

  // Collection procedures/Inkassoverfahren
  case AntiquaCRM::OrderPayment::COLLPROC:
    title = tr("Collection procedures");
    icon = QIcon("://icons/flag-red.png");
    break;

  default:
    title = tr("Not paid");
    icon = QIcon("://icons/action_cancel.png");
    break;
  };

  QTableWidgetItem *i = new QTableWidgetItem(title, Qt::DisplayRole);
  i->setFlags(default_flags);
  i->setData(Qt::DecorationRole, icon);
  return i;
}

QTableWidgetItem *CustomersOrders::numidItem(const QVariant &data) {
  QString num;
  if (data.type() == QVariant::Int) {
    num = AntiquaCRM::AUtil::fileNumber(data.toInt());
  } else {
    num = data.toString().rightJustified(7, '0');
  }
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