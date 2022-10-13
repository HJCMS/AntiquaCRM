// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PLUGIN_ORDERS_H
#define ANTIQUACRM_PLUGIN_ORDERS_H

#include <AGlobal>
#include <QHash>
#include <QList>
#include <QMetaType>
#include <QVariant>

namespace AntiquaCRM {

/**
 * @brief Provider Order class
 */
class ANTIQUACRM_LIBRARAY AProviderOrder {
private:
  const QString bookingId;
  QHash<QString, QVariant> p_data;

public:
  /**
   * @brief Provider Order
   * @param uniqId Order Id
   */
  explicit AProviderOrder(const QString &uniqId);

  /*
  static const QStringList dataKeyList();
  void setValue(const QString &key, const QVariant &value);
  const QVariant getValue(const QString &key);
  */
};

/**
 * @brief AntiquaCRM::AProviderOrders
 */
typedef QList<AProviderOrder> AProviderOrders;

}; // namespace AntiquaCRM

Q_DECLARE_METATYPE(AntiquaCRM::AProviderOrders)

#endif // ANTIQUACRM_PLUGIN_ORDERS_H
