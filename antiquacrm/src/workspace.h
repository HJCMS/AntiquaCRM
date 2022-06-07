// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QContextMenuEvent>
#include <QObject>
#include <QTabBar>
#include <QTabWidget>
#include <QWidget>

class InventoryBooks;
class InventoryPrints;
class InventoryCustomers;
class InventoryOrders;
class InventoryProviders;
class ProviderOrder;
class InventoryViews;

class WorkspaceTabBar : public QTabBar {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  int index = -1;

private Q_SLOTS:
  void checkToClose();

protected:
  void tabInserted(int index);
  void contextMenuEvent(QContextMenuEvent *event);

Q_SIGNALS:
  void s_closeTab(int index);

public:
  explicit WorkspaceTabBar(QWidget *parent = nullptr);
};

class Workspace : public QTabWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  WorkspaceTabBar *m_tabBar;
  InventoryOrders *m_tabOrders;
  InventoryBooks *m_tabBooks;
  InventoryPrints *m_tabPrints;
  InventoryCustomers *m_tabCustomers;
  InventoryProviders *m_tabProviders;
  InventoryViews *m_tabViews;

  /**
   * @brief Bücher Inventar öffnen
   */
  int addInventoryBooks(int index);

  /**
   * @brief Drucke & Stiche Inventar öffnen
   */
  int addInventoryPrints(int index);

  /**
   * @brief Kundenregister Inventar öffnen
   */
  int addInventoryCustomers(int index);

  /**
   * @brief Bücher Inventar öffnen
   */
  int addInventoryOrders(int index);

  /**
   * @brief Dienstleister Seite öffnen
   */
  int addInventoryProviders(int index);

  /**
   * @brief Dienstleister Seite öffnen
   */
  int addInventoryViews(int index);

private Q_SLOTS:
  /**
   * @brief Wenn der Tabe schließen Knopf betätigt wurde.
   */
  void closeTabClicked(int);

  /**
   * @brief Buch editieren
   */
  void editBookEntry(int articleId);

  /**
   * @brief Auftrag von Kunden ID erstellen
   */
  void createOrder(int customerId);
  void createOrder(const ProviderOrder &);

  /**
   * @brief Auftrag von Kunden ID erstellen
   */
  void editCustomer(int customerId);

  /**
   * @brief Artikel zu Auftrag hinzufügen
   */
  void addArticleOrder(int articleId);

  /**
   * @brief Artikel beim Dienstleister Aktivieren/Deaktivieren
   */
  void updateArticleCount(int articleId, int count);

  /**
   * @brief Wenn ein Tab nach vornde geholt wird
   */
  void tabViewClicked(int);

protected:
  /**
    @brief Kann der Tab aus dem Speicher freigegeben werden?
  */
  virtual void tabRemoved(int index);

public Q_SLOTS:
  /**
     @brief Signal Mapper Signale @see Workspace::Tab
     @param int = Workspace::Tab
  */
  void openTab(int index = 0);

Q_SIGNALS:
  void s_windowModified(bool);
  /**
    @brief Nachrichten an das Hauptfenster
  */
  void s_postMessage(const QString &);

public:
  /**
     @brief Helfer zur Identifizierung der Tabs
  */
  enum Tab {
    Books = 0x000001,
    Prints = 0x000010,
    Customers = 0x000100,
    Orders = 0x001000,
    Providers = 0x010000,
    Views =  0x100000
  };
  Q_ENUM(Tab)

  explicit Workspace(QWidget *parent = 0);
};

#endif // WORKSPACE_H
