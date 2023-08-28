// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_SQLFILES_H
#define ANTIQUACRM_SQLFILES_H

#include <AGlobal>
#include <QByteArray>
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>
#include <QString>

namespace AntiquaCRM {

/**
 * @brief Template Class for SQL Queries
 * @ingroup CoreLibrary
 *
 * Following MACROS are replaced!
 *  @li \@SQL_WHERE_CLAUSE\@
 *  @li \@SQL_ORDER_BY\@
 *  @li \@SQL_SORTING\@
 *  @li \@SQL_LIMIT\@
 */
class ANTIQUACRM_LIBRARY ASqlFiles final : public QFileInfo {

private:
  QDir p_dataDir;
  QString p_content;
  static const QRegularExpression commentPattern();
  static const QRegularExpression macrosPattern();

public:
  /**
   * @param file - SQL Template basename (without extension)
   */
  explicit ASqlFiles(const QString &file);

  /**
   * @brief open current template
   */
  bool openTemplate();

  /**
   * @brief Macro SQL_WHERE_CLAUSE replacement.
   * @param replacement
   */
  void setWhereClause(const QString &replacement);

  /**
   * @brief Macro SQL_ORDER_BY replacement.
   * @param replacement
   */
  void setOrderBy(const QString &replacement);

  /**
   * @brief Set Sorting for current Template.
   * @param order
   */
  void setSorting(Qt::SortOrder order);

  /**
   * @brief Set Query limit for current Template.
   * @param limit
   */
  void setLimits(int limit);

  /**
   * @brief Finally get SQL from current Template.
   */
  const QString getQueryContent();

  /**
   * @brief Query statement Template without macros!
   * @param basename - Template basename
   */
  static const QString queryStatement(const QString &basename);
};

}; // namespace AntiquaCRM

#endif // ANTIQUACRM_SQLFILES_H
