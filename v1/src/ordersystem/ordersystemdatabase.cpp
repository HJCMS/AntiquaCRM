// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordersystemdatabase.h"

#include <QDebug>

OrderSystemDatabase::OrderSystemDatabase(
    const QString &provider, const AntiquaCRM::AProviderOrders orders)
    : QThread{}, p_provider{provider}, p_orders{orders} {
  m_sql = new AntiquaCRM::ASqlCore(this);
}

qint64
OrderSystemDatabase::createCustomers(AntiquaCRM::AProviderOrder &order) const {
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
  QSqlQuery q = m_sql->query(sql);
  if (q.size() == 1) {
    q.next();
    return q.value(0).toInt();
  } else if (!m_sql->lastError().isEmpty()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
#else
    qWarning("OrderSystemDatabase: SQL error occurred.");
#endif
  }
  return -1;
}

void OrderSystemDatabase::findCustomers(const QString &orderId) const {
  QStringList findIn({"c_firstname", "c_lastname"});
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
        qint64 customerId = createCustomers(data);
        if (customerId > 0) {
          QString info = QString::number(customerId);
          qInfo("Create Customer with Id:%s.", qPrintable(info));
        }
      }
    }
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
      findCustomers(pr_id);
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
  foreach (QString id, orderIds) {
    qDebug() << Q_FUNC_INFO << p_provider << id;
  }
  return false;
}

void OrderSystemDatabase::run() {
  QStringList newOrders = findNewOrders();
  if (newOrders.size() > 0) {
    emit sendNewOrdersFound();
    if (insertNewOrders(newOrders)) {
      quit();
    }
  }
}
