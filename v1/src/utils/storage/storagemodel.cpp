// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "storagemodel.h"

#include <QSqlField>
#include <QSqlIndex>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QIcon>
#include <QtCore>

StorageModel::StorageModel(const QSqlDatabase &db, QObject *parent)
    : QSqlTableModel{parent, db} {
  setTable("ref_storage_location");
  setPrimaryKey(QSqlIndex("ref_storage_location", "storage_location_pkey"));
  setFilter("sl_id>0 AND sl_identifier IS NOT NULL");
  setSort(fieldIndex("sl_storage"), Qt::AscendingOrder);
}

QVariant StorageModel::headerData(int section, Qt::Orientation orientation,
                                  int role) const {
  if (role == Qt::DecorationRole && orientation == Qt::Horizontal)
    return QIcon(":icons/database.png");

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

    case 4: // sl_zvab_id
      return "ZVAB";

    case 5: // sl_zvab_name
      return "ZVAB " + tr("Category");

    case 6: // sl_booklooker_id
      return "Booklooker";

    case 7: // sl_booklooker_name
      return "Booklooker " + tr("Category");

    default:
      return section;
    }
  }
  return QString("%1").arg(section);
}
