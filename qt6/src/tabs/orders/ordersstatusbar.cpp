// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordersstatusbar.h"
#include "orderssearchbar.h"

using namespace AntiquaCRM;

OrdersStatusBar::OrdersStatusBar(QWidget *parent) : TabsStatusBar{parent} {
  setCreateButtonEnabled(false);
  defaultViewButton();
  btn_history = historyButton();
  setHistoryActionMenu(btn_history);
}

const QString OrdersStatusBar::yearStatement() const {
  OrdersSearchBar *m_sbar = parentWidget()->findChild<OrdersSearchBar *>();
  QString _year("DATE_PART('year',CURRENT_DATE)");
  if (m_sbar != nullptr)
    _year = QString::number(m_sbar->getYear());

  QString _sql("DATE_PART('year',o_since)=" + _year);
  return _sql;
}

void OrdersStatusBar::setHistoryActionMenu(QPushButton *parent) {
  m_mapper = new QSignalMapper(parent);
  QMenu *m = new QMenu(parent);
  m->setTitle(tr("Orders History"));

  m_ac = m->addAction(historyIcon(), tr("Default view"));
  m_ac->setStatusTip(tr("Current orders in progress to delivered."));
  m_mapper->setMapping(m_ac, OrdersStatusBar::QF_DEFAULT);
  connect(m_ac, SIGNAL(triggered()), m_mapper, SLOT(map()));
  m->setDefaultAction(m_ac);

  m_ac = m->addAction(historyIcon(), tr("Orders without payment"));
  m_ac->setStatusTip(tr("Current orders without a payment."));
  m_mapper->setMapping(m_ac, OrdersStatusBar::QF_NOT_PAID);
  connect(m_ac, SIGNAL(triggered()), m_mapper, SLOT(map()));

  m_ac = m->addAction(historyIcon(), tr("Delivered and not paid"));
  m_ac->setStatusTip(tr("Delivered and waiting for payment."));
  m_mapper->setMapping(m_ac, OrdersStatusBar::QF_DELIVERED_NOT_PAID);
  connect(m_ac, SIGNAL(triggered()), m_mapper, SLOT(map()));

  m_ac = m->addAction(historyIcon(), tr("Refundings"));
  m_ac->setStatusTip(tr("Refundings from Year."));
  m_mapper->setMapping(m_ac, OrdersStatusBar::QF_REFUNDS);
  connect(m_ac, SIGNAL(triggered()), m_mapper, SLOT(map()));

  m_ac = m->addAction(historyIcon(), tr("Completed"));
  m_ac->setStatusTip(tr("Completed orders from Year."));
  m_mapper->setMapping(m_ac, OrdersStatusBar::QF_COMPLETED);
  connect(m_ac, SIGNAL(triggered()), m_mapper, SLOT(map()));

  m_ac = m->addAction(historyIcon(), tr("Canceled"));
  m_ac->setStatusTip(tr("Canceled orders from Year."));
  m_mapper->setMapping(m_ac, OrdersStatusBar::QF_CANCELED);
  connect(m_ac, SIGNAL(triggered()), m_mapper, SLOT(map()));

  parent->setMenu(m);
  connect(m_mapper, SIGNAL(mappedInt(int)), SLOT(setHistoryAction(int)));
}

void OrdersStatusBar::setHistoryAction(int index) {
  FilterQuery _filter = static_cast<FilterQuery>(index);
  const QString _delivered = QString::number(OrderStatus::DELIVERED);
  QString _status, _sql;
  switch (_filter) {
  case QF_DEFAULT: {
    // Zeige alle Nicht gelieferten
    _sql = "o_order_status<" + _delivered;
  } break;

  case QF_NOT_PAID: {
    // Zeige "nicht Bezahlt" und "nicht Storniert"
    _status = QString::number(OrderPayment::PAYED);
    _sql = "o_payment_status!=" + _status;
    _status = QString::number(OrderPayment::RETURN);
    _sql.append(" AND o_payment_status!=" + _status);
    _status = QString::number(OrderStatus::CANCELED);
    _sql.append(" AND o_order_status!=" + _status);
  } break;

  case QF_DELIVERED_NOT_PAID: {
    // Zeige geliefert, nicht Bezahlt ohne Erinnerungen.
    _sql = "(o_order_status=" + _delivered + " AND ";
    _sql.append("o_payment_status IN (");
    _sql.append(QString::number(OrderPayment::NOTPAID));
    _sql.append(",");
    _sql.append(QString::number(OrderPayment::REMIND));
    _sql.append(",");
    _sql.append(QString::number(OrderPayment::ADMONISH));
    _sql.append(",");
    _sql.append(QString::number(OrderPayment::COLLPROC));
    _sql.append("))");
  } break;

  case QF_COMPLETED: {
    // Zeige angeschlossene
    _status = QString::number(OrderPayment::PAYED);
    _sql = "(o_payment_status=" + _status;
    _sql.append(" AND o_order_status=" + _delivered + ")");
  } break;

  case QF_CANCELED: {
    // Zeige stornierte
    _status = QString::number(OrderStatus::CANCELED);
    _sql = "o_order_status=" + _status;
  } break;

  case QF_REFUNDS: {
    // Zeige Rückertattungen
    _status = QString::number(OrderStatus::CANCELED);
    _sql = "(o_order_status=" + _delivered;
    _status = QString::number(OrderPayment::RETURN);
    _sql.append(" AND o_payment_status=" + _status + ")");
  } break;

  default:
    break;
  }

  _status.clear();
  if (_sql.isEmpty())
    return;

  _sql.append(" AND " + yearStatement());
  emit sendHistoryQuery(_sql);
}
