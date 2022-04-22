// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORYCUSTOMERS_H
#define INVENTORYCUSTOMERS_H

#include "inventory.h"

#include <QtCore/QObject>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

class SearchBar;
class EditCustomer;

class InventoryCustomers : public Inventory {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
    @brief Fenster gruppierung
  */
  QStackedWidget *m_stackedWidget;

  /**
    @defgroup StackeWidget Page 1
    @{
  */

  /**
    @brief Sucheingabe für Bücher
  */
  SearchBar *m_searchBar;

  /**
    @brief Tabellenansicht
  */
  QTableView *m_tableView;

  /**
    @}

    @defgroup StackeWidget Page 2
    @{
  */

  /**
    @brief BuchEditor Fenster
  */
  EditCustomer *m_editCustomer;

  /** @} */

protected:
  void openEditor(const QString &);

private Q_SLOTS:
  // TODO

public:
  explicit InventoryCustomers(QWidget *parent = nullptr);
};

#endif // INVENTORYCUSTOMERS_H
