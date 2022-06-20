// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORYCOSTUMERS_H
#define INVENTORYCOSTUMERS_H

#include <QObject>
#include <QStackedWidget>
#include <QScrollArea>
#include <QWidget>

#include <Utils>

class EditCustomer;
class CustomerTableView;

/**
 * @brief Sucheingabe für Kunden Adressbuch
 * @ingroup Inventory
 */
class CustomerSearchBar final : public SearchBar {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  const QList<SearchFilter> commonSearchFilter() const;

public:
  CustomerSearchBar(QWidget *parent);
};

/**
 * @brief Kundenadressbuch Seiten
 * @class InventoryCustomers
 * @ingroup Inventory
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
  CustomerSearchBar *m_searchBar;

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
  QScrollArea *m_editorWidget;
  EditCustomer *m_editCustomer;

protected:
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
