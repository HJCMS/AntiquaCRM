// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklookeroperations.h"

#include <QRadioButton>
#include <QVBoxLayout>

BookLookerOperations::BookLookerOperations(QWidget *parent) : QFrame{parent} {
  setFrameStyle(QFrame::StyledPanel);
  setWhatsThis(
      tr("This selection inform your Provider, the current order status."));
  QVBoxLayout *layout = new QVBoxLayout(this);
  QRadioButton *m_rb2 = new QRadioButton(tr("Waiting for Payment"), this);
  m_rb2->setObjectName("WAITING_FOR_PAYMENT");
  layout->addWidget(m_rb2);
  QRadioButton *m_rb3 = new QRadioButton(tr("Ready for Shipment"), this);
  m_rb3->setObjectName("READY_FOR_SHIPMENT");
  layout->addWidget(m_rb3);
  QRadioButton *m_rb4 = new QRadioButton(tr("Shipped, wait for Payment"), this);
  m_rb4->setObjectName("SHIPPED_WAITING_FOR_PAYMENT");
  layout->addWidget(m_rb4);
  QRadioButton *m_rb5 = new QRadioButton(tr("Shipped and Paid"), this);
  m_rb5->setObjectName("SHIPPED_AND_PAID");
  layout->addWidget(m_rb5);
  QRadioButton *m_rb6 = new QRadioButton(tr("Buyer no reaction"), this);
  m_rb6->setObjectName("BUYER_NO_REACTION");
  layout->addWidget(m_rb6);
  layout->addStretch();
  setLayout(layout);

//  QString css("QFrame {border: 1px solid; }");
//  setStyleSheet(css);
}
