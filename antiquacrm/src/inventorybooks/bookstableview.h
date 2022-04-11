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

class BooksTableModel;
class SearchStatement; /**< @ref SearchBar */

class BooksTableView : public QTableView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  int maxRowCount = 2500;
  QSqlDatabase p_db;
  QModelIndex p_modelIndex;
  BooksTableModel *m_queryModel;

private Q_SLOTS:
  void clickedGetArticleID(const QModelIndex &);
  void openBookByContext();
  void createOrderByContext();

protected:
  void contextMenuEvent(QContextMenuEvent *);

Q_SIGNALS:
  void s_rowsChanged(int count);
  void s_articleSelected(int id);

public Q_SLOTS:
  void queryHistory(const QString &);
  void queryStatement(const SearchStatement &);

public:
  explicit BooksTableView(QWidget *parent = nullptr);
};

#endif // BOOKSTABLEVIEW_H
