// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "costumerstablemodel.h"
#include "antiqua_global.h"
#include "myicontheme.h"

/* QtCore */
#include <QDateTime>
#include <QDebug>
#include <QLocale>
#include <QString>
#include <QVariant>
#include <QSqlDatabase>
#include <QSqlRecord>

static const QString setHeaderTitel(const QString &t) {
  QString b;
  b.append(" ");
  b.append(t);
  b.append(" ");
  return b;
}

CostumersTableModel::CostumersTableModel(QObject *parent)
    : QSqlQueryModel{parent} {
  setObjectName("CostumersTableModel");
}

QVariant CostumersTableModel::data(const QModelIndex &index, int role) const {
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
    return (item.toString().trimmed() == "C") ? tr("Company") : tr("Personal");

  case 3: // shurename
  case 4: // phone
  case 5: // mobil
  case 6: // address
    return item.toString().trimmed();

  case 7: // since
    return item.toDateTime().date().toString(Qt::RFC2822Date);

  default:
    return item.toString().trimmed();
  }
}

QVariant CostumersTableModel::headerData(int section,
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

    case 3: // shurename
      return setHeaderTitel(tr("Fullname"));

    case 4: // phone
      return setHeaderTitel(tr("Phone"));

    case 5: // mobil
      return setHeaderTitel(tr("Mobil"));

    case 6: // address
      return setHeaderTitel(tr("Address"));

    case 7: // since
      return setHeaderTitel(tr("Since"));

    default:
      return QString("%1").arg(section);
    }
  }
  return QString("%1").arg(section);
}
