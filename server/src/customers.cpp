// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customers.h"

#include <QJsonArray>

Customers::Customers(SqlPsql *db, const QJsonObject &obj)
    : QJsonObject{obj}, m_sql{db} {}

qint64 Customers::sqlInsert() {
  qint64 cutomer_id = -1;
  QStringList fields;
  QStringList inserts;
  foreach (QString k, keys()) {
    QVariant _value = value(k).toVariant();
    QVariant::Type _type = _value.type();
    if (!_value.isNull()) {
      fields.append(k);
      switch (_type) {
      case QVariant::Bool:
        inserts.append((_value.toBool() ? "true" : "false"));
        break;

      case QVariant::Int:
      case QVariant::LongLong:
        inserts.append(QString::number(_value.toInt()));
        break;

      case QVariant::Double:
        inserts.append(QString::number(_value.toDouble()));
        break;

      default:
        inserts.append("'" + _value.toString() + "'");
      };
    }
  }

  QString sql("INSERT INTO customers (");
  sql.append(fields.join(","));
  sql.append(") VALUES (");
  sql.append(inserts.join(","));
  sql.append(") RETURNING c_id;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    q.next();
    cutomer_id = q.value("c_id").toInt();
  } else {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << sql << Qt::endl << m_sql->lastError();
#else
    qWarning("SQL Customer insert answers with errors!");
#endif
  }

  return cutomer_id;
}

qint64 Customers::sqlQuery() {
  QString buyer_name = value("c_provider_import").toString();
  QStringList findIn({"c_firstname", "c_lastname"});
  QStringList search;
  foreach (QString k, findIn) {
    QString str = value(k).toString();
    if (!str.isEmpty())
      search << k + " ILIKE '" + str + "'";
  }

  QString sql("SELECT c_id FROM customers WHERE (");
  sql.append(search.join(" AND "));
  sql.append(" AND c_postalcode='");
  sql.append(value("c_postalcode").toString() + "'");
  sql.append(") OR (c_provider_import ILIKE '");
  sql.append(buyer_name);
  sql.append("');");

  QList<qint64> ids;
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      ids.append(q.value("c_id").toInt());
    }
  } else if (!m_sql->lastError().isEmpty()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
#else
    qWarning("SQL Customer query answers with errors!");
#endif
    return -1;
  } else {
    qInfo("Customer: %s not found!", qPrintable(buyer_name));
    return -1;
  }

  if (ids.size() > 1) {
    qWarning("Found more then one customers, use first entry!");
  }

  return ids.first();
}

const QString Customers::buyerName() {
  QString name = value("c_provider_import").toString();
  if (name.isEmpty()) {
    name = value("c_firstname").toString();
    name.append(" ");
    name.append(value("c_lastname").toString());
  }
  return name;
}

qint64 Customers::getId() {
  qint64 id = sqlQuery();
  if (id < 1)
    id = sqlInsert();

  return id;
}
