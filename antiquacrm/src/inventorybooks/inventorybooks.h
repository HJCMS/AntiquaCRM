// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORYBOOKS_H
#define INVENTORYBOOKS_H

#include "inventory.h"
#include "searchbar.h"

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

class SearchBar;
class BooksTableView;
class StatsBookBar;

class InventoryBooks : public Inventory {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  int minLength;
  SearchBar *m_searchBar;
  BooksTableView *m_tableView;
  StatsBookBar *m_statsBookBar;

private Q_SLOTS:
  void parseLineInput(const QString &);
  void searchConvert();
  void selectArticleId(const QHash<QString, QString> &);
  void updatePlaceHolder(int);

public:
  explicit InventoryBooks(int index, QTabWidget *parent = nullptr);
};

#endif // INVENTORYBOOKS_H
