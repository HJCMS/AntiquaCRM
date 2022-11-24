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
  hash.insert("sendungsnummer", "o_delivery_send_id");
  hash.insert("telefon", "c_phone_0");
  hash.insert("titel", "a_title");
  hash.insert("versandart", "o_delivery_service");
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
  AntiquaCRM::ASettings cfg(this);
  cfg.beginGroup(BUCHFREUND_CONFIG_GROUP);
  url.setScheme("https");
  url.setHost(cfg.value("api_host", "buchfreund.de").toString());
  apiKey = cfg.value("api_key", QString()).toString();
  apiPath = cfg.value("api_basepath").toString();
  historyCall = cfg.value("api_history_call", -7).toInt();
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

  QString value = configProvider();
  value.append("_");
  value.append(section);
  value.append(".json");

  // NOTE Action Cookie setz auch den Dateinamen
  actionsCookie = QNetworkCookie("action", value.toLocal8Bit());
  actionsCookie.setDomain(url.host());
  actionsCookie.setSecure(true);

  return url;
}

const QString Buchfreund::dateString(const QDate &date) const {
  return date.toString(BUCHFREUND_DATE_FORMAT);
}

const AntiquaCRM::ArticleOrderItem
Buchfreund::articleItem(const QString &key, const QJsonValue &value) const {
  QString _key = p_articleTranslate.value(key);
  QHash<QString, QMetaType::Type> keys =
      AntiquaCRM::AProviderOrder::articleKeys();
  if (!keys.contains(_key)) {
    qWarning("Buchfreund: Unknown Article Key(%s)!", qPrintable(key));
    return AntiquaCRM::ArticleOrderItem();
  }

  QVariant _value;
  if (_key == "a_article_id") {
    // Artikel Nummer
    _value = value.toInt();
  } else if (_key == "a_provider_id") {
    // Dienstleister Bestellnummer
    _value = value.toString();
  } else if (_key == "a_payment_id") {
    // Interne Bestellnummer kann hier nicht gesetzt werden!
    _value = 0;
  } else if (_key.contains("_price")) {
    // Preise
    if (value.type() == QJsonValue::String) {
      _value = QString(value.toString()).toDouble();
    } else if (value.type() == QJsonValue::Double) {
      _value = value.toDouble();
    }
  } else if (_key.contains("type")) {
    _value = AntiquaCRM::ArticleType::BOOK;
  } else if (keys.value(_key) == QMetaType::Int) {
    _value = value.toInt();
  } else if (keys.value(_key) == QMetaType::QString) {
    _value = value.toString();
  }

  AntiquaCRM::ArticleOrderItem item;
  item.key = _key;
  item.value = _value;
  return item;
}

void Buchfreund::prepareResponse(const QJsonDocument &js) {
  QString fileName = configProvider();
  if (!actionsCookie.name().isNull()) {
    fileName = QString(actionsCookie.value());
  }

  AntiquaCRM::ASharedCacheFiles cacheFile;
  QString jsonData = js.toJson(QJsonDocument::Indented);
  cacheFile.storeTempFile(fileName.toLower(), jsonData);

  emit sendQueryFinished();
}

void Buchfreund::queryFinished(QNetworkReply *reply) {
  if (reply->error() != QNetworkReply::NoError) {
    emit sendErrorResponse(AntiquaCRM::WARNING,
                           tr("Buchfreund response with errors!"));
  }
}

void Buchfreund::queryNewOrders(int waitSecs) {
  Q_UNUSED(waitSecs);

  QDateTime curr = QDateTime::currentDateTime();
  QDateTime past = curr.addDays(-14);
  QJsonObject obj;
  obj.insert("datum_von", past.toString(BUCHFREUND_DATE_FORMAT));
  obj.insert("datum_bis", curr.toString(BUCHFREUND_DATE_FORMAT));

  QUrl url = apiQuery("bestellungen");
  m_network->jsonPostRequest(url, QJsonDocument(obj));
}

void Buchfreund::queryOrder(const QString &orderId) {
  QUrl url = apiQuery("order");
  QUrlQuery q;
  q.addQueryItem("token", QString(authenticCookie.value()));
  q.addQueryItem("orderId", orderId);
  url.setQuery(q);
  m_network->getRequest(url);
}

void Buchfreund::orderUpdateAction(const QJsonObject &options) {
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
  AntiquaCRM::AProviderOrders allOrders;

  QString fileName = configProvider();
  if (!actionsCookie.name().isNull()) {
    fileName = QString(actionsCookie.value());
  }

  if (fileName.isEmpty())
    return allOrders;

  QString display_name = displayName().toLower();
  AntiquaCRM::ASharedCacheFiles cacheFile;
  // QString data = cacheFile.getTempFile(fileName.toLower());
  QString data = cacheFile.getTempFile(display_name + "_test.json");
  qWarning("Buchfreund in Test Mode!");
  if (data.isEmpty())
    return allOrders;

  QJsonDocument doc = QJsonDocument::fromJson(data.toLocal8Bit());
  bool hasErrors = doc.object().value("error").toBool();
  if (hasErrors) {
    qWarning("Buchfreund response with errors!");
    return allOrders;
  } else {
    QJsonArray array = QJsonValue(doc["response"]).toArray();
    qDebug() << Q_FUNC_INFO << array;
    // for (int i = 0; i < array.count(); i++) {}
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
