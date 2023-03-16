// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_PLUGIN_ORDERS_H
#define ANTIQUACRM_PLUGIN_ORDERS_H

#include <AGlobal>
#include <QHash>
#include <QList>
#include <QMetaType>
#include <QRegularExpression>
#include <QVariant>

namespace AntiquaCRM {

/**
 * @brief Article Order Item Class
 * @ingroup OrderSystem
 */
struct ANTIQUACRM_LIBRARY ArticleOrderItem {
  QString key;
  QVariant value;
};
typedef QList<ArticleOrderItem> OrderArticleItems;

/**
 * @brief Primary Provider Orders Class
 * @ingroup OrderSystem
 *
 * It Contains a Overview from current Buyer Ordering.
 *
 * This class is used to import Orders from Different Providers.
 * All valid registered Keywords from Antigua CRM are inherit.
 *
 * If you create a new Plugin, it is important to use this Class to convert your
 * data for the main Application.
 */
class ANTIQUACRM_LIBRARY AProviderOrder final {
private:
  QString providerName;
  QString bookingId;
  QHash<QString, QVariant> p_data;
  QList<OrderArticleItems> p_orderItems;

  /**
   * @brief Regular Expression pattern for Key finding.
   */
  static const QRegularExpression keysPattern();

public:
  /**
   * @brief Provider Order constructor
   * @param provider  Provider name.
   * @param uniqId    Booking or Order Id.
   */
  explicit AProviderOrder(const QString &provider, const QString &uniqId);
  AProviderOrder(const AProviderOrder &other);
  AProviderOrder &operator=(const AProviderOrder &other);

  /**
   * @brief Current Provider name for this Object.
   */
  inline const QString provider() const { return providerName; };

  /**
   * @brief Imported Provider - Booking or Order Id.
   */
  inline const QString id() const { return bookingId; };

  /**
   * @brief Current Data Size
   */
  inline int count() const { return p_data.size(); };

  /**
   * @brief Uniq md5sum for store cache files
   */
  const QString md5sum() const;

  /**
   * @brief Contains all valid Configuration-keys for Ordering entries.
   * @code
   *  QHash<"registered keyword","required data type">
   * @endcode
   * For keyword descriptions see inside this function.
   */
  static const QHash<QString, QMetaType::Type> orderEditKeys();

  /**
   * @brief Returning all Customer keys from orderEditKeys()
   */
  static const QHash<QString, QMetaType::Type> customerKeys();

  /**
   * @brief Returning all Provider Order-Keys from orderEditKeys()
   */
  static const QHash<QString, QMetaType::Type> orderKeys();

  /**
   * @brief All valid Payment "Article" Keys.
   */
  static const QHash<QString, QMetaType::Type> articleKeys();

  /**
   * @brief returning all orderKeys() where a value is set
   */
  const QStringList filledKeys() const;

  /**
   * @brief set/update value with orderKey
   */
  bool setValue(const QString &key, const QVariant &value);

  /**
   * @brief get value from orderKey
   */
  const QVariant getValue(const QString &key);

  /**
   * @brief Create an new Article Order Item
   */
  static AntiquaCRM::ArticleOrderItem createItem(const QString &key,
                                                 const QVariant &value);

  /**
   * @brief Current Article Ordered Items
   */
  const QList<AntiquaCRM::OrderArticleItems> orders();

  /**
   * @brief Insert Article into Order Item list
   */
  bool insertOrderItems(const AntiquaCRM::OrderArticleItems &article);

  /**
   * @brief Remove Article from Order Item list.
   */
  bool removeOrderItem(const QString &orderItemId);

  /**
   * @brief Current Order Article Items Size
   */
  inline int orderItemCount() const { return p_orderItems.size(); };
};

/**
 * @brief AntiquaCRM::AProviderOrders
 */
typedef QList<AProviderOrder> AProviderOrders;

}; // namespace AntiquaCRM

Q_DECLARE_METATYPE(AntiquaCRM::OrderArticleItems)
Q_DECLARE_METATYPE(AntiquaCRM::AProviderOrders)

#endif // ANTIQUACRM_PLUGIN_ORDERS_H
