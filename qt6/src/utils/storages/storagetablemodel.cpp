// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "storagetablemodel.h"

StorageTableModel::StorageTableModel(const QString &table, QObject *parent)
    : AntiquaCRM::ASqlQueryModel{table, parent} {}

const QMap<int, QString> StorageTableModel::headerList() const {
  int _c = 0;
  QMap<int, QString> _m;
  _m.insert(_c++, tr("ID"));                  // sl_id
  _m.insert(_c++, tr("Storage"));             // sl_storage
  _m.insert(_c++, tr("Identifier"));          // sl_identifier
  _m.insert(_c++, tr("Location"));            // sl_location
  _m.insert(_c++, tr("ZVAB"));                // sl_zvab_id
  _m.insert(_c++, tr("ZVAB Category"));       // sl_zvab_name
  _m.insert(_c++, tr("Booklooker"));          // sl_booklooker_id
  _m.insert(_c++, tr("Booklooker Category")); // sl_booklooker_name
  return _m;
}

const QString StorageTableModel::statement(const QString &custom) const {
  QString _sql("SELECT * FROM ref_storage_location WHERE ");
  _sql.append("sl_id>0 AND sl_identifier IS NOT NULL");
  if (custom.length() > 3)
    _sql.append(" AND " + custom);

  _sql.append(" ORDER BY sl_storage;");
  return _sql;
}

bool StorageTableModel::select() { return querySelect(statement()); }

bool StorageTableModel::update(const QString &sql) {
  m_sql->query(sql);
  return (m_sql->lastError().isEmpty());
}

QVariant StorageTableModel::headerData(int section, Qt::Orientation orientation,
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

QVariant StorageTableModel::data(const QModelIndex &item, int role) const {
  if (!item.isValid())
    return QVariant();

  return AntiquaCRM::ASqlQueryModel::data(item, role);
}
