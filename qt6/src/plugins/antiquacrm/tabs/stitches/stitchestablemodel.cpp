// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "stitchestablemodel.h"
#include "abstractinput.h"

StitchesTableModel::StitchesTableModel(QObject *parent)
    : AntiquaCRM::ASqlQueryModel{"inventory_prints", parent} {
  setObjectName("stitches_query_model");
}

const QMap<int, QString> StitchesTableModel::headerList() const {
  int i = 0;
  QMap<int, QString> map;
  map.insert(i++, tr("Article Id"));
  map.insert(i++, tr("Duration"));
  map.insert(i++, tr("Image"));
  map.insert(i++, tr("Title"));
  map.insert(i++, tr("Price"));
  map.insert(i++, tr("Storage"));
  map.insert(i++, tr("Changed"));
  map.insert(i++, tr("Author"));
  map.insert(i++, tr("Year"));
  map.insert(i++, tr("Since"));
  return map;
}

QVariant StitchesTableModel::headerData(int section,
                                        Qt::Orientation orientation,
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

QVariant StitchesTableModel::data(const QModelIndex &item, int role) const {
  if (!item.isValid())
    return QVariant();

  if (role == Qt::DecorationRole &&
      fieldName(item.column()).contains("image")) {
    return AntiquaCRM::AntiquaApplIcon("view-image");
  }

  return AntiquaCRM::ASqlQueryModel::data(item, role);
}
