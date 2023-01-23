// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cdvtablemodel.h"

#ifndef CDV_TABLENAME
// \d inventory_cdvinyl
#define CDV_TABLENAME "inventory_cdvinyl"
#endif

CDVTableModel::CDVTableModel(QObject *parent)
    : AntiquaCRM::ASqlQueryModel{CDV_TABLENAME, parent} {
  setObjectName("cdv_query_model");
}

const QMap<int, QString> CDVTableModel::headerList() const {
  int i = 0;
  QMap<int, QString> map;
  map.insert(i++, tr("Article Id"));
  map.insert(i++, tr("Duration"));
  map.insert(i++, tr("Image"));
  map.insert(i++, tr("Title"));
  map.insert(i++, tr("Price"));
  map.insert(i++, tr("Author"));
  map.insert(i++, tr("Barcode"));
  map.insert(i++, tr("Year"));
  map.insert(i++, tr("Changed"));
  map.insert(i++, tr("Created"));
  return map;
}

QVariant CDVTableModel::headerData(int section, Qt::Orientation orientation,
                                   int role) const {
  if (orientation == Qt::Vertical && role == Qt::DisplayRole)
    return verticalHeader(section, role);

  if (orientation == Qt::Horizontal && role == Qt::DecorationRole)
    return QVariant();

  if (role == Qt::EditRole) {
    return record().field(section).name();
  } else if (role != Qt::DisplayRole) {
    return QSqlQueryModel::headerData(section, orientation, role);
  }

  QMap<int, QString> map = headerList();
  if (section > map.size())
    return tr("Unknown");

  return setHeaderTitle(map.value(section));
}

QVariant CDVTableModel::data(const QModelIndex &item, int role) const {
  if (!item.isValid())
    return QVariant();

  if (role == Qt::DecorationRole &&
      fieldName(item.column()).contains("image")) {
    return QIcon(":icons/view_image.png");
  } else if (fieldName(item.column()).contains("eangtin")) {
    if (role == Qt::DisplayRole) {
      QString gtin = AntiquaCRM::ASqlQueryModel::data(item, role).toString();
      return (gtin.length() < 12) ? QString() : gtin;
    }
  }
  return AntiquaCRM::ASqlQueryModel::data(item, role);
}
