// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklooker.h"

/**
 * @def DATE_FORMAT
 * @ingroup Providers SQL Statements
 */
#ifndef BOOKLOOKER_DATE_FORMAT
#define BOOKLOOKER_DATE_FORMAT "yyyy-MM-dd"
#endif

/** @brief Konfigurationsgruppe */
#ifndef BOOKLOOKER_CONFIG_GROUP
#define BOOKLOOKER_CONFIG_GROUP "provider/booklooker"
#endif

/** @brief Wird für Menüeintrag und Gruppenzuweisung benötigt! */
#ifndef BOOKLOOKER_CONFIG_PROVIDER
#define BOOKLOOKER_CONFIG_PROVIDER "Booklooker"
#endif

#ifndef BOOKLOOKER_API_VERSION
#define BOOKLOOKER_API_VERSION "2.0"
#endif

// Headers
#include <AntiquaCRM>
#include <QDateTime>
#include <QDebug>
#include <QTimeZone>
#include <QUrlQuery>

/**
 * @brief Translate Article Ids
 * @code
  "orderItems": [{
    "amount": 1,
    "author": "James, Henry",
    "mediaType": 0,
    "orderItemId": 31453032,
    "orderNo": "112999",
    "orderTitle": "Die Flügel der Taube",
    "singlePrice": "13.00",
    "status": "Ordered",
    "totalPriceRebated": "13.00"
  }], ...
* @endcode
*/
static const QHash<QString, QString> translateIds() {
  QHash<QString, QString> hash;
  // AntiquaCRM Article Number
  hash.insert("orderNo", "a_article_id");
  // Provider Article Order Id
  hash.insert("orderItemId", "a_provider_id");
  // @see AntiquaCRM::ArticleType e.g. Book=1
  hash.insert("mediaType", "a_type");
  // Article amount
  hash.insert("amount", "a_count");
  // Article basic price
  hash.insert("singlePrice", "a_price");
  // Article sell price
  hash.insert("totalPriceRebated", "a_sell_price");
  // Article Titel
  hash.insert("orderTitle", "a_title");
  return hash;
}

Booklooker::Booklooker(QObject *parent)
    : AntiquaCRM::APluginInterface{parent}, p_articleTranslate{translateIds()} {
  setObjectName(BOOKLOOKER_CONFIG_PROVIDER);

  m_network = new AntiquaCRM::ANetworker(AntiquaCRM::JSON_QUERY, this);
  initConfigurations();

  connect(m_network, SIGNAL(sendJsonResponse(const QJsonDocument &)),
          SLOT(prepareResponse(const QJsonDocument &)));
  connect(m_network, SIGNAL(sendContentCodec(QTextCodec *)),
          SLOT(setContentDecoder(QTextCodec *)));
  connect(m_network, SIGNAL(finished(QNetworkReply *)),
          SLOT(queryFinished(QNetworkReply *)));
}

void Booklooker::initConfigurations() {
  QUrl url;
  AntiquaCRM::ASettings cfg(this);
  cfg.beginGroup(BOOKLOOKER_CONFIG_GROUP);
  url.setScheme("https");
  url.setHost(cfg.value("api_host", "api.booklooker.de").toString());
  apiKey = cfg.value("api_key", QString()).toString();
  historyCall = cfg.value("api_history_call", -7).toInt();
  cfg.endGroup();
  apiUrl = url;
}

const QUrl Booklooker::apiQuery(const QString &section) {
  QString p("/");
  p.append(BOOKLOOKER_API_VERSION);
  p.append("/");
  p.append(section);

  QUrl url(apiUrl);
  url.setPath(p);

  QString value = configProvider();
  value.append("_");
  value.append(section);
  value.append(".json");

  actionsCookie = QNetworkCookie("action", value.toLocal8Bit());
  actionsCookie.setDomain(url.host());
  actionsCookie.setSecure(true);

  return url;
}

const QString Booklooker::dateString(const QDate &date) const {
  return date.toString(BOOKLOOKER_DATE_FORMAT);
}

const AntiquaCRM::AProviderOrderItem
Booklooker::articleItem(const QString &key, const QJsonValue &value) const {
  QString _key = p_articleTranslate.value(key);
  QHash<QString, QMetaType::Type> keys =
      AntiquaCRM::AProviderOrder::articleKeys();
  if (!keys.contains(_key)) {
    qWarning("Booklooker: Unknown Article Key(%s)!", qPrintable(key));
    return AntiquaCRM::AProviderOrderItem();
  }

  QVariant _value;
  if (_key == "a_article_id") {
    // Artikel Nummer
    _value = value.toInt();
  } else if (_key == "a_provider_id") {
    // Dienstleister Bestellnummer
    _value = QString::number(value.toInt());
  } else if (_key.contains("_price")) {
    // Preise
    if (value.type() == QJsonValue::String) {
      _value = QString(value.toString()).toDouble();
    } else if (value.type() == QJsonValue::Double) {
      _value = value.toDouble();
    }
  } else if (keys.value(_key) == QMetaType::Int) {
    _value = value.toInt();
  } else if (keys.value(_key) == QMetaType::QString) {
    _value = value.toString();
  }

  AntiquaCRM::AProviderOrderItem item;
  item.key = _key;
  item.value = _value;
  return item;
}

void Booklooker::setTokenCookie(const QString &token) {
  QDateTime dt = QDateTime::currentDateTime();
  dt.setTimeSpec(Qt::UTC);
  qint64 cookie_lifetime = (9 * 60);
  authenticCookie = QNetworkCookie("token", token.toLocal8Bit());
  authenticCookie.setDomain(apiUrl.host());
  authenticCookie.setSecure(true);
  authenticCookie.setExpirationDate(dt.addSecs(cookie_lifetime));
  if (!authenticCookie.value().isNull()) {
    qInfo("New Token add (%s)", qPrintable(authenticCookie.value()));
    queryNewOrders();
  }
}

bool Booklooker::isCookieExpired() {
  if (authenticCookie.value().isNull())
    return true;

  QDateTime dt = QDateTime::currentDateTime();
  dt.setTimeSpec(Qt::UTC);
  return (authenticCookie.expirationDate() <= dt);
}

void Booklooker::authenticate() {
  QUrl url = apiQuery("authenticate");

  QString pd("apiKey=");
  pd.append(apiKey);

  actionsCookie = QNetworkCookie("action", "authenticate");
  actionsCookie.setDomain(url.host());
  actionsCookie.setSecure(true);

  m_network->loginRequest(url, pd.toLocal8Bit());
}

void Booklooker::prepareResponse(const QJsonDocument &js) {
  if (actionsCookie.value().contains("authenticate")) {
    QJsonObject obj = js.object();
    if (obj.value("status").toString().toLower() == "ok") {
      QString token = obj.value("returnValue").toString();
      setTokenCookie(token);
    }
    return;
  }

  QString fileName = configProvider();
  if (!actionsCookie.name().isNull()) {
    fileName = QString(actionsCookie.value());
  }

  AntiquaCRM::ASharedCacheFiles cacheFile;
  QString jsonData = js.toJson(QJsonDocument::Indented);
  cacheFile.storeTempFile(fileName.toLower(), jsonData);

  emit sendQueryFinished();
}

void Booklooker::prepareResponse(const QDomDocument &xml) { Q_UNUSED(xml); }

void Booklooker::queryFinished(QNetworkReply *reply) {
  if (reply->error() != QNetworkReply::NoError) {
    emit sendErrorResponse(AntiquaCRM::WARNING,
                           tr("Booklooker response with errors!"));
  }
}

void Booklooker::queryNewOrders(int waitSecs) {
  Q_UNUSED(waitSecs);
  if (isCookieExpired()) {
    authenticate();
    return;
  }

  QUrl url = apiQuery("order");
  QDate past = QDate::currentDate().addDays(historyCall);
  QUrlQuery q;
  q.addQueryItem("token", QString(authenticCookie.value()));
  q.addQueryItem("dateFrom", dateString(past));
  q.addQueryItem("dateTo", dateString());
  url.setQuery(q);
  m_network->getRequest(url);
}

void Booklooker::queryOrder(const QString &orderId) {
  if (isCookieExpired()) {
    authenticate();
    return;
  }

  QUrl url = apiQuery("order");
  QDate past = QDate::currentDate().addDays(historyCall);
  QUrlQuery q;
  q.addQueryItem("token", QString(authenticCookie.value()));
  q.addQueryItem("orderId", orderId);
  url.setQuery(q);
  m_network->getRequest(url);
}

const QString Booklooker::configProvider() const {
  return QString(BOOKLOOKER_CONFIG_PROVIDER).toLower();
}

const QString Booklooker::displayName() const {
  return QString(BOOKLOOKER_CONFIG_PROVIDER).trimmed();
}

const AntiquaCRM::AProviderOrders Booklooker::getOrders() const {
  AntiquaCRM::AProviderOrders allOrders;

  QString fileName = configProvider();
  if (!actionsCookie.name().isNull()) {
    fileName = QString(actionsCookie.value());
  }

  if (fileName.isEmpty())
    return allOrders;

  AntiquaCRM::ASharedCacheFiles cacheFile;
  // QString data = cacheFile.getTempFile(fileName.toLower());
  QString data = cacheFile.getTempFile("booklooker_order_test.json");
  if (data.isEmpty())
    return allOrders;

  QString providerName = displayName().toLower();
  QJsonDocument doc = QJsonDocument::fromJson(data.toLocal8Bit());
  QJsonArray orders = doc.object().value("returnValue").toArray();
  if (orders.size() > 0) {
    // BEGIN INSERT ORDERS
    for (int i = 0; i < orders.size(); i++) {
      QJsonObject order = orders[i].toObject();
      qint64 orderId = order.value("orderId").toInt();
      QString strOrderId = QString::number(orderId);
      QDateTime dateTime = getDateTime(order.value("orderDate").toString(),
                                       order.value("orderTime").toString());
      // Start fill
      AntiquaCRM::AProviderOrder item(providerName, strOrderId);
      item.setValue("o_provider_order_id", strOrderId);
      item.setValue("o_provider_purchase_id", orderId);
      item.setValue("o_since", dateTime);
      item.setValue("o_media_type", AntiquaCRM::BOOK);
      // AntiquaCRM::PaymentStatus
      QString orderStatus = order.value("status").toString();
      if (orderStatus == "READY_FOR_SHIPMENT") {
        item.setValue("o_provider_order_status", AntiquaCRM::SHIPMENT_CREATED);
      } else if (orderStatus == "WAITING_FOR_PAYMENT") {
        item.setValue("o_provider_order_status", AntiquaCRM::WAIT_FOR_PAYMENT);
      } else if (orderStatus == "ORDER_CANCEL_ACTION") {
        item.setValue("o_provider_order_status", AntiquaCRM::ORDER_CANCELED);
      } else {
        item.setValue("o_provider_order_status", AntiquaCRM::STATUS_NOT_SET);
      }
      // AntiquaCRM::PaymentMethod
      AntiquaCRM::PaymentMethod payment_method;
      switch (order.value("paymentId").toInt()) {
      case 1: // Banküberweisung (Vorkasse)
        payment_method = AntiquaCRM::BANK_PREPAYMENT;
        break;

      case 2: // Offene Rechnung
        payment_method = AntiquaCRM::DELIVER_WITH_INVOICE;
        break;

      case 3: // Lastschrift (Vorkasse)
        payment_method = AntiquaCRM::DIRECT_DEBIT_PREPAYMENT;
        break;

      case 4: // Kreditkarte (Vorkasse)
        payment_method = AntiquaCRM::CREDIT_CARD_PREPAYMENT;
        break;

      case 5: // Nachnahme
        payment_method = AntiquaCRM::PAYMENT_NOT_SET;
        break;

      case 6: // PayPal (Vorkasse)
        payment_method = AntiquaCRM::PAYPAL_PREPAYMENT;
        break;

      case 8: // Skrill (Vorkasse)
        payment_method = AntiquaCRM::SKRILL_PREPAYMENT;
        break;

      case 9: // Selbstabholung und Barzahlung
        payment_method = AntiquaCRM::PAYMENT_NOT_SET;
        break;

      case 10: // Sofortüberweisung
        payment_method = AntiquaCRM::PAYMENT_NOT_SET;
        break;

      case 11: // Offene Rechnung (Vorkasse vorbehalten)
        payment_method = AntiquaCRM::INVOICE_PREPAYMENT_RESERVED;
        break;

      default:
        payment_method = AntiquaCRM::PAYMENT_NOT_SET;
      }
      item.setValue("o_payment_method", payment_method);

      // Delivery Cost
      if (order.contains("calculatedShippingCost")) {
        QString d_cost = order.value("calculatedShippingCost").toString();
        double d_coast_double = d_cost.toDouble();
        if (d_coast_double > 0) {
          item.setValue("d_price", d_coast_double);
        }
      }

      // Payment Confirmed
      if (order.contains("paymentConfirmed")) {
        QString date_time = order.value("paymentConfirmed").toString();
        item.setValue("o_payment_confirmed", getDateTime(date_time));
      }

      // PayPal TransactionId
      if (order.contains("transactionId")) {
        QString paypal_txn = order.value("transactionId").toString();
        item.setValue("o_payment_paypal_txn_id", paypal_txn);
      }

      // Invoice Address
      if (!order.value("invoiceAddress").toObject().isEmpty()) {
        QJsonObject address = order.value("invoiceAddress").toObject();
        item.setValue("c_location", address.value("city").toString());
        item.setValue("c_country", address.value("country").toString());
        item.setValue("c_firstname", address.value("firstName").toString());
        item.setValue("c_lastname", address.value("name").toString());
        item.setValue("c_street", address.value("street").toString());
        item.setValue("c_gender", address.value("title").toString());
        item.setValue("c_postalcode", address.value("zip").toString());
      }

      if (order.contains("email"))
        item.setValue("c_email_0", order.value("email").toString());

      if (order.contains("tel"))
        item.setValue("c_phone_0", order.value("tel").toString());

      if (order.contains("company"))
        item.setValue("c_company_name", order.value("company").toString());

      if (order.contains("accountHolder"))
        item.setValue("c_comments", order.value("accountHolder").toString());

      if (order.contains("accountIban"))
        item.setValue("c_iban", order.value("accountIban").toString());

      if (order.contains("accountBic"))
        item.setValue("c_swift_bic", order.value("accountBic").toString());

      if (order.contains("ustIdNr"))
        item.setValue("c_tax_id", order.value("ustIdNr").toString());

      // Delivery Address Body
      if (!order.value("deliveryAddress").toObject().isEmpty()) {
        QJsonObject address = order.value("deliveryAddress").toObject();
        QString firstname = address.value("firstName").toString();
        QString lastname = address.value("name").toString();
        QString postalcode = address.value("zip").toString();
        QString location = address.value("city").toString();
        QString country = address.value("country").toString();
        QString state =
            AntiquaCRM::AEuropeanCountries().value(country.toUpper());
        if (!state.isEmpty())
          location.append("/" + state);

        QString street = address.value("street").toString();
        if (address.contains("addressSupplement")) {
          QString pkgNumber = address.value("addressSupplement").toString();
          street.prepend(pkgNumber + " ");
        }
        QStringList buffer(firstname + " " + lastname);
        buffer << street << postalcode + " " + location;
        item.setValue("c_shipping_address", buffer.join("\n"));
      }
      // Article Orders
      QJsonArray articles = order.value("orderItems").toArray();
      for (int a = 0; a < articles.count(); a++) {
        if (articles[a].type() == QJsonValue::Object) {
          AntiquaCRM::AProviderOrderItems booking;
          QJsonObject itemData = articles[a].toObject();
          foreach (QString k, itemData.keys()) {
            if (!p_articleTranslate.value(k).isEmpty()) {
              booking.append(articleItem(k, itemData.value(k)));
            }
          }
          item.insertOrderItems(booking);
        }
      }
      allOrders.append(item);
    }
    // END INSERT ORDERS
  }
  //#ifdef ANTIQUA_DEVELOPEMENT
  //  QListIterator<AntiquaCRM::AProviderOrder> get_orders(allOrders);
  //  while (get_orders.hasNext()) {
  //    AntiquaCRM::AProviderOrder data = get_orders.next();
  //    qDebug() << data.provider() << data.id() << data.orders().size();
  //  }
  //#endif
  return allOrders;
}

bool Booklooker::createInterface(QObject *parent) { return true; }
