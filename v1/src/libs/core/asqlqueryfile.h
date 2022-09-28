// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_SQLQUERYFILE_H
#define ANTIQUACRM_SQLQUERYFILE_H

#include <QByteArray>
#include <QDir>
#include <QFileInfo>
#include <QString>

namespace AntiquaCRM {

/**
 * @brief Template Class for SQL Queries
 *
 * Following MACROS are replaced!
 * @li @SQL_WHERE_CLAUSE@
 * @li @SQL_ORDER_BY@
 * @li @SQL_SORTING@
 * @li @SQL_LIMIT@
 */
class ASqlQueryFile : public QFileInfo {

private:
  QDir p_dataDir;
  QString p_content;

public:
  explicit ASqlQueryFile(const QString &file);
  bool openTemplate();
  void setWhereClause(const QString &replacement);
  void setOrderBy(const QString &replacement);
  void setSorting(Qt::SortOrder order);
  void setLimits(int limit);
  const QString getQueryContent();
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_SQLQUERYFILE_H
