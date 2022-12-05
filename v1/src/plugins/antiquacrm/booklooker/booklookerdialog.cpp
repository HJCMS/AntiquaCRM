// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklookerdialog.h"

#include <QDebug>
#include <QJsonValue>
#include <QLabel>
#include <QLayout>
#include <QTimer>

BooklookerDialog::BooklookerDialog(QWidget *parent)
    : AntiquaCRM::UpdateDialog{parent} {
  setWindowTitle(tr("Booklooker %1").arg(QString()) + "[*]");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  QLabel *m_lb = new QLabel(this);
  m_lb->setWordWrap(false);
  m_lb->setText(tr("Changes the Provider status of the current order."));
  m_lb->setStyleSheet("QLabel {font-weight:bold;}");
  layout->addWidget(m_lb);

  QLabel *m_info = new QLabel(this);
  m_info->setWordWrap(true);
  QStringList notes;
  notes << tr("Not all service providers fully support these features.");
  notes << tr(
      "Please make sure to check it regularly on the service provider side.");
  m_info->setText(notes.join(" "));
  m_info->setStyleSheet("QLabel {font-style:italic; font-size:smaller;}");
  layout->addWidget(m_info);

  m_rb2 = new QRadioButton(tr("Waiting for Payment"), this);
  m_rb2->setObjectName("WAITING_FOR_PAYMENT");
  layout->addWidget(m_rb2);

  m_rb3 = new QRadioButton(tr("Ready for Shipment"), this);
  m_rb3->setObjectName("READY_FOR_SHIPMENT");
  layout->addWidget(m_rb3);

  m_rb4 = new QRadioButton(tr("Shipped, wait for Payment"), this);
  m_rb4->setObjectName("SHIPPED_WAITING_FOR_PAYMENT");
  layout->addWidget(m_rb4);

  m_rb5 = new QRadioButton(tr("Shipped and Paid"), this);
  m_rb5->setObjectName("SHIPPED_AND_PAID");
  layout->addWidget(m_rb5);

  m_rb6 = new QRadioButton(tr("Buyer no reaction"), this);
  m_rb6->setObjectName("BUYER_NO_REACTION");
  layout->addWidget(m_rb6);

  m_rb7 = new QRadioButton(tr("Cancel action"), this);
  m_rb7->setObjectName("ORDER_CANCEL_ACTION");
  layout->addWidget(m_rb7);

  m_buttonBar = new QDialogButtonBox(
      QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
  layout->addWidget(m_buttonBar);

  setLayout(layout);

  connect(m_buttonBar, SIGNAL(accepted()), SLOT(networkAction()));
  connect(m_buttonBar, SIGNAL(rejected()), SLOT(reject()));
}

void BooklookerDialog::prepareAction() {
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

  p_post_data = QJsonObject();
  p_post_data.insert("paymentstatus", QJsonValue(status));
}

void BooklookerDialog::networkAction() {
  prepareAction();
  qDebug() << Q_FUNC_INFO << "TODO" << p_post_data;
}

int BooklookerDialog::exec(const QString &orderId, const QJsonObject &data) {
  p_order_id = orderId;
  p_system_data = data;
  setWindowTitle(tr("Booklooker %1").arg(p_order_id) + "[*]");
  return QDialog::exec();
}
