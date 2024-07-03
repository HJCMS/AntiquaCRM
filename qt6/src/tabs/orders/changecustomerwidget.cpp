// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "changecustomerwidget.h"

#include <QGridLayout>

ChangeCustomerWidget::ChangeCustomerWidget(QWidget* parent) : QWidget{parent} {
  setContentsMargins(1, 1, 1, 1);
  QGridLayout* gridLayout = new QGridLayout(this);
  gridLayout->setContentsMargins(contentsMargins());
  gridLayout->setColumnStretch(0, 1);
  gridLayout->setColumnStretch(1, 1);
  gridLayout->setRowStretch(1, 1);

  m_customerId = new AntiquaCRM::SerialId(this);
  m_customerId->setBuddyLabel(tr("Change to Customer Id"));
  gridLayout->addWidget(m_customerId, 0, 0, 1, 1);

  m_fullName = new AntiquaCRM::TextLine(this);
  m_fullName->setReadOnly(true);
  m_fullName->setBuddyLabel(tr("Full name"));
  m_fullName->setInputToolTip(tr("Identified by Providerimport"));
  m_fullName->setWhatsThisText(
      tr("Imported unique user name from the service provider. This is compared by the AntiquaCRM "
         "server with other parameters such as postcode, city, first and last name in order to "
         "find existing customers in the database."));
  gridLayout->addWidget(m_fullName, 0, 1, 1, 1);

  m_invoice = new AntiquaCRM::TextField(this);
  m_invoice->setBuddyLabel(tr("Invoice Address"));
  gridLayout->addWidget(m_invoice, 1, 0, 1, 1);

  m_delivery = new AntiquaCRM::TextField(this);
  m_delivery->setBuddyLabel(tr("Delivery Address"));
  gridLayout->addWidget(m_delivery, 1, 1, 1, 1);

  setLayout(gridLayout);
}

ChangeCustomerWidget::~ChangeCustomerWidget() {
  customerId = -1;
}

void ChangeCustomerWidget::setCustomerId(qint64 cid) {
  if (cid < 1)
    return;

  customerId = cid;
  emit sendCustomerChanged();
}

qint64 ChangeCustomerWidget::getCustomerId() {
  return customerId;
  if (m_sql != nullptr)
    m_sql->deleteLater();
}

bool ChangeCustomerWidget::loadCostumerData(qint64 cid) {
  bool _status = false;
  if (cid > 0) {
    customerId = cid;
  }

  if (customerId < 1) {
    qWarning("CustomerId is not set!");
    return _status;
  }

  QString _scid = QString::number(customerId);
  if (_scid.isEmpty())
    return _status;

  if (m_sql == nullptr)
    m_sql = new AntiquaCRM::ASqlCore(this);

  AntiquaCRM::ASqlFiles _tpl("query_customer_overview");
  if (!_tpl.openTemplate())
    return _status;

  _tpl.setWhereClause("c_id=" + _scid);

  QSqlQuery _q = m_sql->query(_tpl.getQueryContent());
  if (_q.size() > 0) {
    _q.next();
    m_customerId->setValue(cid);
    m_fullName->setValue(_q.value("importname"));
    m_invoice->setValue(_q.value("invoice"));
    m_delivery->setValue(_q.value("delivery"));
    _status = true;
  }

  return _status;
}
