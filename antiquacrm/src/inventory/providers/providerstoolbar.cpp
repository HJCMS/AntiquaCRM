// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerstoolbar.h"
#include "myicontheme.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QMenu>
#include <QStringList>
#include <QStyle>
#include <QTimer>
#include <QVBoxLayout>

ProvidersToolBar::ProvidersToolBar(QWidget *parent) : QFrame{parent} {
  setObjectName("inventory_providers_toolbar");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setObjectName("providers_statusbar_layout");

  m_status = new QLabel(this);
  layout->addWidget(m_status);
  layout->addStretch(1);

  QString btnInfo = tr("Open view/edit Customer");
  btn_customer = new QPushButton(this);
  btn_customer->setObjectName("btn_customer");
  btn_customer->setToolTip(btnInfo);
  btn_customer->setStatusTip(btnInfo);
  btn_customer->setIcon(myIcon("group"));
  btn_customer->setText(tr("Customer context"));
  layout->addWidget(btn_customer);

  btn_order = new QPushButton(myIcon("db_add"), tr("Create Order"), this);
  QString oInfo = tr("first open the customer before an order can be placed.");
  btn_order->setToolTip(oInfo);
  btn_order->setStatusTip(oInfo);
  btn_order->setEnabled(false);
  layout->addWidget(btn_order);
  btn_refresh = new QPushButton(myIcon("reload"), tr("Refresh"), this);
  layout->addWidget(btn_refresh);
  setLayout(layout);

  connect(btn_customer, SIGNAL(clicked()), this, SIGNAL(s_customerAction()));
  connect(btn_order, SIGNAL(clicked()), this, SIGNAL(s_createOrder()));
  connect(btn_refresh, SIGNAL(clicked()), this, SIGNAL(s_refresh()));
}

void ProvidersToolBar::statusMessage(const QString &msg) {
  m_status->setText(msg);
  QTimer::singleShot((15 * 1000), m_status, SLOT(clear()));
}

void ProvidersToolBar::enableOrderButton(bool b) { btn_order->setEnabled(b); }
