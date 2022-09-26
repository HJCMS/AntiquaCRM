// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <AntiquaCRM>
#include <QMetaType>
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
class WorkspaceTabBar;

/**
 * @brief Arbeistbereich
 * @class Workspace
 * @ingroup Inventory
 * Verwaltet die Tabulaturen und verteilt die Signale
 * an die einzelnen Tabs.
 */
class Workspace : public QTabWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  WorkspaceTabBar *m_tabBar;          /**< @brief Tabs verwalten */
  InventoryOrders *m_tabOrders;       /**< @brief Aufträge */
  InventoryBooks *m_tabBooks;         /**< @brief Bücher */
  InventoryPrints *m_tabPrints;       /**< @brief Drucke und Stiche */
  InventoryCustomers *m_tabCustomers; /**< @brief Kunden */
  InventoryProviders *m_tabProviders; /**< @brief Dienstleister */
  InventoryViews *m_tabViews;         /**< @brief Ansichten */

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
   * @brief Buch editieren
   */
  void editBookEntry(int articleId);

  /**
   * @brief Drucke und Stiche editieren
   */
  void editPrintsEntry(int articleId);

  /**
   * @brief Wird von InventoryProvider ausgelöst
   * Suche mit Artikel Nummer und "section" nach dem Tab das geöffnet werden
   * soll.
   * @param articleId
   * @param section    'BOOKS'|'PRINTS'|''
   */
  void editArticleEntry(int articleId, const QString &section);

  /**
   * @brief Auftrag von Kunden ID erstellen
   */
  void editCustomerEntry(int customerId);

  /**
   * @brief Auftrag von Kunden ID erstellen
   */
  void createOrder(int customerId);
  void createOrder(const ProviderOrder &);

  /**
   * @brief Artikel zu Auftrag hinzufügen
   */
  void addArticleOrder(int articleId);

  /**
   * @brief Einen vorhandenen Auftrag öffnen!
   */
  void openOrderByOrderId(int orderId);

  /**
   * @brief Artikel beim Dienstleister Aktivieren/Deaktivieren
   */
  void updateArticleCount(int articleId, int count);

  /**
   * @brief Bild mit Artikel Nummer zu einem Dienstleister hochladen
   * @param articleId
   */
  void updateProviderImage(int articleId);

  /**
   * @brief Wenn der Tab schließen Knopf betätigt wurde.
   */
  void closeTabClicked(int);

  /**
   * @brief Wenn ein Tab nach vornde geholt wird
   */
  void tabViewClicked(int);

  /**
   * @brief Einfache Nachrichten an das neue System weiterleiten!
   */
  void prepareMessanger(const QString &);

protected:
  /**
   * @brief Kann der Tab aus dem Speicher freigegeben werden?
   */
  virtual void tabRemoved(int index);

public Q_SLOTS:
  /**
   * @brief Ein Tab öffnen wenn es noch nicht geladen ist.
   * @param int = Workspace::Tab
   */
  void openTab(int index = 0);

Q_SIGNALS:
  /**
   * @brief Änderungen an Huaptfenster weiter geben.
   */
  void s_windowModified(bool);

  /**
   * @brief Nachrichten an das Hauptfenster
   */
  void sendPostMessage(Antiqua::ErrorStatus, const QString &);

public:
  /**
   * @brief Helfer zur Identifizierung der Tabs
   */
  enum Tab {
    Books = 0x000001,
    Prints = 0x000010,
    Customers = 0x000100,
    Orders = 0x001000,
    Providers = 0x010000,
    Views = 0x100000
  };
  Q_ENUM(Tab)

  explicit Workspace(QWidget *parent = 0);
};

#endif // WORKSPACE_H
