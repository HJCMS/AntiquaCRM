// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderscustomerinfo.h"
#include "orderscustomertoolbar.h"

#include <QLayout>
#include <QSizePolicy>
#include <QToolButton>

OrdersCustomerInfo::OrdersCustomerInfo(QWidget *parent) : QWidget{parent} {
  setContentsMargins(0, 0, 0, 0);
  setSizePolicy(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);

  QString css("QTextEdit {background: transparent;");
  css.append("border:none; border-top:1px solid palette(text);}");

  QVBoxLayout *layout = new QVBoxLayout(this);
  // OrdersCustomerToolBar
  OrdersCustomerToolBar *m_oc_toolbar = new OrdersCustomerToolBar(this);
  layout->addWidget(m_oc_toolbar);

  o_customer_id = new AntiquaCRM::SerialId(this);
  o_customer_id->setObjectName("o_customer_id");
  o_customer_id->setBuddyLabel(tr("Customer Number"));
  m_oc_toolbar->addEditWidget(o_customer_id);

  m_stackedWidget = new QStackedWidget(this);
  c_postal_address = new AntiquaCRM::TextField(this);
  c_postal_address->setObjectName("c_postal_address");
  c_postal_address->setToolTip(tr("Invoice Address"));
  c_postal_address->setStyleSheet(css);
  m_stackedWidget->addWidget(c_postal_address);

  c_shipping_address = new AntiquaCRM::TextField(this);
  c_shipping_address->setObjectName("c_shipping_address");
  c_shipping_address->setToolTip(tr("Delivery Address"));
  c_shipping_address->setStyleSheet(css);
  m_stackedWidget->addWidget(c_shipping_address);
  layout->addWidget(m_stackedWidget);

  QFrame *frame = new QFrame(this);
  QHBoxLayout *frameLayout = new QHBoxLayout(frame);
  QToolButton *ac_left = new QToolButton(frame);
  ac_left->setIcon(AntiquaCRM::antiquaIcon("arrow-left"));
  frameLayout->addWidget(ac_left);
  frameLayout->addStretch(1);
  m_pageLabel = new QLabel(c_postal_address->toolTip(), frame);
  frameLayout->addWidget(m_pageLabel);
  frameLayout->addStretch(1);
  QToolButton *ac_right = new QToolButton(frame);
  ac_right->setIcon(AntiquaCRM::antiquaIcon("arrow-right"));
  frameLayout->addWidget(ac_right);
  frame->setLayout(frameLayout);
  layout->addWidget(frame);

  setLayout(layout);

  connect(ac_left, SIGNAL(clicked()), SLOT(swapPage()));
  connect(ac_right, SIGNAL(clicked()), SLOT(swapPage()));
}

void OrdersCustomerInfo::setInfoText(int index) {
  if (index == 0)
    m_pageLabel->setText(c_postal_address->toolTip());
  else
    m_pageLabel->setText(c_shipping_address->toolTip());
}

void OrdersCustomerInfo::swapPage() {
  int i = (m_stackedWidget->currentIndex() == 0) ? 1 : 0;
  m_stackedWidget->setCurrentIndex(i);
  setInfoText(i);
}
