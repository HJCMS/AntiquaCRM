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
class ANTIQUACRM_LIBRARAY AProviderOrder final {
private:
  QString bookingId;
  QHash<QString, QVariant> p_data;

public:
  /**
   * @brief Provider Order
   * @param uniqId Order Id
   */
  AProviderOrder(const QString &uniqId);
  AProviderOrder(const AProviderOrder &other);
  AProviderOrder &operator=(const AProviderOrder &other);

  inline const QString id() const { return bookingId; };

  inline int count() const { return p_data.size(); };

  /**
   * @brief valid configuration keys
   * @code
   *  QHash<"registered key","data type">
   * @endcode
   */
  static const QHash<QString, QMetaType::Type> validKeys() {
    QHash<QString, QMetaType::Type> hash;
    /**
     * Customers Data @{
     */
    hash.insert("c_gender", QMetaType::QString);
    hash.insert("c_firstname", QMetaType::QString);
    hash.insert("c_lastname", QMetaType::QString);
    hash.insert("c_street", QMetaType::QString);
    hash.insert("c_postalcode", QMetaType::QString);
    hash.insert("c_location", QMetaType::QString);
    hash.insert("c_country", QMetaType::QString);
    hash.insert("c_phone_0", QMetaType::QString);
    hash.insert("c_email_0", QMetaType::QString);
    hash.insert("c_company_name", QMetaType::QString);
    hash.insert("c_iban", QMetaType::QString);
    hash.insert("c_swift_bic", QMetaType::QString);
    hash.insert("c_tax_id", QMetaType::QString);
    hash.insert("c_postal_address", QMetaType::QString);
    hash.insert("c_shipping_address", QMetaType::QString);
    hash.insert("c_comments", QMetaType::QString);
    /**
     * @}
     * @brief payment section
     * @{
     */
    hash.insert("o_delivery_comment", QMetaType::QString);
    hash.insert("o_payment_confirmed", QMetaType::QDateTime);
    hash.insert("o_payment_paypal_txn_id", QMetaType::QString);
    hash.insert("o_payment_method", QMetaType::Int);
    hash.insert("o_provider_order_id", QMetaType::QString);
    hash.insert("o_provider_order_status", QMetaType::Int);
    hash.insert("o_since", QMetaType::QDateTime);
    hash.insert("o_media_type", QMetaType::Int);
    hash.insert("o_provider_purchase_id", QMetaType::Int);
    /**
     * @}
     * @brief Articles
     */
    hash.insert("a_article_id", QMetaType::Int);
    hash.insert("ib_author", QMetaType::QString);
    hash.insert("ib_price", QMetaType::Double);
    hash.insert("a_count", QMetaType::Int);
    hash.insert("a_title", QMetaType::QString);
    hash.insert("a_sell_price", QMetaType::Double);
    return hash;
  };

  /**
   * @brief return all keys where a value is set
   */
  const QStringList currentKeys() const;

  /**
   * @brief set/update value with key
   */
  bool setValue(const QString &key, const QVariant &value);

  /**
   * @brief get value with key
   */
  const QVariant getValue(const QString &key);
};

/**
 * @brief AntiquaCRM::AProviderOrders
 */
typedef QList<AProviderOrder> AProviderOrders;

}; // namespace AntiquaCRM

Q_DECLARE_METATYPE(AntiquaCRM::AProviderOrders)

#endif // ANTIQUACRM_PLUGIN_ORDERS_H
