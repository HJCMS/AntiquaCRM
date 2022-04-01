// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookstablemodel.h"
#include "version.h"

/* QtCore */
#include <QtCore/QDebug>
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

BooksTableModel::BooksTableModel(QObject *parent) : QSqlQueryModel{parent} {
  setObjectName("BooksTableModel");
}

QVariant BooksTableModel::data(const QModelIndex &index, int role) const {
  const QVariant val;
  if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
    return val;

  QVariant item = QSqlQueryModel::data(index, role);
  switch (index.column()) {
  case 0: // ib_id
    return item.toUInt();

  case 1: // ib_count
    return item.toUInt();

  case 2: // ib_title
    return item.toString();

  case 3: // ib_author
  {
    QString buf = item.toString();
    if (buf.contains("NOT_SET")) {
      return QString();
    } else {
      return buf;
    }
  }

  case 4: // ib_publisher
    return item.toString();

  case 5: // ib_year
  {
    int y = item.toInt();
    if (y != 0) {
      return y;
    } else {
      return QString();
    }
  }

  case 6: // ib_price
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

  case 7: // ib_storage
    return item.toString();

  case 8: // ib_isbn
  {
    bool b = true;
    uint num = item.toUInt(&b);
    if (b && num != 0) {
      return num;
    } else {
      return QString();
    }
  } break;

  default: // nicht registrierter Datentype !!!
    return item;
  }
}

QVariant BooksTableModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0: // ib_id
      return setHeaderTitel(tr("Article"));

    case 1: // ib_count
      return setHeaderTitel(tr(" Count "));

    case 2: // ib_title
      return setHeaderTitel(tr("Titel"));

    case 3: // ib_author
      return setHeaderTitel(tr("Author"));

    case 4: // ib_publisher
      return setHeaderTitel(tr("Publisher"));

    case 5: // ib_year
      return setHeaderTitel(tr("Year"));

    case 6: // ib_price
      return setHeaderTitel(tr("Price"));

    case 7: // ib_storage
      return setHeaderTitel(tr("Storage Location"));

    case 8:
      return setHeaderTitel(tr("ISBN"));

    default:
      return QString("%1").arg(section);
    }
  }
  return QString("%1").arg(section);
}
