// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABBOOKS_H
#define ANTIQUACRM_TABBOOKS_H

#include <QObject>
#include <QWidget>

#ifndef ANTIQUACRM_INVENTORY_H
#include "inventory.h"
#endif

class BooksTable;

class TabBooks final : public Inventory {
  Q_OBJECT

private:
  BooksTable *m_table;

public Q_SLOTS:
  void createEntry();
  void openEntry(qint64 id);
  void onEnterChanged();

public:
  explicit TabBooks(QWidget *parent = nullptr);
};

#endif // TABBOOKS_H
