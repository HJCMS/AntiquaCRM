// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORYBOOKS_H
#define INVENTORYBOOKS_H

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

class SearchBar;
class BooksTableView;
class StatsBookBar;
class BookEditor;

class InventoryBooks : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int minLength;
  QStackedWidget *m_stackedWidget;
  SearchBar *m_searchBar;
  BooksTableView *m_tableView;
  StatsBookBar *m_statsBookBar;
  BookEditor *m_bookEditor;
  void openEditor(const QString &);

private Q_SLOTS:
  void parseLineInput(const QString &);
  void searchConvert();

  /**
     @brief Gehe zurück zur Tabellenansicht
  */
  void backToTableView();

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

Q_SIGNALS:
  void s_postMessage(const QString &);

public Q_SLOTS:
  void displayMessageBox(const QString &);
  void createBookEntry();

public:
  explicit InventoryBooks(int index, QTabWidget *parent = nullptr);
};

#endif // INVENTORYBOOKS_H
