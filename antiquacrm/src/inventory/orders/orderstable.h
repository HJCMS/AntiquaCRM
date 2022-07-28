// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ORDERSTABLE_H
#define ORDERSTABLE_H

#include <QContextMenuEvent>
#include <QHash>
#include <QObject>
#include <QString>
#include <QTableView>

#include <SqlCore>

class OrdersTableModel;

/**
 * @ingroup InventoryOrders
 * @class OrdersTable
 */
class OrdersTable : public QTableView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int maxRowCount = 500;
  int status_column = 2;  /**< o_order_status */
  int payment_column = 3; /**< o_payment_status */
  int close_column = 7;   /**< o_closed */
  HJCMS::SqlCore *m_sql;
  QModelIndex p_modelIndex;
  OrdersTableModel *m_queryModel;
  QString p_historyQuery;

  /**
   * @brief SQL Query Database
   */
  bool sqlExecQuery(const QString &statement);

private Q_SLOTS:
  /**
   * @brief Suche Datensatz mit Index
   * Wenn vorhanden Sende Signal @ref s_articleSelected
   */
  void queryOrder(const QModelIndex &);

  /**
   * @brief Ableitung für @ref clickedGetArticleID
   */
  void openByContext();

  /**
   * @brief Kundendaten öffnen/einsehen
   */
  void openCustomer();

protected:
  void contextMenuEvent(QContextMenuEvent *);

Q_SIGNALS:
  void s_reportQuery(const QString &);
  void s_editOrder(int orderId);
  void s_openCustomer(int cutomerId);

public Q_SLOTS:
  /**
   Anzeige auffrischen in dem der zuletzt
   in @ref p_historyQuery gespeicherte SQL
   Befehl noch einmal aufgerufen wird.
  */
  void refreshView();

  void initOrders();

public:
  explicit OrdersTable(QWidget *parent = nullptr);
};

#endif // ORDERSTABLE_H
