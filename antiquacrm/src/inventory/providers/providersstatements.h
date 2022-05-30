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
 * @brief Sucht Artikel der einen Bestand aufweist!
 * @ingroup Providers SQL Statements
 * @param aid
 * @return SqlQuery
 */
static const QString queryArticleExists(const QString &aid) {
  QString s("SELECT DISTINCT ");
  s.append("nullif(coalesce(b.ib_id),p.ip_id) AS aid,");
  s.append("nullif(coalesce(b.ib_count),p.ip_count) AS counts,");
  s.append("nullif(coalesce(b.ib_price),p.ip_price) AS price,");
  s.append("nullif(coalesce(b.ib_title),p.ip_title) AS title");
  s.append(" FROM inventory_books AS b");
  s.append(" LEFT JOIN inventory_prints AS p ON p.ip_id=");
  s.append(aid + " WHERE (b.ib_id=" + aid);
  s.append(" AND b.ib_count>0) OR (p.ip_count>0);");
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
  sql.append("c_firstname ILIKE '" + c_firstname + "'");
  sql.append(" AND ");
  sql.append("c_lastname ILIKE '" + c_lastname + "'");
  sql.append(" AND ");
  sql.append("c_postalcode ILIKE '" + c_postalcode + "'");
  sql.append(" AND ");
  sql.append("c_location ILIKE '" + c_location + "'");
  sql.append(" ORDER BY c_id;");
  return sql;
}

#endif // INVENTORY_PROVIDERSSTATEMENTS_H
