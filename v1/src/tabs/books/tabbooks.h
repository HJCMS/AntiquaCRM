// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABBOOKS_H
#define ANTIQUACRM_TABBOOKS_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

class BookSearchBar;
class BooksTable;

class TabBooks final : public Inventory {
  Q_OBJECT

private:
  QWidget *m_mainPage;
  QWidget *m_pageEdit;
  BookSearchBar *m_searchBar;
  BooksTable *m_table;

public Q_SLOTS:
  void createSearchQuery();
  void createNewEntry();
  void openEntry(qint64 id);
  void onEnterChanged();

public:
  explicit TabBooks(QWidget *parent = nullptr);
};

#endif // TABBOOKS_H
