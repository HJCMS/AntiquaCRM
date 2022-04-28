// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORYCOSTUMERS_H
#define INVENTORYCOSTUMERS_H

#include "inventory.h"

#include <QtCore/QObject>
#include <QtWidgets/QLabel>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

class SearchBar;
class EditCostumer;
class CostumerTableView;

class InventoryCostumers : public Inventory {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  const QString primaryIndex = "c_id";

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
  CostumerTableView *m_tableView;

  /**
   * @brief Status Informationen
   */
  QLabel *m_statusInfo;

  /**
    @}

    @defgroup StackeWidget Page 2
    @{
  */

  /**
    @brief BuchEditor Fenster
  */
  EditCostumer *m_editCostumer;

  /** @} */

  void openEditor(const QString &);

private Q_SLOTS:
  void editCostumer(int);
  void createCostumer();
  void searchConvert(const QString &);
  void searchConvert();
  void openTableView();

public:
  explicit InventoryCostumers(QWidget *parent = nullptr);
};

#endif // INVENTORYCOSTUMERS_H
