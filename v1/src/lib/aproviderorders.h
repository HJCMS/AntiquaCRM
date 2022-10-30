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

struct ANTIQUACRM_LIBRARAY AProviderOrderItem {
  QString key;
  QVariant value;
};
typedef QList<AProviderOrderItem> AProviderOrderItems;

/**
 * @brief Provider Order class
 */
class ANTIQUACRM_LIBRARAY AProviderOrder final {
private:
  QString providerName;
  QString bookingId;
  QHash<QString, QVariant> p_data;
  QList<AProviderOrderItems> p_orderItems;

public:
  /**
   * @brief Provider Order
   * @param uniqId Order Id
   */
  AProviderOrder(const QString &provider, const QString &uniqId);
  AProviderOrder(const AProviderOrder &other);
  AProviderOrder &operator=(const AProviderOrder &other);

  /**
   * @brief Provider name
   */
  inline const QString provider() const { return providerName; };

  /**
   * @brief Booking Id
   */
  inline const QString id() const { return bookingId; };

  /**
   * @brief Current Data Size
   */
  inline int count() const { return p_data.size(); };

  /**
   * @brief valid configuration keys
   * @code
   *  QHash<"registered key","data type">
   * @endcode
   */
  static const QHash<QString, QMetaType::Type> orderKeys() {
    QHash<QString, QMetaType::Type> hash;
    /**
     * @brief uniq customer Name to prevent duplicate enries!
     */
    hash.insert("c_provider_import", QMetaType::QString);

    /**
     * @brief customer gender
     */
    hash.insert("c_gender", QMetaType::QString);

    /**
     * @brief customer firstname
     */
    hash.insert("c_firstname", QMetaType::QString);

    /**
     * @brief customer surname
     */
    hash.insert("c_lastname", QMetaType::QString);

    /**
     * @brief customer street
     */
    hash.insert("c_street", QMetaType::QString);

    /**
     * @brief customer postalcode
     */
    hash.insert("c_postalcode", QMetaType::QString);

    /**
     * @brief customer location
     */
    hash.insert("c_location", QMetaType::QString);

    /**
     * @brief customer country (Fullname)
     */
    hash.insert("c_country", QMetaType::QString);

    /**
     * @brief customer phone number
     */
    hash.insert("c_phone_0", QMetaType::QString);

    /**
     * @brief customer eMail
     */
    hash.insert("c_email_0", QMetaType::QString);

    /**
     * @brief company name
     */
    hash.insert("c_company_name", QMetaType::QString);

    /**
     * @brief banking IBAN
     */
    hash.insert("c_iban", QMetaType::QString);

    /**
     * @brief banking SWIFT BIC
     */
    hash.insert("c_swift_bic", QMetaType::QString);

    /**
     * @brief company TAX Id
     */
    hash.insert("c_tax_id", QMetaType::QString);

    /**
     * @brief complete Postal Address
     */
    hash.insert("c_postal_address", QMetaType::QString);

    /**
     * @brief complete Delivery Address
     */
    hash.insert("c_shipping_address", QMetaType::QString);

    /**
     * @brief customer Comments
     */
    hash.insert("c_comments", QMetaType::QString);

    /**
     * @brief Provider Order Id String
     */
    hash.insert("o_provider_order_id", QMetaType::QString);

    /**
     * @brief Provider Order Id Numeric
     */
    hash.insert("o_provider_purchase_id", QMetaType::Int);

    /**
     * @brief Delivery Comment
     */
    hash.insert("o_delivery_comment", QMetaType::QString);

    /**
     * @brief Payment Confirmed
     */
    hash.insert("o_payment_confirmed", QMetaType::QDateTime);

    /**
     * @brief PayPal TransactionId
     */
    hash.insert("o_payment_paypal_txn_id", QMetaType::QString);

    /**
     * @brief Payment Method
     * @see AntiquaCRM::PaymentMethod
     */
    hash.insert("o_payment_method", QMetaType::Int);

    /**
     * @brief Provider Order Status
     * @see AntiquaCRM::PaymentStatus
     */
    hash.insert("o_provider_order_status", QMetaType::Int);

    /**
     * @brief Order Date & Time
     */
    hash.insert("o_since", QMetaType::QDateTime);

    /**
     * @brief Media type
     * @see AntiquaCRM::ArticleType
     */
    hash.insert("o_media_type", QMetaType::Int);

    /**
     * @brief Delivery cost: "postage"
     */
    hash.insert("d_price", QMetaType::Double);

    return hash;
  };

  static const QHash<QString, QMetaType::Type> articleKeys() {
    QHash<QString, QMetaType::Type> hash;
    /**
     * @brief AntiquaCRM Article Number
     */
    hash.insert("a_article_id", QMetaType::Int);

    /**
     * @brief Provider Article Order Id
     */
    hash.insert("a_provider_id", QMetaType::QString);

    /**
     * @brief Article Type
     * @see AntiquaCRM::ArticleType e.g. Book=1
     */
    hash.insert("a_type", QMetaType::Int);

    /**
     * @brief Article amount
     */
    hash.insert("a_count", QMetaType::Int);

    /**
     * @brief Article basic price
     */
    hash.insert("a_price", QMetaType::Double);

    /**
     * @brief Article sell price
     */
    hash.insert("a_sell_price", QMetaType::Double);

    /**
     * @brief Article Titel
     */
    hash.insert("a_title", QMetaType::QString);

    return hash;
  }

  /**
   * @brief return all keys where a value is set
   */
  const QStringList filledKeys() const;

  /**
   * @brief set/update value with key
   */
  bool setValue(const QString &key, const QVariant &value);

  /**
   * @brief get value with key
   */
  const QVariant getValue(const QString &key);

  /**
   * @brief Ordered Items
   */
  const QList<AProviderOrderItems> orders();

  /**
   * @brief Insert Order Item
   */
  bool insertOrderItems(const AProviderOrderItems &article);

  /**
   * @brief Remove Order Item from list.
   */
  bool removeOrderItem(const QString &orderItemId);
};

/**
 * @brief AntiquaCRM::AProviderOrders
 */
typedef QList<AProviderOrder> AProviderOrders;

}; // namespace AntiquaCRM

Q_DECLARE_METATYPE(AntiquaCRM::AProviderOrderItems)
Q_DECLARE_METATYPE(AntiquaCRM::AProviderOrders)

#endif // ANTIQUACRM_PLUGIN_ORDERS_H
