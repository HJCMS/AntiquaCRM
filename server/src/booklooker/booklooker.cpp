// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklooker.h"
#include "networker.h"
#include "networkrequest.h"
#include "settings.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonValue>
#include <QNetworkReply>
#include <QUrlQuery>

BookLooker::BookLooker(QObject *parent) : Provider{parent} {
  connect(m_networker, SIGNAL(sendResponse(const QByteArray &)),
          SLOT(responsed(const QByteArray &)));
}

void BookLooker::initConfiguration() {
  m_config->beginGroup("provider/booklooker");
  baseUrl.setScheme("https");
  baseUrl.setHost(m_config->value("api_host", "api.booklooker.de").toString());
  apiKey = m_config->value("api_key", QString()).toString();
  apiPath = QString("/2.0/");
  m_config->endGroup();
}

const QString BookLooker::dateString(const QDate &date) const {
  return date.toString("yyyy-MM-dd");
}

void BookLooker::prepareContent(const QJsonDocument &doc) {
  QJsonObject obj = doc.object();
  if (obj.value("status").toString().toUpper() != "OK") {
    qWarning("Booklooker Document not OK!");
    return;
  }

  QStringList imported = currentOrderIds("Booklooker");
  QJsonArray orders = obj.value("returnValue").toArray();
  // Starte durchlauf Bestellungen
  if (orders.size() > 0) {
    QList<QJsonObject> ordersList;
    // BEGIN::QUERY_ORDERS
    for (int i = 0; i < orders.size(); i++) {
      QJsonObject order = orders[i].toObject();
      qint64 order_id = order.value("orderId").toInt();
      QString order_str = QString::number(order_id);
      if (imported.contains(order_str)) {
        continue;
      }

      QDateTime datetime = getDateTime(order.value("orderDate").toString(),
                                       order.value("orderTime").toString());
      // BEGIN::CONVERT
      QJsonObject antiqua_order; // Bestellkopfdaten
      antiqua_order.insert("provider", QJsonValue(providerName));
      antiqua_order.insert("orderid", QJsonValue(order_str));

      // Kundendaten
      QJsonObject antiqua_customer;
      QString bcp47 = "DE"; // Siehe orderinfo
      if (!order.value("invoiceAddress").toObject().isEmpty()) {
        QJsonObject address = order.value("invoiceAddress").toObject();
        QString firstname = address.value("firstName").toString();
        QString lastname = address.value("name").toString();
        QString street = address.value("street").toString();
        QString postalcode = address.value("zip").toString();
        QString location = address.value("city").toString();
        bcp47 = address.value("country").toString().toUpper();
        QString gender = address.value("title").toString();
        antiqua_customer.insert("c_gender", convertGender(gender));
        antiqua_customer.insert("c_firstname", ucFirst(firstname));
        antiqua_customer.insert("c_lastname", ucFirst(lastname));
        antiqua_customer.insert("c_street", street);
        antiqua_customer.insert("c_postalcode", postalcode);
        antiqua_customer.insert("c_location", ucFirst(location));
        antiqua_customer.insert("c_country_bcp47", bcp47);
        antiqua_customer.insert("c_country", getCountry(bcp47));

        QStringList postalAddress;
        postalAddress << firstname + " " + lastname;
        postalAddress << street;
        postalAddress << postalcode + " " + location;
        antiqua_customer.insert("c_postal_address", postalAddress.join("\n"));
        antiqua_customer.insert("c_provider_import",
                                firstname + " " + lastname);

        if (order.contains("email")) {
          antiqua_customer.insert("c_email_0", order.value("email").toString());
        }

        if (order.contains("tel")) {
          QString phone = order.value("tel").toString();
          phone.replace("+", "0");
          phone.replace(QRegExp("^0{2,}"), "0");
          antiqua_customer.insert("c_phone_0", phone);
        }

        if (order.contains("company")) {
          antiqua_customer.insert("c_company_name",
                                  order.value("company").toString());
        }

        if (order.contains("accountHolder")) {
          antiqua_customer.insert("c_comments",
                                  order.value("accountHolder").toString());
        }

        if (order.contains("accountIban")) {
          antiqua_customer.insert("c_iban",
                                  order.value("accountIban").toString());
        }

        if (order.contains("accountBic")) {
          antiqua_customer.insert("c_swift_bic",
                                  order.value("accountBic").toString());
        }

        if (order.contains("ustIdNr")) {
          antiqua_customer.insert("c_tax_id",
                                  order.value("ustIdNr").toString());
        }

        // Delivery Address Body
        if (!order.value("deliveryAddress").toObject().isEmpty()) {
          QJsonObject address = order.value("deliveryAddress").toObject();
          QString firstname = address.value("firstName").toString();
          QString lastname = address.value("name").toString();
          QString postalcode = address.value("zip").toString();
          QString location = address.value("city").toString();
          QString country = address.value("country").toString();
          QString state = country.toUpper();
          if (!state.isEmpty())
            location.append("/" + state);

          QString street = address.value("street").toString();
          if (address.contains("addressSupplement")) {
            QString pkgNumber = address.value("addressSupplement").toString();
            street.prepend(pkgNumber + " ");
          }
          QStringList buffer(firstname + " " + lastname);
          buffer << street << postalcode + " " + location;
          antiqua_customer.insert("c_shipping_address", buffer.join("\n"));
        }
      }
      antiqua_order.insert("customer", QJsonValue(antiqua_customer));

      // Bestellinfos
      QJsonObject antiqua_orderinfo;
      antiqua_orderinfo.insert("o_provider_name", "Booklooker");
      antiqua_orderinfo.insert("o_provider_order_id", order_str);
      antiqua_orderinfo.insert("o_provider_purchase_id", order_id);
      antiqua_orderinfo.insert("o_since", datetime.toString(Qt::ISODate));
      antiqua_orderinfo.insert("o_media_type", AntiquaCRM::BOOK);
      antiqua_orderinfo.insert("o_vat_country", bcp47);

      /**
       * @brief Konvertieren Provider PaymentStatus => OrderStatus!
       * Wir verwenden im Auftragssystem nur den OrderStatus!
       */
      QString orderStatus = order.value("status").toString();
      if (orderStatus == "READY_FOR_SHIPMENT") {
        antiqua_orderinfo.insert("o_order_status",
                                 AntiquaCRM::OrderStatus::STARTED);
      } else if (orderStatus == "ORDER_CANCEL_ACTION") {
        antiqua_orderinfo.insert("o_order_status",
                                 AntiquaCRM::OrderStatus::CANCELED);
      } else {
        antiqua_orderinfo.insert("o_order_status",
                                 AntiquaCRM::OrderStatus::OPEN);
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
      antiqua_orderinfo.insert("o_payment_method", payment_method);

      // Delivery Cost
      if (order.contains("calculatedShippingCost")) {
        QString d_cost = order.value("calculatedShippingCost").toString();
        double d_coast_double = d_cost.toDouble();
        if (d_coast_double > 0) {
          antiqua_orderinfo.insert("o_delivery_add_price", true);
        }
      } else {
        antiqua_orderinfo.insert("o_delivery_add_price", false);
      }

      // Buyer payment comment
      if (order.contains("comments")) {
        antiqua_orderinfo.insert("o_delivery_comment",
                                 order.value("comments").toString());
      }

      // Payment Confirmed
      if (order.contains("paymentConfirmed")) {
        QString date_time = order.value("paymentConfirmed").toString();
        QDateTime payment_time = getDateTime(date_time);
        antiqua_orderinfo.insert("o_payment_confirmed",
                                 payment_time.toString(Qt::ISODate));
      }

      // PayPal TransactionId
      if (order.contains("transactionId")) {
        QString paypal_txn = order.value("transactionId").toString();
        antiqua_orderinfo.insert("o_payment_paypal_txn_id", paypal_txn);
        antiqua_orderinfo.insert("o_payment_status", true);
      } else {
        antiqua_orderinfo.insert("o_payment_status", false);
      }

      antiqua_order.insert("orderinfo", QJsonValue(antiqua_orderinfo));

      // Artikelliste
      QJsonArray antiqua_articles;
      QJsonArray articles = order.value("orderItems").toArray();
      for (int a = 0; a < articles.count(); a++) {
        if (articles[a].type() == QJsonValue::Object) {
          QJsonObject booking;
          QJsonObject sub = articles[a].toObject();
          booking.insert("a_payment_id", 0);
          booking.insert("a_count", sub.value("amount"));
          booking.insert("a_article_id",
                         convertArticleId(sub.value("orderNo")));
          qint64 prItemId = sub.value("orderItemId").toInt();
          booking.insert("a_provider_id", QString::number(prItemId));
          booking.insert("a_price", getPrice(sub.value("singlePrice")));
          booking.insert("a_sell_price",
                         getPrice(sub.value("totalPriceRebated")));
          booking.insert("a_title", sub.value("orderTitle"));
          booking.insert("a_type", AntiquaCRM::BOOK);
          antiqua_articles.append(booking);
        }
      }
      antiqua_order.insert("articles", QJsonValue(antiqua_articles));

      ordersList.append(antiqua_order);
      // END::CONVERT
    }
    // END::QUERY_ORDERS

    if (createOrders(ordersList)) {
      emit sendFinished();
      return;
    }
  }
  emit sendDisjointed();
}

void BookLooker::setTokenCookie(const QString &token) {
  QDateTime dt = QDateTime::currentDateTime();
  dt.setTimeSpec(Qt::UTC);
  qint64 cookie_lifetime = (9 * 60);
  authenticCookie = QNetworkCookie("token", token.toLocal8Bit());
  authenticCookie.setDomain(baseUrl.host());
  authenticCookie.setSecure(true);
  authenticCookie.setExpirationDate(dt.addSecs(cookie_lifetime));
  if (!authenticCookie.value().isNull()) {
    qInfo("New Token add (%s)", qPrintable(authenticCookie.value()));
    queryOrders();
  }
}

bool BookLooker::isCookieExpired() {
  if (authenticCookie.value().isNull())
    return true;

  QDateTime dt = QDateTime::currentDateTime();
  dt.setTimeSpec(Qt::UTC);
  return (authenticCookie.expirationDate() <= dt);
}

void BookLooker::authenticate() {
  QUrl url(baseUrl);
  url.setPath("/2.0/authenticate");

  QString pd("apiKey=");
  pd.append(apiKey);

  actionsCookie = QNetworkCookie("action", "authenticate");
  actionsCookie.setDomain(url.host());
  actionsCookie.setSecure(true);

  NetworkRequest request(url);
  m_networker->loginRequest(request, pd.toLocal8Bit());
}

void BookLooker::responsed(const QByteArray &data) {
  QJsonParseError parser;
  QJsonDocument doc = QJsonDocument::fromJson(data, &parser);
  if (parser.error != QJsonParseError::NoError) {
#ifdef ANTIQUA_DEVELOPEMENT
    qWarning("Booklooker::ParseError:(%s)!", qPrintable(parser.errorString()));
#endif
    emit sendDisjointed();
    return;
  }

  if (actionsCookie.value().contains("authenticate")) {
    QJsonObject obj = doc.object();
    if (obj.value("status").toString().toLower() == "ok") {
      QString token = obj.value("returnValue").toString();
      setTokenCookie(token);
    }
    return;
  }

  prepareContent(doc);
}

void BookLooker::queryOrders() {
  if (isCookieExpired()) {
    authenticate();
    return;
  }

  QUrl url(baseUrl);
  url.setPath("/2.0/order");

  QString value("booklooker_orders");
  actionsCookie = QNetworkCookie("action", value.toLocal8Bit());
  actionsCookie.setDomain(url.host());
  actionsCookie.setSecure(true);

  QDate past = QDate::currentDate().addDays(-4);
  QUrlQuery q;
  q.addQueryItem("token", QString(authenticCookie.value()));
  q.addQueryItem("dateFrom", dateString(past));
  q.addQueryItem("dateTo", dateString());
  url.setQuery(q);

  NetworkRequest request(url);
  request.setHeaderContentTypeJson();

  m_networker->getRequest(request);
}

bool BookLooker::isAccessible() {
  initConfiguration();

  if (baseUrl.isEmpty())
    return false;

  if (apiKey.isEmpty())
    return false;

  return true;
}
