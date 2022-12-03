// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "provider.h"
#include "customers.h"
#include "networker.h"
#include "settings.h"
#include "sqlpsql.h"

Provider::Provider(QObject *parent) : QObject{parent} {
  m_config = new Settings(this);
  m_networker = new Networker(this);
  m_sql = new SqlPsql(this);
}

const QString Provider::ucFirst(const QString &str) {
  QStringList array = str.trimmed().split(" ", Qt::SkipEmptyParts);
  for (int i = 0; i < array.size(); i++) {
    array[i].replace(0, 1, array[i][0].toUpper());
  }
  return array.join(" ");
}

AntiquaCRM::Gender Provider::convertGender(const QString &from) const {
  QStringList female({"ms.", "mss", "mses", "madam", "frau", "freifrau"});
  QStringList male({"mr", "mister", "sir", "herr", "herrn", "freiherr"});
  if (female.contains(from, Qt::CaseInsensitive))
    return AntiquaCRM::Gender::FEMALE;
  else if (male.contains(from, Qt::CaseInsensitive))
    return AntiquaCRM::Gender::MALE;
  else if (from.contains("diverse", Qt::CaseInsensitive))
    return AntiquaCRM::Gender::VARIOUS;
  else
    return AntiquaCRM::Gender::NO_GENDER;
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

double Provider::getPrice(const QJsonValue &value) const {
  if (value.type() == QJsonValue::Double)
    return value.toDouble();

  if (value.type() == QJsonValue::String) {
    QString str = value.toString();
    if (!str.isEmpty())
      return str.toDouble();
  }
  return 0.00;
}

qint64 Provider::convertArticleId(const QJsonValue &value) const {
  if (value.type() == QJsonValue::String) {
    QString str = value.toString();
    if (!str.isEmpty())
      return str.toInt();
  }
  return value.toInt();
}

const QStringList Provider::currentOrderIds(const QString &provider) {
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
    QJsonObject customer = order.value("customer").toObject();
    QPair<qint64, QString> c_data = findInsertCustomer(customer);
    QString pr_buyer = c_data.second;
    if (c_data.first < 1) {
      qWarning("Canceled for '%s' no customer Id.", qPrintable(pr_order));
      continue;
    }

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
    sql.append("'Booklooker'");
    sql.append(",'" + pr_order + "'");
    sql.append(",'" + pr_buyer + "'");
    sql.append("," + QString::number(c_data.first));
    sql.append(",0");
    sql.append(",CURRENT_TIMESTAMP");
    sql.append(",'" + pr_order_data + "');");
    inserts.append(sql);
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << inserts.size();
#endif

  m_sql->query(inserts.join("\n"));
  if (!m_sql->lastError().isEmpty()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << m_sql->lastError() << Qt::endl;
#else
    qWarning("SQL Provider insert answers with errors!");
#endif
    return false;
  }

  return true;
}

QPair<qint64, QString> Provider::findInsertCustomer(const QJsonObject &json) {
  Customers *mc = new Customers(m_sql, json);
  qint64 c_id = mc->getId();

#ifdef ANTIQUA_DEVELOPEMENT
  if (c_id < 1)
    qDebug() << Q_FUNC_INFO << c_id;

  Q_ASSERT(c_id > 1);
#endif

  QPair<qint64, QString> out;
  out.first = c_id;
  out.second = mc->buyerName();
  return out;
}
