// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customersorders.h"

#include <AGlobal>
#include <AntiquaInput>
#include <QDateTime>
#include <QHeaderView>
#include <QIcon>
#include <QMenu>

CustomersOrders::CustomersOrders(QWidget *parent) : QTableWidget{parent} {
  setWindowTitle(tr("Orders"));
  setWindowIcon(AntiquaCRM::AntiquaApplIcon("view-financial-payment-mode"));
  restore();
  setSelectionBehavior(QAbstractItemView::SelectRows);
}

void CustomersOrders::contextMenuEvent(QContextMenuEvent *event) {
  p_model = indexAt(event->pos());
  QMenu *m = new QMenu(this);
  QAction *ac_open =
      m->addAction(windowIcon(), tr("Open it in Orders System."));
  ac_open->setEnabled(p_model.isValid());
  connect(ac_open, SIGNAL(triggered()), SLOT(openOrder()));
  m->exec(event->globalPos());
  delete m;
}

void CustomersOrders::openOrder() {
  if (!p_model.isValid())
    return;

  qint64 o_id = item(p_model.row(), 1)->text().toInt();
  QJsonObject obj;
  obj.insert("ACTION", "open_order");
  obj.insert("TARGET", "orders_tab");
  obj.insert("VALUE", o_id);

  AntiquaCRM::ATransmitter *m_sock = new AntiquaCRM::ATransmitter(this);
  connect(m_sock, SIGNAL(disconnected()), m_sock, SLOT(deleteLater()));
  if (m_sock->pushOperation(obj))
    m_sock->close();
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
    icon = AntiquaCRM::AntiquaApplIcon("dialog-warning");
    break;

  case AntiquaCRM::OrderPayment::PAYED:
    title = tr("Payed");
    icon = AntiquaCRM::AntiquaApplIcon("dialog-ok");
    break;

  // Erinnert
  case AntiquaCRM::OrderPayment::REMIND:
    title = tr("Remindet");
    icon = AntiquaCRM::AntiquaApplIcon("flag-yellow");
    break;

  // Mahnung
  case AntiquaCRM::OrderPayment::ADMONISH:
    title = tr("Admonished");
    icon = AntiquaCRM::AntiquaApplIcon("flag-yellow");
    break;

  // Retour
  case AntiquaCRM::OrderPayment::RETURN:
    title = tr("Returning");
    icon = AntiquaCRM::AntiquaApplIcon("action_redo");
    break;

  // Collection procedures/Inkassoverfahren
  case AntiquaCRM::OrderPayment::COLLPROC:
    title = tr("Collection procedures");
    icon = AntiquaCRM::AntiquaApplIcon("flag-red");
    break;

  default:
    title = tr("Not paid");
    icon = AntiquaCRM::AntiquaApplIcon("dialog-cancel");
    break;
  };

  QTableWidgetItem *i = new QTableWidgetItem(title, Qt::DisplayRole);
  i->setFlags(default_flags);
  i->setData(Qt::DecorationRole, icon);
  return i;
}

QTableWidgetItem *CustomersOrders::numidItem(const QVariant &data) {
  QString num;
  if (data.metaType().id() == QMetaType::Int) {
    num = AntiquaCRM::AUtil::zerofill(data.toInt());
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
