// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QObject>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

class ApplSettings;
class InventoryBooks;
class InventoryPrints;
class InventoryCustomers;

class Workspace : public QTabWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  ApplSettings *m_cfg;
  InventoryBooks *m_tabBooks;
  InventoryPrints *m_tabPrints;
  InventoryCustomers *m_tabCustomers;
  void addInventoryBooks(int);
  void addInventoryPrints(int);
  void addInventoryCustomers(int);

private Q_SLOTS:
  void closeTabClicked(int);

protected:
  virtual void tabRemoved(int index);

Q_SIGNALS:
  void postMessage(const QString &);
  void tabEntered(int);

public Q_SLOTS:
  /**
     @brief Signal Mapper Signale @see Workspace::Tab
     @param int = Workspace::Tab
  */
  void openTab(int index = 0);

public:
  enum Tab { Books = 1, Prints = 2, Customers = 3 };
  Q_ENUM(Tab)

  explicit Workspace(QWidget *parent = 0);
};

#endif // WORKSPACE_H
