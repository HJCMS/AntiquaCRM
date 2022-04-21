// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "datatablemodel.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtSql>

DataTableModel::DataTableModel(const QString &field, QTableView *parent,
                               QSqlDatabase db)
    : QSqlTableModel{parent, db}, p_db(db), p_type(field) {
  setObjectName("DataTableModel");
  setTable(p_table);
  setFilter("k_table_cell LIKE '" + p_type + "'");
  setSort(1, Qt::AscendingOrder); /**< ORDER BY */
  setEditStrategy(QSqlTableModel::OnManualSubmit);
}

bool DataTableModel::updateData(const QModelIndex &index,
                                const QVariant &value) {
  if (!index.isValid())
    return false;

  QString set;
  QStringList list;
  QSqlRecord rec = record(index.row());
  for (int i = 0; i < rec.count(); i++) {
    if (rec.field(i).name() == rec.field(index.column()).name()) {
      set = rec.field(i).name() + "='" + value.toString() + "'";
    } else {
      QString w(rec.field(i).name());
      w.append(" LIKE '");
      w.append(rec.value(i).toString());
      w.append("'");
      list.append(w);
    }
  }

  QString sql("UPDATE " + p_table + " SET ");
  sql.append(set);
  sql.append(" WHERE ");
  sql.append(list.join(" AND "));
  sql.append(";");

  QSqlQuery query(p_db);
  if (!query.exec(sql)) {
    QString err = query.lastError().text();
    qDebug() << Q_FUNC_INFO << sql << err;
    emit s_sqlError(err);
    return false;
  }
  submitAll();
  return true;
}

void DataTableModel::removeData(const QModelIndex &index) {
  selectRow(index.row());
  if (removeRow(index.row()))
    submitAll();
}

bool DataTableModel::insertSqlQuery(const QSqlRecord &record) {
  QStringList fields;
  QStringList values;
  for (int i = 0; i < record.count(); i++) {
    fields.append(record.fieldName(i));
    values.append("'" + record.value(i).toString() + "'");
  }
  QString sql("INSERT INTO " + p_table + " (");
  sql.append(fields.join(","));
  sql.append(") VALUES (");
  sql.append(values.join(","));
  sql.append(");");

  QSqlQuery query(p_db);
  if (!query.exec(sql)) {
    QString err = query.lastError().text();
    qDebug() << Q_FUNC_INFO << sql << err;
    emit s_sqlError(err);
    return false;
  }
  submitAll();
  return true;
}

QVariant DataTableModel::data(const QModelIndex &index, int role) const {
  const QVariant val;
  if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
    return val;

  QVariant item = QSqlTableModel::data(index, role);
  switch (index.column()) {
  case 0: // id k_table_cell
    return p_type;

  case 1: // k_keyword
    return item.toString();

  case 2: // location
    return item.toString();

  default: // Unknown
    return item;
  }
}

QVariant DataTableModel::headerData(int section, Qt::Orientation orientation,
                                    int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0: // id k_table_cell
      return tr("Group");

    case 1: // k_keyword
      return tr("Keyword");

    case 2: // k_description
      return tr("Description");

    default: // Unknown
      return QString("%1").arg(section);
    }
  }
  // Vertikaler eintrag
  return QString("%1").arg(section);
}

bool DataTableModel::setData(const QModelIndex &index, const QVariant &value,
                             int role) {
  if (role != Qt::EditRole)
    return QSqlQueryModel::setData(index, value, role);

  if (!index.isValid() || index.row() >= rowCount())
    return false;

  QVariant buffer;
  switch (index.column()) {
  case 0: // id k_table_cell
    buffer = p_type;
    break;

  case 1: // k_keyword
    buffer = value.toString();
    break;

  case 2: // k_description
    buffer = value.toString();
    break;

  default: // Unknown
    return false;
  }
  return updateData(index, buffer);
}
