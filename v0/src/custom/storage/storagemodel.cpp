// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "storagemodel.h"
#include "myicontheme.h"

#include <QSqlField>
#include <QSqlIndex>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QtCore>

StorageModel::StorageModel(const QSqlDatabase &db, QObject *parent)
    : QSqlTableModel{parent, db} {
  setTable("ref_storage_location");
  setPrimaryKey(QSqlIndex("ref_storage_location", "storage_location_pkey"));
  setFilter("sl_id>0");
  setSort(fieldIndex("sl_storage"), Qt::AscendingOrder);
}

QVariant StorageModel::headerData(int section, Qt::Orientation orientation,
                                  int role) const {
  if (role == Qt::DecorationRole && orientation == Qt::Horizontal)
    return myIcon("database");

  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0: // sl_id
      return tr("ID");

    case 1: // sl_storage
      return tr("Storage") + " ";

    case 2: // sl_identifier
      return tr("Identifier");

    case 3: // sl_location
      return tr("Location");

    default:
      return section;
    }
  }
  return QString("%1").arg(section);
}
