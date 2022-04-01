// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORYBOOKS_H
#define INVENTORYBOOKS_H

#include <QtCore/QObject>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

class SearchBookBar;
class BooksTableView;
struct SearchStatement;

class InventoryBooks : public QWidget {
  Q_OBJECT

private:
  SearchBookBar *m_toolBar;
  BooksTableView *m_tableView;

public:
  explicit InventoryBooks(QWidget *parent = nullptr);
};

#endif // INVENTORYBOOKS_H
