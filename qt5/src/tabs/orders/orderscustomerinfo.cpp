// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderscustomerinfo.h"

#include <QLayout>
#include <QToolButton>

OrdersCustomerInfo::OrdersCustomerInfo(QWidget *parent) : QWidget{parent} {
  setContentsMargins(0, 0, 0, 0);
  setFixedHeight(200);

  QString css("QTextEdit {background: transparent;");
  css.append("border-top: 1px solid palette(text);}");

  QVBoxLayout *layout = new QVBoxLayout(this);
  o_customer_id = new SerialID(this);
  o_customer_id->setObjectName("o_customer_id");
  o_customer_id->setInfo(tr("Customer Number"));
  layout->addWidget(o_customer_id);
  m_stackedWidget = new QStackedWidget(this);
  c_postal_address = new TextField(this);
  c_postal_address->setObjectName("c_postal_address");
  c_postal_address->setToolTip(tr("Invoice Address"));
  c_postal_address->setEditable(false);
  c_postal_address->setStyleSheet(css);
  m_stackedWidget->addWidget(c_postal_address);
  c_shipping_address = new TextField(this);
  c_shipping_address->setObjectName("c_shipping_address");
  c_shipping_address->setToolTip(tr("Delivery Address"));
  c_shipping_address->setEditable(false);
  c_shipping_address->setStyleSheet(css);
  m_stackedWidget->addWidget(c_shipping_address);
  layout->addWidget(m_stackedWidget);
  QFrame *frame = new QFrame(this);
  QHBoxLayout *frameLayout = new QHBoxLayout(frame);
  QToolButton *ac_left = new QToolButton(frame);
  ac_left->setIcon(style()->standardIcon(QStyle::SP_ArrowLeft));
  frameLayout->addWidget(ac_left);
  frameLayout->addStretch(1);
  m_pageLabel = new QLabel(c_postal_address->toolTip(), frame);
  frameLayout->addWidget(m_pageLabel);
  frameLayout->addStretch(1);
  QToolButton *ac_right = new QToolButton(frame);
  ac_right->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
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
