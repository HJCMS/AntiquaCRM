// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "buchfreund.h"
#include "antiqua_global.h"
#include "applsettings.h"
#include "myicontheme.h"
#include "provider.h"
#include "providersstatements.h"
#include "sqlcore.h"

#include <QtCore>
#include <QtWidgets>

BF_Translater::BF_Translater() : QMap<QString, QString>{} {
  // public.costumers @{
  insert("anrede", "c_gender");
  insert("vorname", "c_firstname");
  insert("name", "c_lastname");
  insert("adresse", "c_street");
  insert("plz", "c_postalcode");
  insert("ort", "c_location");
  insert("land", "c_country");
  insert("telefon", "c_phone_0");
  insert("email", "c_email_0");
  // @}

  // public.article_orders @{
  insert("id", "a_provider_id");
  insert("bestellnr", "a_article_id");
  insert("menge_bestellt", "a_count");
  insert("preis_pro_einheit", "a_sell_price");
  insert("datum", "a_modified");
  insert("titel", "a_title");
  // @}
}

const QString BF_Translater::sqlParam(const QString &key) {
  QMap<QString, QString>::iterator fi;
  for (fi = begin(); fi != end(); ++fi) {
    if (fi.key() == key)
      return fi.value();
  }
  return QString();
}

const QString BF_Translater::jsonParam(const QString &key) {
  QMap<QString, QString>::iterator fi;
  for (fi = begin(); fi != end(); ++fi) {
    if (fi.value() == key)
      return fi.key();
  }
  return QString();
}

Buchfreundlist::Buchfreundlist(QWidget *parent) : QListWidget{parent} {
  setObjectName("inventory_buchfreund_liste");

  connect(this, SIGNAL(itemClicked(QListWidgetItem *)), this,
          SLOT(getItemData(QListWidgetItem *)));
}

void Buchfreundlist::addOrder(const QString &id, const QDateTime &date) {
  QListWidgetItem *item = new QListWidgetItem(this, QListWidgetItem::UserType);
  item->setData(Qt::DecorationRole, myIcon("autostart"));
  item->setData(Qt::DisplayRole, date.toString(Qt::SystemLocaleShortDate));
  item->setData(Qt::ToolTipRole, date.toString(Qt::SystemLocaleLongDate));
  item->setData(Qt::UserRole, id);
  addItem(item);
}

void Buchfreundlist::getItemData(QListWidgetItem *item) {
  QJsonObject obj;
  obj.insert("id", QJsonValue(item->data(Qt::UserRole).toString()));
  QJsonDocument doc(obj);
  emit orderClicked(doc);
}

void Buchfreundlist::setView(const QJsonDocument &doc) {
  if (doc.isEmpty())
    return;

  int errors = QJsonValue(doc["error"]).toBool();
  if (!errors) {
    clear();
    QJsonArray array = QJsonValue(doc["response"]).toArray();
    for (int i = 0; i < array.count(); i++) {
      QJsonObject obj = array[i].toObject();
      QString id = obj["id"].toString();
      QString ds = obj["datum"].toString();
      QDateTime dt = QDateTime::fromString(ds, BF_DATE_FORMAT);
      addOrder(id, dt);
    }
  }
}

BuchfreundDisplay::BuchfreundDisplay(QWidget *parent) : QWidget{parent} {
  setObjectName("inventory_buchfreund_anzeige");

  m_sql = new HJCMS::SqlCore(this);

  QVBoxLayout *layout = new QVBoxLayout(this);

  QGroupBox *orderBox = new QGroupBox(this);
  orderBox->setObjectName("display_order_box");
  orderBox->setTitle(tr("Order Details"));
  QGridLayout *orderLayout = new QGridLayout(orderBox);
  a_article_id = new SerialID(orderBox);
  a_article_id->setObjectName("a_article_id");
  a_article_id->setInfo(tr("Article Id"));
  orderLayout->addWidget(a_article_id, 0, 0, 1, 1);
  a_provider_id = new LineEdit(orderBox);
  a_provider_id->setObjectName("a_provider_id");
  a_provider_id->setInfo(tr("Provider Id"));
  orderLayout->addWidget(a_provider_id, 0, 1, 1, 1);
  a_count = new IntSpinBox(orderBox);
  a_count->setObjectName("a_count");
  a_count->setInfo(tr("Quantity"));
  orderLayout->addWidget(a_count, 0, 2, 1, 1, Qt::AlignRight);
  a_sell_price = new PriceEdit(orderBox);
  a_sell_price->setObjectName("a_sell_price");
  a_sell_price->setInfo(tr("Price"));
  orderLayout->addWidget(a_sell_price, 0, 3, 1, 1);
  a_title = new QLineEdit(orderBox);
  a_title->setObjectName("a_title");
  orderLayout->addWidget(a_title, 1, 0, 1, 4);
  a_modified = new DateTimeEdit(orderBox);
  a_modified->setObjectName("a_modified");
  a_modified->setReadOnly(true);
  a_modified->setInfo(tr("Ordered on"));
  orderLayout->addWidget(a_modified, 2, 0, 1, 4);
  orderBox->setLayout(orderLayout);
  layout->addWidget(orderBox);

  // BEGIN Costumer
  QGroupBox *costumerBox = new QGroupBox(this);
  costumerBox->setObjectName("display_purchaser");
  costumerBox->setTitle(tr("purchaser"));
  QGridLayout *costumerLayout = new QGridLayout(costumerBox);
  c_gender = new GenderBox(costumerBox);
  c_gender->setObjectName("c_gender");
  c_gender->setInfo(tr("Form"));
  costumerLayout->addWidget(c_gender, 0, 0, 1, 1);
  c_firstname = new LineEdit(costumerBox);
  c_firstname->setObjectName("c_firstname");
  c_firstname->setInfo(tr("Firstname"));
  costumerLayout->addWidget(c_firstname, 0, 1, 1, 1);
  c_lastname = new LineEdit(costumerBox);
  c_lastname->setObjectName("c_lastname");
  c_lastname->setInfo(tr("Lastname"));
  costumerLayout->addWidget(c_lastname, 0, 2, 1, 1);
  c_postalcode = new PostalCode(costumerBox);
  c_postalcode->setObjectName("c_postalcode");
  c_postalcode->setInfo(tr("Postalcode"));
  costumerLayout->addWidget(c_postalcode, 1, 0, 1, 1);
  c_location = new LineEdit(costumerBox);
  c_location->setObjectName("c_location");
  c_location->setInfo(tr("Location"));
  costumerLayout->addWidget(c_location, 1, 1, 1, 2);
  c_street = new LineEdit(costumerBox);
  c_street->setObjectName("c_street");
  c_street->setInfo(tr("Street"));
  costumerLayout->addWidget(c_street, 2, 0, 1, 1);
  c_country = new LineEdit(costumerBox);
  c_country->setObjectName("c_country");
  c_country->setInfo(tr("Country"));
  costumerLayout->addWidget(c_country, 2, 1, 1, 2);
  c_phone_0 = new PhoneEdit(costumerBox);
  c_phone_0->setObjectName("c_phone_0");
  c_phone_0->setInfo(tr("Phone"));
  costumerLayout->addWidget(c_phone_0, 3, 0, 1, 1);
  c_email_0 = new EMailEdit(costumerBox);
  c_email_0->setObjectName("c_email_0");
  c_email_0->setInfo(tr("eMail"));
  costumerLayout->addWidget(c_email_0, 3, 1, 1, 2);
  QHBoxLayout *actions_layout = new QHBoxLayout();
  btn_create_costumer = new QPushButton(costumerBox);
  btn_create_costumer->setText(tr("Create"));
  btn_create_costumer->setToolTip(tr("create a new database entry"));
  btn_create_costumer->setIcon(myIcon("db_add"));
  btn_create_costumer->setEnabled(false);
  actions_layout->addWidget(btn_create_costumer);
  btn_search_costumer = new QPushButton(costumerBox);
  btn_search_costumer->setText(tr("Search"));
  btn_search_costumer->setToolTip(tr("Search Costumer in Database"));
  btn_search_costumer->setIcon(myIcon("search"));
  actions_layout->addWidget(btn_search_costumer);
  c_id = new SerialID(costumerBox);
  c_id->setObjectName("c_id");
  c_id->setInfo(tr("Costumer Id"));
  c_id->setRequired(false);
  actions_layout->addWidget(c_id);
  costumer_info = new QLabel(costumerBox);
  actions_layout->addWidget(costumer_info);
  costumerLayout->addLayout(actions_layout, 4, 0, 1, 3, Qt::AlignLeft);
  costumerBox->setLayout(costumerLayout);
  layout->addWidget(costumerBox);
  // END

  layout->addStretch(1);
  setLayout(layout);

  connect(btn_search_costumer, SIGNAL(clicked()), this,
          SLOT(searchSqlCostumer()));
  connect(btn_create_costumer, SIGNAL(clicked()), this,
          SLOT(createSqlCostumer()));
}

const QHash<QString, QVariant> BuchfreundDisplay::createDataset() {
  QHash<QString, QVariant> data;
  MessageBox messanger(this);
  QList<UtilsMain *> list =
      findChildren<UtilsMain *>(p_objPattern, Qt::FindChildrenRecursively);
  QList<UtilsMain *>::Iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    UtilsMain *cur = *it;
    if (cur->isRequired() && !cur->isValid()) {
      messanger.notice(cur->notes());
      cur->setFocus();
      data.clear();
      return data;
    }
    data.insert(cur->objectName(), cur->value());
  }
  list.clear();
  return data;
}

void BuchfreundDisplay::createSqlCostumer() {
  QHash<QString, QVariant> data = createDataset();
  if (data.size() < 3)
    return;

  QStringList column; /**< SQL Columns */
  QStringList values; /**< SQL Values */
  QHash<QString, QVariant>::iterator it;
  for (it = data.begin(); it != data.end(); ++it) {
    if (it.value().toString().isEmpty())
      continue;

    column.append(it.key());
    if (it.value().type() == QVariant::String) {
      values.append("'" + it.value().toString() + "'");
    } else {
      values.append(it.value().toString());
    }
  }

  QString sql("INSERT INTO costumers (");
  sql.append(column.join(","));
  sql.append(",c_since,c_changed) VALUES (");
  sql.append(values.join(","));
  sql.append(",CURRENT_TIMESTAMP,CURRENT_TIMESTAMP)");
  sql.append(" RETURNING c_id;");
  if (SHOW_SQL_QUERIES) {
    qDebug() << Q_FUNC_INFO << sql;
  }
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    q.next();
    if (q.value("c_id").toInt() > 0) {
      int cid = q.value("c_id").toInt();
      c_id->setValue(cid);
      emit s_costumer(cid);
      costumer_info->setText(tr("costumer add"));
      btn_create_costumer->setEnabled(false);
    }
  } else {
    QString errors = m_sql->lastError();
    if (!errors.isEmpty()) {
      costumer_info->setText(tr("SQL errors!"));
      qDebug() << Q_FUNC_INFO << errors;
    }
  }
}

void BuchfreundDisplay::searchSqlCostumer() {
  QString buffer;
  QString sql("SELECT c_id FROM costumers WHERE ");
  buffer = c_firstname->value().toString();
  sql.append("c_firstname ILIKE '" + buffer + "'");
  sql.append(" AND ");
  buffer = c_lastname->value().toString();
  sql.append("c_lastname ILIKE '" + buffer + "'");
  sql.append(" AND ");
  buffer = c_postalcode->value().toString();
  sql.append("c_postalcode ILIKE '" + buffer + "'");
  sql.append(" AND ");
  buffer = c_location->value().toString();
  sql.append("c_location ILIKE '" + buffer + "'");
  sql.append(" ORDER BY c_id;");
  if (SHOW_SQL_QUERIES) {
    qDebug() << Q_FUNC_INFO << sql;
  }
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    q.next();
    if (q.value("c_id").toInt() > 0) {
      int cid = q.value("c_id").toInt();
      c_id->setValue(cid);
      emit s_costumer(cid);
      costumer_info->setText(tr("found Costumer"));
    }
  } else {
    QString errors = m_sql->lastError();
    if (errors.isEmpty()) {
      btn_create_costumer->setEnabled(true);
      costumer_info->setText(tr("Costumer not exists!"));
    } else {
      costumer_info->setText(tr("SQL errors!"));
      qDebug() << Q_FUNC_INFO << errors;
    }
  }
}

void BuchfreundDisplay::checkArticleId() {
  if (a_article_id->value().toInt() < 1)
    return;

  QString sql = queryArticleExists(a_article_id->value().toString());
  if (SHOW_SQL_QUERIES) {
    qDebug() << Q_FUNC_INFO << sql;
  }
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    q.next();
    if (q.value("a_article_id").toString().isEmpty()) {
      qDebug() << Q_FUNC_INFO << m_sql->lastError();
    }
  } else {
    emit s_warning(tr("Article is not available!"));
  }
}

void BuchfreundDisplay::setContent(const QJsonDocument &doc) {
  if (doc.isEmpty())
    return;

  int errors = QJsonValue(doc["error"]).toBool();
  if (errors)
    return;

  BF_Translater bfTr;
  QJsonObject response = QJsonValue(doc["response"]).toObject();
  if (!response.isEmpty()) {
    QJsonObject::iterator it; // Iterator
    for (it = response.begin(); it != response.end(); ++it) {
      QString f = bfTr.sqlParam(it.key());
      QJsonValue val = it.value();
      if (!f.isEmpty() && !val.toString().isEmpty()) {
        setValue(f, val.toVariant());
      }
    }
    QJsonObject shippingsaddr =
        QJsonValue(doc["response"]["rechnungsadresse"]).toObject();
    if (!shippingsaddr.isEmpty()) {
      for (it = shippingsaddr.begin(); it != shippingsaddr.end(); ++it) {
        QString f = bfTr.sqlParam(it.key());
        QJsonValue val = it.value();
        if (!f.isEmpty() && !val.toString().isEmpty()) {
          setValue(f, val.toVariant());
        }
      }
    }
  }

  QJsonArray positionen = QJsonValue(doc["response"]["positionen"]).toArray();
  if (positionen.count() > 0) {
    QJsonArray::iterator at;
    for (at = positionen.begin(); at != positionen.end(); ++at) {
      QJsonObject article = (*at).toObject();
      QJsonObject::iterator it;
      for (it = article.begin(); it != article.end(); ++it) {
        QString f = bfTr.sqlParam(it.key());
        QVariant val = it.value().toVariant();
        if (!f.isEmpty() && !val.isNull()) {
          setValue(f, val);
        }
      }
    }
  }
  // Jetzt nach Kunde suchen
  searchSqlCostumer();
  // Artikel Prüfen
  checkArticleId();
}

void BuchfreundDisplay::setValue(const QString &objName,
                                 const QVariant &value) {
  if (objName == "a_title") {
    a_title->setText(value.toString());
  }
  if (objName == "a_modified") {
    a_modified->setValue(value.toDateTime());
  }
  UtilsMain *obj = findChild<UtilsMain *>(objName, Qt::FindChildrenRecursively);
  if (obj != nullptr) {
    obj->setValue(value);
    return;
  }
}

Buchfreund::Buchfreund(QWidget *parent)
    : QSplitter{parent}, configGroup("provider/whsoft") {
  setObjectName("inventory_buchfreund");
  setOrientation(Qt::Horizontal);
  setWindowTitle("buchfreund.de");

  QScrollArea *scroolArea = new QScrollArea(this);
  scroolArea->setWidgetResizable(true);
  insertWidget(0, scroolArea);

  bfDisplay = new BuchfreundDisplay(scroolArea);
  scroolArea->setWidget(bfDisplay);

  bfList = new Buchfreundlist(this);
  bfList->setMinimumWidth(150);
  insertWidget(1, bfList);

  setStretchFactor(0, 70);
  setStretchFactor(1, 30);

  // SIGNALS
  connect(bfList, SIGNAL(orderClicked(const QJsonDocument &)), this,
          SLOT(getOrderContent(const QJsonDocument &)));

  testing();
}

void Buchfreund::testing() {
  QFile fp("/tmp/testfile.json");
  if (fp.open(QIODevice::ReadOnly)) {
    QTextStream r(&fp);
    QString data(r.readAll());
    QJsonDocument jDoc = QJsonDocument::fromJson(data.toLocal8Bit());
    fp.close();

    bfDisplay->setContent(jDoc);
  }
}

const QUrl Buchfreund::apiQuery(const QString &operation) {
  QRegExp pattern("([\\/]{2,})");
  ApplSettings cfg;

  QUrl url;
  cfg.beginGroup(configGroup);
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

void Buchfreund::queryListViewContent() {
  QDateTime curDt = QDateTime::currentDateTime();
  QTime t(curDt.time().hour(), curDt.time().minute(), 0);
  curDt.setTime(t);
  QDateTime from = curDt.addDays(-14);
  QJsonObject obj;
  obj.insert("datum_von",
             QJsonValue::fromVariant(from.toString(BF_DATE_FORMAT)));
  obj.insert("datum_bis",
             QJsonValue::fromVariant(curDt.toString(BF_DATE_FORMAT)));
  QJsonDocument doc(obj);
  QByteArray data = doc.toJson(QJsonDocument::Compact);
  QUrl url = apiQuery("bestellungen");
  Provider *prQuery = new Provider(this, false);
  prQuery->setObjectName("buchfreund_query_list");
  connect(prQuery, SIGNAL(responsed(const QJsonDocument &)), bfList,
          SLOT(setView(const QJsonDocument &)));
  connect(prQuery, SIGNAL(finished()), prQuery, SLOT(deleteLater()));

  prQuery->sendPost(url, data);
}

void Buchfreund::getOpenOrders() { queryListViewContent(); }

void Buchfreund::getOrderContent(const QJsonDocument &doc) {
  QByteArray data = doc.toJson(QJsonDocument::Compact);
  QUrl url = apiQuery("bestellung");
  Provider *prQuery = new Provider(this, false);
  prQuery->setObjectName("buchfreund_query_view");
  connect(prQuery, SIGNAL(responsed(const QJsonDocument &)), bfDisplay,
          SLOT(setContent(const QJsonDocument &)));
  connect(prQuery, SIGNAL(finished()), prQuery, SLOT(deleteLater()));

  prQuery->sendPost(url, data);
}
