// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookstable.h"
#include "bookstablemodel.h"

#include <AntiquaCRM>
#include <QDebug>
#include <QtSql>

BooksTable::BooksTable(QWidget *parent) : InventoryTable{parent} {
  m_model = new BooksTableModel(this);
}

void BooksTable::refreshTable() { qDebug() << Q_FUNC_INFO << "TODO"; }

bool BooksTable::initTable() {
  AntiquaCRM::ASqlQueryFile query("query_tab_books_main");
  if (query.openTemplate()) {
    query.setWhereClause("ib_count>0");
    query.setOrderBy("ib_id");
    query.setSorting(Qt::AscendingOrder);
    query.setLimits(-1);
  }

  QString sql = query.getQueryContent();
  if (m_model->querySelect(sql)) {
    setModel(m_model);
    return true;
  }
  return false;
}
