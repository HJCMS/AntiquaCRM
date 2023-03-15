// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "provider.h"
#include "customers.h"

Provider::Provider(AntiquaCRM::NetworkQueryType type, QObject *parent)
    : QObject{parent} {
  m_config = new AntiquaCRM::ASettings("antiquacmd", this);
  m_sql = new AntiquaCRM::ASqlCore("antiquacmd", this);
  m_networker = new AntiquaCRM::ANetworker(type, this);

  // Verlaufsabfrage
  if (!m_config->contains("history_query"))
    m_config->setValue("history_query", -3);

  history_query = m_config->value("history_query", -3).toInt();

  connect(m_networker, SIGNAL(sendJsonResponse(const QJsonDocument &)),
          SLOT(getNetworkResponse(const QJsonDocument &)));
  connect(m_networker, SIGNAL(sendXmlResponse(const QDomDocument &)),
          SLOT(getNetworkResponse(const QDomDocument &)));
}

void Provider::getNetworkResponse(const QJsonDocument &doc) {
  if (doc.isEmpty())
    return;

  QByteArray data = doc.toJson(QJsonDocument::Compact);
  if (data.size() > 32)
    responsed(data);
}

void Provider::getNetworkResponse(const QDomDocument &doc) {
  if (doc.isNull())
    return;

  QByteArray data = doc.toByteArray(-1);
  if (data.size() > 32)
    responsed(data);
}

const QString Provider::ucFirst(const QString &str) {
  QString convert = str.trimmed().toLower();
  // Wenn mehr als ein Leerzeichen, zusammenschieben!
  QRegExp pattern("\\b\\s\\b");
  pattern.indexIn(convert);
  if (pattern.captureCount() > 1)
    convert.replace(pattern, "");

  // qDebug() << str << convert;
  QStringList array = convert.split(" ", Qt::SkipEmptyParts);
  for (int i = 0; i < array.size(); i++) {
    array[i].replace(0, 1, array[i][0].toUpper());
  }
  return array.join(" ");
}

int Provider::convertGender(const QString &gender) const {
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

const QString Provider::findBCP47(const QString &country) const {
  QString sql("SELECT rc_iso2 FROM ref_countries WHERE ");
  sql.append("rc_country_en ILIKE '" + country + "'");
  sql.append("OR rc_country_de ILIKE '" + country + "';");
  QStringList found;
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      found << q.value("rc_iso2").toString();
    }
  }
  if (found.size() < 1)
    return "DE";

  if (found.size() > 1) {
    qWarning("Found more then one BCP47 Code use first!");
  }
  return found.first();
}

const QString Provider::getCountry(const QString &bcp47) const {
  QString sql("SELECT COALESCE(rc_country_de, rc_country_en)");
  sql.append("FROM ref_countries WHERE rc_iso2='" + bcp47 + "';");
  QStringList found;
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      found << q.value(0).toString();
    }
  }
  if (found.size() < 1)
    return "Deutschland";

  if (found.size() > 1) {
    qWarning("Found more then one Countries use first!");
  }
  return found.first();
}

const QDateTime Provider::getDateTime(const QString &dateString,
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

const QDateTime Provider::getDateTime(const QString &dateTimeString,
                                      Qt::TimeSpec spec) const {
  QDateTime dateTime;
  dateTime = QDateTime::fromString(dateTimeString, "yyyy-MM-dd HH:mm:ss");
  dateTime.setTimeSpec(spec);
  return dateTime;
}

const QDateTime Provider::timeSpecDate(const QDateTime &dateTime,
                                       Qt::TimeSpec fromSpec) const {
  QDateTime dt(dateTime);
  dt.setTimeSpec(fromSpec);
  return dt;
}

const QJsonValue Provider::convert(const QString &key,
                                   const QJsonValue &value) const {
  if (key == "a_article_id") {
    if (value.type() == QJsonValue::String) {
      QString str = value.toString();
      return str.toInt();
    } else {
      return value.toInt();
    }
  }

  if (key.contains("_price")) {
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

  default:
    return value.toString();
  };

  return value;
}

const QStringList Provider::currProviderIds(const QString &provider) {
  QStringList ids;
  if (provider.isEmpty()) {
    qWarning("Required query field 'pr_name' is empty!");
    return ids;
  }

  QString sql("SELECT pr_order FROM provider_orders");
  sql.append(" WHERE pr_name='" + provider + "';");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      ids << q.value("pr_order").toString();
    }
  }
  return ids;
}

bool Provider::createOrders(const QList<QJsonObject> &orders) {
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

  m_sql->query(inserts.join("\n"));
  if (!m_sql->lastError().isEmpty()) {
    qWarning("SQL Provider insert answers with errors!");
    return false;
  }

  return true;
}

QPair<qint64, QString> Provider::findInsertCustomer(const QJsonObject &json) {
  Customers *mc = new Customers(m_sql, json);
  qint64 c_id = mc->getId();
  //#ifdef ANTIQUA_DEVELOPEMENT
  //  if (c_id < 1)
  //    qDebug() << Q_FUNC_INFO << c_id;
  //#endif

  Q_ASSERT(c_id > 1);

  QPair<qint64, QString> out;
  out.first = c_id;
  out.second = mc->buyerName();
  return out;
}
