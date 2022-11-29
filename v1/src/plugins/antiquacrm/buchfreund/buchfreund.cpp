// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "buchfreund.h"

/**
 * @def DATE_FORMAT
 * @ingroup Providers SQL Statements
 */
#ifndef BUCHFREUND_DATE_FORMAT
#define BUCHFREUND_DATE_FORMAT "yyyy-MM-dd hh:mm:ss"
#endif

/** @brief Konfigurationsgruppe */
#ifndef BUCHFREUND_CONFIG_GROUP
#define BUCHFREUND_CONFIG_GROUP "provider/buchfreund"
#endif

/** @brief Wird für Menüeintrag und Gruppenzuweisung benötigt! */
#ifndef BUCHFREUND_CONFIG_PROVIDER
#define BUCHFREUND_CONFIG_PROVIDER "Buchfreund"
#endif

// Headers
#include <AntiquaCRM>
#include <QDateTime>
#include <QDebug>
#include <QDialog>
#include <QJsonArray>
#include <QTimeZone>
#include <QUrlQuery>

/**
 * @brief Translate Article Ids
 */
static const QHash<QString, QString> translateIds() {
  QHash<QString, QString> hash;
  hash.insert("adresse", "c_street");
  hash.insert("anrede", "c_gender");
  hash.insert("bestellnr", "a_article_id");
  hash.insert("einlieferungsdatum", "o_delivery_fullfill");
  hash.insert("email", "c_email_0");
  hash.insert("id", "a_provider_id");
  hash.insert("kundenkommentar", "o_delivery_comment");
  hash.insert("land", "c_country_name");
  hash.insert("land_iso2", "c_country_bcp47");
  hash.insert("menge_bestellt", "a_count");
  hash.insert("name", "c_lastname");
  hash.insert("ort", "c_location");
  hash.insert("payment_id", "a_payment_id");
  hash.insert("person", "a_customer_id");
  hash.insert("plz", "c_postalcode");
  hash.insert("preis_pro_einheit", "a_price");
  hash.insert("telefon", "c_phone_0");
  hash.insert("titel", "a_title");
  hash.insert("versandkosten", "o_delivery_cost");
  hash.insert("versender", "o_delivery_name");
  hash.insert("vorname", "c_firstname");
  hash.insert("waehrung", "o_payment_currency");
  hash.insert("zusatz", "c_title");
  return hash;
}

Buchfreund::Buchfreund(QObject *parent)
    : AntiquaCRM::APluginInterface{parent}, p_articleTranslate{translateIds()} {
  setObjectName(BUCHFREUND_CONFIG_PROVIDER);
  m_network = nullptr;
}

bool Buchfreund::initConfigurations() {
  QUrl url;
  QString apiHost("www.buchfreund.de");
  AntiquaCRM::ASettings cfg(this);
  cfg.beginGroup(BUCHFREUND_CONFIG_GROUP);
  url.setScheme("https");
  url.setHost(cfg.value("api_host", apiHost).toString());
  apiKey = cfg.value("api_key", QString()).toString();
  cfg.endGroup();
  apiUrl = url;
  return true;
}

const QUrl Buchfreund::apiQuery(const QString &section) {
  QString p("/verkaeufer/api/");
  p.append(apiKey);
  p.append("/");
  p.append(section);

  QUrl url(apiUrl);
  url.setPath(p);

  return url;
}

const QString Buchfreund::dateString(const QDate &date) const {
  return date.toString(BUCHFREUND_DATE_FORMAT);
}

const QVariant Buchfreund::createValue(QMetaType::Type id,
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
    return QDateTime::fromString(str, BUCHFREUND_DATE_FORMAT);
  } break;

  case QMetaType::Double:
    return value.toDouble();

  default:
    return value.toString();
  }
}

void Buchfreund::setOrderItem(AntiquaCRM::AProviderOrder *order,
                              const QString &key, // param
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
Buchfreund::setArticleItem(AntiquaCRM::AProviderOrder *order,
                           const QString &key, const QJsonValue &value) const {
  AntiquaCRM::ArticleOrderItem item;
  QHashIterator<QString, QMetaType::Type> it(order->articleKeys());
  while (it.hasNext()) {
    it.next();
    if (it.key() == key) {
      item.key = key;
      if (key.contains("price")) {
        item.value = getPrice(value);
      } else if (key == "a_article_id") {
        QString str = value.toString();
        bool b;
        qint64 aid = str.toLongLong(&b);
        if (b) {
          item.value = aid;
        } else {
          qWarning("Can't convert Article Id from Buchfreund order!");
          item.value = 0;
        }
      } else
        item.value = createValue(it.value(), value);

      break;
    }
  }
  return item;
}

void Buchfreund::prepareResponse(const QJsonDocument &js) {
  QString fileName = configProvider();
  if (!actionsCookie.name().isNull()) {
    fileName = QString(actionsCookie.value());
  }

  if (actionsCookie.value().contains("_bestellungen")) {
    QJsonObject obj = js.object();
    if (obj.value("error").toBool()) {
      emit sendQueryAborted();
      return;
    }
    QJsonArray array = obj.value("response").toArray();
    for (int i = 0; i < array.size(); i++) {
      QString id = array[i].toObject().value("id").toString();
      queryOrder(id);
    }
  } else if (actionsCookie.value().contains("_bestellung")) {
    AntiquaCRM::ASharedCacheFiles cacheFile;
    QString jsonData = js.toJson(QJsonDocument::Indented);
    cacheFile.storeTempFile(fileName.toLower(), jsonData);
    emit sendQueryFinished();
  }
}

void Buchfreund::queryFinished(QNetworkReply *reply) {
  if (reply->error() != QNetworkReply::NoError) {
    emit sendErrorResponse(AntiquaCRM::WARNING,
                           tr("Buchfreund response with errors!"));
  }
}

void Buchfreund::queryNewOrders() {
  QDateTime curr = QDateTime::currentDateTime();
  QDateTime past = curr.addDays(ANTIQUACRM_QUERY_PASTDAYS);
  QJsonObject obj;
  obj.insert("datum_von", past.toString(BUCHFREUND_DATE_FORMAT));
  obj.insert("datum_bis", curr.toString(BUCHFREUND_DATE_FORMAT));

  QUrl url = apiQuery("bestellungen");
  QString value = configProvider();
  value.append("_bestellungen");
  value.append(".json");
  actionsCookie = QNetworkCookie("action", value.toLocal8Bit());
  actionsCookie.setDomain(url.host());
  actionsCookie.setSecure(true);
  m_network->jsonPostRequest(url, QJsonDocument(obj));
}

void Buchfreund::queryOrder(const QString &orderId) {
  QJsonObject obj;
  obj.insert("id", QJsonValue(orderId));

  QUrl url = apiQuery("bestellung");
  QString value = configProvider();
  value.append("_bestellung_");
  value.append(orderId);
  value.append(".json");
  actionsCookie = QNetworkCookie("action", value.toLocal8Bit());
  actionsCookie.setDomain(url.host());
  actionsCookie.setSecure(true);
  m_network->jsonPostRequest(url, QJsonDocument(obj));
}

void Buchfreund::orderUpdateAction(const QJsonObject &options) {
  Q_UNUSED(options);
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << Qt::endl << options;
#endif
}

bool Buchfreund::authenticationRequired() { return false; }

const QString Buchfreund::configProvider() const {
  return QString(BUCHFREUND_CONFIG_PROVIDER).toLower();
}

const QString Buchfreund::displayName() const {
  return QString(BUCHFREUND_CONFIG_PROVIDER).trimmed();
}

const AntiquaCRM::AProviderOrders Buchfreund::getOrders() const {
  QString fileName = configProvider();
  if (!actionsCookie.name().isNull()) {
    fileName = QString(actionsCookie.value());
  }
  AntiquaCRM::AProviderOrders allOrders;
  QString display_name = displayName().toLower();
  AntiquaCRM::ASharedCacheFiles cacheFile;
  QString data = cacheFile.getTempFile(fileName.toLower());
  // QString data = cacheFile.getTempFile(display_name + "_test.json");
  // qInfo("Buchfreund ist im Test Modus!");

  QJsonDocument doc = QJsonDocument::fromJson(data.toLocal8Bit());
  QString index("response");
  QJsonObject buchfreund = doc.object();
  bool errors = buchfreund.value("error").toBool();
  if (errors) {
    qWarning("Buchfreund: Response with errors!");
  }

  // Rückgabe auf Typ prüfen
  QJsonValue response = buchfreund.value(index);
  if (response.type() == QJsonValue::String) {
    qInfo("Buchfreund: %s", qPrintable(response.toString()));
    return allOrders;
  }

  // BEGIN:Bestellliste erstellen
  QList<QJsonObject> orders;
  if (response.type() == QJsonValue::Object) {
    // Einzel Bestellung
    orders.append(response.toObject());
  } else if (response.type() == QJsonValue::Array) {
    // Mehrere Bestellungen
    QJsonArray array = response.toArray();
    for (int o = 0; o < array.count(); o++) {
      QJsonObject order = array[o].toObject();
      if (order.size() > 0)
        orders.append(order);
    }
  }
  // END:Bestellliste

  if (orders.size() < 1) // Aussteigen
    return allOrders;

  QListIterator<QJsonObject> payments(orders);
  while (payments.hasNext()) {
    QJsonObject bf_order = payments.next();
    QString bf_id = bf_order.value("id").toString();
    if (bf_id.isEmpty()) {
      // Aussteigen wenn das schon nicht stimmt!
      qWarning("Buchfreund: Invalid order detected!");
      break;
    }

    // Starte Konvertierung
    AntiquaCRM::AProviderOrder order(display_name, bf_id);
    order.setValue("o_provider_name", displayName());
    order.setValue("o_provider_order_id", bf_id);
    order.setValue("o_provider_purchase_id", -1);
    order.setValue("o_since", getDateTime(bf_order.value("datum").toString()));
    order.setValue("o_media_type", AntiquaCRM::BOOK);
    order.setValue("o_order_status", AntiquaCRM::OrderStatus::STARTED);
    order.setValue("o_payment_status",
                   bf_order.value("bestellzeichen").toBool());
    order.setValue("o_delivery_add_price", false);

    QJsonObject::iterator it;
    for (it = bf_order.begin(); it != bf_order.end(); ++it) {
      QString _k = it.key();
      QJsonValue _v = it.value();
      if (_k.isEmpty() || _v.isNull())
        continue;

      if (p_articleTranslate.contains(_k))
        setOrderItem(&order, p_articleTranslate.value(_k), _v);
      else if (_v.isObject() && _k != "lieferadresse") {
        // Warning: Nicht mit "lieferadresse" überschreiben!
        QJsonObject _sub = _v.toObject();
        foreach (QString k, _sub.keys()) {
          setOrderItem(&order, p_articleTranslate.value(k), _sub.value(k));
        }
      }
    }

    // Article Orders
    QJsonArray articles = bf_order.value("positionen").toArray();
    for (int a = 0; a < articles.count(); a++) {
      if (articles[a].type() == QJsonValue::Object) {
        AntiquaCRM::OrderArticleItems bf_article;
        QJsonObject jso = articles[a].toObject();
        foreach (QString k, jso.keys()) {
          if (!p_articleTranslate.value(k).isEmpty()) {
            bf_article.append(setArticleItem(
                &order, p_articleTranslate.value(k), jso.value(k)));
          }
        }
        if (bf_article.size() > 0) {
          setOrderItem(&order, "payment_id", QJsonValue(0));
          bf_article.append(
              order.createItem("a_provider_id", QJsonValue(bf_id)));
          double price = getPrice(jso.value("preis_pro_einheit"));
          bf_article.append(
              order.createItem("a_sell_price", QJsonValue(price)));
          bf_article.append(
              order.createItem("a_type", QJsonValue(AntiquaCRM::BOOK)));
          order.insertOrderItems(bf_article);
        }
      }
    }

    // AntiquaCRM::PaymentMethod
    AntiquaCRM::PaymentMethod payment_method;
    payment_method = AntiquaCRM::PAYMENT_NOT_SET;
    QString bezahlart = bf_order.value("bezahlart").toString();
    if (bezahlart.contains("Banküberweisung"))
      payment_method = AntiquaCRM::BANK_PREPAYMENT;
    else if (bezahlart == "Rechnung") // Offene Rechnung
      payment_method = AntiquaCRM::DELIVER_WITH_INVOICE;
    else if (bezahlart.contains("Kreditkarte"))
      payment_method = AntiquaCRM::CREDIT_CARD_PREPAYMENT;
    else if (bezahlart.contains("PayPal"))
      payment_method = AntiquaCRM::PAYPAL_PREPAYMENT;
    else if (bezahlart.contains("Rechnung"))
      payment_method = AntiquaCRM::INVOICE_PREPAYMENT_RESERVED;
    else
      payment_method = AntiquaCRM::PAYMENT_NOT_SET;

    order.setValue("o_payment_method", payment_method);

    QString vat_str = order.getValue("c_country_bcp47").toString();
    order.setValue("o_vat_country", vat_str.toLower());
    QString state = AntiquaCRM::AEuropeanCountries().value(vat_str.toUpper());
    order.setValue("c_country", state);

    if (bf_order.contains("versandart")) {
      QString versandart = bf_order.value("versandart").toString();
      if (versandart.contains("hermes", Qt::CaseInsensitive))
        order.setValue("o_delivery_service", 1);
      else if (versandart.contains("dhl", Qt::CaseInsensitive))
        order.setValue("o_delivery_service", 2);
      else
        order.setValue("o_delivery_service", 0);

      order.setValue("o_delivery_send_id", "");
    }

    if (bf_order.contains("lieferadresse")) {
      QJsonObject bf_delivery = bf_order.value("lieferadresse").toObject();
      QStringList daddr;
      QString da_gender = bf_delivery.value("anrede").toString();
      if (!da_gender.isEmpty())
        daddr << QString(da_gender + " ").trimmed();

      QString da_fName = bf_delivery.value("vorname").toString();
      QString da_lName = bf_delivery.value("name").toString();
      daddr << QString(da_fName + " " + da_lName);

      QString da_addr = bf_delivery.value("adresse").toString();
      QString da_postal = bf_delivery.value("plz").toString();
      QString da_location = bf_delivery.value("ort").toString();
      daddr << da_addr << da_postal + " " + da_location;
      order.setValue("c_shipping_address", daddr.join("\n"));
    }

    QString firstname = order.getValue("c_firstname").toString();
    QString lastname = order.getValue("c_lastname").toString();
    QString street = order.getValue("c_street").toString();
    QString postalcode = order.getValue("c_postalcode").toString();
    QString location = order.getValue("c_location").toString();
    order.setValue("c_provider_import", (firstname + " " + lastname));
    QStringList postalAddress;
    postalAddress << firstname + " " + lastname;
    postalAddress << street;
    postalAddress << postalcode + " " + location;
    order.setValue("c_postal_address", postalAddress.join("\n"));

    allOrders.append(order);
  }
  // QList - leeren
  orders.clear();

  return allOrders;
}

bool Buchfreund::createInterface(QObject *parent) {
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
