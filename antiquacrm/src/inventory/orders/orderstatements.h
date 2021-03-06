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
 * @def SHOW_ORDER_SQL_QUERIES
 * @ingroup Order SQL Statements
 * @section Debugging
 * @brief SQL Abfrageanzeige
 * Schaltet die SQL-Statements im Terminal
 * auf sichtbar.
 */
#ifndef SHOW_ORDER_SQL_QUERIES
#define SHOW_ORDER_SQL_QUERIES false
#endif

/**
 * @brief An welcher Stufe gilt der Eintrag als geschlossen
 * @note Siehe libAntiquaCRM::OrderStatus Klasse
 */
#ifndef STATUS_ORDER_CLOSED
#define STATUS_ORDER_CLOSED 5
#endif

/**
 * @brief Paket wurde ausgeliefert
 * @note Siehe libAntiquaCRM::OrderStatus Klasse
 */
#ifndef STATUS_ORDER_DELIVERED
#define STATUS_ORDER_DELIVERED 3
#endif

/**
 * @brief Standard auf den ersten Externen Paketdienst setzen!
 */
#ifndef ORDER_DELIVERY_SERVICE
#define ORDER_DELIVERY_SERVICE 1
#endif

/**
 * @brief InventoryOrdersSelect
 * @return QUERY_FIELD_SET
 */
static const QString InventoryOrdersSelect() {
  QString fieldSelect;
  QString fs("a.o_id,a.o_since,a.o_order_status,a.o_payment_status,");
  fs.append("CASE WHEN c.c_company=true THEN c.c_company_name ELSE ");
  fs.append("CONCAT(c.c_firstname,' ',c.c_lastname,' (',c.c_country,')') ");
  fs.append("END AS customer,");
  fs.append("d.d_name");
  fieldSelect.append(fs);

  QString prInfo("CONCAT_WS(': ', o_provider_name, o_provider_order_id)");
  prInfo.append(" AS o_provider_info");
  fieldSelect.append(",");
  fieldSelect.append(prInfo);

  QString epoch("(EXTRACT(EPOCH FROM timestamptz (CURRENT_TIMESTAMP))");

  QString age(epoch + " - ");
  age.append("EXTRACT(EPOCH FROM timestamptz (a.o_since))) AS age");
  fieldSelect.append(",");
  fieldSelect.append(age);

  QString delivered(epoch + " - ");
  delivered.append("EXTRACT(EPOCH FROM timestamptz (a.o_delivered))) AS fin");
  fieldSelect.append(",");
  fieldSelect.append(delivered);

  return fieldSelect;
}

/**
 * @ingroup Order SQL Statements
 * @brief Standard Auftrags Tabellenausgabe
 * @param id - Artikel Id
 * @return SQL Query
 */
static const QString defaultOrdersQuery(int id = 0) {
  QString fields = InventoryOrdersSelect();

  QString sql("SELECT " + fields + " ");
  sql.append("FROM inventory_orders AS a ");
  sql.append("LEFT JOIN customers AS c ON c.c_id=a.o_customer_id LEFT JOIN ");
  sql.append("ref_delivery_service AS d ON d.d_id=a.o_delivery_service ");
  if (id > 0) {
    sql.append("WHERE a.o_id=");
    sql.append(QString::number(id));
    sql.append(" ORDER BY a.o_since DESC;");
  } else {
    sql.append("WHERE a.o_order_status<" +
               QString::number(STATUS_ORDER_CLOSED));
    sql.append(" ORDER BY a.o_since DESC;");
  }
  return sql;
}

/**
 * @brief Kunden Adressen Darstellen
 * @code
 *  SELECT c_postal_address,c_shipping_address FROM customers WHERE c_id=1;
 * @endcode
 * @return SQL Query
 */
static const QString queryCustomerInfoAddress(int customerId) {
  QString sql("SELECT c_postal_address, c_shipping_address ");
  sql.append("FROM customers WHERE c_id=");
  sql.append(QString::number(customerId));
  sql.append(";");
  return sql;
}

/**
 * @brief Kunden Rechnungsadresse auslesen
 * @param customerId
 * @code
 *  SELECT c_postal_address FROM customers WHERE c_id=1;
 * @endcode
 * @return SQL Query
 */
static const QString queryCustomerInvoiceAddress(int customerId) {
  QString sql("SELECT c_postal_address ");
  sql.append("FROM customers WHERE c_id=");
  sql.append(QString::number(customerId));
  sql.append(";");
  return sql;
}

/**
 * @brief Kunden Lieferadresse auslesen
 * @param customerId
 * @code
 * SELECT func_shipping_address AS address FROM func_shipping_address(3);
 * @endcode
 * @return SQL Query
 */
static const QString queryCustomerDeliveryAddress(int customerId) {
  QString sql("SELECT func_shipping_address AS address");
  sql.append(" FROM func_shipping_address(");
  sql.append(QString::number(customerId));
  sql.append(");");
  return sql;
}

/**
 * @brief Kundennummer aus Auftrag ermitteln
 * @param orderId
 * @return SQL-Query
 */
static const QString queryCustomerByOrderId(int orderId) {
  QString o_id = QString::number(orderId);
  QString sql("SELECT o_customer_id AS c_id");
  sql.append(" FROM inventory_orders");
  sql.append(" WHERE o_id=" + o_id + ";");
  return sql;
}

/**
 * @brief Füge die Lieferscheinnummer ein!
 * @param oId
 * @param dId
 * @return SQL UPDATE
 */
static const QString setOrderDeliveryId(int oId, const QString &dId) {
  QString sql("UPDATE inventory_orders SET o_delivery='");
  sql.append(dId);
  sql.append("', o_modified=CURRENT_TIMESTAMP WHERE o_id=");
  sql.append(QString::number(oId));
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
  if (status == STATUS_ORDER_DELIVERED) {
    sql.append(",o_since=CURRENT_TIMESTAMP");
  }
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

static const QString finalizeTransaction(int customerId) {
  QString status = QString::number(STATUS_ORDER_CLOSED);
  QString c_id = QString::number(customerId);
  QString sql("UPDATE ONLY customers SET");
  sql.append(" c_purchases=+1, c_transactions=+1");
  sql.append(" FROM inventory_orders WHERE c_id=" + c_id);
  sql.append(" AND o_customer_id=c_id");
  sql.append(" AND o_payment_status=true");
  sql.append(" AND o_order_status=" + status + ";");
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
  sql.append(" AND a_customer_id=");
  sql.append(QString::number(cid));
  sql.append(";");
  return sql;
}

/**
 * @brief Erstelle Lieferscheinabfrage
 * @param oid
 * @param cid
 * @return (aid,quant,title)
 * @bug TODO Falsches JOIN Statement
 */
static const QString queryDeliveryNotes(int oid, int cid) {
  QString s("SELECT a.a_article_id AS aid, a.a_count AS quant,");
  s.append(" (CASE WHEN b.ib_title IS NOT NULL THEN");
  s.append(" CONCAT_WS(' - ',b.ib_title,b.ib_author,b.ib_publisher,b.ib_year)");
  s.append(" ELSE CONCAT_WS(' -  ',p.ip_title,p.ip_author) END) AS title");
  s.append(" FROM article_orders AS a");
  s.append(" LEFT JOIN inventory_books AS b ON b.ib_id=a.a_article_id");
  s.append(" LEFT JOIN inventory_prints AS p ON p.ip_id=a.a_article_id");
  s.append(" WHERE a.a_order_id=");
  s.append(QString::number(oid));
  s.append(" AND a.a_customer_id=");
  s.append(QString::number(cid));
  s.append(";");
  return s;
}

static const QString queryBillingInfo(int oid, int cid) {
  QString s("SELECT a.a_article_id AS aid, a.a_count AS quant,");
  s.append("a.a_sell_price AS sellPrice,");
  s.append(" (CASE WHEN b.ib_title IS NOT NULL THEN");
  s.append(" CONCAT_WS(' - ',b.ib_title,b.ib_author,b.ib_publisher,b.ib_year)");
  s.append(" ELSE CONCAT_WS(' -  ',p.ip_title,p.ip_author) END) AS title");
  s.append(" FROM article_orders AS a");
  s.append(" LEFT JOIN inventory_books AS b ON b.ib_id=a.a_article_id");
  s.append(" LEFT JOIN inventory_prints AS p ON p.ip_id=a.a_article_id");
  s.append(" WHERE a.a_order_id=");
  s.append(QString::number(oid));
  s.append(" AND a.a_customer_id=");
  s.append(QString::number(cid));
  s.append(";");
  return s;
}

/**
 * @brief Datensatz aus "article_orders" löschen.
 * Eintrag mit "a_payment_id" und "a_article_id" löschen.
 * @param pid - Payment ID
 * @param aid - Article ID
 * @return
 */
static const QString paymentRemove(const QString &pid, const QString &aid) {
  QString sql("DELETE FROM article_orders WHERE a_payment_id=");
  sql.append(pid);
  sql.append(" AND a_article_id=");
  sql.append(aid);
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
  QString s("SELECT COALESCE(b.ib_id,p.ip_id) AS aid,");
  s.append("COALESCE(b.ib_count,p.ip_count) AS counts,");
  s.append("COALESCE(b.ib_price,p.ip_price) AS price,");
  s.append("COALESCE(b.ib_title,p.ip_title) AS title");
  s.append(" FROM inventory AS i");
  s.append(" LEFT JOIN inventory_books AS b ON i.i_id=b.ib_id");
  s.append(" LEFT JOIN inventory_prints AS p ON i.i_id=p.ip_id");
  s.append(" WHERE i.i_id=" + QString::number(id));
  s.append(" AND (b.ib_count>0 OR p.ip_count>0);");
  return s;
}

/**
 * @brief Artikelbestands abfrage
 * @param oid - Order ID
 * @return SQL QUERY
 */
static const QString inventoryArticleCounts(int oid) {
  QString sql("SELECT COALESCE(ib_count,ip_count) AS count");
  sql.append(", a_article_id");
  sql.append(" FROM article_orders");
  sql.append(" LEFT JOIN inventory_books ON ib_id=a_article_id");
  sql.append(" LEFT JOIN inventory_prints ON ip_id=a_article_id");
  sql.append(" WHERE a_order_id=" + QString::number(oid));
  sql.append(";");
  return sql;
}

#endif // INVENTORY_ORDERSTATEMENTS_H
