// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aorderimport.h"

#ifdef ANTIQUA_DEVELOPMENT
#include <QDebug>
#endif

namespace AntiquaCRM {

// BEGIN::AOrderArticle
AOrderArticle::AOrderArticle(qint64 articleId)
    : QJsonObject{}, a_article_id{articleId}, permitted{keys()} {
  insert("a_article_id", QJsonValue(articleId));
}

AntiquaCRM::SalesTax AOrderArticle::tax(const QJsonValue &v) const {
  return static_cast<AntiquaCRM::SalesTax>(v.toInt());
}

AntiquaCRM::ArticleType AOrderArticle::media(const QJsonValue &v) const {
  return static_cast<AntiquaCRM::ArticleType>(v.toInt());
}

const QMap<QString, QJsonValue::Type> AOrderArticle::keys() {
  QMap<QString, QJsonValue::Type> map;
  map.insert("a_article_id", QJsonValue::Double);
  map.insert("a_count", QJsonValue::Double);
  map.insert("a_payment_id", QJsonValue::Double);
  map.insert("a_price", QJsonValue::Double);
  map.insert("a_provider_id", QJsonValue::String);
  map.insert("a_sell_price", QJsonValue::Double);
  map.insert("a_tax", QJsonValue::Double);
  map.insert("a_title", QJsonValue::String);
  map.insert("a_type", QJsonValue::Double);
  return map;
}

bool AOrderArticle::add(const QString &key, const QJsonValue &value) {
  QMapIterator<QString, QJsonValue::Type> it(permitted);
  while (it.hasNext()) {
    if (it.key() == key && value.type() == it.value()) {
      if (key == "a_tax")
        insert(key, tax(value));
      else if (key == "a_type")
        insert(key, media(value));
      else
        insert(key, value);

      return true;
    }
  }
  return false;
}
// END::AOrderArticle

// BEGIN::ACustomer
AOrderCustomer::AOrderCustomer() : QJsonObject{}, permitted{keys()} {};

const QMap<QString, QJsonValue::Type> AOrderCustomer::keys() {
  QMap<QString, QJsonValue::Type> map;
  map.insert("c_country", QJsonValue::String);
  map.insert("c_country_bcp47", QJsonValue::String);
  map.insert("c_email_0", QJsonValue::String);
  map.insert("c_firstname", QJsonValue::String);
  map.insert("c_gender", QJsonValue::Double);
  map.insert("c_lastname", QJsonValue::String);
  map.insert("c_location", QJsonValue::String);
  map.insert("c_phone_0", QJsonValue::String);
  map.insert("c_postal_address", QJsonValue::String);
  map.insert("c_postalcode", QJsonValue::String);
  map.insert("c_provider_import", QJsonValue::String);
  map.insert("c_shipping_address", QJsonValue::String);
  map.insert("c_street", QJsonValue::String);
  return map;
}

bool AOrderCustomer::add(const QString &key, const QJsonValue &value) {
  QMapIterator<QString, QJsonValue::Type> it(permitted);
  while (it.hasNext()) {
    if (it.key() == key && value.type() == it.value()) {
      insert(key, value);
      return true;
    }
  }
  return false;
}
// END::ACustomer

// BEGIN::Articles
AOrderArticles::AOrderArticles() : QJsonArray{} {};

bool AOrderArticles::add(const AOrderArticle &article) {
  if (article.isEmpty() || !article.contains("a_article_id"))
    return false;

  append(article);
  return true;
}
// END::Articles

// BEGIN::AOrderInfo
AOrderInfo::AOrderInfo() : QJsonObject{}, permitted{keys()} {}

AntiquaCRM::OrderStatus AOrderInfo::orderStatus(const QJsonValue &val) const {
  return static_cast<AntiquaCRM::OrderStatus>(val.toInt());
}

AntiquaCRM::ArticleType AOrderInfo::mediaType(const QJsonValue &v) const {
  return static_cast<AntiquaCRM::ArticleType>(v.toInt());
}

AntiquaCRM::PaymentMethod
AOrderInfo::paymentMethod(const QJsonValue &val) const {
  return static_cast<AntiquaCRM::PaymentMethod>(val.toInt());
}

AntiquaCRM::ProviderPaymentStatus
AOrderInfo::paymentStatus(const QJsonValue &val) const {
  return static_cast<AntiquaCRM::ProviderPaymentStatus>(val.toInt());
}

const QMap<QString, QJsonValue::Type> AOrderInfo::keys() {
  QMap<QString, QJsonValue::Type> map;
  map.insert("o_delivery_add_price", QJsonValue::Bool);
  map.insert("o_media_type", QJsonValue::Double);
  map.insert("o_order_status", QJsonValue::Double);
  map.insert("o_payment_method", QJsonValue::Double);
  map.insert("o_payment_status", QJsonValue::Double);
  map.insert("o_provider_name", QJsonValue::String);
  map.insert("o_provider_order_id", QJsonValue::String);
  map.insert("o_provider_purchase_id", QJsonValue::Double);
  map.insert("o_since", QJsonValue::String);
  map.insert("o_vat_country", QJsonValue::String);
  return map;
}

bool AOrderInfo::add(const QString &key, const QJsonValue &value) {
  QMapIterator<QString, QJsonValue::Type> it(permitted);
  while (it.hasNext()) {
    if (it.key() == key && value.type() == it.value()) {
      if (key == "o_order_status")
        insert(key, orderStatus(value));
      else if (key == "o_media_type")
        insert(key, mediaType(value));
      else if (key == "o_payment_method")
        insert(key, paymentMethod(value));
      else if (key == "o_payment_status")
        insert(key, paymentStatus(value));
      else
        insert(key, value);
      return true;
    }
  }
  return false;
}
// END::AOrderInfo

// BEGIN::AOrder
AOrderImport::AOrderImport(const QString &provider, const QString &id)
    : QJsonObject{}, p_provider{provider}, p_order_id{id} {
  insert("provider", QJsonValue(provider.trimmed()));
  insert("orderid", QJsonValue(id.trimmed()));
}

bool AOrderImport::checkObject(const QString &section) {
  if (!contains(section) || value(section).type() != QJsonValue::Object) {
    qWarning("order document does not have valid %s object!",
             qPrintable(section));
    return false;
  }
  return true;
}

bool AOrderImport::checkArray(const QString &section) {
  if (!contains(section) || value(section).type() != QJsonValue::Array) {
    qWarning("order document does not have valid %s array!",
             qPrintable(section));
    return false;
  }
  return true;
}

bool AOrderImport::add(const AOrderInfo &info) {
  if (info.isEmpty() || !info.contains("o_provider_order_id"))
    return false;

  insert("orderinfo", QJsonObject(info));
  return true;
}

bool AOrderImport::add(const AOrderCustomer &customer) {
  if (customer.isEmpty() || !customer.contains("c_provider_import"))
    return false;

  insert("customer", QJsonObject(customer));
  return true;
}

bool AOrderImport::add(const AOrderArticles &array) {
  if (array.size() < 1)
    return false;

  insert("articles", QJsonArray(array));
  return true;
}

const QJsonDocument AOrderImport::create() {
  if (!checkObject("orderinfo")) {
    QJsonObject invalid;
    invalid.insert("error", "NO_INFO_OBJECT");
    return QJsonDocument(invalid);
  }

  if (!checkObject("customer")) {
    QJsonObject invalid;
    invalid.insert("error", "NO_CUSTOMER_OBJECT");
    return QJsonDocument(invalid);
  }

  if (!checkArray("articles")) {
    QJsonObject invalid;
    invalid.insert("error", "NO_ORDER_ARTICLES");
    return QJsonDocument(invalid);
  }

  return QJsonDocument(*this);
}
// END::AOrder

} // namespace AntiquaCRM
