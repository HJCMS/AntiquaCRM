// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "costumerstablemodel.h"
#include "version.h"

/* QtCore */
#include <QtCore/QDateTime>
#include <QtCore/QDebug>
#include <QtCore/QLocale>
#include <QtCore/QString>
#include <QtCore/QVariant>

/* QtSql */
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlRecord>

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
  if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole &&
                           role != Qt::DecorationRole))
    return val;

  QVariant item = QSqlQueryModel::data(index, role);

  switch (index.column()) {
  case 0: //
    return item;

  case 1: //
    return item;

  case 2: //
    return item;

  case 3: //
    return item;

  case 4: //
    return item;

  case 5: //
    return item;

  case 6: //
    return item;

  case 7: //
    return item;

  case 8: //
    return item;

  case 9: //
    return item;

  case 10: //
    return item;

  default: // nicht registrierter Datentype !!!
    return item;
  }
}

QVariant CostumersTableModel::headerData(int section,
                                         Qt::Orientation orientation,
                                         int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0: //
      return setHeaderTitel(tr("1"));

    case 1: //
      return setHeaderTitel(tr("2"));

    case 2: //
      return setHeaderTitel(tr("3"));

    case 3: //
      return setHeaderTitel(tr("4"));

    case 4: // ib_publisher
      return setHeaderTitel(tr("5"));

    case 5: //
      return setHeaderTitel(tr("6"));

    case 6: //
      return setHeaderTitel(tr("7"));

    case 7: //
      return setHeaderTitel(tr("8"));

    case 8: //
      return setHeaderTitel(tr("9"));

    case 9: //
      return setHeaderTitel(tr("10"));

    case 10: //
      return setHeaderTitel(tr("11"));

    default:
      return QString("%1").arg(section);
    }
  }
  return QString("%1").arg(section);
}
