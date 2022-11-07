// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERSYSTEMDATABASE_H
#define ANTIQUACRM_ORDERSYSTEMDATABASE_H

#include <AntiquaCRM>
#include <QThread>

class OrderSystemDatabase final : public QThread {
  Q_OBJECT

private:
  Q_DISABLE_COPY(OrderSystemDatabase);

  AntiquaCRM::ASqlCore *m_sql;

  /**
   * @brief Current Provider
   */
  const QString p_provider;

  /**
   * @brief Current Providers Orders
   */
  const AntiquaCRM::AProviderOrders p_orders;

  /**
   * @brief Create Customer with dataset.
   * @return customerId
   */
  qint64 createCustomers(AntiquaCRM::AProviderOrder &order) const;

  /**
   * @brief find Customer in Database witdh Current Dataset
   * If not exists create it ...
   */
  void findCustomers(const QString &orderId) const;

  /**
   * @brief Create a list with all new Orders
   */
  const QStringList findNewOrders() const;

  /**
   * @brief Insert New Orders in the Database
   * @param orderIds
   */
  bool insertNewOrders(const QStringList &orderIds);

Q_SIGNALS:
  void sendNewOrdersFound();

public:
  explicit OrderSystemDatabase(const QString &provider,
                               const AntiquaCRM::AProviderOrders orders);

  void run() override;
};

#endif // ANTIQUACRM_ORDERSYSTEMDATABASE_H
