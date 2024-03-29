// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "publishertablemodel.h"

PublisherTableModel::PublisherTableModel(QObject *parent)
    : AntiquaCRM::ASqlQueryModel{"ref_book_publisher_de", parent} {}

const QMap<int, QString> PublisherTableModel::headerList() const {
  QMap<int, QString> _m;
  _m.insert(0, tr("Publisher")); // p_name
  _m.insert(1, tr("Location"));  // p_location
  return _m;
}

QVariant PublisherTableModel::headerData(int section,
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

QVariant PublisherTableModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();

  return AntiquaCRM::ASqlQueryModel::data(index, role);
}
