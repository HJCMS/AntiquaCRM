// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookstablemodel.h"

BooksTableModel::BooksTableModel(QObject *parent)
    : AntiquaCRM::ASqlQueryModel{parent} {
  setObjectName("books_query_model");
}
