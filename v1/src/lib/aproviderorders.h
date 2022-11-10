// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_PLUGIN_ORDERS_H
#define ANTIQUACRM_PLUGIN_ORDERS_H

#include <AGlobal>
#include <QHash>
#include <QList>
#include <QMetaType>
#include <QRegExp>
#include <QVariant>

namespace AntiquaCRM {

/**
 * @brief Article Order Item Class
 * @ingroup OrderSystem
 */
struct ANTIQUACRM_LIBRARAY ArticleOrderItem {
  QString key;
  QVariant value;
};
typedef QList<ArticleOrderItem> OrderArticleItems;

/**
 * @brief Primary Provider Orders Class
 * @ingroup OrderSystem
 * @section Containers
 *
 * It Contains a Overview from current Buyer Ordering.
 *
 * This class is used to import Orders from Different Providers.
 * All valid registered Keywords from Antigua CRM are inherit.
 *
 * If you create a new Plugin, it is important to use this Class to convert your
 * data for the main Application.
 */
class ANTIQUACRM_LIBRARAY AProviderOrder final {
private:
  QString providerName;
  QString bookingId;
  QHash<QString, QVariant> p_data;
  QList<OrderArticleItems> p_orderItems;

  /**
   * @brief Regular Expression pattern for Key finding.
   */
  static const QRegExp keysPattern();

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
  static const QHash<QString, QMetaType::Type> orderEditKeys() {
    QHash<QString, QMetaType::Type> hash;

    /**
     * @brief Customer Provider \b Unique Import key.
     * Some Providers didn't support separate Name Fields for Firstname and
     * Surname. To prevent duplicate Entries in the Database System, this value
     * can help.
     */
    hash.insert("c_provider_import", QMetaType::QString);

    /**
     * @brief Customer Gender
     * @see AntiquaCRM::Gender
     */
    hash.insert("c_gender", QMetaType::Int);

    /**
     * @brief Customer Firstname
     */
    hash.insert("c_firstname", QMetaType::QString);

    /**
     * @brief Customer Surname
     */
    hash.insert("c_lastname", QMetaType::QString);

    /**
     * @brief Customer Street
     */
    hash.insert("c_street", QMetaType::QString);

    /**
     * @brief Customer Postalcode
     */
    hash.insert("c_postalcode", QMetaType::QString);

    /**
     * @brief Customer Location
     */
    hash.insert("c_location", QMetaType::QString);

    /**
     * @brief Customer Country (Fullname)
     */
    hash.insert("c_country", QMetaType::QString);

    /**
     * @brief Customer Country IETF BCP 47 language tag
     */
    hash.insert("c_country_bcp47", QMetaType::QString);

    /**
     * @brief Customer Primary Phone number
     */
    hash.insert("c_phone_0", QMetaType::QString);

    /**
     * @brief customer eMail
     */
    hash.insert("c_email_0", QMetaType::QString);

    /**
     * @brief Customer - Company name
     */
    hash.insert("c_company_name", QMetaType::QString);

    /**
     * @brief Customer Banking IBAN
     */
    hash.insert("c_iban", QMetaType::QString);

    /**
     * @brief Customer Banking SWIFT BIC
     */
    hash.insert("c_swift_bic", QMetaType::QString);

    /**
     * @brief Customer - Company TAX Id
     */
    hash.insert("c_tax_id", QMetaType::QString);

    /**
     * @brief Customer Postal Address (Invoice Address)
     */
    hash.insert("c_postal_address", QMetaType::QString);

    /**
     * @brief Customer Delivery Address
     */
    hash.insert("c_shipping_address", QMetaType::QString);

    /**
     * @brief Customer Comments (Optional)
     */
    hash.insert("c_comments", QMetaType::QString);

    /**
     * @brief AntiquaCRM - System Order Id
     */
    hash.insert("o_id", QMetaType::Int);

    /**
     * @brief AntiquaCRM - Invoice Id
     */
    hash.insert("o_invoice_id", QMetaType::Int);

    /**
     * @brief e.g. Provider name
     */
    hash.insert("o_provider_name", QMetaType::QString);

    /**
     * @brief Order Provider - Order Id (String)
     */
    hash.insert("o_provider_order_id", QMetaType::QString);

    /**
     * @brief Order Provider Purchase Id (Numeric)
     */
    hash.insert("o_provider_purchase_id", QMetaType::Int);

    /**
     * @brief e.g. Order Customer Id
     */
    hash.insert("o_customer_id", QMetaType::Int);

    /**
     * @brief Order Delivery Comment from Buyer
     */
    hash.insert("o_delivery_comment", QMetaType::QString);

    /**
     * @brief Order Payment Confirmed
     */
    hash.insert("o_payment_confirmed", QMetaType::QDateTime);

    /**
     * @brief Order Payment PayPal TransactionId
     */
    hash.insert("o_payment_paypal_txn_id", QMetaType::QString);

    /**
     * @brief If already payed?
     */
    hash.insert("o_payment_status", QMetaType::Bool);

    /**
     * @brief Provider Order Payment Method
     * @see AntiquaCRM::PaymentMethod
     */
    hash.insert("o_payment_method", QMetaType::Int);

    /**
     * @brief Current Provider Order Status
     * @see AntiquaCRM::PaymentStatus
     */
    hash.insert("o_order_status", QMetaType::Int);

    /**
     * @brief Order add/since DateTime
     */
    hash.insert("o_since", QMetaType::QDateTime);

    /**
     * @brief Ordered Media type
     * @see AntiquaCRM::ArticleType
     */
    hash.insert("o_media_type", QMetaType::Int);

    /**
     * @brief Country IETF BCP 47 language tag
     */
    hash.insert("o_vat_country", QMetaType::QString);

    /**
     * @brief Delivery Service name
     * @see Antiqua::DeliveryService
     */
    hash.insert("o_delivery_service", QMetaType::Int);

    /**
     * @brief Delivery Package Send ID
     * @see Antiqua::DeliveryService
     */
    hash.insert("o_delivery_send_id", QMetaType::QString);

    /**
     * @brief Delivery
     * @see Antiqua::DeliveryService
     */
    hash.insert("o_delivery", QMetaType::QString);

    /**
     * @brief Delivery Package type
     * @see Antiqua::DeliveryService
     */
    hash.insert("o_delivery_package", QMetaType::Int);

    /**
     * @brief Delivery is VAT included?
     */
    hash.insert("o_vat_included", QMetaType::Bool);

    /**
     * @brief Delivery VAT Level
     */
    hash.insert("o_vat_levels", QMetaType::Int);

    /**
     * @brief Delivery add cost: \b postage
     */
    hash.insert("o_delivery_add_price", QMetaType::Bool);

    return hash;
  };

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
  static const QHash<QString, QMetaType::Type> articleKeys() {
    QHash<QString, QMetaType::Type> hash;
    /**
     * @brief AntiquaCRM Article Id
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
     * @brief Article Quantity
     */
    hash.insert("a_count", QMetaType::Int);

    /**
     * @brief Article "inventory price"
     */
    hash.insert("a_price", QMetaType::Double);

    /**
     * @brief Article "selling price"
     */
    hash.insert("a_sell_price", QMetaType::Double);

    /**
     * @brief Article Titel
     */
    hash.insert("a_title", QMetaType::QString);

    return hash;
  }

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
