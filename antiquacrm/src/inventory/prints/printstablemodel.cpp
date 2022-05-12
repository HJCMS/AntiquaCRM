// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printstablemodel.h"
#include "antiqua_global.h"
#include "myicontheme.h"

/* QtCore */
#include <QDateTime>
#include <QDebug>
#include <QLocale>
#include <QString>
#include <QVariant>

/* QtSql */
#include <QSqlDatabase>
#include <QSqlRecord>

static const QString setHeaderTitel(const QString &t) {
  QString b;
  b.append(" ");
  b.append(t);
  b.append(" ");
  return b;
}

PrintsTableModel::PrintsTableModel(QObject *parent) : QSqlQueryModel{parent} {
  setObjectName("PrintsTableModel");
}

QVariant PrintsTableModel::data(const QModelIndex &index, int role) const {
  const QVariant val;
  if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole &&
                           role != Qt::DecorationRole))
    return val;

  QVariant item = QSqlQueryModel::data(index, role);

  if (!index.isValid() || (role == Qt::DecorationRole)) {
    if (index.column() == 10) // image_exists
      return myIcon("image");

    return val;
  }

  switch (index.column()) {
  case 0: // ip_id
    return item.toUInt();

  case 1: // ip_count
    return item.toUInt();

  case 2: // ip_title
    return item.toString();

  case 3: // ip_author
  {
    QString buf = item.toString();
    if (buf.contains("NOT_SET")) {
      return QString();
    } else {
      return buf;
    }
  }

  case 4: // ip_technique
    return item.toString();

  case 5: // ip_year
  {
    int y = item.toInt();
    if (y != 0) {
      return y;
    } else {
      return QString();
    }
  }

  case 6: // ip_price
  {
    bool b = false;
    QVariant buffer = item.toDouble(&b);
    if (!b) {
      buffer = 0.00;
    }
    if (role == Qt::DisplayRole) {
      QString str;
      buffer = str.asprintf("%.02f €", buffer.toFloat());
    }
    return buffer;
  }

  case 7: // storage
    return item.toString();

  case 8: // ip_landscape
    return (item.toBool()) ? tr("Yes") : tr("No");

  case 9: // ip_changed
    return item.toDateTime().date().toString(Qt::RFC2822Date);

  case 10: // image_exists
    return (item.toBool()) ? tr("Yes") : tr("No");

  default: // nicht registrierter Datentype !!!
    return item;
  }
}

QVariant PrintsTableModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0: // ip_id
      return setHeaderTitel(tr("Article"));

    case 1: // ip_count
      return setHeaderTitel(tr("Count"));

    case 2: // ip_title
      return setHeaderTitel(tr("Title"));

    case 3: // ip_author
      return setHeaderTitel(tr("Author"));

    case 4: // ip_technique
      return setHeaderTitel(tr("Technique"));

    case 5: // ib_year
      return setHeaderTitel(tr("Year"));

    case 6: // ib_price
      return setHeaderTitel(tr("Price"));

    case 7: // storage
      return setHeaderTitel(tr("Storage Location"));

    case 8: // ip_landscape
      return setHeaderTitel(tr("Landscape"));

    case 9: // ip_changed
      return setHeaderTitel(tr("Last change"));

    case 10: // image_exists
      return setHeaderTitel(tr("Image exits"));

    default:
      return QString("%1").arg(section);
    }
  }
  return QString("%1").arg(section);
}
