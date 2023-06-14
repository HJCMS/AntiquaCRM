// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_SQLQUERYMODEL_H
#define ANTIQUACRM_SQLQUERYMODEL_H

#include <AGlobal>
#include <QMap>
#include <QModelIndex>
#include <QObject>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QString>
#include <QVariant>

namespace AntiquaCRM {

class ASqlCore;

/**
 * @class ASqlQueryModel
 * @brief Standard SqlQueryModel for table views
 *
 * Abstract SQL query model class for all SQL table views.
 *
 * @ingroup CoreLibrary
 */
class ANTIQUACRM_LIBRARY ASqlQueryModel : public QSqlQueryModel {
  Q_OBJECT

private:
  /**
   * @brief table name
   */
  const QString p_table;

protected:
  /**
   * @brief Database Connection class
   */
  AntiquaCRM::ASqlCore *m_sql;

  /**
   * @brief Standard formatting for the column headers!
   * @param text - Title
   */
  const QString setHeaderTitle(const QString &text) const;

  /**
   * @brief Convert date to readable format!
   * @param value
   */
  const QString displayDate(const QVariant &value) const;

  /**
   * @brief Standard formatting for line numbers
   * @param row
   * @param role
   */
  const QString verticalHeader(int row, int role = Qt::DisplayRole) const;

Q_SIGNALS:
  /**
   * @brief In the case of SQL errors, the message is hereby transmitted!
   * @note The signal handling must be done in the table view!
   * @param table
   * @param message
   */
  void sqlErrorMessage(const QString &table, const QString &message);

public:
  /**
   * @param table  - table name
   * @param parent - parent object
   */
  explicit ASqlQueryModel(const QString &table, QObject *parent = nullptr);

  /**
   * @brief Predefined table columns title
   */
  virtual const QMap<int, QString> headerList() const = 0;

  /**
   * @brief SQL query ...
   */
  bool querySelect(const QString &sql);

  /**
   * @brief Current used table name
   */
  const QString tableName() const;

  /**
   * @brief SqlRecord from table
   */
  const QSqlRecord tableRecord() const;

  /**
   * @brief SqlRecord of the query
   */
  const QSqlRecord queryRecord() const;

  /**
   * @brief Query field name/alias name!
   * @param column
   */
  const QString fieldName(int column = 0) const;

  /**
   * @brief Column number of the query!
   * @param fieldName
   */
  int column(const QString &fieldName) const;

  /**
   * @brief Query record
   * @param item
   * @param role
   */
  QVariant data(const QModelIndex &item, int role = Qt::DisplayRole) const;

  /**
   * @brief Returns an info line with time and result size!
   */
  const QString queryResultInfo();
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_SQLQUERYMODEL_H
