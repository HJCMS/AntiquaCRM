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
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  int tabIndex;
  ApplSettings *m_cfg;
  InventoryBooks *m_inventoryBooks;
  InventoryPrints *m_inventoryPrints;
  InventoryCustomers *m_inventoryCustomers;
  void addCustomersTab();

private Q_SLOTS:
  void closeTabClicked(int);

public:
  explicit Workspace(QWidget *parent = 0);
};

#endif // WORKSPACE_H
