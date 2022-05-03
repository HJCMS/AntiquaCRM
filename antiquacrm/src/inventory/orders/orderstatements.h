// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_ORDERSTATEMENTS_H
#define INVENTORY_ORDERSTATEMENTS_H

#include <QObject>
#include <QString>

// Schalte SQL ausgaben ein
#ifndef SHOW_SQL_QUERIES
#define SHOW_SQL_QUERIES false
#endif

static const QString defaultQuery(int id = 0) {
  QString hh(QObject::tr("Hours"));
  QString dd(QObject::tr("Days"));
  QString mm(QObject::tr("Months"));
  // QString yy(QObject::tr("Years"));
  QString fs("a.o_id,a.o_since,a.o_order_status,a.o_payment_status,");
  fs.append("CASE WHEN c.c_company=true THEN c.c_company_name ELSE ");
  fs.append("concat_ws(' ',c.c_firstname,c.c_lastname) END AS costumer,");
  fs.append("d.d_name,a.o_locked,a.o_closed,");
  fs.append("TO_CHAR(age(CURRENT_TIMESTAMP,o_since),");
  fs.append("'HH:MI \"" + hh + "\" DD \"" + dd + "\"");
  fs.append(" mm \"" + mm + "\"') AS age");

  QString sql("SELECT " + fs + " ");
  sql.append("FROM inventory_orders AS a ");
  sql.append("LEFT JOIN costumers AS c ON c.c_id=a.o_costumer_id ");
  sql.append(
      "LEFT JOIN ref_delivery_service AS d ON d.d_id=a.o_delivery_service ");
  if (id > 0) {
    sql.append("WHERE a.o_id=");
    sql.append(QString::number(id));
    sql.append(" ORDER BY a.o_since DESC;");
  } else {
    sql.append("WHERE a.o_closed=false ORDER BY a.o_since DESC;");
  }
  return sql;
}

static const QString progresUpdate(int id, int status) {
  QString sql("UPDATE inventory_orders SET o_order_status=");
  sql.append(QString::number(status));
  sql.append(" WHERE o_id=");
  sql.append(QString::number(id));
  sql.append(";");
  return sql;
}

static const QString paymentUpdate(int id, bool status) {
  QString sql("UPDATE inventory_orders SET o_payment_status=");
  sql.append(((status) ? "true" : "false"));
  sql.append(" WHERE o_id=");
  sql.append(QString::number(id));
  sql.append(";");
  return sql;
}

static const QString closeOrder(int id) {
  QString sql("UPDATE inventory_orders SET");
  sql.append(" o_closed=true WHERE o_id=");
  sql.append(QString::number(id));
  sql.append(";");
  return sql;
}

#endif // INVENTORY_ORDERSTATEMENTS_H
