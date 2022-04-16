// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PRINTSTABLEVIEW_H
#define PRINTSTABLEVIEW_H

#include <QtCore/QHash>
#include <QtCore/QItemSelectionModel>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtGui/QContextMenuEvent>
#include <QtSql/QSqlTableModel>
#include <QtWidgets/QTableView>

class PrintsTableModel;
class SearchStatement; /**< @ref SearchBar */

class PrintsTableView : public QTableView {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  int maxRowCount = 2500;
  QSqlDatabase p_db;
  QModelIndex p_modelIndex;
  PrintsTableModel *m_queryModel;

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
  explicit PrintsTableView(QWidget *parent = nullptr);
};

#endif // PRINTSTABLEVIEW_H
