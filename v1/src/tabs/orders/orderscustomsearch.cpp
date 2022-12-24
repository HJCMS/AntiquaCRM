// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderscustomsearch.h"
#include "ordershistorybutton.h"

#include <QLayout>

OrdersCustomSearch::OrdersCustomSearch(QWidget *parent)
    : QDialog{parent, Qt::Tool}, p_date(QDate::currentDate()) {
  setSizeGripEnabled(true);
  setWindowTitle(tr("History queries"));

  int row = 0;
  QGridLayout *layout = new QGridLayout(this);

  m_filterSelecter = new AntiquaComboBox(this);
  QMapIterator<OrdersHistoryButton::HistoryQuery, QString> it(
      OrdersHistoryButton::entries());
  while (it.hasNext()) {
    it.next();
    m_filterSelecter->addItem(it.value(), it.key());
  }
  layout->addWidget(m_filterSelecter, row, 0, 1, 1);

  m_yearSelect = new AntiquaComboBox(this);
  int past_years = p_date.addDays(-(5 * 365)).year();
  for (int i = p_date.year(); i > past_years; i--) {
    QString title = tr("Year %1").arg(QString::number(i));
    m_yearSelect->addItem(title, i);
  }
  m_yearSelect->setCurrentIndex(m_yearSelect->findData(p_date.year()));
  layout->addWidget(m_yearSelect, row++, 1, 1, 1);

  layout->setRowStretch(row++, 1);

  m_btnBox = new QDialogButtonBox(
      QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
  layout->addWidget(m_btnBox, row, 0, 1, 2);

  setLayout(layout);

  connect(m_btnBox, SIGNAL(accepted()), SLOT(accept()));
  connect(m_btnBox, SIGNAL(rejected()), SLOT(reject()));
}

int OrdersCustomSearch::currentFilter() {
  int i = m_filterSelecter->currentIndex();
  return m_filterSelecter->itemData(i, Qt::UserRole).toInt();
}

int OrdersCustomSearch::currentYear() {
  int i = m_yearSelect->currentIndex();
  return m_yearSelect->itemData(i, Qt::UserRole).toInt();
}

const QString OrdersCustomSearch::customQuery() {
  QString status;
  QString sqlQuery;
  switch (static_cast<OrdersHistoryButton::HistoryQuery>(currentFilter())) {
  // Zeige alle Nicht bezahlten
  case OrdersHistoryButton::HistoryQuery::FILTER_DEFAULT:
    status = QString::number(AntiquaCRM::OrderStatus::DELIVERED);
    sqlQuery = "o_order_status<" + status;
    break;

  // Zeige "nicht Bezahlt" und "nicht Storniert"
  case OrdersHistoryButton::HistoryQuery::FILTER_NOT_PAID:
    status = QString::number(AntiquaCRM::OrderPayment::PAYED);
    sqlQuery = "o_payment_status!=" + status;
    status = QString::number(AntiquaCRM::OrderStatus::CANCELED);
    sqlQuery.append(" AND o_order_status !=" + status);
    break;

  // Zeige geliefert und Nicht bezahlt
  case OrdersHistoryButton::HistoryQuery::FILTER_DELIVERED_NOT_PAID:
    status = QString::number(AntiquaCRM::OrderPayment::PAYED);
    sqlQuery = "o_payment_status!=" + status;
    status = QString::number(AntiquaCRM::OrderStatus::DELIVERED);
    sqlQuery.append(" AND o_order_status=" + status);
    break;

  // Zeige geliefert mit Zahlungserinnerung oder Mahnung
  case OrdersHistoryButton::HistoryQuery::FILTER_PAYMENT_REMINDED:
    status = QString::number(AntiquaCRM::OrderPayment::REMIND);
    sqlQuery = "o_payment_status IN (" + status;
    status = QString::number(AntiquaCRM::OrderPayment::ADMONISH);
    sqlQuery.append("," + status);
    status = QString::number(AntiquaCRM::OrderPayment::COLLPROC);
    sqlQuery.append("," + status + ")");
    status = QString::number(AntiquaCRM::OrderStatus::DELIVERED);
    sqlQuery.append(" AND o_order_status=" + status);
    break;

  case OrdersHistoryButton::HistoryQuery::FILTER_COMPLETED:
    status = QString::number(AntiquaCRM::OrderPayment::PAYED);
    sqlQuery = "o_payment_status=" + status;
    status = QString::number(AntiquaCRM::OrderStatus::DELIVERED);
    sqlQuery.append(" AND o_order_status=" + status);
    break;

  case OrdersHistoryButton::HistoryQuery::FILTER_CANCELED:
    status = QString::number(AntiquaCRM::OrderStatus::CANCELED);
    sqlQuery.append("o_order_status=" + status);
    break;

  default:
    sqlQuery = "o_order_status=0 AND o_payment_status=0";
    break;
  };

  sqlQuery.append(" AND date_part('year',o_since)=");
  sqlQuery.append(QString::number(currentYear()));

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << sqlQuery;
#endif

  return sqlQuery;
}

int OrdersCustomSearch::exec() { return QDialog::exec(); }
