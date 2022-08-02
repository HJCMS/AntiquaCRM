// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklookerremoteactions.h"

#include <QDebug>
#include <QGridLayout>
#include <QStyle>
#include <QVBoxLayout>

BooklookerRemoteActions::BooklookerRemoteActions(QWidget *parent)
    : QDialog{parent} {
  setSizeGripEnabled(true);
  setMinimumSize(QSize(580, 480));
  setWindowTitle("Booklooker [*]");

  QIcon qIcAppl = style()->standardIcon(QStyle::SP_DialogApplyButton);
  // QIcon qIcFile = style()->standardIcon(QStyle::SP_DialogOpenButton);
  QIcon qIcQuit = style()->standardIcon(QStyle::SP_DialogCloseButton);

  QVBoxLayout *layout = new QVBoxLayout(this);

  m_toolBar = new QToolBar(this);
  m_toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
  ac_quit = m_toolBar->addAction(qIcQuit, " " + tr("Close"));
  ac_commit = m_toolBar->addAction(qIcAppl, " " + tr("Apply"));
  layout->addWidget(m_toolBar);

  m_centralWidget = new QWidget(this);
  int gRow = 0;
  QGridLayout *gridLayout = new QGridLayout(m_centralWidget);
  m_orderUpdate = new Antiqua::PaymentOrderUpdate(m_centralWidget);
  gridLayout->addWidget(m_orderUpdate, gRow, 1, 1, 1);

  m_centralWidget->setLayout(gridLayout);
  layout->addWidget(m_centralWidget);
  layout->addStretch(1);
  setLayout(layout);

  connect(m_orderUpdate, SIGNAL(currentIndexChanged(int)), this,
          SLOT(updateOrderChanged(int)));
  connect(ac_quit, SIGNAL(triggered()), this, SLOT(reject()));
}

void BooklookerRemoteActions::updateOrderChanged(int index) {
  Antiqua::PaymentStatus status = static_cast<Antiqua::PaymentStatus>(index);
  QString order_status;
  switch (status) {
  case (Antiqua::PaymentStatus::ORDER_WAIT_FOR_PAYMENT): {
    order_status = QString("WAITING_FOR_PAYMENT");
    break;
  }
  // fertig zum Versand
  case (Antiqua::PaymentStatus::ORDER_READY_FOR_SHIPMENT): {
    order_status = QString("READY_FOR_SHIPMENT");
    break;
  }
  // versendet, warte auf Zahlung
  case (Antiqua::PaymentStatus::ORDER_SHIPPED_WAIT_FOR_PAYMENT): {
    order_status = QString("SHIPPED_WAITING_FOR_PAYMENT");
    break;
  }
  // versendet & bezahlt
  case (Antiqua::PaymentStatus::ORDER_SHIPPED_AND_PAID): {
    order_status = QString("SHIPPED_AND_PAID");
    break;
  }
  // Kunde reagiert nicht
  case (Antiqua::PaymentStatus::ORDER_BUYER_NO_REACTION): {
    order_status = QString("BUYER_NO_REACTION");
    break;
  }
  default:
    qWarning("Unknown: order_status Action");
  };
  if (order_status.isEmpty())
    return;

  qDebug() << Q_FUNC_INFO << p_orderId << order_status;
  emit sendUpdateOrderStatus(p_orderId, order_status);
}

int BooklookerRemoteActions::exec(const QString &orderId) {
  if (orderId.length() < 4)
    return QDialog::Rejected;

  p_orderId = orderId;
  return QDialog::exec();
}
