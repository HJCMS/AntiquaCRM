// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_TABLE_HEADER_COLUMN_H
#define ANTIQUACRM_TABLE_HEADER_COLUMN_H

#include <AGlobal>
#include <QMetaType>
#include <QSqlField>
#include <QString>

namespace AntiquaCRM {

/**
 * @brief Tabellenkopfspalten
 * @ingroup CoreLibrary
 * Helferklasse für die Darstellung der Tabellenkopfspalten.
 */
class ANTIQUACRM_LIBRARY ATableHeaderColumn final {
private:
  QString p_f; /** @brief SQL-Fieldname */
  QString p_n; /** @brief Displayname */

public:
  enum HeaderSection { ARTICLE = 0, ORDER = 1, CUSTOMER = 2 };

  /**
   * @brief ATableHeaderColumn;
   */
  explicit ATableHeaderColumn();

  /**
   * @param field - SQL Feldname
   * @param displayname - Anzeigename
   */
  explicit ATableHeaderColumn(const QSqlField &field, const QString &displayname);

  /**
   * @param field - SQL Feldname
   * @param name - Anzeigename
   */
  explicit ATableHeaderColumn(const QString &field, const QString &name);

  /**
   * @brief Der Original SQL Feldname
   */
  const QString field();

  /**
   * @brief Anzeige Titel in der Tabelle
   */
  const QString name();

  /**
   * @brief Gibt den Erforderlichen Datentyp für die Spalte zurück.
   * Ist abhängig von den Definitionen in der Klasse AProviderOrder.
   * @ref AntiquaCRM::AProviderOrder
   */
  QMetaType::Type type(HeaderSection section = HeaderSection::ARTICLE);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_TABLE_HEADER_COLUMN_H
