// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERSSTATEMENTS_H
#define INVENTORY_PROVIDERSSTATEMENTS_H

#include <QMap>
#include <QObject>
#include <QString>

/**
 * @group Providers SQL Statements
 * Hier werden für das Dienstleistersystem die
 * SQL Abfragen definiert.
 */

/**
 * @def SHOW_SQL_QUERIES
 * @ingroup Providers SQL Statements
 * @section Debugging
 * @brief SQL Abfrageanzeige
 * Schaltet die SQL-Statements im Terminal
 * auf sichtbar.
 */
#ifndef SHOW_SQL_QUERIES
#define SHOW_SQL_QUERIES false
#endif

/**
 * @brief Suche Artikel und gebe die Menge zurück!
 * @ingroup Providers SQL Statements
 * @param aid
 * @return SqlQuery count
 */
static const QString queryArticleCount(int aid) {
  QString s("SELECT func_get_article_count(");
  s.append(QString::number(aid));
  s.append(") AS count;");
  return s;
}

/**
 * @brief Kunden abfrage
 * @ingroup Providers SQL Statements
 * @param c_firstname
 * @param c_lastname
 * @param c_postalcode
 * @param c_location
 * @return SqlQuery
 */
static const QString queryCustomerExists(const QString &c_firstname,
                                         const QString &c_lastname,
                                         const QString &c_postalcode,
                                         const QString &c_location) {
  QString sql("SELECT c_id FROM customers WHERE ");
  // Wegen AbeBooks und doppelten einträgen erst Firma prüfen!
  QString company(c_firstname);
  company.append(" " + c_lastname);
  sql.append("c_company_name ILIKE '" + company + "'");
  sql.append(" OR (");
  sql.append("c_firstname ILIKE '" + c_firstname + "'");
  sql.append(" AND ");
  sql.append("c_lastname ILIKE '" + c_lastname + "'");
  sql.append(" AND ");
  sql.append("c_postalcode ILIKE '" + c_postalcode + "'");
  sql.append(" AND ");
  sql.append("c_location ILIKE '" + c_location + "')");
  sql.append(" ORDER BY c_id;");
  return sql;
}

/**
 * @brief Suche Tab Zurordnung für Artikelnummer
 * Rückgabe Werte sind: 'BOOKS'|'PRINTS'|''
 * @param aid - Article Id
 * @return SqlQuery
 */
static const QString queryFindArticleSection(int aid) {
  QString sql("SELECT (CASE WHEN ib_id>0 THEN");
  sql.append(" 'BOOKS' ELSE 'PRINTS' END) AS section");
  sql.append(" FROM inventory");
  sql.append(" LEFT JOIN inventory_books ON ib_id=i_id");
  sql.append(" LEFT JOIN inventory_prints ON ip_id=i_id");
  sql.append(" WHERE i_id=");
  sql.append(QString::number(aid));
  sql.append(";");
  return sql;
}

/**
 * @brief Suche Bestellungen die bereits bearbeitet werden!
 * @param provider
 * @param orderId
 * @return SqlQuery
 */
static const QString queryOrderStatusList(const QString &provider,
                                          const QStringList &orders) {
  QString list = "'" + orders.join("','").trimmed() + "'";
  QString sql("SELECT o_provider_order_id AS id,");
  sql.append(" o_order_status AS status");
  sql.append(" FROM inventory_orders");
  sql.append(" WHERE o_provider_name='" + provider + "'");
  sql.append(" AND o_provider_order_id IN (" + list + ");");
  return sql;
}

/**
 * @brief Suche nach Bereits erstellten Auftrag
 * @param pName - Dienstleister
 * @param pId   - Auftrags Nummer
 * @param cId   - Kunden Nummer
 * @return SqlQuery
 */
static const QString queryFindExistingOrders(const QString &pName,
                                             const QString &pId, int cId) {
  QString sql("SELECT o_id AS id FROM inventory_orders");
  sql.append(" WHERE o_provider_name ILIKE '");
  sql.append(pName);
  sql.append("' AND o_provider_order_id ILIKE '");
  sql.append(pId);
  sql.append("' AND o_customer_id=");
  sql.append(QString::number(cId));
  sql.append(";");
  return sql;
}

/**
 * @brief SQL Abfrage für Bilderdaten
 * @return  SqlQuery
 */
static const QString queryUploadImageData(int articleId) {
  QString im_id = QString::number(articleId);
  QString sql("SELECT im_id AS id, im_imgdata AS data");
  sql.append(" FROM inventory_images");
  sql.append(" WHERE im_id=" + im_id + ";");
  return sql;
}

#endif // INVENTORY_PROVIDERSSTATEMENTS_H
