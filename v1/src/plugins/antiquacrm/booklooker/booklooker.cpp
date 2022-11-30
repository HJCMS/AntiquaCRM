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
#include <QDialog>
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
  // Provider Article Order Payment Id
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
  // Interanal payments id
  hash.insert("payment_id", "a_payment_id");
  return hash;
}

Booklooker::Booklooker(QObject *parent)
    : AntiquaCRM::APluginInterface{parent}, p_articleTranslate{translateIds()} {
  setObjectName(BOOKLOOKER_CONFIG_PROVIDER);
  m_network = nullptr;
}

bool Booklooker::initConfigurations() {
  QUrl url;
  AntiquaCRM::ASettings cfg(this);
  cfg.beginGroup(BOOKLOOKER_CONFIG_GROUP);
  url.setScheme("https");
  url.setHost(cfg.value("api_host", "api.booklooker.de").toString());
  apiKey = cfg.value("api_key", QString()).toString();
  cfg.endGroup();
  apiUrl = url;
  return true;
}

const QUrl Booklooker::apiQuery(const QString &section) {
  QString p("/");
  p.append(BOOKLOOKER_API_VERSION);
  p.append("/");
  p.append(section);

  QUrl url(apiUrl);
  url.setPath(p);

  return url;
}

const QString Booklooker::dateString(const QDate &date) const {
  return date.toString(BOOKLOOKER_DATE_FORMAT);
}

const QVariant Booklooker::createValue(QMetaType::Type id,
                                       const QJsonValue &value) const {
  switch (id) {
  case QMetaType::Bool:
    return value.toBool();

  case QMetaType::Int:
  case QMetaType::Long:
  case QMetaType::LongLong:
    return value.toInt();

  case QMetaType::QDateTime: {
    QString str = value.toString();
    return QDateTime::fromString(str, BOOKLOOKER_DATE_FORMAT);
  } break;

  case QMetaType::Double:
    return value.toDouble();

  default:
    return value.toString();
  }
}

void Booklooker::setOrderItem(AntiquaCRM::AProviderOrder *order,
                              const QString &key,
                              const QJsonValue &value) const {
  QHashIterator<QString, QMetaType::Type> it(order->orderEditKeys());
  while (it.hasNext()) {
    it.next();
    if (it.key() == key) {
      order->setValue(it.key(), createValue(it.value(), value));
      break;
    }
  }
}

const AntiquaCRM::ArticleOrderItem
Booklooker::setArticleItem(AntiquaCRM::AProviderOrder *order,
                           const QString &key, const QJsonValue &value) const {
  AntiquaCRM::ArticleOrderItem item;
  QString crm_key = p_articleTranslate.value(key);
  if (crm_key.isEmpty())
    qWarning("Booklooker - Unknown ArticleKey '%s'!", qPrintable(key));

  QHashIterator<QString, QMetaType::Type> it(order->articleKeys());
  while (it.hasNext()) {
    it.next();
    if (it.key() == crm_key) {
      item.key = crm_key;
      if (crm_key.contains("price")) {
        item.value = getPrice(value);
      } else if (crm_key == "a_article_id") {
        QString str = value.toString();
        bool b;
        qint64 aid = str.toLongLong(&b);
        if (b) {
          item.value = aid;
        } else {
          qWarning("Can't convert Article Id from Booklooker order!");
          item.value = 0;
        }
      } else if (crm_key == "a_provider_id") {
        // Dienstleister Bestellnummer
        if (value.type() == QJsonValue::Double) {
          qint64 d = value.toInt();
          item.value = QString::number(d);
        } else {
          item.value = value.toString();
        }
      } else if (crm_key.contains("_type")) {
        int t = value.toInt();
        if (t == 0)
          item.value = AntiquaCRM::ArticleType::BOOK;
        else if (t > 0 && t < 4)
          item.value = AntiquaCRM::ArticleType::MEDIA;
        else
          item.value = AntiquaCRM::ArticleType::UNKNOWN;
      } else if (crm_key == "a_payment_id") {
        // Interne Bestellnummer kann hier nicht gesetzt werden!
        item.value = 0;
      } else {
        item.value = createValue(it.value(), value);
      }
      break;
    }
  }
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

void Booklooker::queryNewOrders() {
  if (isCookieExpired()) {
    authenticate();
    return;
  }

  QUrl url = apiQuery("order");
  QString value = configProvider();
  value.append("_orders.json");
  actionsCookie = QNetworkCookie("action", value.toLocal8Bit());
  actionsCookie.setDomain(url.host());
  actionsCookie.setSecure(true);

  QDate past = QDate::currentDate().addDays(ANTIQUACRM_QUERY_PASTDAYS);
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
  QString value = configProvider();
  value.append("_order_" + orderId + ".json");
  actionsCookie = QNetworkCookie("action", value.toLocal8Bit());
  actionsCookie.setDomain(url.host());
  actionsCookie.setSecure(true);

  QUrlQuery q;
  q.addQueryItem("token", QString(authenticCookie.value()));
  q.addQueryItem("orderId", orderId);
  url.setQuery(q);
  m_network->getRequest(url);
}

void Booklooker::orderUpdateAction(const QJsonObject &options) {
  if (!options.contains("provider")) {
    qWarning("Invalid caller for Booklooker::postOperation!");
    return;
  }
  // Prüfen ob es für ihn ist!
  if (options.value("provider").toString() != configProvider())
    return;

  if (isCookieExpired())
    authenticate();

  QJsonObject action = options.value("plugin_operation").toObject();
  QString orderId = action.value("orderid").toString();

  if (action.contains("paymentstatus")) {
    // BEGIN PaymentStatus
    QString query_action;
    QUrlQuery query;
    query.addQueryItem("token", QString(authenticCookie.value()));
    query.addQueryItem("orderId", orderId);

    int status = action.value("paymentstatus").toInt();
    switch (static_cast<AntiquaCRM::PaymentStatus>(status)) {
    case AntiquaCRM::PaymentStatus::WAIT_FOR_PAYMENT:
      query_action = "order_status";
      query.addQueryItem("status", "WAITING_FOR_PAYMENT");
      break;

    case AntiquaCRM::PaymentStatus::SHIPMENT_CREATED:
      query_action = "order_status";
      query.addQueryItem("status", "READY_FOR_SHIPMENT");
      break;

    case AntiquaCRM::PaymentStatus::SHIPPED_WAIT_FOR_PAYMENT:
      query_action = "order_status";
      query.addQueryItem("status", "SHIPPED_WAITING_FOR_PAYMENT");
      break;

    case AntiquaCRM::PaymentStatus::SHIPPED_AND_PAID:
      query_action = "order_status";
      query.addQueryItem("status", "SHIPPED_AND_PAID");
      break;

    case AntiquaCRM::PaymentStatus::BUYER_NO_REACTION:
      query_action = "order_status";
      query.addQueryItem("status", "BUYER_NO_REACTION");
      break;

    case AntiquaCRM::PaymentStatus::ORDER_CANCELED:
      query_action = "order_cancel";
      break;

    default:
      break;
    };

    QUrl url = apiQuery(query_action);
    if (!query.isEmpty())
      url.setQuery(query);

#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << Qt::endl << "Abgeschaltet:" << url.toString();
#else
    m_network->putRequest(url, QByteArray());
#endif
  } // END PaymentStatus
}

bool Booklooker::authenticationRequired() { return isCookieExpired(); }

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
  QString data = cacheFile.getTempFile(fileName.toLower());
  /*
   * QString data = cacheFile.getTempFile("booklooker_order_test.json");
   * qWarning("Booklooker in Test Mode!");
   */
  if (data.isEmpty())
    return allOrders;

  QString display_name = displayName().toLower();
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
      AntiquaCRM::AProviderOrder item(display_name, strOrderId);
      item.setValue("o_provider_name", displayName());
      item.setValue("o_provider_order_id", strOrderId);
      item.setValue("o_provider_purchase_id", orderId);
      item.setValue("o_since", dateTime);
      item.setValue("o_media_type", AntiquaCRM::BOOK);

      /*
       * @brief Konvertiere Provider PaymentStatus => OrderStatus!
       * Wir verwenden im Auftragssystem nur den OrderStatus!
       * Der PaymentStatus ist eine reine Dienstleistergeschichte!
       */
      QString orderStatus = order.value("status").toString();
      if (orderStatus == "READY_FOR_SHIPMENT") {
        // AntiquaCRM::SHIPMENT_CREATED => AntiquaCRM::STARTED
        item.setValue("o_order_status", AntiquaCRM::OrderStatus::STARTED);
      } else if (orderStatus == "ORDER_CANCEL_ACTION") {
        // AntiquaCRM::ORDER_CANCELED => AntiquaCRM::CANCELED
        item.setValue("o_order_status", AntiquaCRM::OrderStatus::CANCELED);
      } else {
        // AntiquaCRM::STATUS_NOT_SET => AntiquaCRM::OPEN
        item.setValue("o_order_status", AntiquaCRM::OrderStatus::OPEN);
      }

      // AntiquaCRM::PaymentMethod
      AntiquaCRM::PaymentMethod payment_method;
      payment_method = AntiquaCRM::PAYMENT_NOT_SET;
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
          item.setValue("o_delivery_add_price", true);
        }
      } else {
        item.setValue("o_delivery_add_price", false);
      }

      // Buyer payment comment
      if (order.contains("comments")) {
        item.setValue("o_delivery_comment", order.value("comments").toString());
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
        item.setValue("o_payment_status", true);
      } else {
        item.setValue("o_payment_status", false);
      }

      // Invoice Address
      if (!order.value("invoiceAddress").toObject().isEmpty()) {
        QJsonObject address = order.value("invoiceAddress").toObject();
        QString firstname = address.value("firstName").toString();
        QString lastname = address.value("name").toString();
        QString street = address.value("street").toString();
        QString postalcode = address.value("zip").toString();
        QString location = address.value("city").toString();
        QString bcp47 = address.value("country").toString();
        QString gender = address.value("title").toString();
        item.setValue("c_gender", convertGender(gender));
        item.setValue("c_firstname", AntiquaCRM::AUtil::ucFirst(firstname));
        item.setValue("c_lastname", AntiquaCRM::AUtil::ucFirst(lastname));
        item.setValue("c_street", street);
        item.setValue("c_postalcode", postalcode);
        item.setValue("c_location", AntiquaCRM::AUtil::ucFirst(location));
        item.setValue("o_vat_country", bcp47);
        QString state = AntiquaCRM::AEuropeanCountries().value(bcp47.toUpper());
        if (!state.isEmpty()) {
          item.setValue("c_country", state);
        }
        item.setValue("c_provider_import", (firstname + " " + lastname));
        QStringList postalAddress;
        postalAddress << firstname + " " + lastname;
        postalAddress << street;
        postalAddress << postalcode + " " + location;
        item.setValue("c_postal_address", postalAddress.join("\n"));
      }

      if (order.contains("email"))
        item.setValue("c_email_0", order.value("email").toString());

      if (order.contains("tel")) {
        QString phone = order.value("tel").toString();
        item.setValue("c_phone_0", phone.replace("+", "0"));
      }

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
          AntiquaCRM::OrderArticleItems booking;
          QJsonObject itemData = articles[a].toObject();
          foreach (QString k, itemData.keys()) {
            if (!p_articleTranslate.value(k).isEmpty()) {
              booking.append(setArticleItem(&item, k, itemData.value(k)));
            }
          }
          if (booking.size() > 0) {
            booking.append(setArticleItem(&item, "payment_id", 0));
            item.insertOrderItems(booking);
          }
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

bool Booklooker::createInterface(QObject *parent) {
  Q_UNUSED(parent);

  m_network = new AntiquaCRM::ANetworker(AntiquaCRM::JSON_QUERY, this);
  connect(m_network, SIGNAL(sendJsonResponse(const QJsonDocument &)),
          SLOT(prepareResponse(const QJsonDocument &)));
  connect(m_network, SIGNAL(sendContentCodec(QTextCodec *)),
          SLOT(setContentDecoder(QTextCodec *)));
  connect(m_network, SIGNAL(finished(QNetworkReply *)),
          SLOT(queryFinished(QNetworkReply *)));

  return initConfigurations();
}
