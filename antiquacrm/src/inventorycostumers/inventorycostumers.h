// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORYCOSTUMERS_H
#define INVENTORYCOSTUMERS_H

#include "inventory.h"

#include <QtCore/QObject>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QWidget>

class EditCostumer;
class CostumerTableView;

class InventoryCostumers : public Inventory {
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
  QWidget *m_searchBar;

  /**
    @brief Tabellenansicht
  */
  CostumerTableView *m_tableView;

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

protected:
  void openEditor(const QString &);

private Q_SLOTS:
  // TODO

public:
  explicit InventoryCostumers(QWidget *parent = nullptr);
};

#endif // INVENTORYCOSTUMERS_H
