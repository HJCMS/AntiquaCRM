// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookstable.h"
#include "bookstablemodel.h"

#include <QDebug>
#include <QtSql>

BooksTable::BooksTable(QWidget *parent) : InventoryTable{parent} {
  //  m_model = new BooksTableModel(this);
  //  setModel(m_model);
  AntiquaCRM::ASqlQueryModel *model = new AntiquaCRM::ASqlQueryModel(this);
  if (model->querySelect("SELECT * FROM inventory_books ORDER BY ib_id;"))
    setModel(model);
}

void BooksTable::refreshTable() { qDebug() << Q_FUNC_INFO << "TODO"; }
