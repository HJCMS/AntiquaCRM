// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordersstatusbar.h"

OrdersStatusBar::OrdersStatusBar(QWidget *parent)
    : AntiquaCRM::TabsStatusBar{parent} {
  setCreateButtonEnabled(false);
  defaultViewButton();
  btn_history = historyButton();
  setHistoryActionMenu(btn_history);
}

const QString OrdersStatusBar::yearStatement() const {
  const QString _year("DATE_PART('year',CURRENT_DATE)");
  QString _sql("(DATE_PART('year',o_since)=" + _year + " OR ");
  _sql.append("DATE_PART('year',o_modified)=" + _year + ")");
  return _sql;
}

void OrdersStatusBar::setHistoryActionMenu(QPushButton *parent) {
  m_mapper = new QSignalMapper(parent);
  QMenu *m = new QMenu(parent);
  m->setTitle(tr("Orders History"));

  QAction *ac1 = m->addAction(historyIcon(), tr("Default view"));
  ac1->setStatusTip(tr("Current orders in progress to delivered."));
  m_mapper->setMapping(ac1, OrdersStatusBar::QF_DEFAULT);
  connect(ac1, SIGNAL(triggered()), m_mapper, SLOT(map()));
  m->setDefaultAction(ac1);

  QAction *ac2 = m->addAction(historyIcon(), tr("Orders without payment."));
  ac2->setStatusTip(tr("Current orders without a payment."));
  m_mapper->setMapping(ac2, OrdersStatusBar::QF_NOT_PAID);
  connect(ac2, SIGNAL(triggered()), m_mapper, SLOT(map()));

  QAction *ac3 = m->addAction(historyIcon(), tr("Delivered and not payed."));
  ac3->setStatusTip(tr("Delivered and waiting for payment."));
  m_mapper->setMapping(ac3, OrdersStatusBar::QF_DELIVERED_NOT_PAID);
  connect(ac3, SIGNAL(triggered()), m_mapper, SLOT(map()));

  QAction *ac4 = m->addAction(historyIcon(),
                              tr("Reminded, admonished - Payment Collection."));
  ac4->setStatusTip(tr("Without payment with reminder or Payment Collection."));
  m_mapper->setMapping(ac4, OrdersStatusBar::QF_PAYMENT_REMINDED);
  connect(ac4, SIGNAL(triggered()), m_mapper, SLOT(map()));

  QAction *ac5 = m->addAction(historyIcon(), tr("Completed this Year."));
  ac5->setStatusTip(tr("Completed orders from this Year."));
  m_mapper->setMapping(ac5, OrdersStatusBar::QF_COMPLETED);
  connect(ac5, SIGNAL(triggered()), m_mapper, SLOT(map()));

  QAction *ac6 = m->addAction(historyIcon(), tr("Canceled this Year."));
  ac6->setStatusTip(tr("Canceled orders from this Year."));
  m_mapper->setMapping(ac6, OrdersStatusBar::QF_CANCELED);
  connect(ac6, SIGNAL(triggered()), m_mapper, SLOT(map()));

  parent->setMenu(m);
  connect(m_mapper, SIGNAL(mappedInt(int)), SLOT(setHistoryAction(int)));
}

void OrdersStatusBar::setHistoryAction(int index) {
  FilterQuery _filter = static_cast<FilterQuery>(index);
  QString _status;
  QString _sql;
  switch (_filter) {
  case QF_DEFAULT: {
    // Zeige alle Nicht bezahlten
    _status = QString::number(AntiquaCRM::OrderStatus::DELIVERED);
    _sql = "o_order_status<" + _status;
  } break;

  case QF_NOT_PAID: {
    // Zeige "nicht Bezahlt" und "nicht Storniert"
    _status = QString::number(AntiquaCRM::OrderPayment::PAYED);
    _sql = "o_payment_status!=" + _status;
    _status = QString::number(AntiquaCRM::OrderStatus::CANCELED);
    _sql.append(" AND o_order_status!=" + _status);
  } break;

  case QF_DELIVERED_NOT_PAID: {
    // Zeige geliefert und Nicht bezahlt
    _status = QString::number(AntiquaCRM::OrderPayment::PAYED);
    _sql = "o_payment_status!=" + _status;
    _status = QString::number(AntiquaCRM::OrderStatus::DELIVERED);
    _sql.append(" AND o_order_status=" + _status);
  } break;

  case QF_PAYMENT_REMINDED: {
    // Zeige geliefert mit Zahlungserinnerung oder Mahnung
    _status = QString::number(AntiquaCRM::OrderPayment::REMIND);
    _sql = "(o_payment_status IN (" + _status;
    _status = QString::number(AntiquaCRM::OrderPayment::ADMONISH);
    _sql.append("," + _status);
    _status = QString::number(AntiquaCRM::OrderPayment::COLLPROC);
    _sql.append("," + _status + ")");
    _status = QString::number(AntiquaCRM::OrderStatus::DELIVERED);
    _sql.append(" AND o_order_status=" + _status + ")");
  } break;

  case QF_COMPLETED: {
    // Zeige angeschlossene
    _status = QString::number(AntiquaCRM::OrderPayment::PAYED);
    _sql = "(o_payment_status=" + _status;
    _status = QString::number(AntiquaCRM::OrderStatus::DELIVERED);
    _sql.append(" AND o_order_status=" + _status + ")");
    _sql.append(" AND " + yearStatement());
  } break;

  case QF_CANCELED: {
    // Zeige stornierte
    _status = QString::number(AntiquaCRM::OrderStatus::CANCELED);
    _sql = "o_order_status=" + _status + " AND " + yearStatement();
  } break;

  default:
    break;
  }
  emit sendHistoryQuery(_sql);
}
