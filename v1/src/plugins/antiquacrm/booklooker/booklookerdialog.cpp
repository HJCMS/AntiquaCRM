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
  setMinimumWidth(550);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  int col = 0;
  QLabel *m_lb = new QLabel(this);
  m_lb->setWordWrap(false);
  m_lb->setText(tr("Changes the Provider status of the current order."));
  m_lb->setStyleSheet("QLabel {font-weight:bold;}");
  layout->insertWidget(col++, m_lb);

  QLabel *m_info = new QLabel(this);
  m_info->setWordWrap(true);
  QStringList notes;
  notes << tr("Not all service providers fully support these features.");
  notes << tr(
      "Please make sure to check it regularly on the service provider side.");
  m_info->setText(notes.join("<br>"));
  m_info->setStyleSheet("QLabel {font-style:italic; font-size:smaller;}");
  layout->insertWidget(col++, m_info);

  QFrame *m_frame = new QFrame(this);
  m_frame->setWhatsThis(tr("This selection inform your Provider, the current order status."));
  QVBoxLayout *frLayout = new QVBoxLayout(m_frame);
  m_rb2 = new QRadioButton(tr("Waiting for Payment"), m_frame);
  m_rb2->setObjectName("WAITING_FOR_PAYMENT");
  frLayout->addWidget(m_rb2);
  m_rb3 = new QRadioButton(tr("Ready for Shipment"), m_frame);
  m_rb3->setObjectName("READY_FOR_SHIPMENT");
  frLayout->addWidget(m_rb3);
  m_rb4 = new QRadioButton(tr("Shipped, wait for Payment"), m_frame);
  m_rb4->setObjectName("SHIPPED_WAITING_FOR_PAYMENT");
  frLayout->addWidget(m_rb4);
  m_rb5 = new QRadioButton(tr("Shipped and Paid"), m_frame);
  m_rb5->setObjectName("SHIPPED_AND_PAID");
  frLayout->addWidget(m_rb5);
  m_rb6 = new QRadioButton(tr("Buyer no reaction"), m_frame);
  m_rb6->setObjectName("BUYER_NO_REACTION");
  frLayout->addWidget(m_rb6);
  frLayout->addStretch();
  m_frame->setLayout(frLayout);
  layout->insertWidget(col++, m_frame);

  m_statusBar = new QStatusBar(this);
  m_statusBar->setSizeGripEnabled(false);
  layout->insertWidget(col++, m_statusBar);

  m_buttonBar = new QDialogButtonBox(QDialogButtonBox::Close, this);
  QPushButton *apply = m_buttonBar->addButton(QDialogButtonBox::Apply);
  apply->setText(tr("Send update"));
  layout->insertWidget(col++, m_buttonBar);

  setLayout(layout);

  connect(apply, SIGNAL(clicked()), SLOT(networkAction()));
  connect(m_buttonBar, SIGNAL(rejected()), SLOT(reject()));
}

bool BooklookerDialog::prepareAction() {
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

  if (action.isEmpty()) {
    m_statusBar->showMessage(tr("We need a Selection for this operations."));
    return false;
  }

  QJsonObject query;
  query.insert("orderId", QJsonValue(p_order_id));
  query.insert("status", QJsonValue(action));

  // Reset
  p_post_data = QJsonObject();
  p_post_data.insert("action", QJsonValue("order_status"));
  p_post_data.insert("query", QJsonValue(query));

  return true;
}

void BooklookerDialog::networkAction() {
  if (prepareAction())
    emit sendPluginAction(p_post_data);
}

void BooklookerDialog::statusMessage(const QString &message) {
  if (message == "SUCCESS")
    m_statusBar->showMessage(tr("Update success."));
  else
    m_statusBar->showMessage(message);
}

int BooklookerDialog::exec(const QString &orderId, const QJsonObject &data) {
  Q_UNUSED(data);
  if (orderId.isEmpty()) {
    qWarning("BooklookerDialog: Empty OrderId not accepted!");
    return QDialog::Rejected;
  }

  p_order_id = orderId;
  setWindowTitle(tr("Booklooker %1").arg(p_order_id) + "[*]");

  return QDialog::exec();
}
