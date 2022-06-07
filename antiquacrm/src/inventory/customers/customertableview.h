// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef COSTUMERTABLEVIEW_H
#define COSTUMERTABLEVIEW_H

#include <QContextMenuEvent>
#include <QModelIndex>
#include <QObject>
#include <QString>
#include <QTableView>

#include <SqlCore>

class CustomersTableModel;
class SearchStatement;

class CustomerTableView : public QTableView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  enum QueryType { Create = 1, Update = 2, Order = 3 };
  Q_ENUM(QueryType);

private:
  int maxRowCount = 1000;
  HJCMS::SqlCore *m_sql;
  QModelIndex p_modelIndex;
  CustomersTableModel *m_tableModel;
  QString p_historyQuery;

  bool sqlExecQuery(const QString &statement);

  bool queryCustomerID(const QModelIndex &, CustomerTableView::QueryType type);

private Q_SLOTS:
  void queryCustomerID(const QModelIndex &);
  void openByContext();
  void createByContext();
  void orderByContext();

protected:
  void contextMenuEvent(QContextMenuEvent *);

Q_SIGNALS:
  void s_reportQuery(const QString &);
  void s_updateCustomer(int id);
  void s_insertCustomer();
  void s_createOrder(int customerId);

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
  void queryStatement(const SearchStatement &search);

public:
  explicit CustomerTableView(QWidget *parent = nullptr);
};

#endif // COSTUMERTABLEVIEW_H
