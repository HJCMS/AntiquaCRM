// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ASSIGNMENTS_TABLEVIEW_H
#define ASSIGNMENTS_TABLEVIEW_H

#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QContextMenuEvent>
#include <QtWidgets/QTableView>

namespace HJCMS {
class SqlCore;
};

class TableModel;

class TableView : public QTableView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int maxRowCount = 2500;
  HJCMS::SqlCore *m_sql;
  QModelIndex p_modelIndex;
  TableModel *m_queryModel;
  QString p_historyQuery;

  /**
   @brief SQL Query Database
  */
  bool sqlExecQuery(const QString &statement);

private Q_SLOTS:
  /**
   @brief Suche Datensatz mit Index
   Wenn vorhanden Sende Signal @ref s_articleSelected
  */
  void queryOrder(const QModelIndex &);

  /**
   @brief Ableitung für @ref clickedGetArticleID
  */
  void openByContext();

  /**
     @brief Einen neuen Eintrag erstellen wenn ...
     @todo Die Suchanfrage kein Ergebnis lieferte
  */
  void createByContext();

  /**
     @brief  Auftrage Erstellung
     @todo Im Moment noch verfügbar
  */
  void orderByContext();

protected:
  void contextMenuEvent(QContextMenuEvent *);

Q_SIGNALS:
  void s_reportQuery(const QString &);
  void s_editOrder(int id);
  void s_createOrder();

public Q_SLOTS:
  /**
   Anzeige auffrischen in dem der zuletzt
   in @ref p_historyQuery gespeicherte SQL
   Befehl noch einmal aufgerufen wird.
  */
  void refreshView();

  void initOrders();

public:
  explicit TableView(QWidget *parent = nullptr);
};

#endif // ASSIGNMENTS_TABLEVIEW_H
