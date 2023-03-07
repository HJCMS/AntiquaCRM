// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SQLDATAQUERY_H
#define ANTIQUACRM_SQLDATAQUERY_H

#include <AGlobal>
#include <QHash>
#include <QMetaType>
#include <QSqlField>
#include <QSqlRecord>
#include <QStringList>

namespace AntiquaCRM {

/**
 * @brief SQL Data Query Container Class
 * @ingroup core
 * This class is used to import and export Single Query to different class
 * structures, it is mostly used by the Plugin Interface.
 * This is a final class, because it depends on the PgSQL Database.
 * @note It's a pure Container Class without Database write operations!
 */
class ANTIQUACRM_LIBRARY ASqlDataQuery final {

private:
  const QSqlRecord p_record;
  QHash<QString, QVariant> p_data;

public:
  explicit ASqlDataQuery(const QString &tableName);
  explicit ASqlDataQuery(const QSqlRecord &record);
  explicit ASqlDataQuery(const ASqlDataQuery &other);

  /**
   * @brief Current SQL Table Record
   */
  const QSqlRecord record() const;

  /**
   * @brief Current SQL Table Record
   */
  int size() const;

  /**
   * @brief Current Tablename
   */
  const QString tableName() const;

  /**
   * @brief Does the SQL Record contain a field set?
   */
  bool isValid() const;

  /**
   * @brief a list of all table column field names
   */
  const QStringList columnNames() const;

  /**
   * @brief Read SQL Table Column properties from SQLField
   * @note Don't think, the returned SQL Field contains a column value!
   * @param column
   */
  const QSqlField getProperties(const QString &column) const;

  /**
   * @brief Table column Data type
   */
  const QMetaType getType(const QString &column) const;

  /**
   * @brief Set Value for column
   * @param column
   * @param value
   */
  void setValue(const QString &column, const QVariant &value);

  /**
   * @brief Read Value from column
   * @param column
   * @return
   */
  const QVariant getValue(const QString &column);

  /**
   * @brief Returning current data set
   * @return Hash<Column-Name, Column-Value>
   */
  const QHash<QString, QVariant> getDataset();
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_SQLDATAQUERY_H
