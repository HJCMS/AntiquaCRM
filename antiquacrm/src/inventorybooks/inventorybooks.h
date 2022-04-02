// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORYBOOKS_H
#define INVENTORYBOOKS_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

class SearchBookBar;
class BooksTableView;
class StatsBookBar;

class InventoryBooks : public QWidget {
  Q_OBJECT

private:
  SearchBookBar *m_toolBar;
  BooksTableView *m_tableView;
  StatsBookBar *m_statsBookBar;

private Q_SLOTS:
  void selectArticleId(const QHash<QString,QString>&);

public:
  explicit InventoryBooks(QWidget *parent = nullptr);
};

#endif // INVENTORYBOOKS_H
