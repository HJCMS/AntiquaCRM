// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKSTABLEVIEW_H
#define BOOKSTABLEVIEW_H

#include <QtCore/QHash>
#include <QtCore/QItemSelectionModel>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtSql/QSqlTableModel>
#include <QtWidgets/QTableView>

class BooksTableModel;
class SearchStatement;

class BooksTableView : public QTableView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  QSqlDatabase p_db;
  BooksTableModel *m_queryModel;

private Q_SLOTS:
  void clickedGetArticleID(const QModelIndex &);

Q_SIGNALS:
  void s_articleSelected(int id);

public Q_SLOTS:
  void queryHistory(const QString &);
  void queryStatement(const SearchStatement &);

public:
  explicit BooksTableView(QWidget *parent = nullptr);
};

#endif // BOOKSTABLEVIEW_H
