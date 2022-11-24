// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SQLFILES_H
#define ANTIQUACRM_SQLFILES_H

#include <AGlobal>
#include <QByteArray>
#include <QDir>
#include <QFileInfo>
#include <QRegExp>
#include <QString>

namespace AntiquaCRM {

/**
 * @brief Template Class for SQL Queries
 * @section sql
 *
 * Following MACROS are replaced!
 * @li @SQL_WHERE_CLAUSE@
 * @li @SQL_ORDER_BY@
 * @li @SQL_SORTING@
 * @li @SQL_LIMIT@
 */
class ANTIQUACRM_LIBRARY ASqlFiles final : public QFileInfo {

private:
  QDir p_dataDir;
  QString p_content;
  const QRegExp comments;

public:
  explicit ASqlFiles(const QString &file);
  bool openTemplate();
  void setWhereClause(const QString &replacement);
  void setOrderBy(const QString &replacement);
  void setSorting(Qt::SortOrder order);
  void setLimits(int limit);
  const QString getQueryContent();
  static const QString selectStatement(const QString &name);
  static const QString queryStatement(const QString &name);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_SQLFILES_H
