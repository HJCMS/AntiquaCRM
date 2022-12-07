// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "buchfreund.h"
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

#ifndef BUCHFREUND_DATE_FORMAT
#define BUCHFREUND_DATE_FORMAT "yyyy-MM-dd hh:mm:ss"
#endif

constexpr bool BUCHFREUND_DEBUG = false;

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
  hash.insert("land", "c_country");
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

Buchfreund::Buchfreund(QObject *parent) : Provider{parent} {}

void Buchfreund::initConfiguration() {
  m_config->beginGroup("provider/buchfreund");
  baseUrl.setScheme("https");
  baseUrl.setHost(m_config->value("api_host", "www.buchfreund.de").toString());
  apiKey = m_config->value("api_key", QString()).toString();
  apiPath = QString("/verkaeufer/api/");
  m_config->endGroup();
}

const QUrl Buchfreund::apiQuery(const QString &section) {
  QString p(apiPath);
  p.append(apiKey);
  p.append("/");
  p.append(section);

  QUrl url(baseUrl);
  url.setPath(p);

  actionsCookie = QNetworkCookie("action", section.toLocal8Bit());
  actionsCookie.setDomain(url.host());
  actionsCookie.setSecure(true);

  return url;
}

void Buchfreund::queryOrdersById(const QStringList &ids) {
  foreach (QString id, ids) {
    QJsonObject obj;
    obj.insert("id", QJsonValue(id));
    NetworkRequest request(apiQuery("bestellung"));
    request.setHeaderContentTypeJson();
    QJsonDocument doc(obj);
    QByteArray body = doc.toJson(QJsonDocument::Compact);
    m_networker->postRequest(request, body);
  }
}

void Buchfreund::prepareContent(const QJsonDocument &doc) {
  QJsonObject obj = doc.object();
  QJsonValue response = obj.value("response");
  if (obj.value("error").toBool()) {
    if (response.type() == QJsonValue::String) {
      qInfo("Buchfreund: %s", qPrintable(response.toString()));
    } else {
      qWarning("Buchfreund document errors!");
    }
    emit sendDisjointed();
    return;
  } else if (response.type() == QJsonValue::String) {
    qInfo("Buchfreund: %s", qPrintable(response.toString()));
    emit sendDisjointed();
    return;
  }

  // Liste der verfügbaren Bestellungen abfragen!
  if (actionsCookie.value() == "bestellungen") {
    QStringList orderIds;
    QJsonArray array = response.toArray();
    for (int i = 0; i < array.size(); i++) {
      QJsonObject entry = array[i].toObject();
      if (!entry.isEmpty())
        orderIds << entry.value("id").toString();
    }
    queryOrdersById(orderIds);
    return;
  }

  if (actionsCookie.value() != "bestellung") {
    return;
  }

  if (BUCHFREUND_DEBUG) {
    QDir dir(QDir::homePath() + "/.cache/antiquacrm/");
    QFileInfo info(dir, "buchfreund_bestellung.json");
    QFile fp(info.filePath());
    if (fp.open(QIODevice::WriteOnly)) {
      QTextStream data(&fp);
      data.setCodec(ANTIQUACRM_TEXTCODEC);
      data << doc.toJson(QJsonDocument::Indented);
      fp.close();
    }
  }

  // Einzelne Bestellung entgegen nehmen
  const QHash<QString, QString> keys = translateIds();
  QJsonObject bf_order = response.toObject();
  QString bf_id = bf_order.value("id").toString();
  if (bf_id.isEmpty()) {
    qWarning("Buchfreund: Invalid order detected!");
    emit sendDisjointed();
    return;
  }

  QStringList imported = currProviderIds(provider());
  if (imported.contains(bf_id)) {
    return;
  }

  // BEGIN::CONVERT
  QList<QJsonObject> ordersList;
  QJsonObject antiqua_order; // Bestellkopfdaten
  antiqua_order.insert("provider", QJsonValue(provider()));
  antiqua_order.insert("orderid", QJsonValue(bf_id));
  // BUCHFREUND_DATE_FORMAT
  QDateTime datetime = getDateTime(bf_order.value("datum").toString());

  // Bestellinfos
  QJsonObject antiqua_orderinfo;
  antiqua_orderinfo.insert("o_provider_name", provider());
  antiqua_orderinfo.insert("o_provider_order_id", QJsonValue(bf_id));
  antiqua_orderinfo.insert("o_since", datetime.toString(Qt::ISODate));
  antiqua_orderinfo.insert("o_media_type", AntiquaCRM::BOOK);

  // AntiquaCRM::PaymentMethod
  AntiquaCRM::PaymentMethod payment_method;
  payment_method = AntiquaCRM::PAYMENT_NOT_SET;
  QString bezahlart = bf_order.value("bezahlart").toString();
  if (bezahlart.contains("Banküberweisung", Qt::CaseInsensitive))
    payment_method = AntiquaCRM::BANK_PREPAYMENT;
  else if (bezahlart == "Rechnung") // Offene Rechnung
    payment_method = AntiquaCRM::DELIVER_WITH_INVOICE;
  else if (bezahlart.contains("Kreditkarte", Qt::CaseInsensitive))
    payment_method = AntiquaCRM::CREDIT_CARD_PREPAYMENT;
  else if (bezahlart.contains("PayPal", Qt::CaseInsensitive))
    payment_method = AntiquaCRM::PAYPAL_PREPAYMENT;
  else if (bezahlart.contains("Rechnung", Qt::CaseInsensitive))
    payment_method = AntiquaCRM::INVOICE_PREPAYMENT_RESERVED;
  else
    payment_method = AntiquaCRM::PAYMENT_NOT_SET;

  antiqua_orderinfo.insert("o_payment_method", payment_method);

  if (bf_order.contains("versandart")) {
    QString versandart = bf_order.value("versandart").toString();
    if (versandart.contains("hermes", Qt::CaseInsensitive))
      antiqua_orderinfo.insert("o_delivery_service", 1);
    else if (versandart.contains("dhl", Qt::CaseInsensitive))
      antiqua_orderinfo.insert("o_delivery_service", 2);
    else
      antiqua_orderinfo.insert("o_delivery_service", 0);

    antiqua_orderinfo.insert("o_delivery_send_id", "");
  }

  // Buyer payment comment
  if (bf_order.contains("kundenkommentar")) {
    antiqua_order.insert("o_delivery_comment",
                         bf_order.value("kundenkommentar").toString());
  }

  // Kundendaten
  QJsonObject antiqua_customer;
  QString bcp47 = "DE"; // Siehe auch Kundendaten
  if (bf_order.contains("email")) {
    antiqua_customer.insert("c_email_0", bf_order.value("email").toString());
  }

  QJsonObject::iterator it;
  QJsonObject address = bf_order.value("rechnungsadresse").toObject();
  QString c_provider_import;
  c_provider_import.append(address.value("vorname").toString().trimmed());
  c_provider_import.append(" ");
  c_provider_import.append(address.value("name").toString().trimmed());
  antiqua_customer.insert("c_provider_import", c_provider_import);

  // Müssen gesondert behandelt werden!
  QStringList naming({"c_firstname", "c_lastname"});
  for (it = address.begin(); it != address.end(); ++it) {
    QString _k = it.key();
    QJsonValue _v = it.value();
    if (_k.isEmpty() || _v.isNull())
      continue;

    if (keys.contains(_k)) {
      QString field = keys.value(_k);
      if (field == "c_gender") {
        antiqua_customer.insert("c_gender", convertGender(_v.toString()));
      } else if (field == "c_country_bcp47") {
        bcp47 = _v.toString().toUpper();
        antiqua_customer.insert(field, bcp47);
        antiqua_orderinfo.insert("o_vat_country", bcp47);
      } else if (naming.contains(field)) {
        antiqua_customer.insert(field, ucFirst(_v.toString()));
      } else {
        antiqua_customer.insert(field, _v.toString());
      }
    }
  }

  QString firstname = antiqua_customer.value("c_firstname").toString();
  QString lastname = antiqua_customer.value("c_lastname").toString();
  QString street = antiqua_customer.value("c_street").toString();
  QString postalcode = antiqua_customer.value("c_postalcode").toString();
  QString location = antiqua_customer.value("c_location").toString();
  QStringList postalAddress;
  postalAddress << firstname + " " + lastname;
  postalAddress << street;
  postalAddress << postalcode + " " + location;
  antiqua_customer.insert("c_postal_address", postalAddress.join("\n"));

  if (bf_order.contains("lieferadresse")) {
    QJsonObject bf_delivery = bf_order.value("lieferadresse").toObject();
    QStringList daddr;
    QString da_fName = bf_delivery.value("vorname").toString();
    QString da_lName = bf_delivery.value("name").toString();
    daddr << QString(da_fName + " " + da_lName);

    QString da_addr = bf_delivery.value("adresse").toString();
    QString da_postal = bf_delivery.value("plz").toString();
    QString da_location = bf_delivery.value("ort").toString();
    daddr << da_addr << da_postal + " " + da_location;
    antiqua_customer.insert("c_shipping_address", daddr.join("\n"));
  }

  // Artikelliste
  QJsonArray antiqua_articles;
  QJsonArray articles = bf_order.value("positionen").toArray();
  for (int a = 0; a < articles.count(); a++) {
    if (articles[a].type() == QJsonValue::Object) {
      QJsonObject antiqua_articles_item;
      QJsonObject jso = articles[a].toObject();
      foreach (QString k, jso.keys()) {
        if (!keys.value(k).isEmpty()) {
          QString key = keys.value(k);
          QJsonValue value = convert(key, jso.value(k));
          antiqua_articles_item.insert(key, value);
        }
      }
      antiqua_articles_item.insert("payment_id", QJsonValue(0));
      antiqua_articles_item.insert("a_provider_id", QJsonValue(bf_id));
      QJsonValue price =
          convert("a_sell_price", jso.value("preis_pro_einheit"));
      antiqua_articles_item.insert("a_sell_price", price);
      antiqua_articles_item.insert("a_type", QJsonValue(AntiquaCRM::BOOK));
      antiqua_articles.append(antiqua_articles_item);
    }
  }

  antiqua_order.insert("orderinfo", antiqua_orderinfo);
  antiqua_order.insert("customer", antiqua_customer);
  antiqua_order.insert("articles", antiqua_articles);
  ordersList.append(antiqua_order);
  // END::CONVERT

  if (BUCHFREUND_DEBUG) {
    qDebug() << Q_FUNC_INFO << antiqua_order;
    emit sendDisjointed();
    return;
  }

  if (createOrders(ordersList)) {
    qInfo("%s: New orders arrived!", qPrintable(provider()));
    emit sendFinished();
    return;
  }

  qInfo("%s: Nothing todo!", qPrintable(provider()));
  emit sendDisjointed();
}

void Buchfreund::responsed(const QByteArray &data) {
  QJsonParseError parser;
  QJsonDocument doc = QJsonDocument::fromJson(data, &parser);
  if (parser.error != QJsonParseError::NoError) {
#ifdef ANTIQUA_DEVELOPEMENT
    qWarning("Buchfreund::ParseError:(%s)!", qPrintable(parser.errorString()));
#endif
    emit sendDisjointed();
    return;
  }
  prepareContent(doc);
}

void Buchfreund::start() {
  QDateTime curr = QDateTime::currentDateTime();
  QDateTime past = curr.addDays(history_query);
  QJsonObject obj;
  obj.insert("datum_von", past.toString(BUCHFREUND_DATE_FORMAT));
  obj.insert("datum_bis", curr.toString(BUCHFREUND_DATE_FORMAT));

  NetworkRequest request(apiQuery("bestellungen"));
  request.setHeaderContentTypeJson();

  QJsonDocument doc(obj);
  QByteArray body = doc.toJson(QJsonDocument::Compact);
  m_networker->postRequest(request, body);
}

bool Buchfreund::init() {
  initConfiguration();

  if (baseUrl.isEmpty())
    return false;

  if (apiKey.isEmpty())
    return false;

  return true;
}
