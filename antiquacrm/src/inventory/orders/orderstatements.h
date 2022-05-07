// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_ORDERSTATEMENTS_H
#define INVENTORY_ORDERSTATEMENTS_H

#include <QMetaType>
#include <QObject>
#include <QString>

/**
 * @group Order SQL Statements
 * Hier werden für das Auftragssystem die
 * SQL Abfragen definiert.
 */

/**
 * @def SHOW_SQL_QUERIES
 * @ingroup Order SQL Statements
 * @section Debugging
 * @brief SQL Abfrageanzeige
 * Schaltet die SQL-Statements im Terminal
 * auf sichtbar.
 */
#ifndef SHOW_SQL_QUERIES
#define SHOW_SQL_QUERIES false
#endif

/**
 * @ingroup Order SQL Statements
 * @brief Standard Auftrags Tabellenausgabe
 * @param id - Artikel Id
 * @return SQL Query
 */
static const QString defaultQuery(int id = 0) {
  QString hh(QObject::tr("Hours"));
  QString dd(QObject::tr("Days"));
  QString mm(QObject::tr("Months"));
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

/**
 * @brief Kunden Addressen finden
 * @param costumerId
 * @return SQL Query
 */
static const QString queryCostumerAddress(int costumerId) {
  QString sql("SELECT c_postal_address,c_shipping_address");
  sql.append(" FROM costumers WHERE c_id=");
  sql.append(QString::number(costumerId));
  sql.append(";");
  return sql;
}

/**
 * @ingroup Order SQL Statements
 * @brief Update auf den Abwicklungs Status
 * Setzt in der Spalte "o_order_status" den Fortschritt.
 * @note Die Stufen werden in Klasse @ref OrderStatus definiert!
 * @param id     - Artikel Id
 * @param status - Stufe
 * @return SQL Update
 */
static const QString progresUpdate(int id, int status) {
  QString sql("UPDATE inventory_orders SET o_order_status=");
  sql.append(QString::number(status));
  sql.append(" WHERE o_id=");
  sql.append(QString::number(id));
  sql.append(";");
  return sql;
}

/**
 * @ingroup Order SQL Statements
 * @brief Setzt den Verkausstatus für den Autrag.
 * Update auf "o_payment_status" in Tabelle "inventory_orders"
 * @param id     - Artikel Id
 * @param status - (true/false) (Bezahlt/nicht Bezahlt)
 * @return SQL Update
 */
static const QString paymentUpdate(int id, bool status) {
  QString sql("UPDATE inventory_orders SET o_payment_status=");
  sql.append(((status) ? "true" : "false"));
  sql.append(" WHERE o_id=");
  sql.append(QString::number(id));
  sql.append(";");
  return sql;
}

/**
 * @ingroup Order SQL Statements
 * @brief Schliest einen Auftrag
 * @param id - Artikel Id
 * @return SQL Update
 */
static const QString closeOrder(int id) {
  QString sql("UPDATE inventory_orders SET");
  sql.append(" o_closed=true WHERE o_id=");
  sql.append(QString::number(id));
  sql.append(";");
  return sql;
}

/**
 * @ingroup Order SQL Statements
 * @brief Suche mit "a_order_id" nach Bestellungen
 * @param id - Bestellungs Id
 * @return SQL Query
 */
static const QString paymentArticleOrders(int oid, int cid) {
  QString sql("SELECT * FROM article_orders ");
  sql.append("WHERE a_order_id=");
  sql.append(QString::number(oid));
  sql.append(" AND a_costumer_id=");
  sql.append(QString::number(cid));
  sql.append(";");
  return sql;
}

static const QString paymentRemove(const QString &pId, const QString &aId) {
  QString sql("DELETE FROM article_orders WHERE a_payment_id=");
  sql.append(pId);
  sql.append(" AND a_article_id=");
  sql.append(aId);
  sql.append(";");
  return sql;
}

/**
 * @ingroup Order SQL Statements
 * @brief Inventory* Artikel ermitteln.
 * Es wir in den folgenden Tabellen gesucht:
 *  @li "inventory_books"
 *  @li "inventory_prints"
 * Wird vom Autragseditor @ref OrderEditor benötigt damit
 * der Benutzer den richtigen Artikel einfügt!
 * Wenn ein Treffer erfolgt werden die folgende Rückgabewerte erwartet.
 *  @li aid    = Artikel ID
 *  @li counts = Lagerbestand (Anzahl)
 *  @li price  = Aktuelle Verkaufspreis
 *  @li title  = Titel des Artikels
 *
 * @param id - Suche mit Artikelnummer
 * @return SQL Query
 */
static const QString inventoryArticle(int id) {
  QString aid = QString::number(id);
  QString sql("SELECT ib_id AS aid,");
  sql.append("ib_count AS counts,");
  sql.append("ib_price AS price,");
  sql.append("ib_title AS title ");
  sql.append("FROM inventory_books ");
  sql.append("WHERE (ib_id=" + aid + " AND ib_count>0) ");
  sql.append("UNION SELECT ");
  sql.append("ip_id,ip_count,ip_price,ip_title ");
  sql.append("FROM inventory_prints ");
  sql.append("WHERE (ip_id=" + aid + " AND ip_count>0)");
  sql.append(";");
  return sql;
}

#endif // INVENTORY_ORDERSTATEMENTS_H
