// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customerstablemodel.h"
#include "antiqua_global.h"
#include "myicontheme.h"

/* QtCore */
#include <QDateTime>
#include <QDebug>
#include <QLocale>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QString>
#include <QVariant>

static const QString setHeaderTitel(const QString &t) {
  QString b;
  b.append(" ");
  b.append(t);
  b.append(" ");
  return b;
}

CustomersTableModel::CustomersTableModel(QObject *parent)
    : QSqlQueryModel{parent} {
  setObjectName("CustomersTableModel");
}

const QString CustomersTableModel::displayDate(const QVariant &value) const {
  QDateTime dt(value.toDateTime());
  return QLocale::system().toString(dt, "dd MMMM yyyy");
}

QVariant CustomersTableModel::data(const QModelIndex &index, int role) const {
  const QVariant val;
  if ((role == Qt::DecorationRole) && (index.column() == 1))
    return myIcon("toggle_log");

  if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
    return val;

  QVariant item = QSqlQueryModel::data(index, role);

  switch (index.column()) {
  case 0: // id
    return item.toInt();

  case 1: // c_purchases
    return item.toInt();

  case 2: // company
    if (item.toString().trimmed() == "#PR")
      return tr("Personal");
    else
      return item.toString().trimmed();

  case 3: // since
    return displayDate(item);

  case 4: // shurename
  case 5: // phone
  case 6: // mobil
  case 7: // address
    return item.toString().trimmed();

  default:
    return item.toString().trimmed();
  }
}

QVariant CustomersTableModel::headerData(int section,
                                         Qt::Orientation orientation,
                                         int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0: // id
      return setHeaderTitel(tr("ID"));

    case 1: // c_purchases
      return setHeaderTitel(tr("Purchases"));

    case 2: // company
      return setHeaderTitel(tr("Type"));

    case 3: // since
      return setHeaderTitel(tr("Since"));

    case 4: // shurename
      return setHeaderTitel(tr("Fullname"));

    case 5: // phone
      return setHeaderTitel(tr("Phone"));

    case 6: // mobil
      return setHeaderTitel(tr("Mobil"));

    case 7: // address
      return setHeaderTitel(tr("Address"));

    default:
      return QString("%1").arg(section);
    }
  }
  return QString("%1").arg(section);
}
