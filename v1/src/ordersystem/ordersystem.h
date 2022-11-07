// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERSYSTEM_H
#define ANTIQUACRM_ORDERSYSTEM_H

#include <AntiquaCRM>
#include <QObject>
#include <QString>

/**
 * @brief The OrderSystem class import new Orders into the System
 * @section database
 */
class OrderSystem final : public QObject {
  Q_OBJECT

Q_SIGNALS:
  /**
   * @brief This Signal  will emmite when a new Order exists!
   */
  void sendNewOrdersArrived();

public:
  explicit OrderSystem(QObject *parent = nullptr);

  /**
   * @brief Running Orders Thread in background
   * @param provider - Provider Id
   * @param orders - Orders List
   */
  void updateOrders(const QString &provider, const AntiquaCRM::AProviderOrders orders);
};

#endif
