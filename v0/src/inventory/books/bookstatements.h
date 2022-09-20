// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKSTATEMENTS_H
#define BOOKSTATEMENTS_H

//#ifndef BOOK_INVENTORY_DEBUG
//#define BOOK_INVENTORY_DEBUG
//#endif

#include <QString>

/**
 * @brief InventoryBooksSelect
 * Die Feldabfragen für die Suche sind immer gleich!
 * Wenn sich etwas ändern sollte, dann muss in Klasse
 * \ref BooksTableModel die Feld Definition geändert werden!
 */
static const QString InventoryBooksSelect() {
  QString s("ib_id,ib_count");
  s.append(",(CASE WHEN im_id IS NOT NULL THEN true ELSE false END)");
  s.append(" AS image_exists,ib_title,ib_price::MONEY,ib_author,ib_publisher");
  s.append(",ib_year,sl_storage,ib_isbn,ib_changed");
  return s;
}

/**
 * @brief InventoryBooksTables
 * Tabellen Definition
 */
static const QString InventoryBooksTables() {
  QString s(" FROM inventory_books");
  s.append(" LEFT JOIN ref_storage_location ON sl_id=ib_storage");
  s.append(" LEFT JOIN inventory_images ON im_id=ib_id ");
  return s;
}

#endif
