// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QObject>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

class InventoryBooks;
class InventoryPrints;
class InventoryCustomers;

class Workspace : public QTabWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  InventoryBooks *m_tabBooks;
  InventoryPrints *m_tabPrints;
  InventoryCustomers *m_tabCustomers;

  /**
     @brief Bücher Inventar öffnen
  */
  void addInventoryBooks(int);

  /**
     @brief Drucke & Stiche Inventar öffnen
  */
  void addInventoryPrints(int);

  /**
     @brief Kundenregister Inventar öffnen
  */
  void addInventoryCustomers(int);

private Q_SLOTS:
  /**
    @brief Wenn der Tabe schließen Knopf betätigt wurde.
  */
  void closeTabClicked(int);

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
  /**
    @brief Nachrichten an das Hauptfenster
  */
  void s_postMessage(const QString &);

public:
  /**
     @brief Helfer zur Identifizierung der Tabs
  */
  enum Tab { Books = 1, Prints = 2, Customers = 3 };
  Q_ENUM(Tab)

  explicit Workspace(QWidget *parent = 0);
};

#endif // WORKSPACE_H
