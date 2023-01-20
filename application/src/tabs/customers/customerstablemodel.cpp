// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customerstablemodel.h"

#include <QDebug>

#ifndef CUSTOMERS_TABLENAME
#define CUSTOMERS_TABLENAME "customers"
#endif

CustomersTableModel::CustomersTableModel(QObject *parent)
    : AntiquaCRM::ASqlQueryModel{CUSTOMERS_TABLENAME, parent} {
  setObjectName("customers_table_model");
}

const QIcon CustomersTableModel::getHeaderIcon(int column) const {
  QMap<int, QIcon> map;
  map.insert(0, QIcon(":icons/user_identity.png"));
  map.insert(1, QIcon(":icons/view_info.png"));
  map.insert(2, QIcon(":icons/warning.png"));
  return map.value(column);
}

const QIcon CustomersTableModel::getStatusIcon(int status) const {
  if (status < 3)
    return QIcon(":icons/flag-green.png");

  if (status == 3)
    return QIcon(":icons/flag-yellow.png");

  return QIcon(":icons/flag-red.png");
}

const QString CustomersTableModel::getToolTip(int column) const {
  QMap<int, QString> map;
  map.insert(0, tr("Customer Id"));
  map.insert(1, tr("Trustworthiness"));
  map.insert(2, tr("Customer locked?"));
  map.insert(3, tr("Customer's full name"));
  map.insert(4, tr("Company or Person?"));
  map.insert(5, tr("Customer address"));
  map.insert(6, tr("Customer created at"));
  map.insert(7, tr("Customer last changed at"));
  return map.value(column);
}

const QMap<int, QString> CustomersTableModel::headerList() const {
  QMap<int, QString> map;
  map.insert(0, QString());
  map.insert(1, QString());
  map.insert(2, QString());
  map.insert(3, tr("Fullname"));
  map.insert(4, tr("Commercially"));
  map.insert(5, tr("Location"));
  map.insert(6, tr("Created"));
  map.insert(7, tr("Changed"));
  return map;
}

QVariant CustomersTableModel::headerData(int section,
                                         Qt::Orientation orientation,
                                         int role) const {
  if (orientation == Qt::Vertical && role == Qt::DisplayRole)
    return verticalHeader(section, role);

  QMap<int, QString> map = headerList();
  if (orientation == Qt::Horizontal) {
    if (section > map.size())
      return QVariant();

    if (role == Qt::DecorationRole && map.value(section).isEmpty()) {
      return getHeaderIcon(section);
    }

    if (role == Qt::ToolTipRole) {
      return getToolTip(section);
    }
  }

  if (role != Qt::DisplayRole)
    return AntiquaCRM::ASqlQueryModel::headerData(section, orientation, role);

  if (section > map.size())
    return tr("Unknown");

  return setHeaderTitle(map.value(section));
}

QVariant CustomersTableModel::data(const QModelIndex &index, int role) const {
  const QVariant val;
  if (!index.isValid())
    return val;

  if (role == Qt::EditRole)
    return AntiquaCRM::ASqlQueryModel::data(index, role);

  QVariant item = AntiquaCRM::ASqlQueryModel::data(index, role);
  int column = index.column();
  if (role == Qt::DecorationRole) {
    QVariant sub = AntiquaCRM::ASqlQueryModel::data(index, Qt::EditRole);
    if (column == 1) // c_trusted
      return getStatusIcon(sub.toInt());

    if (column == 2) // c_clocked
      return (sub.toBool()) ? getStatusIcon(6) : getStatusIcon(0);

    return val;
  }
  if (column == 1 || column == 2)
    return val; // hidden

  // company
  if (item.toString().trimmed() == "#PR")
    return tr("Personal");

  return item;
}
