// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "datatablemodel.h"
#include "addentrydialog.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtSql/QSqlError>
#include <QtSql/QSqlField>
#include <QtSql/QSqlIndex>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRelationalDelegate>

DataTableModel::DataTableModel(const QString &field, QTableView *parent,
                               QSqlDatabase db)
    : QSqlTableModel{parent, db}, p_db(db), p_type(field) {
  setObjectName("DataTableModel");
  setTable("ui_autofill_keywords");
  setPrimaryKey(p_db.primaryIndex("ui_autofill_keywords"));
  setFilter("k_table_cell LIKE '" + p_type + "'");
  setSort(1, Qt::AscendingOrder);
  setEditStrategy(QSqlTableModel::OnManualSubmit);

  connect(this, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
          this, SLOT(dataUpdate(const QModelIndex &, const QModelIndex &)));
}

void DataTableModel::dataUpdate(const QModelIndex &topLeft,
                                const QModelIndex &bottomRight) {
  // if (topLeft.isValid() || bottomRight.isValid())
    emit s_dataChanged(true);
}

void DataTableModel::insertEntry() {
  QSqlRecord rec = record();
  rec.setValue("k_table_cell", p_type);

  AddEntryDialog dialog;
  if (dialog.exec()) {
    QString data = dialog.value().toString();
    if (data.isEmpty())
      return;

    rec.setValue("k_keyword", data);
  } else {
    return;
  }

  if (insertRowIntoTable(rec)) {
    submitAll();
  } else {
    qDebug() << p_db.lastError().text();
  }
}

void DataTableModel::removeData(const QModelIndex &index) {
  selectRow(index.row());
  if (removeRow(index.row()))
    submitAll();
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
  return QSqlTableModel::setData(index, buffer, role);
}

DataTableModel::~DataTableModel() {}
