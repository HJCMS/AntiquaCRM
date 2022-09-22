// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordtablemodel.h"

#include <QtCore>
#include <QtWidgets>

KeywordTableModel::KeywordTableModel(const QSqlDatabase &db, QObject *parent)
    : QSqlTableModel{parent, db} {
  setObjectName("keyword_table_model");
  setTable("categories_intern");
}

QVariant KeywordTableModel::data(const QModelIndex &index, int role) const {
  const QVariant val;
  if (!index.isValid() || (role != Qt::DisplayRole))
    return val;

  QVariant value = QSqlTableModel::data(index, role);

  if (index.column() == 2)
    return value.toBool() ? tr("Book Keyword") : tr("Not set");

  return value;
}

QVariant KeywordTableModel::headerData(int section, Qt::Orientation orientation,
                                       int role) const {
  if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
    return QVariant();

  switch (section) {
  case 0:
    return " " + tr("Id") + " ";

  case 1:
    return " " + tr("Keyword") + " ";

  case 2:
    return " " + tr("Books keyword") + " ";

  default:
    return QString(" %1 ").arg(section);
  }

  return QString("%1").arg(section);
}
