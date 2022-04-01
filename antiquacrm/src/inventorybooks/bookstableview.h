// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKSTABLEVIEW_H
#define BOOKSTABLEVIEW_H

#include <QtCore/QObject>
#include <QtCore/QHash>
#include <QtCore/QString>
#include <QtCore/QItemSelectionModel>
#include <QtSql/QSqlTableModel>
#include <QtWidgets/QTableView>

class BooksTableModel;
struct SearchStatement;

class BooksTableView : public QTableView
{
  Q_OBJECT

private:
  QSqlDatabase p_db;
  BooksTableModel *m_queryModel;

private Q_SLOTS:
  void clickedGetArticleID(const QModelIndex &);

Q_SIGNALS:
  void articleSelected(const QHash<int,QString>&);

public Q_SLOTS:
  void queryStatement(const SearchStatement &);

public:
  explicit BooksTableView(QWidget *parent = nullptr);

};

#endif // BOOKSTABLEVIEW_H

