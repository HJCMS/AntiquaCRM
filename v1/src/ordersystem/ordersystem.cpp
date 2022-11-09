// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordersystem.h"
#include "ordersystemdatabase.h"

#include <QDebug>

OrderSystem::OrderSystem(QObject *parent) : QObject{parent} {
  setObjectName("antiqua_order_system");
}

void OrderSystem::createNewOrdersArrived(const QString &provider) {
  QString txt = tr("New orders from %1 have arrived.").arg(provider);
  emit sendNewOrdersArrived(txt);
}

void OrderSystem::updateOrders(const QString &provider,
                               const AntiquaCRM::AProviderOrders orders) {
  OrderSystemDatabase *m_thread = new OrderSystemDatabase(provider, orders);
  connect(m_thread, SIGNAL(finished()), m_thread, SLOT(deleteLater()));
  connect(m_thread, SIGNAL(sendNewOrdersFound(const QString &)),
          SLOT(createNewOrdersArrived(const QString &)));
  m_thread->start();
}
