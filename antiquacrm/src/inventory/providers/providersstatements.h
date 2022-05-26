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
#define SHOW_SQL_QUERIES true
#endif

/**
 * @brief Sucht Artikel der einen Bestand ausfweist!
 * @param aid
 * @return
 */
static const QString queryArticleExists(const QString &aid)
{
  QString sql("SELECT ib_id AS a_article_id");
  sql.append(" FROM inventory_books ");
  sql.append("WHERE (ib_id=" + aid + " AND ib_count>0) ");
  sql.append("UNION SELECT ");
  sql.append("ip_id AS a_article_id ");
  sql.append("FROM inventory_prints ");
  sql.append("WHERE (ip_id=" + aid + " AND ip_count>0)");
  sql.append(";");
  return sql;
}

#endif // INVENTORY_PROVIDERSSTATEMENTS_H
