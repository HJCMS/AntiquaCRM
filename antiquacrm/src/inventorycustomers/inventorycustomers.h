// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORYCUSTOMERS_H
#define INVENTORYCUSTOMERS_H

#include <QtCore/QObject>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

class SearchBar;

class InventoryCustomers : public QWidget {
  Q_OBJECT

private:
  /**
   @brief Gibt es Änderungen?
  */
  bool closable = false;

  SearchBar *m_searchBar;

private Q_SLOTS:
  /**
     @brief Änderungen wie isModified abfangen.
  */
  void setClosable(bool b) { closable = ((b) ? false : true); };

public:
  explicit InventoryCustomers(int index, QTabWidget *parent = nullptr);

  Q_INVOKABLE bool isClosable() { return closable; };
};

#endif // INVENTORYCUSTOMERS_H
