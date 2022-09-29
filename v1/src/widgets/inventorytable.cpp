// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventorytable.h"

InventoryTable::InventoryTable(QWidget *parent) : QTableView{parent} {
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setCornerButtonEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);
  setAlternatingRowColors(true);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);

  /* Kopfzeilen anpassen */
  m_header = horizontalHeader();
  m_header->setDefaultAlignment(Qt::AlignCenter);
  m_header->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_header->setStretchLastSection(true);
  setEnableTableViewSorting(false);

  connect(m_header, SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this,
          SLOT(setSortByColumn(int, Qt::SortOrder)));
}

void InventoryTable::setEnableTableViewSorting(bool b)
{
  setSortingEnabled(b);
  m_header->setSectionsClickable(b);
}

void InventoryTable::makeHistoryQuery() {
  if (QueryHistory.contains("SELECT ", Qt::CaseInsensitive))
    sqlQueryTable(QueryHistory);
}

void InventoryTable::setQueryLimit(int limit) {
  if (limit < 1)
    return;

  QueryLimit = limit;
}

int InventoryTable::getQueryLimit() { return QueryLimit; }
