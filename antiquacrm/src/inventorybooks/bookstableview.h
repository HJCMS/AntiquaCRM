// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKSTABLEVIEW_H
#define BOOKSTABLEVIEW_H

#include <QtCore/QHash>
#include <QtCore/QItemSelectionModel>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QContextMenuEvent>
#include <QtSql/QSqlTableModel>
#include <QtWidgets/QTableView>

namespace HJCMS {
class SqlCore;
};

class BooksTableModel;
class SearchStatement; /**< @ref SearchBar */

class BooksTableView : public QTableView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int maxRowCount = 2500;
  HJCMS::SqlCore *m_sql;
  QModelIndex p_modelIndex;
  BooksTableModel *m_queryModel;

private Q_SLOTS:
  /**
   @brief Suche Datensatz mit Index
   Wenn vorhanden Sende Signal @ref s_articleSelected
  */
  void queryArticleID(const QModelIndex &);

  /**
   @brief Ableitung für @ref clickedGetArticleID
  */
  void openBookByContext();

  /**
     @brief Einen neuen Eintrag erstellen wenn ...
     @todo Die Suchanfrage kein Ergebnis lieferte
  */
  void newBookByContext();

  /**
     @brief  Auftrage Erstellung
     @todo Im Moment noch verfügbar
  */
  void createOrderByContext();

protected:
  void contextMenuEvent(QContextMenuEvent *);

Q_SIGNALS:
  void s_rowsChanged(int count);
  void s_articleSelected(int id);
  void s_newEntryPlease();

public Q_SLOTS:
  void queryHistory(const QString &);
  void queryStatement(const SearchStatement &);

public:
  explicit BooksTableView(QWidget *parent = nullptr);
};

#endif // BOOKSTABLEVIEW_H
