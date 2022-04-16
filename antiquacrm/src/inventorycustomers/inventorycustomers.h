// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORYCUSTOMERS_H
#define INVENTORYCUSTOMERS_H

#include <QtCore/QObject>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

class SearchBar;

class InventoryCustomers : public QWidget
{
  Q_OBJECT

private:
  SearchBar *m_searchBar;

public:
  explicit InventoryCustomers(int index,QTabWidget *parent = nullptr);

};

#endif // INVENTORYCUSTOMERS_H

