// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customerstablemodel.h"
#include "antiquaicon.h"

CustomersTableModel::CustomersTableModel(QObject *parent)
    : AntiquaCRM::ASqlQueryModel{"customers", parent} {
  setObjectName("customers_query_model");
}

const QIcon CustomersTableModel::headerIcon(int column) const {
  QMap<int, QIcon> map;
  map.insert(1, AntiquaCRM::antiquaIcon("view-statistics"));
  map.insert(2, AntiquaCRM::antiquaIcon("document-decrypt"));
  return map.value(column);
}

const QIcon CustomersTableModel::trustIcon(int status) const {
  switch (static_cast<AntiquaCRM::CustomerTrustLevel>(status)) {
  case (AntiquaCRM::CustomerTrustLevel::WITH_DELAY):
  case (AntiquaCRM::CustomerTrustLevel::PREPAYMENT):
    return AntiquaCRM::antiquaIcon("flag-yellow");

  case (AntiquaCRM::CustomerTrustLevel::NO_DELIVERY):
    return AntiquaCRM::antiquaIcon("flag-red");

  default:
    return AntiquaCRM::antiquaIcon("flag-green");
  }
}

const QIcon CustomersTableModel::lockIcon(bool locked) const {
  if (locked)
    return AntiquaCRM::antiquaIcon("document-encrypt");

  return AntiquaCRM::antiquaIcon("document-decrypt");
}

const QString CustomersTableModel::toolTip(int column) const {
  int i = 0;
  QMap<int, QString> map;
  map.insert(i++, tr("Customer Id"));
  map.insert(i++, tr("Trustworthiness"));
  map.insert(i++, tr("Customer locked?"));
  map.insert(i++, tr("Customer's full name"));
  map.insert(i++, tr("Company or Person?"));
  map.insert(i++, tr("Customer address"));
  map.insert(i++, tr("Customer created at"));
  map.insert(i++, tr("Customer last changed at"));
  return map.value(column);
}

const QMap<int, QString> CustomersTableModel::headerList() const {
  int i = 0;
  QMap<int, QString> map;
  map.insert(i++, tr("Id"));
  map.insert(i++, QString());
  map.insert(i++, QString());
  map.insert(i++, tr("Fullname"));
  map.insert(i++, tr("Commercially"));
  map.insert(i++, tr("Location"));
  map.insert(i++, tr("Created"));
  map.insert(i++, tr("Changed"));
  return map;
}

QVariant CustomersTableModel::headerData(int section,
                                         Qt::Orientation orientation,
                                         int role) const {
  if (orientation == Qt::Vertical && role == Qt::DisplayRole)
    return verticalHeader(section, role);

  QMap<int, QString> _map = headerList();
  if (section > _map.size())
    return tr("Unknown");

  if (orientation == Qt::Horizontal) {
    if (role == Qt::DecorationRole && _map.value(section).isEmpty()) {
      return headerIcon(section);
    }

    if (role == Qt::ToolTipRole) {
      return toolTip(section);
    }
  }

  if (role != Qt::DisplayRole)
    return AntiquaCRM::ASqlQueryModel::headerData(section, orientation, role);

  return setHeaderTitle(_map.value(section));
}

QVariant CustomersTableModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid())
    return QVariant();

  int _column = index.column();
  QVariant _value = AntiquaCRM::ASqlQueryModel::data(index, role);
  if (role == Qt::EditRole)
    return _value;

  if (role == Qt::DecorationRole) {
    QVariant _s = AntiquaCRM::ASqlQueryModel::data(index, Qt::EditRole);
    if (_column == 1) // c_trusted
      return trustIcon(_s.toInt());

    if (_column == 2) // c_clocked
      return lockIcon(_s.toBool());

    return _value;
  }

  if (_column == 1 || _column == 2)
    return QVariant(); // hidden

  // company
  if (_value.toString().trimmed() == "#PR")
    return tr("Personal");

  return _value;
}
