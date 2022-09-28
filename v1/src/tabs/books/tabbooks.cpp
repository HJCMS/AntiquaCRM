// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabbooks.h"
#include "bookstable.h"

#include <QDebug>
#include <QLayout>

TabBooks::TabBooks(QWidget *parent) : Inventory{parent} {
  setObjectName("inventory_books");
  setWindowTitle(tr("Books"));

  m_table = new BooksTable(this);
  insertWidget(0, m_table);
}

void TabBooks::createEntry() { qDebug() << Q_FUNC_INFO << "TODO"; }

void TabBooks::openEntry(qint64 id) { qDebug() << Q_FUNC_INFO; }

void TabBooks::onEnterChanged() {
  if(!initialed) {
    initialed = m_table->initTable();
  } else {
    qDebug() << Q_FUNC_INFO << "Nothing TODO";
  }
}
