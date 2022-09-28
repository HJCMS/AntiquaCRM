// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookstablemodel.h"

#include <QDebug>

#ifndef BOOKS_TABLENAME
#define BOOKS_TABLENAME "inventory_books"
#endif

BooksTableModel::BooksTableModel(QObject *parent)
    : AntiquaCRM::ASqlQueryModel{BOOKS_TABLENAME, parent} {
  setObjectName("books_query_model");
}

const QMap<int, QString> BooksTableModel::headerList() const {
  QMap<int, QString> map;
  map.insert(0, tr("Article Id"));
  map.insert(1, tr("Duration"));
  map.insert(2, tr("Image"));
  map.insert(3, tr("Title"));
  map.insert(4, tr("Price"));
  map.insert(5, tr("Author"));
  map.insert(6, tr("Publisher"));
  map.insert(7, tr("Year"));
  map.insert(8, tr("Storage"));
  map.insert(9, tr("ISBN"));
  map.insert(10, tr("Changed"));
  return map;
}

QVariant BooksTableModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const {
  if (orientation == Qt::Vertical && role == Qt::DisplayRole)
    return verticalHeader(section, role);

  if (orientation == Qt::Horizontal && role == Qt::DecorationRole)
    return setHeaderIcon(section);

  if (role != Qt::DisplayRole)
    return QSqlQueryModel::headerData(section, orientation, role);

  QMap<int, QString> map = headerList();
  if (section > map.size())
    return tr("Unknown");

  return map.value(section) + " ";
}
