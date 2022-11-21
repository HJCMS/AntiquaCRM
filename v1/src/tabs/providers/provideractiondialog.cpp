// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "provideractiondialog.h"

#include <AntiquaCRM>
#include <QDebug>
#include <QJsonValue>
#include <QLabel>
#include <QLayout>
#include <QTimer>

ProviderActionDialog::ProviderActionDialog(QWidget *parent) : QDialog{parent} {
  setWindowTitle(tr("Provider Operations"));
  setContentsMargins(5, 0, 5, 0);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  QLabel *m_lb = new QLabel(this);
  m_lb->setText(tr("Changes the status of the current order."));
  layout->addWidget(m_lb);

  m_rb1 = new QRadioButton(tr("Provider Status - no disclosures"), this);
  m_rb1->setChecked(true);
  layout->addWidget(m_rb1);

  m_rb2 = new QRadioButton(tr("Wait for Order Payment"), this);
  m_rb2->setObjectName("WAITING_FOR_PAYMENT");
  layout->addWidget(m_rb2);

  m_rb3 = new QRadioButton(tr("Order is ready for Shipment"), this);
  m_rb3->setObjectName("READY_FOR_SHIPMENT");
  layout->addWidget(m_rb3);

  m_rb4 = new QRadioButton(tr("Order shipped, wait for Payment."), this);
  m_rb4->setObjectName("SHIPPED_WAITING_FOR_PAYMENT");
  layout->addWidget(m_rb4);

  m_rb5 = new QRadioButton(tr("Order Shipped and Paid"), this);
  m_rb5->setObjectName("SHIPPED_AND_PAID");
  layout->addWidget(m_rb5);

  m_rb6 = new QRadioButton(tr("Order Buyer no reaction."), this);
  m_rb6->setObjectName("BUYER_NO_REACTION");
  layout->addWidget(m_rb6);

  m_rb7 = new QRadioButton(tr("Order Cancel action."), this);
  m_rb7->setObjectName("ORDER_CANCEL_ACTION");
  layout->addWidget(m_rb7);

  layout->addStretch(1);

  m_buttonBar = new QDialogButtonBox(
      QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
  layout->addWidget(m_buttonBar);

  setLayout(layout);

  connect(m_buttonBar, SIGNAL(accepted()), SLOT(prepareAction()));
  connect(m_buttonBar, SIGNAL(rejected()), SLOT(reject()));
}

void ProviderActionDialog::prepareAction() {
  QString action;
  QList<QRadioButton *> l = findChildren<QRadioButton *>(QString());
  for (int i = 0; i < l.count(); i++) {
    QRadioButton *rb = l.at(i);
    if (rb != nullptr && !rb->objectName().isEmpty()) {
      if (rb->isChecked()) {
        action = rb->objectName();
        break;
      }
    }
  }

  AntiquaCRM::PaymentStatus status;
  if (action == "WAITING_FOR_PAYMENT") {
    status = AntiquaCRM::PaymentStatus::WAIT_FOR_PAYMENT;
  } else if (action == "READY_FOR_SHIPMENT") {
    status = AntiquaCRM::PaymentStatus::SHIPMENT_CREATED;
  } else if (action == "SHIPPED_WAITING_FOR_PAYMENT") {
    status = AntiquaCRM::PaymentStatus::SHIPPED_WAIT_FOR_PAYMENT;
  } else if (action == "SHIPPED_AND_PAID") {
    status = AntiquaCRM::PaymentStatus::SHIPPED_AND_PAID;
  } else if (action == "BUYER_NO_REACTION") {
    status = AntiquaCRM::PaymentStatus::BUYER_NO_REACTION;
  } else if (action == "ORDER_CANCEL_ACTION") {
    status = AntiquaCRM::PaymentStatus::ORDER_CANCELED;
  } else {
    return;
  }

  QJsonObject obj;
  obj.insert("paymentstatus", QJsonValue(status));
  emit sendPluginAction(obj);

  QTimer::singleShot(800, this, SLOT(accept()));
}

int ProviderActionDialog::exec() { return QDialog::exec(); }
