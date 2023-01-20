// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABLE_HEADER_COLUMN_H
#define ANTIQUACRM_TABLE_HEADER_COLUMN_H

#include <AGlobal>
#include <QMetaType>
#include <QSqlField>
#include <QString>

namespace AntiquaCRM {

/**
 * @brief Tabellenkopfspalten
 * @section utils TableHeaderColumn
 * Helferklasse für die Darstellung der Tabellenkopfspalten.
 */
class ANTIQUACRM_LIBRARY ATableHeaderColumn final {
private:
  QString p_f;
  QString p_n;

public:
  enum HeaderSection { ARTICLE = 0, ORDER = 1, CUSTOMER = 2 };

  /**
   * @brief ATableHeaderColumn;
   */
  ATableHeaderColumn();

  /**
   * @param field - SQL Feldname
   * @param displayname - Anzeigename
   */
  ATableHeaderColumn(const QSqlField &field, const QString &displayname);

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