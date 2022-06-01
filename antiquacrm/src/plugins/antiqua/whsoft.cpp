// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "whsoft.h"

#include <QAction>
#include <QDateTime>
#include <QJsonArray>
#include <QJsonValue>
#include <QMenu>
#include <QPushButton>
#include <QStyle>
#include <QTabWidget>
#include <QTableWidget>
#include <QTime>
#include <QVBoxLayout>

/**
 * @def DATE_FORMAT
 * @ingroup Providers SQL Statements
 * @brief Datumsformat von Buchfreund.de
 * Wird für lesen/schreiben der JSon Datenfelder benötigt.
 */
#ifndef DATE_FORMAT
#define DATE_FORMAT "yyyy-MM-dd hh:mm:ss"
#endif

/** @brief Konfigurationsgruppe */
#ifndef CONFIG_GROUP
#define CONFIG_GROUP "provider/whsoft"
#endif

/** @brief Wird für Menüeintrag und Gruppenzuweisung benötigt! */
#ifndef CONFIG_PROVIDER
#define CONFIG_PROVIDER "Buchfreund"
#endif

#ifndef PLUGIN_TEST_MODE
#define PLUGIN_TEST_MODE false
#endif

/**
 * Wir verwenden keine Anrede in der Datenbank.
 * Es ist kein Adresskopf-Pflichtfeld, zu dem nicht mehr
 * Zeitgemäß. @see Landesgleichstellungsgesetz - LGG
 * In der Datenbank werden 4 Definitionen behandelt.
 * @li 0 = without disclosures => Keine Angaben
 * @li 1 = Male  => Männlich
 * @li 2 = Female => Weiblich
 * @li 3 = Various => Diverse
 * @return int
 */
static int genderFromString(const QString &anrede) {
  QString str = anrede.trimmed();
  if (str.startsWith("herr", Qt::CaseInsensitive) ||
      str.startsWith("mr.", Qt::CaseInsensitive))
    return 1;
  else if (str.startsWith("frau", Qt::CaseInsensitive) ||
           str.startsWith("mrs", Qt::CaseInsensitive))
    return 2;
  else
    return 0;
}

WHSoftJSonQuery::WHSoftJSonQuery(QObject *parent) : QObject{parent} {
  setObjectName("whsoft_json_query");
}

const QUrl WHSoftJSonQuery::apiQuery(const QString &operation) {
  QRegExp pattern("([\\/]{2,})");
  ApplSettings cfg;

  QUrl url;
  cfg.beginGroup(CONFIG_GROUP);
  url.setScheme(cfg.value("api_scheme").toString());
  url.setHost(cfg.value("api_host").toString());
  QString path(cfg.value("api_basepath").toString());
  path.append("/");
  path.append(cfg.value("api_key").toString());
  path.append("/" + operation);
  cfg.endGroup();
  url.setPath(path.replace(pattern, "/"));
  return url;
}

void WHSoftJSonQuery::queryList() {
  QDateTime curDt = QDateTime::currentDateTime();
  QTime t(curDt.time().hour(), curDt.time().minute(), 0);
  curDt.setTime(t);
  QDateTime from = curDt.addDays(-14);
  QJsonObject obj;
  obj.insert("datum_von", QJsonValue::fromVariant(from.toString(DATE_FORMAT)));
  obj.insert("datum_bis", QJsonValue::fromVariant(curDt.toString(DATE_FORMAT)));
  QJsonDocument doc(obj);
  QByteArray data = doc.toJson(QJsonDocument::Compact);
  QUrl url = apiQuery("bestellungen");
  Antiqua::Provider *prQuery = new Antiqua::Provider(this, false);
  prQuery->setObjectName("buchfreund_query_list");
  connect(prQuery, SIGNAL(responsed(const QJsonDocument &)), this,
          SIGNAL(listResponsed(const QJsonDocument &)));
  connect(prQuery, SIGNAL(finished()), prQuery, SLOT(deleteLater()));
  prQuery->sendPost(url, data);
}

void WHSoftJSonQuery::queryOrder(const QString &bfId) {
  QJsonObject obj;
  obj.insert("id", QJsonValue(bfId));
  QJsonDocument doc(obj);
  QByteArray data = doc.toJson(QJsonDocument::Compact);
  QUrl url = apiQuery("bestellung");
  Antiqua::Provider *prQuery = new Antiqua::Provider(this, false);
  prQuery->setObjectName("buchfreund_query_view");
  connect(prQuery, SIGNAL(responsed(const QJsonDocument &)), this,
          SIGNAL(orderResponsed(const QJsonDocument &)));
  connect(prQuery, SIGNAL(finished()), prQuery, SLOT(deleteLater()));
  prQuery->sendPost(url, data);
}

void WHSoftJSonQuery::customQuery(const QString &operation,
                                  const QJsonDocument &doc) {
  QByteArray data = doc.toJson(QJsonDocument::Compact);
  QUrl url = apiQuery(operation);
  Antiqua::Provider *prQuery = new Antiqua::Provider(this, false);
  prQuery->setObjectName("buchfreund_query_" + operation);
  connect(prQuery, SIGNAL(responsed(const QJsonDocument &)), this,
          SIGNAL(orderResponsed(const QJsonDocument &)));
  connect(prQuery, SIGNAL(finished()), prQuery, SLOT(deleteLater()));
  prQuery->sendPost(url, data);
}

WHSoftTable::WHSoftTable(QWidget *parent)
    : Antiqua::PurchaserOrderTable{parent} {}

void WHSoftTable::contextMenuEvent(QContextMenuEvent *e) {
  QMenu *m = new QMenu("Actions", this);
  QAction *ac_remove = m->addAction(
      style()->standardIcon(QStyle::SP_ComputerIcon), tr("inspect article"));
  ac_remove->setObjectName("ac_context_search_article");
  connect(ac_remove, SIGNAL(triggered()), this, SIGNAL(findArticleNumbers()));
  m->exec(e->globalPos());
  delete m;
}

WHSoftPurchaser::WHSoftPurchaser(QWidget *parent)
    : Antiqua::PurchaserWidget{parent} {
  QStyle *wStyle = style();
  QVBoxLayout *customerLayout = new QVBoxLayout(this);
  QHBoxLayout *h1l = new QHBoxLayout();
  QLabel *anrede = new QLabel(this);
  anrede->setObjectName("anrede");
  anrede->setIndent(5);
  h1l->addWidget(anrede);
  QLabel *vorname = new QLabel(this);
  vorname->setObjectName("vorname");
  h1l->addWidget(vorname);
  QLabel *name = new QLabel(this);
  name->setObjectName("name");
  h1l->addWidget(name);
  QLabel *email = new QLabel(this);
  email->setObjectName("email");
  email->setTextInteractionFlags(Qt::TextEditorInteraction);
  email->setIndent(5);
  h1l->addWidget(email);
  h1l->addStretch(1);
  QLabel *info_CustomerId = new QLabel(this);
  info_CustomerId->setIndent(5);
  info_CustomerId->setText(tr("Customer") + ":");
  h1l->addWidget(info_CustomerId);
  QLabel *costumerId = new QLabel(this);
  costumerId->setObjectName("costumerId");
  costumerId->setText("<b>" + tr("Not exists") + "</b>");
  h1l->addWidget(costumerId);
  customerLayout->addLayout(h1l);

  QTabWidget *m_tabWidget = new QTabWidget(this);
  m_tabWidget->setMinimumHeight(100);
  customerLayout->addWidget(m_tabWidget);

  rechnungsadresse = new QTextEdit(this);
  rechnungsadresse->setObjectName("rechnungsadresse");
  m_tabWidget->addTab(rechnungsadresse,
                      wStyle->standardIcon(QStyle::SP_FileIcon),
                      tr("shipping address"));

  lieferadresse = new QTextEdit(this);
  lieferadresse->setObjectName("lieferadresse");
  m_tabWidget->addTab(lieferadresse, wStyle->standardIcon(QStyle::SP_FileIcon),
                      tr("delivery address"));

  QTextEdit *kundenkommentar = new QTextEdit(this);
  kundenkommentar->setObjectName("kundenkommentar");
  kundenkommentar->setReadOnly(true);
  m_tabWidget->addTab(kundenkommentar,
                      wStyle->standardIcon(QStyle::SP_MessageBoxQuestion),
                      tr("Comments"));

  setLayout(customerLayout);
}

void WHSoftPurchaser::setCustomerId(int customerId) {
  id = customerId;
  setValue("costumerId", QString::number(id));
  emit customerIdChanged(id);
}

void WHSoftPurchaser::setValue(const QString &objName, const QVariant &value) {
  if (objName.isEmpty() || value.isNull())
    return;

  QLabel *lb = findChild<QLabel *>(objName, Qt::FindChildrenRecursively);
  if (lb != nullptr) {
    lb->setText(value.toString());
  }
  QTextEdit *tx = findChild<QTextEdit *>(objName, Qt::FindChildrenRecursively);
  if (tx != nullptr) {
    tx->setPlainText(value.toString());
  }
}

const QVariant WHSoftPurchaser::getValue(const QString &objName) {
  QLabel *lb = findChild<QLabel *>(objName, Qt::FindChildrenRecursively);
  if (lb != nullptr)
    return lb->text();

  QTextEdit *tx = findChild<QTextEdit *>(objName, Qt::FindChildrenRecursively);
  if (tx != nullptr) {
    return tx->toPlainText();
  }

  return QVariant();
}

Buchfreund::Buchfreund(const QString &widgetId, QWidget *parent)
    : Antiqua::ProviderWidget{widgetId, parent} {
  setObjectName("buchfreund_main");
  QVBoxLayout *layout = new QVBoxLayout(this);

  // Artikel Bestandsänderungen
  QHBoxLayout *r0Layout = new QHBoxLayout();
  QGroupBox *m_duration = new QGroupBox(this);
  m_duration->setTitle(tr("Modify Item inventory count"));
  QGridLayout *durLayout = new QGridLayout(m_duration);
  QLabel *lb1 = new QLabel(tr("Article Id") + ":", m_duration);
  durLayout->addWidget(lb1, 0, 0, 1, 1, Qt::AlignRight);
  m_articleId = new QLineEdit(m_duration);
  durLayout->addWidget(m_articleId, 0, 1, 1, 1, Qt::AlignLeft);
  m_count = new QSpinBox(m_duration);
  m_count->setToolTip(tr("itemcount"));
  m_count->setRange(0, 99);
  durLayout->addWidget(m_count, 1, 0, 1, 1, Qt::AlignRight);
  QPushButton *btn_queryArticle = new QPushButton(m_duration);
  btn_queryArticle->setText(tr("Update"));
  durLayout->addWidget(btn_queryArticle, 1, 1, 1, 1, Qt::AlignLeft);
  m_duration->setLayout(durLayout);
  r0Layout->addWidget(m_duration);
  r0Layout->addStretch(1);
  layout->addLayout(r0Layout);

  layout->addStretch(1);
  QLabel *lb2 = new QLabel(tr("Response Information"), this);
  layout->addWidget(lb2);

  m_response = new QTextEdit(this);
  layout->addWidget(m_response);

  layout->addStretch(1);
  setLayout(layout);

  connect(btn_queryArticle, SIGNAL(clicked()), this,
          SLOT(updateArticleCount()));
}

void Buchfreund::jsonQuery(const QString &operation, const QJsonDocument &doc) {
  WHSoftJSonQuery *mq = new WHSoftJSonQuery(this);
  mq->setObjectName("json_query_buchfreund_counts");
  connect(mq, SIGNAL(orderResponsed(const QJsonDocument &)), this,
          SLOT(queryResponse(const QJsonDocument &)));

  mq->customQuery(operation, doc);
}

void Buchfreund::updateArticleCount() {
  QString id = m_articleId->text();
  if (id.isEmpty())
    return;

  int count = m_count->value();

  QJsonObject obj;
  obj.insert("bestellnr", QJsonValue(id));
  obj.insert("bestand", QJsonValue(count));
  QJsonArray arr;
  arr.append(obj);

  QJsonObject sender;
  sender.insert("produkte", arr);

  QJsonDocument doc(sender);
  jsonQuery("bestand", doc);
}

void Buchfreund::queryResponse(const QJsonDocument &doc) {
  QString d = QString::fromLocal8Bit(doc.toJson(QJsonDocument::Indented));
  m_response->setPlainText(d);
}

WHSoftWidget::WHSoftWidget(const QString &widgetId, QWidget *parent)
    : Antiqua::InterfaceWidget{widgetId, parent} {
  setWidgetResizable(true);
  setObjectName(widgetId);
  setWindowTitle(widgetId);

  QWidget *mainWidget = new QWidget(this);
  QVBoxLayout *layout = new QVBoxLayout(mainWidget);
  layout->setContentsMargins(0, 0, 0, 0);
  // BEGIN Artikelanzeige
  m_orderTable = new WHSoftTable(mainWidget);
  layout->addWidget(m_orderTable);
  // END

  QLabel *lbInfo = new QLabel(tr("purchaser"));
  lbInfo->setIndent(10);
  layout->addWidget(lbInfo);

  // BEGIN Verkäufer
  m_purchaserWidget = new WHSoftPurchaser(this);
  layout->addWidget(m_purchaserWidget);
  // END

  layout->addStretch(1);
  mainWidget->setLayout(layout);
  setWidget(mainWidget);

  connect(m_orderTable, SIGNAL(findArticleNumbers()), this,
          SLOT(readCurrentArticleIds()));
}

const QVariant WHSoftWidget::tableData(int row, int column) {
  QTableWidgetItem *item = m_orderTable->item(row, column);
  if (item != nullptr)
    return item->data(Qt::DisplayRole);

  return QVariant();
}

void WHSoftWidget::createCustomerDocument() {
  if (p_currentDocument.isEmpty()) {
    qWarning("Current Json Document is empty!");
    return;
  }

  QString mainKey("response");
  QJsonDocument doc = p_currentDocument;
  QJsonObject obj = QJsonValue(doc[mainKey]).toObject();

  QJsonObject queryObject;
  queryObject.insert("provider", QJsonValue(CONFIG_PROVIDER));
  queryObject.insert("type", "customer_create");

  QJsonObject::iterator it; // Iterator
  for (it = obj.begin(); it != obj.end(); ++it) {
    QString f = sqlParam(it.key());
    QString v = it.value().toString();
    if (!v.isEmpty())
      queryObject.insert(f, v);
  }

  QJsonObject addr = QJsonValue(doc[mainKey]["rechnungsadresse"]).toObject();
  if (!addr.isEmpty()) {
    for (it = addr.begin(); it != addr.end(); ++it) {
      QString f = sqlParam(it.key());
      QString v = it.value().toString();
      if ((f == "c_gender" || f == "anrede") && !v.isEmpty()) {
        queryObject.insert(f, QString::number(genderFromString(v)));
      } else if (!f.isEmpty() && !v.isEmpty()) {
        queryObject.insert(f, v);
      }
    }
  }

  emit createCustomer(QJsonDocument(queryObject));
}

const QJsonDocument WHSoftWidget::customerRequest(const QJsonObject &object) {
  QJsonObject queryObject;
  queryObject.insert("provider", QJsonValue(CONFIG_PROVIDER));
  queryObject.insert("orderid", QJsonValue(objectName()));
  queryObject.insert("type", "customer_request");
  queryObject.insert("c_firstname", object["vorname"]);
  queryObject.insert("c_lastname", object["name"]);
  queryObject.insert("c_postalcode", object["plz"]);
  queryObject.insert("c_location", object["ort"]);
  return QJsonDocument(queryObject);
}

void WHSoftWidget::parseAddressBody(const QString &section,
                                    const QJsonObject &object) {
  QStringList buffer;
  QString person;
  QString gender(object["anrede"].toString().trimmed());
  if (!gender.isEmpty()) {
    person.append(gender);
    person.append(" ");
  }
  person.append(object["vorname"].toString().trimmed());
  person.append(" ");
  person.append(object["name"].toString().trimmed());
  buffer.append(person);

  QString street(object["adresse"].toString().trimmed());
  buffer.append(street);

  QString location(object["plz"].toString().trimmed());
  location.append(" ");
  location.append(object["ort"].toString().trimmed());
  QString country = object["land"].toString().trimmed();
  if (!country.isEmpty()) {
    location.append("/" + country);
  }
  buffer.append(location);

  m_purchaserWidget->setValue(section, buffer.join("\n"));
  buffer.clear();

  // Sende SQL Abfrage an Hauptfenster!
  if (section == "rechnungsadresse") {
    QJsonDocument qDoc = customerRequest(object);
    if (!qDoc.isEmpty())
      emit checkCustomer(qDoc);
  }
}

void WHSoftWidget::readCurrentArticleIds() {
  QList<int> ids;
  for (int r = 0; r < m_orderTable->rowCount(); r++) {
    QTableWidgetItem *item = m_orderTable->item(r, 1);
    if (item != nullptr) {
      ids.append(item->data(Qt::DisplayRole).toInt());
    }
  }
  emit s_checkArticles(ids);
}

void WHSoftWidget::setContent(const QJsonDocument &doc) {
  if (doc.isEmpty())
    return;

  int errors = QJsonValue(doc["error"]).toBool();
  if (errors)
    return;

  // Speichern
  p_currentDocument = doc;

  QString mainKey("response");
  QJsonObject response = QJsonValue(doc[mainKey]).toObject();
  if (!response.isEmpty()) {
    QJsonObject::iterator it;
    for (it = response.begin(); it != response.end(); ++it) {
      QString f = it.key();
      QJsonValue val = it.value();
      if (!f.isEmpty() && !val.toString().isEmpty()) {
        m_purchaserWidget->setValue(f, val.toVariant());
      }
    }

    QJsonObject addr1 = QJsonValue(doc[mainKey]["rechnungsadresse"]).toObject();
    if (!addr1.isEmpty()) {
      for (it = addr1.begin(); it != addr1.end(); ++it) {
        QString f = it.key();
        QJsonValue val = it.value();
        if (!f.isEmpty() && !val.toString().isEmpty()) {
          m_purchaserWidget->setValue(f, val.toString());
        }
      }
      parseAddressBody("rechnungsadresse", addr1);
    }
    QJsonObject addr2 = QJsonValue(doc[mainKey]["lieferadresse"]).toObject();
    if (!addr2.isEmpty()) {
      parseAddressBody("lieferadresse", addr2);
    }
  }

  // Bestellartikel einfügen
  m_orderTable->setRowCount(0);
  QJsonArray positionen = QJsonValue(doc["response"]["positionen"]).toArray();
  if (positionen.count() > 0) {
    QJsonArray::iterator at;
    for (at = positionen.begin(); at != positionen.end(); ++at) {
      QJsonObject article = (*at).toObject();
      QJsonObject::iterator it;
      int column = 0;
      int row = m_orderTable->rowCount();
      m_orderTable->setRowCount((m_orderTable->rowCount() + 1));
      QTableWidgetItem *item = m_orderTable->createItem(windowTitle());
      m_orderTable->setItem(row, column++, item);
      for (it = article.begin(); it != article.end(); ++it) {
        QString f = sqlParam(it.key());
        QVariant curValue = it.value().toVariant();
        if (!f.isEmpty() && !curValue.isNull() && f.contains("a_")) {
          QString txt = curValue.toString().trimmed();
          QTableWidgetItem *item = m_orderTable->createItem(txt);
          m_orderTable->setItem(row, column++, item);
        }
      }
    }
  }
}

void WHSoftWidget::createOrderRequest(const QString &bfId) {
  if (PLUGIN_TEST_MODE) {
    // DUMMY TEST
    QString buffer;
    QString p("/Developement/antiqua/antiquacrm/src/plugins/antiqua/");
    p.append("example-whsoft-order.json");

    QFile fp(QDir::homePath() + p);
    if (fp.open(QIODevice::ReadOnly)) {
      QTextStream str(&fp);
      str.setCodec("UTF8");
      buffer.append(str.readAll());
      fp.close();
    }

    QJsonParseError parser;
    QJsonDocument doc = QJsonDocument::fromJson(buffer.toLocal8Bit(), &parser);
    if (parser.error != QJsonParseError::NoError) {
      qWarning("Json Parse Error!");
      return;
    }
    setContent(doc);

    qDebug() << Q_FUNC_INFO << "DUMMY" << bfId;
    return;
  }

  WHSoftJSonQuery *mq = new WHSoftJSonQuery(this);
  mq->setObjectName("json_query_" + objectName());
  connect(mq, SIGNAL(orderResponsed(const QJsonDocument &)), this,
          SLOT(setContent(const QJsonDocument &)));

  mq->queryOrder(bfId);
}

void WHSoftWidget::setCustomerId(int customerId) {
  if (customerId > 0) {
    currentCustomerId = customerId;
    m_purchaserWidget->setCustomerId(customerId);
  }
}

const QMap<QString, QString> WHSoftWidget::fieldTranslate() const {
  QMap<QString, QString> map;
  // public.customers @{
  map.insert("person", "a_customer_id");
  map.insert("anrede", "c_gender");
  map.insert("vorname", "c_firstname");
  map.insert("name", "c_lastname");
  map.insert("adresse", "c_street");
  map.insert("plz", "c_postalcode");
  map.insert("ort", "c_location");
  map.insert("land", "c_country");
  map.insert("telefon", "c_phone_0");
  map.insert("email", "c_email_0");
  // @}

  // public.article_orders @{
  map.insert("bestellnr", "a_article_id");
  map.insert("menge_bestellt", "a_count");
  map.insert("preis_pro_einheit", "a_sell_price");
  map.insert("titel", "a_title");
  // @}
  return map;
}

const ProviderOrder WHSoftWidget::getProviderOrder() {
  ProviderOrder order;
  int cid = m_purchaserWidget->getValue("costumerId").toInt();
  order.setProvider(CONFIG_PROVIDER);
  order.setProviderId(objectName());
  order.setCustomerId(cid);
  int col = 1; /**< CustomerId Cell */
  QStringList ids;
  for (int r = 0; r < m_orderTable->rowCount(); r++) {
    QString aid = tableData(r, col).toString();
    if (!aid.isEmpty())
      ids.append(aid);
  }
  order.setArticleIds(ids);
  return order;
}

void WHSoft::prepareJsonListResponse(const QJsonDocument &doc) {
  bool errors = QJsonValue(doc["error"]).toBool();
  if (!errors) {
    QJsonArray senderArray;
    QJsonArray array = QJsonValue(doc["response"]).toArray();
    for (int i = 0; i < array.count(); i++) {
      QJsonObject convert;
      QJsonObject obj = array[i].toObject();
      convert.insert("id", obj["id"]);
      QDateTime d = QDateTime::fromString(obj["datum"].toString(), DATE_FORMAT);
      convert.insert("datum", QJsonValue(d.toString(Qt::ISODate)));
      senderArray.append(convert);
    }
    QJsonObject senderObject;
    senderObject.insert("provider", QJsonValue(CONFIG_PROVIDER));
    senderObject.insert("items", senderArray);
    QJsonDocument jsDoc(senderObject);
    emit listResponse(jsDoc);
  }
}

bool WHSoft::createInterface(QObject *parent) {
  if (parent) {
    m_whsoft = new QObject(parent);
    m_whsoft->setObjectName("whsoft_plugin");
    return true;
  }
  return false;
}

Antiqua::ProviderWidget *WHSoft::providerWidget(const QString &widgetId,
                                                QWidget *parent) {
  m_buchfreundWidget = new Buchfreund(widgetId, parent);
  m_buchfreundWidget->setObjectName(widgetId);
  return m_buchfreundWidget;
}

Antiqua::InterfaceWidget *WHSoft::addWidget(const QString &widgetId,
                                            QWidget *parent) {
  m_whsoftWidget = new WHSoftWidget(widgetId, parent);
  m_whsoftWidget->setObjectName(widgetId);
  return m_whsoftWidget;
}

const QString WHSoft::provider() const { return QString(CONFIG_PROVIDER); }

const QString WHSoft::configGroup() const { return QString(CONFIG_GROUP); }

void WHSoft::queryMenueEntries() {
  WHSoftJSonQuery *mjs = new WHSoftJSonQuery(this);
  mjs->setObjectName(CONFIG_PROVIDER);
  connect(mjs, SIGNAL(listResponsed(const QJsonDocument &)), this,
          SLOT(prepareJsonListResponse(const QJsonDocument &)));

  mjs->queryList();
}
