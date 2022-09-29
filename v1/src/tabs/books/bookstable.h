// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_BOOKSTABLE_H
#define ANTIQUACRM_BOOKSTABLE_H

#include <QObject>
#include <QWidget>

#ifndef ANTIQUACRM_INVENTORYTABLE_H
#include "inventorytable.h"
#endif

class BooksTableModel;
class BooksTable final : public InventoryTable {
  Q_OBJECT

private:
  BooksTableModel *m_model;
  bool sqlQueryTable(const QString &query);

private Q_SLOTS:
  void setSortByColumn(int column, Qt::SortOrder order);

// public Q_SLOTS:

public:
  explicit BooksTable(QWidget *parent = nullptr);
  bool initTable();
};

#endif // ANTIQUACRM_BOOKSTABLE_H
