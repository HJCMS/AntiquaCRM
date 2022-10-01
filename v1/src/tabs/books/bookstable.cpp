// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookstable.h"
#include "bookstablemodel.h"

#include <AntiquaCRM>
#include <QDebug>
#include <QtSql>

BooksTable::BooksTable(QWidget *parent) : InventoryTable{parent} {
  setEnableTableViewSorting(true);
  m_model = new BooksTableModel(this);
}

bool BooksTable::sqlQueryTable(const QString &query) {
  // qDebug() << Q_FUNC_INFO << query;
  if (m_model->querySelect(query)) {
    QueryHistory = query;
    setModel(m_model);
    return true;
  }
  return false;
}

void BooksTable::setSortByColumn(int column, Qt::SortOrder order) {
  if (column < 0)
    return;

  QString order_by = m_model->fieldName(column);
  // NOTE Muss hier umgedreht werden!
  Qt::SortOrder sort = Qt::AscendingOrder;
  if (order == Qt::AscendingOrder)
    sort = Qt::DescendingOrder;

  AntiquaCRM::ASqlFiles query("query_tab_books_main");
  if (query.openTemplate()) {
    query.setWhereClause("ib_count>0");
    query.setOrderBy(order_by);
    query.setSorting(sort);
    query.setLimits(getQueryLimit());
  }
  sqlQueryTable(query.getQueryContent());
}

bool BooksTable::initTable() {
  AntiquaCRM::ASqlFiles query("query_tab_books_main");
  if (query.openTemplate()) {
    query.setWhereClause("ib_count>0");
    query.setOrderBy("ib_id");
    query.setSorting(Qt::AscendingOrder);
    query.setLimits(getQueryLimit());
  }
  return sqlQueryTable(query.getQueryContent());
}
