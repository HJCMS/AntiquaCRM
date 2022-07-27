// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_COSTUMERSTATEMENTS_H
#define INVENTORY_COSTUMERSTATEMENTS_H

#include <QMetaType>
#include <QObject>
#include <QString>

/**
 * @group Customer SQL Statements
 * Hier werden für das Auftragssystem die
 * SQL Abfragen definiert.
 */

/**
 * @def SHOW_SQL_QUERIES
 * @ingroup Customer SQL Statements
 * @section Debugging
 * @brief SQL Abfrageanzeige
 * Schaltet die SQL-Statements im Terminal
 * auf sichtbar.
 */
#ifndef SHOW_SQL_QUERIES
#define SHOW_SQL_QUERIES false
#endif

/**
 * @ingroup Customer SQL Statements
 * @brief Standard Select Statement für Tabellenausgabe
 * @return SQL Query
 */
static const QString c_sqlTableQueryBody() {
 QString q("SELECT c_id AS id, c_purchases,");
 q.append("(CASE WHEN c_company IS TRUE THEN c_company_name ELSE '#PR' END) AS ");
 q.append("company,c_since AS since,");
 q.append("CONCAT_WS(' ',c_title,c_firstname,c_lastname) AS shurename,");
 q.append("(CASE WHEN c_phone_0 IS NULL THEN c_phone_1 ELSE c_phone_0 END) AS phone,");
 q.append("(CASE WHEN c_mobil_0 IS NULL THEN c_mobil_1 ELSE c_mobil_0 END) AS mobil,");
 q.append("concat_ws(' ',c_country,c_postalcode,c_location,c_street) AS address");
 q.append(" FROM customers WHERE ");
 return q;
}

static const QString c_sqlDeleteCustomer(int costumerId) {
  QString id = QString::number(costumerId);
  QString sql("DELETE FROM customers WHERE c_id=" + id + ";");
  return sql;
}

#endif // INVENTORY_COSTUMERSTATEMENTS_H
