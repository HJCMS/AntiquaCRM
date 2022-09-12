// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef COSTUMERTABLEVIEW_H
#define COSTUMERTABLEVIEW_H

#include <QContextMenuEvent>
#include <QHeaderView>
#include <QModelIndex>
#include <QObject>
#include <QString>
#include <QTableView>

#include <SqlCore>

class CustomersTableModel;
class SearchFilter;

class CustomerTableView : public QTableView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  enum QueryType { Create = 1, Update = 2, Order = 3, Delete = 4 };
  Q_ENUM(QueryType);

private:
  int maxRowCount = 1000;
  HJCMS::SqlCore *m_sql;
  CustomersTableModel *m_tableModel;
  QHeaderView *m_headerView;
  QModelIndex p_modelIndex;
  QString p_historyQuery;
  QString p_orderBy = QString("c_since DESC");

  bool sqlExecQuery(const QString &statement);

  bool queryCustomerAction(const QModelIndex &,
                           CustomerTableView::QueryType type);

private Q_SLOTS:
  void sortTableView(int, Qt::SortOrder);
  void queryCustomerID(const QModelIndex &);
  void openByContext();
  void createByContext();
  void orderByContext();
  void deleteUserContext();

protected:
  void contextMenuEvent(QContextMenuEvent *);

Q_SIGNALS:
  void s_reportQuery(const QString &);
  void s_updateCustomer(int customerId);
  void s_insertCustomer();
  void s_createOrder(int customerId);
  void s_deleteCustomer(int customerId);

public Q_SLOTS:
  /**
   * Anzeige auffrischen in dem der zuletzt in
   * @ref p_historyQuery gespeicherte SQL
   * Befehl noch einmal aufgerufen wird.
   */
  void refreshView();

  /**
   * @brief Verlaufsabfrage
   */
  void queryHistory(const QString &history);

  /**
   * Starte Abfrage ausgehend von Text/Sucheingabe
   */
  void queryStatement(const SearchFilter &search);

public:
  explicit CustomerTableView(QWidget *parent = nullptr);
  int rowCount();
};

#endif // COSTUMERTABLEVIEW_H
