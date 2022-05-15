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
class InventoryCostumers;
class InventoryOrders;

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
  InventoryCostumers *m_tabCostumers;

  /**
     @brief Bücher Inventar öffnen
  */
  int addInventoryBooks(int index);

  /**
     @brief Drucke & Stiche Inventar öffnen
  */
  int addInventoryPrints(int index);

  /**
     @brief Kundenregister Inventar öffnen
  */
  int addInventoryCostumers(int index);

  /**
     @brief Bücher Inventar öffnen
  */
  int addInventoryOrders(int index);

private Q_SLOTS:
  /**
    @brief Wenn der Tabe schließen Knopf betätigt wurde.
  */
  void closeTabClicked(int);

  /**
   * @brief Auftrag von Kunden ID erstellen
   */
  void createOrder(int costumerId);

  /**
   * @brief Artikel zu Auftrag hinzufügen
   */
  void addArticleOrder(int articleId);

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
  enum Tab { Books = 1, Prints = 2, Costumers = 3, Orders = 4 };
  Q_ENUM(Tab)

  explicit Workspace(QWidget *parent = 0);
};

#endif // WORKSPACE_H
