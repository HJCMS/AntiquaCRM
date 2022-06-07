// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKSTABLE_H
#define BOOKSTABLE_H

#include <QHash>
#include <QObject>
#include <QString>
#include <QContextMenuEvent>
#include <QTableView>

#include <SqlCore>

class BooksTableModel;
class SearchStatement; /**< @ref SearchBar */

class BooksTable : public QTableView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int maxRowCount = 2500;
  HJCMS::SqlCore *m_sql;
  QModelIndex p_modelIndex;
  BooksTableModel *m_queryModel;
  QString p_historyQuery;

  /**
   @brief SQL Query Database
  */
  bool sqlExecQuery(const QString &statement);

  /**
   * @brief Suche Datensatz mit Index
   * Wenn vorhanden Sende Signal @ref s_articleSelected
  */
  int queryArticleID(const QModelIndex &);

private Q_SLOTS:
  /**
   * @brief Wird von Tablemodel ausgelöst
   */
  void articleClicked(const QModelIndex &);

  /**
   * @brief Ableitung für @ref clickedGetArticleID
  */
  void openByContext();

  /**
   * @brief Einen neuen Eintrag erstellen wenn ...
   * @todo Die Suchanfrage kein Ergebnis lieferte
  */
  void createByContext();

  /**
   * @brief Kopiert die ID in das Clipboard
   */
  void copyToClipboard();

  /**
   * @brief Suche Artikel ID und sende @ref s_articleToOrders
   */
  void createOrderSignal();

protected:
  void contextMenuEvent(QContextMenuEvent *);

Q_SIGNALS:
  void s_toClibboard(const QVariant &);
  void s_reportQuery(const QString &);
  void s_articleSelected(int articleId);
  void s_articleToOrders(int articleId);
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
  explicit BooksTable(QWidget *parent = nullptr);
};

#endif // BOOKSTABLE_H
