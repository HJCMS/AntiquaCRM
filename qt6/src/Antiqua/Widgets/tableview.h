// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_WIDGETS_TABLEVIEW_H
#define ANTIQUACRM_WIDGETS_TABLEVIEW_H

#include <AGlobal>
#include <ASettings>
#include <QTableView>

// required by contextMenuAction
#include "tablecontextmenu.h"

namespace AntiquaCRM {

class TableHeader;

class ANTIQUACRM_LIBRARY TableView : public QTableView {
  Q_OBJECT
  Q_PROPERTY(int QueryLimit READ getQueryLimit WRITE setQueryLimit NOTIFY
                 sendQueryLimitChanged)

private:
  /**
   * @brief Limiting sql query in Main view
   * @note default:1000
   * @see Configurations Dialog "Feel and View"
   */
  int QueryLimit;

  /**
   * @brief Only Autoupdate the MainView if smaller then this.
   * @note default:50
   * @see Configurations Dialog "Feel and View"
   */
  int QueryAutoUpdate;

  /**
   * @brief Paint a visual information text for SQL queries!
   */
  void paintEvent(QPaintEvent *) override;

protected:
  AntiquaCRM::ASettings *m_cfg;

  /**
   * @brief Horizontal Table header
   */
  AntiquaCRM::TableHeader *m_header;

  /**
   * @brief Temporary SQL query history
   */
  QString QueryHistory = QString();

  /**
   * @brief enable/disable Table sorting
   */
  void setEnableTableViewSorting(bool);

  /**
   * @brief Get table ID from column
   */
  virtual qint64 getTableID(const QModelIndex &index, int column = 0) = 0;

  /**
   * @brief Method to prepare SQL Model Query.
   * In this function you can do some checks.
   * @note The SQL table model query must do in this method.
   */
  virtual bool sqlModelQuery(const QString &query) = 0;

protected Q_SLOTS:
  /**
   * @brief always print sql model error to terminal or syslog
   */
  void sqlModelError(const QString &table, const QString &message);

  /**
   * @brief create Table context menu actions
   */
  virtual void contextMenuAction(AntiquaCRM::TableContextMenu::Actions,
                                 const QModelIndex &) = 0;

  /**
   * @brief sort table with column and Qt::SortOrder
   * If sorting is enabled, prepare here the SQL Table Query for sorting.
   * @note For custom queries you need self made „BY ORDER“ clauses.
   * @warning If you mix aliases from SELECT clause with table fieldnames in
   * ORDER clauses, it will produce sql database errors.
   */
  virtual void setSortByColumn(int column, Qt::SortOrder order) = 0;

  /**
   * @brief slot to prepare getTableID
   */
  virtual void getSelectedItem(const QModelIndex &) = 0;

  /**
   * @brief create a local socket operation.
   */
  virtual void createSocketOperation(const QModelIndex &) = 0;

public Q_SLOTS:
  /**
   * @brief reload table view slot
   * @warning Do not call this slot every time when the table view is entered.
   * This produce a Network and rendering overhead.
   * The application can freeze when network latency is low or everytime large
   * query results from Sql-Server responses. See also @ref
   * isAutoRefreshEnabled() function to prevent this.
   *
   * Normaly @ref sqlModelQuery will called in this slot.
   */
  virtual void setReloadView() = 0;

  /**
   * @brief set QueryLimit property
   */
  void setQueryLimit(int);

Q_SIGNALS:
  /**
   * @brief Property QueryLimit was changed
   */
  void sendQueryLimitChanged(int);

  /**
   * @brief A Socket operation has been send...
   */
  void sendSocketOperation(const QJsonObject &);

  /**
   * @brief Signal to inform Horizontal Table Header, update or not.
   */
  void sendResultExists(bool);

  /**
   * @brief A Query report was send ...
   * Normally reserved for Status bar messages.
   */
  void sendQueryReport(const QString &report);

  /**
   * @brief Send string to System Clipboard was triggered.
   */
  void sendCopyToClibboard(const QString &str);

  /**
   * @brief Send Open entry by Id
   */
  void sendOpenEntry(qint64 articleId);

  /**
   * @brief A Notify Id was called
   */
  void sendCurrentId(qint64 sectionId);

  /**
   * @brief Send Create a new entry
   */
  void sendCreateNewEntry();

public:
  explicit TableView(QWidget *parent = nullptr);

  /**
   * @brief get application resource icon by name
   */
  static const QIcon cellIcon(const QString &name);

  /**
   * @brief get QueryLimit property
   */
  int getQueryLimit();

  /**
   * @brief compares Auto Update Configuration with current result
   * This is a helper function for application performance issues.
   * It compares the current query result with configuration settings.
   * If the result is smaller than „Auto Update VAR“ then true otherwise false.
   * It’s useful to call the @ref setReloadView() slot.
   */
  bool isAutoRefreshEnabled();

  /**
   * @brief get current table row count
   */
  virtual int rowCount() = 0;

  /**
   * @brief Required function to prepare SQL Queries.
   * @param clause - WHERE CLAUSE: defaultWhereClause()
   * @code
      AntiquaCRM::ASqlFiles tpl("my_custon_query_from_sql_query_file");
      if (tpl.openTemplate()) {
        where_clause = (clause.isEmpty() ? defaultWhereClause() : clause);
        tpl.setWhereClause(where_clause);
        tpl.setOrderBy("(count,changed,id)");
        tpl.setSorting(Qt::DescendingOrder);
        tpl.setLimits(getQueryLimit());
      }
      return sqlModelQuery(tpl.getQueryContent());
   * @endcode
   */
  virtual bool setQuery(const QString &clause = QString()) = 0;

  /**
   * @brief default where clause when nothing has changed.
   */
  virtual const QString defaultWhereClause() = 0;
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_WIDGETS_TABLEVIEW_H
