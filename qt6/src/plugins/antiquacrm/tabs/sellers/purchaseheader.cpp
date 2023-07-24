// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "purchaseheader.h"

PurchaseHeader::PurchaseHeader(QWidget *parent) : QWidget{parent} {
  setContentsMargins(0, 0, 0, 0);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 2, 5, 2);

  QLabel *lb1 = new QLabel(tr("Buyer") + ":", this);
  lb1->setIndent(5);
  layout->addWidget(lb1);

  m_buyer = new QLabel(this);
  m_buyer->setStyleSheet("QLabel {font-weight: bold;}");
  layout->addWidget(m_buyer);

  layout->addStretch(1);

  QLabel *lb2 = new QLabel(tr("Customer Id") + ":", this);
  layout->addWidget(lb2);

  m_cid = new QLabel(this);
  layout->addWidget(m_cid);
}

qint64 PurchaseHeader::getCustomerId() { return p_customer_id; }

void PurchaseHeader::setHeader(const QString &name, qint64 cid) {
  if (cid > 0) {
    p_customer_id = cid;
    m_cid->setText(QString::number(p_customer_id));
    m_cid->setStyleSheet("");
  } else {
    p_customer_id = -1;
    m_cid->setText("0");
    m_cid->setStyleSheet("QLabel {color:red; font-weight:bold;}");
  }
  m_buyer->setText(name);
}
