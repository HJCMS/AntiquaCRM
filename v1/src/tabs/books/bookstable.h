// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_BOOKSTABLE_H
#define ANTIQUACRM_BOOKSTABLE_H

#include <QObject>
#include <QWidget>

#ifndef ANTIQUACRM_INVENTORY_H
#include "inventory.h"
#endif

class BooksTableModel;
class BooksTable final : public InventoryTable {

private:
  BooksTableModel *m_model;

public Q_SLOTS:
  void refreshTable();

public:
  explicit BooksTable(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_BOOKSTABLE_H
