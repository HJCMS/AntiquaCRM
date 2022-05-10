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

namespace HJCMS {
class SqlCore;
};

class CostumersTableModel;
class SearchStatement;

class CostumerTableView : public QTableView {
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
  CostumersTableModel *m_tableModel;
  QString p_historyQuery;

  bool sqlExecQuery(const QString &statement);

  bool queryCostumerID(const QModelIndex &, CostumerTableView::QueryType type);

private Q_SLOTS:
  void queryCostumerID(const QModelIndex &);
  void openByContext();
  void createByContext();
  void orderByContext();

protected:
  void contextMenuEvent(QContextMenuEvent *);

Q_SIGNALS:
  void s_reportQuery(const QString &);
  void s_updateCostumer(int id);
  void s_insertCostumer();
  void s_createOrder(int costumerId);

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
  explicit CostumerTableView(QWidget *parent = nullptr);
};

#endif // COSTUMERTABLEVIEW_H
