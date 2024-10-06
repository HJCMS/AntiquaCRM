// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cmdbuchfreund.h"

#include <QNetworkReply>
#include <QtCore>
#include <unistd.h>

#ifndef BUCHFREUND_DATE_FORMAT
#  define BUCHFREUND_DATE_FORMAT "yyyy-MM-dd hh:mm:ss"
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

CmdBuchfreund::CmdBuchfreund(QObject* parent)
    : ACmdProviders{AntiquaCRM::NetworkQueryType::JSON_QUERY, parent} {
  setObjectName("buchfreund_cmd");
}

void CmdBuchfreund::queryOrdersById(const QStringList& list) {
  QStringList imported = currProviderIds(provider());
  foreach (QString _id, list) {
    if (imported.contains(_id)) {
      // qDebug() << "Buchfreund already exists:" << id;
      continue;
    }

    QJsonObject obj;
    obj.insert("id", QJsonValue(_id));
    QJsonDocument doc(obj);
    if (doc.isNull())
      continue;

    netw->jsonPostRequest(apiQuery("bestellung"), doc);
    sleep(2);
  }
}

const QString CmdBuchfreund::provider() const {
  return QString("Buchfreund");
}

void CmdBuchfreund::initConfiguration() {
  QString _sql("SELECT cfg_jsconfig::json FROM antiquacrm_configs");
  _sql.append(" WHERE cfg_group='CONFIG_BUCHFREUND';");
  QSqlQuery _q = pgsql->query(_sql);
  if (_q.size() == 0) {
    qWarning("Buchfreund: Configuration not found, abort!");
    emit sendDisjointed();
    return;
  }

  _q.next();
  QJsonObject _o = QJsonDocument::fromJson(_q.value(0).toByteArray()).object();
  _q.clear();

  baseUrl.setScheme("https");
  baseUrl.setHost(_o.value("api_host").toString());
  apiKey = _o.value("api_key").toString();
  // apiPath = QString("/verkaeufer/api/");
  apiPath = _o.value("m_api_path").toString();
}

const QUrl CmdBuchfreund::apiQuery(const QString& action) {
  QString p(apiPath);
  p.append(apiKey);
  p.append("/");
  p.append(action);

  QUrl url(baseUrl);
  url.setPath(p);

  actionsCookie = QNetworkCookie("action", action.toLocal8Bit());
  actionsCookie.setDomain(url.host());
  actionsCookie.setSecure(true);

  return url;
}

void CmdBuchfreund::prepareContent(const QJsonDocument& document) {
  QJsonObject obj = document.object();
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
    if (orderIds.size() > 0)
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
      data << document.toJson(QJsonDocument::Indented);
      fp.close();
    }
  }

  // Einzelne Bestellung entgegen nehmen
  // Tabellenfelder
  const QMap<QString, int> _fieldSet = initDataInformation();
  const QHash<QString, QString> keys = translateIds();

  QJsonObject bf_order = response.toObject();
  QString bf_id = bf_order.value("id").toString();
  if (bf_id.isEmpty()) {
    qWarning("Buchfreund: Invalid order detected!");
    emit sendDisjointed();
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
  // antiqua_orderinfo.insert("o_media_type", AntiquaCRM::ArticleType::BOOK);

  // AntiquaCRM::PaymentMethod
  AntiquaCRM::PaymentMethod payment_method;
  QString bezahlart = bf_order.value("bezahlart").toString();
  if (bezahlart.contains("Banküberweisung", Qt::CaseInsensitive))
    payment_method = AntiquaCRM::PaymentMethod::BANK_PREPAYMENT;
  else if (bezahlart == "Rechnung") // Offene Rechnung
    payment_method = AntiquaCRM::PaymentMethod::DELIVER_WITH_INVOICE;
  else if (bezahlart.contains("Kreditkarte", Qt::CaseInsensitive))
    payment_method = AntiquaCRM::PaymentMethod::CREDIT_CARD_PREPAYMENT;
  else if (bezahlart.contains("PayPal", Qt::CaseInsensitive))
    payment_method = AntiquaCRM::PaymentMethod::PAYPAL_PREPAYMENT;
  else if (bezahlart.contains("Rechnung", Qt::CaseInsensitive))
    payment_method = AntiquaCRM::PaymentMethod::INVOICE_PREPAYMENT_RESERVED;
  else
    payment_method = AntiquaCRM::PaymentMethod::PAYMENT_NOT_SET;

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
    antiqua_order.insert("o_delivery_comment", bf_order.value("kundenkommentar").toString());
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
        int _msize = _fieldSet.value(field, 0);
        if ((_msize > 0) && (_v.type() == QJsonValue::String)
            && (_msize < _v.toString().length())) {
          QString _data(_v.toString().left(_msize));
          if (_data.contains(" ")) {
            QStringList _l = _data.split(" ");
            _data = (_l.size() > 0) ? _l.first().trimmed() : _data.trimmed();
          } else {
            _data = _data.trimmed();
          }
          qInfo("Shrink data for key '%s'.", qPrintable(field));
          _v = QJsonValue(_data);
        }
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
      QJsonObject _jso = articles[a].toObject();
      foreach (QString k, _jso.keys()) {
        if (!keys.value(k).isEmpty()) {
          QString key = keys.value(k);
          QJsonValue value = convert(key, _jso.value(k));
          int _msize = _fieldSet.value(key, 0);
          if ((_msize > 0) && (value.type() == QJsonValue::String)
              && (_msize < value.toString().length())) {
            QString _data = value.toString();
            qInfo("Shrink data for key %s ", qPrintable(key));
            antiqua_articles_item.insert(key, _data.left(_msize));
          } else {
            antiqua_articles_item.insert(key, value);
          }
        }
      }
      antiqua_articles_item.insert("payment_id", QJsonValue(0));
      antiqua_articles_item.insert("a_provider_id", QJsonValue(bf_id));
      QJsonValue price = convert("a_sell_price", _jso.value("preis_pro_einheit"));
      antiqua_articles_item.insert("a_sell_price", price);
      // Find valid Article Type ("bestellnr"=>"a_article_id");
      AntiquaCRM::ArticleType _type = findArticlType(_jso.value("bestellnr").toString());
      antiqua_articles_item.insert("a_type", QJsonValue(_type));
      if (_type == AntiquaCRM::ArticleType::OTHER) {
          antiqua_articles_item.insert("a_tax", QJsonValue(AntiquaCRM::SalesTax::TAX_WITH));
      } else {
          antiqua_articles_item.insert("a_tax", QJsonValue(AntiquaCRM::SalesTax::TAX_INCL));
      }
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
  }

  if (createOrders(ordersList)) {
    qInfo("%s: New orders arrived!", qPrintable(provider()));
    emit sendFinished();
    return;
  }

  qInfo("%s: Nothing todo!", qPrintable(provider()));
  emit sendFinished();
}

void CmdBuchfreund::responsed(const QByteArray& bread) {
  QJsonParseError _parser;
  QJsonDocument _doc = QJsonDocument::fromJson(bread, &_parser);
  if (_parser.error != QJsonParseError::NoError) {
#ifdef ANTIQUA_DEVELOPMENT
    qWarning("Buchfreund::ParseError:(%s)!", qPrintable(_parser.errorString()));
#endif
    emit sendDisjointed();
    return;
  }
  prepareContent(_doc);
}

void CmdBuchfreund::start() {
  QDateTime curr = QDateTime::currentDateTime();
  QDateTime past = curr.addDays(history_query);
  QJsonObject obj;
  obj.insert("datum_von", past.toString(BUCHFREUND_DATE_FORMAT));
  obj.insert("datum_bis", curr.toString(BUCHFREUND_DATE_FORMAT));

  QJsonDocument doc(obj);
  if (doc.isNull())
    return;

  netw->jsonPostRequest(apiQuery("bestellungen"), doc);
}

bool CmdBuchfreund::init() {
  if (pgsql == nullptr)
    return false;

  initConfiguration();

  if (baseUrl.isEmpty() || apiKey.isEmpty())
    return false;

  return true;
}
