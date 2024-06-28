// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderscustomertoolbar.h"
#include "changeordercustomerdialog.h"

#include <QJsonObject>
#include <QMessageBox>
#include <QToolBar>

OrdersCustomerToolBar::OrdersCustomerToolBar(QWidget* parent) : QFrame{parent} {
  setContentsMargins(0, 0, 0, 0);
  QHBoxLayout* layout = new QHBoxLayout(this);
  layout->setContentsMargins(contentsMargins());
  // Widget Layout
  m_layout = new QHBoxLayout;
  m_layout->setContentsMargins(contentsMargins());

  layout->addLayout(m_layout);
  layout->addStretch(1);

  QToolBar* m_toolBar = new QToolBar(this);
  m_toolBar->setContentsMargins(contentsMargins());
  m_toolBar->setMovable(false);
  m_toolBar->setFloatable(false);
  m_toolBar->setOrientation(Qt::Horizontal);
  ac_open = m_toolBar->addAction(AntiquaCRM::antiquaIcon("system-users"), // icon
                                 tr("View customer data"));
  ac_edit = m_toolBar->addAction(AntiquaCRM::antiquaIcon("system-switch-user"),
                                 tr("Change customer to order"));
  layout->addWidget(m_toolBar);
  setLayout(layout);

  // SIGNALS
  connect(ac_open, SIGNAL(triggered()), SLOT(openCustomer()));
  connect(ac_edit, SIGNAL(triggered()), SLOT(changeCustomer()));
}

qint64 OrdersCustomerToolBar::customerId() {
  if (m_edit)
    return m_edit->getValue().toInt();

  return -1;
}

void OrdersCustomerToolBar::openCustomer() {
  qint64 _cid = customerId();
  if (_cid < 1)
    return;

  QJsonObject _obj;
  _obj.insert("ACTION", "open_customer");
  _obj.insert("TARGET", "customers_tab");
  _obj.insert("VALUE", _cid);

  AntiquaCRM::ATransmitter* m_sock = new AntiquaCRM::ATransmitter(this);
  connect(m_sock, SIGNAL(disconnected()), m_sock, SLOT(deleteLater()));
  if (m_sock->pushOperation(_obj))
    m_sock->close();

#ifdef ANTIQUA_DEVELOPMENT
  qDebug() << Q_FUNC_INFO << _obj;
#endif
}

void OrdersCustomerToolBar::changeCustomer() {
  qint64 _cid = customerId();
  if (_cid < 1)
    return;

  ChangeOrderCustomerDialog* m_d = new ChangeOrderCustomerDialog(this);
  if (m_d->start(_cid) != QDialog::Accepted)
    return;

  qDebug() << Q_FUNC_INFO << "TODO";
}

void OrdersCustomerToolBar::addEditWidget(AntiquaCRM::SerialId* widget) {
  m_edit = widget;
  m_layout->addWidget(m_edit);
}
