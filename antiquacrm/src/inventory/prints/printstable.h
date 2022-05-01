// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PRINTSTABLE_H
#define PRINTSTABLE_H

#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QContextMenuEvent>
#include <QtWidgets/QTableView>

#include <SqlCore>

namespace HJCMS {
class SqlCore;
};

class PrintsTableModel;
class SearchStatement; /**< @ref SearchBar */

class PrintsTable : public QTableView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  int maxRowCount = 2500;
  HJCMS::SqlCore *m_sql;
  QModelIndex p_modelIndex;
  PrintsTableModel *m_queryModel;
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
  void queryArticleID(const QModelIndex &);

  /**
   @brief Ableitung für @ref clickedGetArticleID
  */
  void openByContext();

  /**
     @brief Einen neuen Eintrag erstellen wenn ...
     @todo Die Suchanfrage kein Ergebnis lieferte
  */
  void createByContext();

protected:
  void contextMenuEvent(QContextMenuEvent *);

Q_SIGNALS:
  void s_rowsChanged(int count);
  void s_articleSelected(int id);
  void s_newEntryPlease();

public Q_SLOTS:
  /**
   Anzeige auffrischen in dem der zuletzt
   in @ref p_historyQuery gespeicherte SQL
   Befehl noch einmal aufgerufen wird.
  */
  void refreshView();

  /**
    Wird von @ref StatsBookBar::m_showHistory()
    aufgerufen und fragt den Verlauf ab.
  */
  void queryHistory(const QString &);

  /**
    Startet Abfrage ausgehend von Text/Sucheingabe
  */
  void queryStatement(const SearchStatement &);

public:
  explicit PrintsTable(QWidget *parent = nullptr);
};

#endif // PRINTSTABLE_H
