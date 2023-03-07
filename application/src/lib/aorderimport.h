// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERIMPORT_H
#define ANTIQUACRM_ORDERIMPORT_H

#include <AGlobal>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

/**
 * @brief Import and convert Provider data for PgSql INSERT.
 * This Classes are used to prepare Provider data for insert into
 * PgSql::provider_orders table.
 */
namespace AntiquaCRM {

/**
 * @brief Order Article object class
 * @ingroup core
 */
class ANTIQUACRM_LIBRARY AOrderArticle final : public QJsonObject {
private:
  qint64 a_article_id;
  const QMap<QString, QJsonValue::Type> permitted;
  inline AntiquaCRM::SalesTax tax(const QJsonValue &) const;
  inline AntiquaCRM::ArticleType media(const QJsonValue &) const;

public:
  /**
   * @brief Create OrderArticle with required Article No.
   */
  explicit AOrderArticle(qint64 articleId);

  /**
   * @brief Returning all valid keys with acceptable type.
   */
  static const QMap<QString, QJsonValue::Type> keys();

  /**
   * @brief Add Article data
   */
  bool add(const QString &key, const QJsonValue &value);
};

/**
 * @brief Customer data object class
 * @ingroup core
 */
class ANTIQUACRM_LIBRARY AOrderCustomer final : public QJsonObject {
private:
  const QMap<QString, QJsonValue::Type> permitted;

public:
  explicit AOrderCustomer();

  /**
   * @brief Returning all valid keys with acceptable type.
   */
  static const QMap<QString, QJsonValue::Type> keys();

  /**
   * @brief Add Customer data
   */
  bool add(const QString &key, const QJsonValue &value);
};

/**
 * @brief Order Articles container class
 * @ingroup core
 */
class ANTIQUACRM_LIBRARY AOrderArticles final : public QJsonArray {
public:
  explicit AOrderArticles();
  bool add(const AOrderArticle &);
};

/**
 * @brief Order Info class
 * @ingroup core
 */
class ANTIQUACRM_LIBRARY AOrderInfo final : public QJsonObject {
private:
  const QMap<QString, QJsonValue::Type> permitted;
  inline AntiquaCRM::OrderStatus orderStatus(const QJsonValue &) const;
  inline AntiquaCRM::ArticleType mediaType(const QJsonValue &) const;
  inline AntiquaCRM::PaymentMethod paymentMethod(const QJsonValue &) const;
  inline AntiquaCRM::ProviderPaymentStatus
  paymentStatus(const QJsonValue &) const;

public:
  explicit AOrderInfo();

  /**
   * @brief Returning all valid keys with acceptable type.
   */
  static const QMap<QString, QJsonValue::Type> keys();

  /**
   * @brief Add info data
   */
  bool add(const QString &key, const QJsonValue &value);
};

/**
 * @brief Provider Order Document class
 * @ingroup core
 */
class ANTIQUACRM_LIBRARY AOrderImport final : public QJsonObject {
private:
  const QString p_provider;
  const QString p_order_id;
  bool checkObject(const QString &section);
  bool checkArray(const QString &section);

public:
  /**
   * @brief create Order document with provider order No.
   */
  explicit AOrderImport(const QString &provider, const QString &id);

  /**
   * @brief add Article orders
   */
  bool add(const AOrderInfo &);

  /**
   * @brief add Customer data
   */
  bool add(const AOrderCustomer &);

  /**
   * @brief add Article orders
   */
  bool add(const AOrderArticles &);

  /**
   * @brief Create JsonDocument with some checks
   * @note If an error has occurred, it responses with „error“ Attribute.
   * The error Attribute answers with:
   *  @li NO_INFO_OBJECT
   *  @li NO_CUSTOMER_OBJECT
   *  @li NO_ORDER_ARTICLES
   */
  const QJsonDocument create();
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_ORDERIMPORT_H
