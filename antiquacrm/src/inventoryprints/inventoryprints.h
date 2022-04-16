// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORYPRINTS_H
#define INVENTORYPRINTS_H

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QWidget>

class SearchBar;
class PrintsTableView;
class PrintsEditor;

class InventoryPrints : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int minLength; /**< Starte Suche ab Zeichen */
  QStackedWidget *m_stackedWidget;
  SearchBar *m_searchBar;
  PrintsTableView *m_tableView;
  PrintsEditor *m_printsEditor;

public:
  explicit InventoryPrints(int index, QTabWidget *parent = nullptr);
};

#endif // INVENTORYPRINTS_H
