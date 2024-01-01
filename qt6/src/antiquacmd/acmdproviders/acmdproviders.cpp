// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "acmdproviders.h"
#include "acmdcustomers.h"

#include <QByteArray>
#include <QRegularExpression>

ACmdProviders::ACmdProviders(AntiquaCRM::NetworkQueryType type, QObject *parent)
    : QObject{parent} {
  setObjectName("acmdproviders");
  cfg = new AntiquaCRM::ASettings("antiquacmd", this);
  pgsql = new AntiquaCRM::ASqlCore(this);
  netw = new AntiquaCRM::ANetworker(type, this);

  // Verlaufsabfrage
  if (!cfg->contains("history_query"))
    cfg->setValue("history_query", -3);

  history_query = cfg->value("history_query", -3).toInt();

  connect(netw, SIGNAL(sendJsonResponse(const QJsonDocument &)),
          SLOT(getNetworkResponse(const QJsonDocument &)));

  connect(netw, SIGNAL(sendXmlResponse(const QDomDocument &)),
          SLOT(getNetworkResponse(const QDomDocument &)));
}

ACmdProviders::~ACmdProviders() {
  pgsql->deleteLater();
  cfg->deleteLater();
  netw->deleteLater();
}

void ACmdProviders::getNetworkResponse(const QJsonDocument &doc) {
  if (doc.isEmpty())
    return;

  QByteArray data = doc.toJson(QJsonDocument::Compact);
  if (data.size() > 32)
    responsed(data);
}

void ACmdProviders::getNetworkResponse(const QDomDocument &doc) {
  if (doc.isNull())
    return;

  QByteArray data = doc.toByteArray(-1);
  if (data.size() > 32)
    responsed(data);
}

QMap<QString, int> ACmdProviders::initDataInformation() {
  QString _query("SELECT column_name, character_maximum_length");
  _query.append(" FROM information_schema.columns WHERE table_name IN");
  _query.append(" ('customers', 'provider_orders','article_orders')");
  _query.append(" AND data_type='character varying'");
  _query.append(" ORDER BY column_name;");

  QMap<QString, int> _m;
  QSqlQuery _q = pgsql->query(_query);
  if (_q.size() > 0) {
    while (_q.next()) {
      QSqlRecord _r = _q.record();
      for (int i = 0; i < _r.count(); i++) {
        const QString _k = _q.value("column_name").toString();
        int _v = _q.value("character_maximum_length").toInt();
        _m.insert(_k, _v);
      }
    }
    return _m;
  }
  return _m;
}

const QString ACmdProviders::ucFirst(const QString &str) {
  QString convert = str.trimmed().toLower();
  convert = convert.replace("-", " ");
  QStringList array = convert.split(" ", Qt::SkipEmptyParts);
  for (int i = 0; i < array.size(); i++) {
    array[i].replace(0, 1, array[i][0].toUpper());
  }
  return array.join(" ");
}

int ACmdProviders::convertGender(const QString &gender) const {
  QString search = gender.toLower().trimmed();
  QStringList female({"ms.", "mss", "mses", "madam", "frau", "freifrau"});
  QStringList male({"mr", "mister", "sir", "herr", "herrn", "freiherr"});
  AntiquaCRM::Gender crm_gender;
  if (female.contains(search)) {
    crm_gender = AntiquaCRM::Gender::FEMALE;
  } else if (male.contains(search)) {
    crm_gender = AntiquaCRM::Gender::MALE;
  } else if (gender.contains("diverse")) {
    crm_gender = AntiquaCRM::Gender::VARIOUS;
  } else {
    crm_gender = AntiquaCRM::Gender::NO_GENDER;
  }
  return static_cast<int>(crm_gender);
}

const QString ACmdProviders::findBCP47(const QString &country) const {
  QString sql("SELECT rc_iso2 FROM ref_countries WHERE ");
  sql.append("rc_country_en ILIKE '" + country + "'");
  sql.append("OR rc_country_de ILIKE '" + country + "';");
  QStringList found;
  QSqlQuery q = pgsql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      found << q.value("rc_iso2").toString();
    }
  }
  if (found.size() < 1)
    return "DE";

  if (found.size() > 1)
    qInfo("Providers (%d): Found more then one BCP47 Code use first!",
          __LINE__);

  return found.first();
}

const QString ACmdProviders::getCountry(const QString &bcp47) const {
  QString sql("SELECT COALESCE(rc_country_de, rc_country_en)");
  sql.append("FROM ref_countries WHERE rc_iso2='" + bcp47 + "';");
  QStringList found;
  QSqlQuery q = pgsql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      found << q.value(0).toString();
    }
  }
  if (found.size() < 1)
    return tr("Germany");

  if (found.size() > 1) {
    qInfo("Providers (%d): Found more then one Country, use first!", __LINE__);
  }
  return found.first();
}

const QDateTime ACmdProviders::getDateTime(const QString &dateString,
                                           const QString &timeString,
                                           Qt::TimeSpec spec) const {
  QDateTime dateTime;
  QDate d = QDate::fromString(dateString, "yyyy-MM-dd");
  dateTime.setDate(d);
  QTime t = QTime::fromString(timeString, "HH:mm:ss");
  dateTime.setTime(t);
  dateTime.setTimeSpec(spec);
  return dateTime;
}

const QDateTime ACmdProviders::getDateTime(const QString &dateTimeString,
                                           Qt::TimeSpec spec) const {
  QDateTime dateTime;
  dateTime = QDateTime::fromString(dateTimeString, "yyyy-MM-dd HH:mm:ss");
  dateTime.setTimeSpec(spec);
  return dateTime;
}

const QDateTime ACmdProviders::timeSpecDate(const QDateTime &dateTime,
                                            Qt::TimeSpec fromSpec) const {
  QDateTime dt(dateTime);
  dt.setTimeSpec(fromSpec);
  return dt;
}

const QJsonValue ACmdProviders::convert(const QString &field,
                                        const QJsonValue &value) const {
  if (field == "a_article_id") {
    if (value.type() == QJsonValue::String) {
      QString str = value.toString();
      return str.toInt();
    } else {
      return value.toInt();
    }
  }

  if (field.contains("_price")) {
    if (value.type() == QJsonValue::String) {
      QString str = value.toString();
      return str.toDouble();
    } else {
      return value.toDouble();
    }
  }

  switch (value.type()) {
  case QJsonValue::Bool:
    return value.toBool();

  case QJsonValue::Double:
    return value.toInt();

  case QJsonValue::String:
    return value.toString();

  default:
    break;
  };

  qWarning("Providers (%d): Invalid type detected!", __LINE__);
  return QString();
}

const QStringList ACmdProviders::currProviderIds(const QString &provider) {
  QStringList ids;
  if (provider.isEmpty()) {
    qWarning("Providers (%d): Query field 'pr_name' is empty!", __LINE__);
    return ids;
  }

  QString sql("SELECT pr_order FROM provider_orders");
  sql.append(" WHERE pr_name='" + provider + "';");
  QSqlQuery q = pgsql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      ids << q.value("pr_order").toString();
    }
  }
  return ids;
}

bool ACmdProviders::createOrders(const QList<QJsonObject> &orders) {
  if (orders.size() < 1)
    return false;

  QStringList inserts;
  QListIterator<QJsonObject> it(orders);
  while (it.hasNext()) {
    QJsonObject order = it.next();
    QString pr_order = order.value("orderid").toString();
    Q_ASSERT(!(pr_order.isEmpty()));

    QString pr_name = order.value("provider").toString();
    Q_ASSERT(!(pr_name.isEmpty()));

    QJsonObject customer = order.value("customer").toObject();
    QPair<qint64, QString> c_data = findInsertCustomer(customer);
    Q_ASSERT(!(c_data.first < 1));

    QString pr_buyer = c_data.second;
    Q_ASSERT(!(pr_buyer.isEmpty()));

    QJsonDocument doc(order);
    QString pr_order_data = doc.toJson(QJsonDocument::Compact);
    QString sql("INSERT INTO provider_orders (");
    sql.append("pr_name");
    sql.append(",pr_order");
    sql.append(",pr_buyer");
    sql.append(",pr_customer_id");
    sql.append(",pr_order_id");
    sql.append(",pr_created");
    sql.append(",pr_order_data");
    sql.append(") VALUES (");
    sql.append("'" + pr_name + "'");
    sql.append(",'" + pr_order + "'");
    sql.append(",'" + pr_buyer + "'");
    sql.append("," + QString::number(c_data.first));
    sql.append(",0");
    sql.append(",CURRENT_TIMESTAMP");
    sql.append(",'" + pr_order_data + "');");
    inserts.append(sql);
  }

  pgsql->query(inserts.join("\n"));
  if (!pgsql->lastError().isEmpty()) {
    qWarning("Providers (%d): Insert answers with errors!", __LINE__);
    return false;
  }

  return true;
}

QPair<qint64, QString>
ACmdProviders::findInsertCustomer(const QJsonObject &json) {
  ACmdCustomers *mc = new ACmdCustomers(pgsql, json);
  qint64 c_id = mc->getId();
#ifdef ANTIQUA_DEVELOPEMENT
  if (c_id < 1)
    qDebug() << Q_FUNC_INFO << c_id;
#endif

  Q_ASSERT(c_id > 1);

  QPair<qint64, QString> out;
  out.first = c_id;
  out.second = mc->buyerName();
  return out;
}
