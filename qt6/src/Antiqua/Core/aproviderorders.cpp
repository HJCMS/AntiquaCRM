// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aproviderorders.h"

#include <QCryptographicHash>
#include <QDate>
#include <QDateTime>
#include <QDebug>

namespace AntiquaCRM {

AProviderOrder::AProviderOrder(const QString &provider, const QString &uniqId) {
  providerName = provider;
  bookingId = uniqId;
  p_data = QHash<QString, QVariant>();
  p_orderItems = QList<OrderArticleItems>();
}

AProviderOrder::AProviderOrder(const AProviderOrder &other) {
  providerName = other.providerName;
  bookingId = other.bookingId;
  p_data = other.p_data;
  p_orderItems = other.p_orderItems;
}

AProviderOrder &AProviderOrder::operator=(const AProviderOrder &other) {
  if (this == &other)
    return *this;

  providerName = other.providerName;
  bookingId = other.bookingId;
  p_data = other.p_data;
  p_orderItems = other.p_orderItems;
  return *this;
}

const QString AProviderOrder::md5sum() const {
  QByteArray buf = QString(provider().toLower() + "_" + id()).toLocal8Bit();
  return QCryptographicHash::hash(buf, QCryptographicHash::Md5).toHex();
}

const QRegularExpression AProviderOrder::keysPattern() {
  return QRegularExpression("^c_[a-z0-9_]+$");
}

const QHash<QString, QMetaType::Type> AProviderOrder::orderEditKeys() {
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
  hash.insert("o_id", QMetaType::LongLong);

  /**
   * @brief AntiquaCRM - Invoice Id
   */
  hash.insert("o_invoice_id", QMetaType::LongLong);

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
  hash.insert("o_provider_purchase_id", QMetaType::LongLong);

  /**
   * @brief e.g. Order Customer Id
   */
  hash.insert("o_customer_id", QMetaType::LongLong);

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
  hash.insert("o_payment_status", QMetaType::Int);

  /**
   * @brief Provider Order Payment Method
   * @see AntiquaCRM::PaymentMethod
   */
  hash.insert("o_payment_method", QMetaType::Int);

  /**
   * @brief Current Provider Order Status
   * @see AntiquaCRM::ProviderPaymentStatus
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
}

const QHash<QString, QMetaType::Type> AProviderOrder::customerKeys() {
  QHash<QString, QMetaType::Type> list;
  QHashIterator<QString, QMetaType::Type> it(orderEditKeys());
  while (it.hasNext()) {
    it.next();
    if (it.key().contains(keysPattern()))
      list.insert(it.key(), it.value());
  }
  return list;
}

const QHash<QString, QMetaType::Type> AProviderOrder::orderKeys() {
  QHash<QString, QMetaType::Type> list;
  QHashIterator<QString, QMetaType::Type> it(orderEditKeys());
  while (it.hasNext()) {
    it.next();
    if (!it.key().contains(keysPattern()))
      list.insert(it.key(), it.value());
  }
  return list;
}

const QHash<QString, QMetaType::Type> AProviderOrder::articleKeys() {
  QHash<QString, QMetaType::Type> hash;
  /**
   * @brief AntiquaCRM a_order_id=(o_id)
   */
  hash.insert("a_order_id", QMetaType::Int);

  /**
   * @brief AntiquaCRM Article Id
   */
  hash.insert("a_article_id", QMetaType::Int);

  /**
   * @brief Provider Article Order Id
   */
  hash.insert("a_provider_id", QMetaType::QString);

  /**
   * @brief Payment Article Order Id Default:0
   */
  hash.insert("a_payment_id", QMetaType::Int);

  /**
   * @brief Customer Id Default:0
   */
  hash.insert("a_customer_id", QMetaType::Int);

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
   * @brief Article Tax Type
   * @see PurchaseTableModel::TaxType
   */
  hash.insert("a_tax", QMetaType::Int);

  /**
   * @brief Article "inventory price"
   */
  hash.insert("a_price", QMetaType::Double);

  /**
   * @brief Article "inventory refunding"
   */
  hash.insert("a_refunds_cost", QMetaType::Double);

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

const QStringList AProviderOrder::filledKeys() const {
  QStringList list;
  QHashIterator<QString, QVariant> it(p_data);
  while (it.hasNext()) {
    it.next();
    if (!it.value().isNull())
      list << it.key();
  }
  return list;
}

bool AProviderOrder::setValue(const QString &key, const QVariant &value) {
  QHashIterator<QString, QMetaType::Type> it(orderEditKeys());
  while (it.hasNext()) {
    it.next();
    QMetaType type(it.value());
    QString typeName(type.name());
    if ((it.key() == key) && (typeName.contains(value.typeName()))) {
      if (type.id() == QMetaType::QString)
        p_data.insert(key, value.toString().trimmed());
      else if (type.id() == QMetaType::Int)
        p_data.insert(key, value.toInt());
      else if (type.id() == QMetaType::Bool)
        p_data.insert(key, value.toBool());
      else if (type.id() == QMetaType::QDateTime)
        p_data.insert(key, value.toDateTime());
      else if (type.id() == QMetaType::Double)
        p_data.insert(key, value.toInt());
      else
        p_data.insert(key, value);

      return true;
    } else if ((it.key() == key) && (value.metaType().id() == QMetaType::LongLong)) {
      p_data.insert(key, value.toInt());
      return true;
    } else if ((it.key() == key) && (value.metaType().id() == QMetaType::Double)) {
      p_data.insert(key, value.toDouble());
      return true;
    } else if ((it.key() == key) && (value.metaType().id() == QMetaType::QDateTime)) {
      p_data.insert(key, value.toDateTime());
      return true;
    }
  }
  return false;
}

const QVariant AProviderOrder::getValue(const QString &key) {
  if (count() < 1)
    return QVariant();

  QHashIterator<QString, QVariant> it(p_data);
  while (it.hasNext()) {
    it.next();
    if (it.key() == key) {
      return it.value();
    }
  }
  return QVariant();
}

AntiquaCRM::ArticleOrderItem AProviderOrder::createItem(const QString &key,
                                                        const QVariant &value) {
  AntiquaCRM::ArticleOrderItem item;
  item.key = key;
  item.value = value;
  return item;
}

const QList<OrderArticleItems> AProviderOrder::orders() { return p_orderItems; }

bool AProviderOrder::insertOrderItems(const OrderArticleItems &article) {
  p_orderItems.append(article);
  return true;
}

bool AProviderOrder::removeOrderItem(const QString &orderItemId) {
  if (p_orderItems.size() < 1) {
    qWarning("AProviderOrderItems ist empty, nothing to remove!");
    return false;
  }

  QListIterator<OrderArticleItems> main(p_orderItems);
  int i = 0;
  while (main.hasNext()) {
    QListIterator<ArticleOrderItem> sub(main.next());
    while (sub.hasNext()) {
      ArticleOrderItem item = sub.next();
      if (item.key == orderItemId) {
#ifdef ANTIQUA_DEVELOPEMENT
        qDebug() << Q_FUNC_INFO << i << item.key;
#endif
        p_orderItems.removeAt(i);
        return true;
      }
    }
    i++;
  }
  return false;
}

}; // namespace AntiquaCRM
