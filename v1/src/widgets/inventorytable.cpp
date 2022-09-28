// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventorytable.h"

InventoryTable::InventoryTable(QWidget *parent) : QTableView{parent} {
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setCornerButtonEnabled(false);
  setSortingEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);
  setAlternatingRowColors(true);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);

  /* Kopfzeilen anpassen */
  QHeaderView *tHeader = horizontalHeader();
  tHeader->setDefaultAlignment(Qt::AlignCenter);
  tHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
  tHeader->setStretchLastSection(true);
}
