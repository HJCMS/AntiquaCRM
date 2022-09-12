// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_VIEWS_H
#define INVENTORY_VIEWS_H

#include "settingswidget.h"

#include <QObject>
#include <QWidget>

class ViewsToolBar;
class ViewsTable;

class InventoryViews : public Inventory {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QString currentView;
  ViewsToolBar *m_toolBar;
  ViewsTable *m_tableView;

  void openEditor(const QString &){};

Q_SIGNALS:
  void s_editBookEntry(int);
  void s_editPrintsEntry(int);
  void s_editCustomerEntry(int);

private Q_SLOTS:
  void searchConvert(const QString &);
  void searchConvert();
  void openTableView();
  void columnEditClicked(int, const QString &);

public:
  explicit InventoryViews(QWidget *parent = nullptr);
  void onEnterChanged();
};

#endif // INVENTORY_VIEWS_H
