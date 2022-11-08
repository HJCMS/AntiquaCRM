// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordersystemdatabase.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>

OrderSystemDatabase::OrderSystemDatabase(
    const QString &provider, const AntiquaCRM::AProviderOrders orders)
    : QThread{}, p_provider{provider}, p_orders{orders} {
  m_sql = new AntiquaCRM::ASqlCore(this);
}

bool OrderSystemDatabase::saveCacheFile(AntiquaCRM::AProviderOrder &data) {
  QJsonObject jObj;
  jObj.insert("provider", data.provider());
  jObj.insert("orderid", data.id());
  QJsonObject customerData;
  QHashIterator<QString, QMetaType::Type> cit(data.customerKeys());
  while(cit.hasNext()) {
    cit.next();
    QString key = cit.key();
    QVariant val = data.getValue(key);
    if (val.isNull())
      continue;

    customerData.insert(key, QJsonValue::fromVariant(val));
  }
  jObj.insert("customer", customerData);

  QJsonObject orderInfo;
  QHashIterator<QString, QMetaType::Type> pit(data.orderKeys());
  while(pit.hasNext()) {
    pit.next();
    QString key = pit.key();
    QVariant val = data.getValue(key);
    if (val.isNull())
      continue;

    orderInfo.insert(key, QJsonValue::fromVariant(val));
  }
  jObj.insert("orderinfo", orderInfo);

  QJsonArray articleArray;
  QListIterator<AntiquaCRM::OrderArticleItems> articles(data.orders());
  while(articles.hasNext()) {
    QJsonObject articleData;
    QListIterator<AntiquaCRM::ArticleOrderItem> ait(articles.next());
    while(ait.hasNext()) {
      AntiquaCRM::ArticleOrderItem item = ait.next();
      articleData.insert(item.key, QJsonValue::fromVariant(item.value));
    }
    articleArray.append(articleData);
  }
  jObj.insert("articles", articleArray);

  QJsonDocument jDoc(jObj);
  AntiquaCRM::ASharedCacheFiles file;
  // Usage: Indented | Compact
  QByteArray stream = jDoc.toJson(QJsonDocument::Indented);
  return file.storeTempFile(data.md5sum() + ".json", stream);
}

const QString OrderSystemDatabase::createCustomerInsert(
    AntiquaCRM::AProviderOrder &order) const {
  QStringList fields;
  QStringList inserts;
  QHashIterator<QString, QMetaType::Type> ci(order.customerKeys());
  while (ci.hasNext()) {
    ci.next();
    QVariant buffer = order.getValue(ci.key());
    if (buffer.isNull())
      continue;

    fields << ci.key();
    if (ci.value() == QMetaType::QString) {
      inserts << "'" + buffer.toString() + "'";
    } else if (ci.value() == QMetaType::Double) {
      double d = buffer.toDouble();
      inserts << QString::number(d);
    } else if (ci.value() == QMetaType::Int) {
      qint64 i = buffer.toDouble();
      inserts << QString::number(i);
    }
  }

  QString sql("INSERT INTO customers (");
  sql.append(fields.join(","));
  sql.append(") VALUES (");
  sql.append(inserts.join(","));
  sql.append(") RETURNING c_id;");
  return sql;
}

void OrderSystemDatabase::findCreateCustomer(const QString &orderId) const {
  QStringList findIn({"c_firstname", "c_lastname"});
  QStringList inserts;
  QListIterator<AntiquaCRM::AProviderOrder> get_orders(p_orders);
  while (get_orders.hasNext()) {
    AntiquaCRM::AProviderOrder data = get_orders.next();
    if (orderId == data.id()) {
      QString sql("SELECT c_id FROM customers WHERE (");
      QStringList search;
      foreach (QString k, findIn) {
        QString str = data.getValue(k).toString();
        if (!str.isEmpty())
          search << k + " ILIKE '" + str + "'";
      }
      sql.append(search.join(" AND "));
      sql.append(" AND c_postalcode='");
      sql.append(data.getValue("c_postalcode").toString() + "'");
      sql.append(") OR (c_provider_import ILIKE '");
      sql.append(data.getValue("c_provider_import").toString());
      sql.append("');");
      if (m_sql->query(sql).size() == 0) {
        inserts << createCustomerInsert(data);
      }
    }
  }

  if (inserts.size() < 1)
    return;

  m_sql->query(inserts.join("\n"));
  if (!m_sql->lastError().isEmpty()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
#else
    qWarning("OrderSystemDatabase: SQL error occurred.");
#endif
  }
}

const QStringList OrderSystemDatabase::findNewOrders() const {
  QStringList newOrdersList;
  QStringList querylist;
  QListIterator<AntiquaCRM::AProviderOrder> get_orders(p_orders);
  while (get_orders.hasNext()) {
    AntiquaCRM::AProviderOrder data = get_orders.next();
    querylist << data.id();
  }

  foreach (QString pr_id, querylist) {
    QString sql("SELECT pr_order FROM provider_order_history WHERE");
    sql.append(" pr_name='" + p_provider + "' ");
    sql.append(" AND pr_order IN ('" + pr_id + "') LIMIT 1;");
    if (m_sql->query(sql).size() == 0) {
      findCreateCustomer(pr_id);
      newOrdersList << pr_id;
    } else if (!m_sql->lastError().isEmpty()) {
#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << Q_FUNC_INFO << m_sql->lastError();
#else
      qWarning("OrderSystemDatabase: SQL error occurred.");
#endif
    }
  }

  return newOrdersList;
}

bool OrderSystemDatabase::insertNewOrders(const QStringList &orderIds) {
  QStringList queryList;
  foreach (QString id, orderIds) {
    QListIterator<AntiquaCRM::AProviderOrder> get_orders(p_orders);
    while (get_orders.hasNext()) {
      AntiquaCRM::AProviderOrder data = get_orders.next();
      if (id == data.id()) {
        QString sql("INSERT INTO provider_order_history (");
        sql.append("pr_name,pr_order,pr_buyer,pr_datetime,pr_key");
        sql.append(") VALUES (");
        sql.append("'" + p_provider + "','" + id + "','");
        sql.append(data.getValue("c_provider_import").toString());
        sql.append("','");
        QDateTime dt = data.getValue("o_since").toDateTime();
        sql.append(dt.toString(ANTIQUACRM_TIMESTAMP_IMPORT));
        sql.append("','");
        sql.append(data.md5sum());
        sql.append("');");
        queryList << sql;
      }
    }
  }

  m_sql->query(queryList.join("\n"));
  if (!m_sql->lastError().isEmpty()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
#else
    qWarning("OrderSystemDatabase: SQL error occurred.");
#endif
    return false;
  }

  return true;
}

bool OrderSystemDatabase::createCacheFiles(const QStringList &orderIds) {
  QListIterator<AntiquaCRM::AProviderOrder> get_orders(p_orders);
  while (get_orders.hasNext()) {
    AntiquaCRM::AProviderOrder data = get_orders.next();
    if (!orderIds.contains(data.id()))
      continue;

    if (!saveCacheFile(data))
      break;
  }
  return true;
}

void OrderSystemDatabase::run() {
  QStringList newOrders = findNewOrders();
  createCacheFiles(QStringList("28049149"));
  if (newOrders.size() > 0) {
    emit sendNewOrdersFound();
    if (insertNewOrders(newOrders)) {
      if (createCacheFiles(newOrders))
        quit();
    }
  }
}
