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
class BookEditor;

class InventoryBooks : public Inventory {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  int minLength;
  SearchBar *m_searchBar;
  BooksTableView *m_tableView;
  StatsBookBar *m_statsBookBar;
  BookEditor *m_bookEditor;
  void openEditor(const QString &);

private Q_SLOTS:
  void parseLineInput(const QString &);
  void searchConvert();

  /**
     @brief articleSelected
     Wird bei der Tabellen Suchansicht mit einem Doppelklick
     das Signal @ref BooksTableView::s_articleSelected ausgelöst.
     Wird an Hand des @b "ib_id" Feldes eine SQL Abfrage erstellt
     und der Editor mit @ref openEditor("ib_id={$id}") geöffnet!
     @param id
  */
  void articleSelected(int id);

  /**
     @brief updateValidator
     Wird der Suchfilter geändert dann wird hier an
     @ref SearchBar::setValidation ein update gesendet.
   */
  void updateValidator(int);

public Q_SLOTS:
  void createBookArticle();

public:
  explicit InventoryBooks(int index, QTabWidget *parent = nullptr);
};

#endif // INVENTORYBOOKS_H
