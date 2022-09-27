// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabbooks.h"
#include "bookstable.h"

#include <QDebug>
#include <QLayout>

TabBooks::TabBooks(QWidget *parent) : Inventory{parent} {
  setObjectName("inventory_books");
  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_table = new BooksTable(this);
  layout->addWidget(m_table);

  setLayout(layout);
}

void TabBooks::createEntry() { qDebug() << Q_FUNC_INFO << "TODO"; }

void TabBooks::openEntry(qint64 id) { qDebug() << Q_FUNC_INFO; }
