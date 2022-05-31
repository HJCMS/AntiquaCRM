// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORYCOSTUMERS_H
#define INVENTORYCOSTUMERS_H

#include "inventory.h"

#include <QObject>
#include <QStackedWidget>
#include <QWidget>

class SearchBar;
class StatsActionBar;
class EditCustomer;
class CustomerTableView;

/**
 * @brief The InventoryCustomers class
 * @class InventoryCustomers
 */
class InventoryCustomers final : public Inventory {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  const QString primaryIndex = "c_id";

  /**
   * @brief Fenster gruppierung
   */
  QStackedWidget *m_stackedWidget;

  /**
   * @brief Sucheingabe für Bücher
   */
  SearchBar *m_searchBar;

  /**
   * @brief Tabellenansicht
   */
  CustomerTableView *m_tableView;

  /**
   * @brief Status Informationen und Verlauf
   */
  StatsActionBar *m_statsActionBar;

  /**
   * @brief BuchEditor Fenster
   */
  EditCustomer *m_editCustomer;

  void openEditor(const QString &);

private Q_SLOTS:
  void searchConvert(const QString &);
  void searchConvert();
  void openTableView();

public Q_SLOTS:
  void editCustomer(int);
  void createCustomer();

  /**
   * @brief Wenn das Tab im Vordergund geht, dann eine Aktionen starten
   */
  void onEnterChanged(){/* TODO */};

public:
  explicit InventoryCustomers(QWidget *parent = nullptr);
};

#endif // INVENTORYCOSTUMERS_H
