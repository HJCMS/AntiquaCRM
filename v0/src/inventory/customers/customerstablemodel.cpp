// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customerstablemodel.h"
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

const QIcon CustomersTableModel::getHeaderIcon(int section) const {
  switch (section) {
  case 1:
    return myIcon("toggle_log");

  case 2:
    return myIcon("group");

  case 3:
    return myIcon("messagebox_warning");

  case 5:
    return myIcon("identity");

  default:
    return QIcon();
  };
}

const QIcon CustomersTableModel::getTrustIcon(int level) const {
  if (level < 3) {
    return myIcon("flag-green");
  } else if (level == 3) {
    return myIcon("flag-yellow");
  } else {
    return myIcon("flag-red");
  }
}

QVariant CustomersTableModel::data(const QModelIndex &index, int role) const {
  const QVariant val;
  if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole &&
                           role != Qt::DecorationRole))
    return val;

  QVariant item = QSqlQueryModel::data(index, role);
  if (role == Qt::DecorationRole) {
    QVariant edit = QSqlQueryModel::data(index, Qt::EditRole);
    if (index.column() == 2) // c_trusted
      return getTrustIcon(edit.toInt());

    if (index.column() == 3) // c_clocked
      return (edit.toBool()) ? myIcon("flag-red") : myIcon("flag-green");

  } else if (role == Qt::DisplayRole) {
    switch (index.column()) {
    case 0: // id
      return item.toInt();

    case 1: // c_purchases
      return item.toInt();

    case 3: // locked
      return (item.toBool()) ? tr("Yes") : QString();

    case 4: // shurename
      return item.toString();

    case 5: // company
    {
      QString cpn(item.toString().trimmed());
      return (cpn == "#PR") ? tr("Personal") : cpn;
    }

    case 6: // since
      return displayDate(item);

    case 7: // location
      return item.toString();

    default:
      return QString();
    }
  }
  return item;
}

QVariant CustomersTableModel::headerData(int section,
                                         Qt::Orientation orientation,
                                         int role) const {
  if (role == Qt::DecorationRole && orientation == Qt::Horizontal)
    return getHeaderIcon(section);

  if (role == Qt::ToolTipRole && orientation == Qt::Horizontal) {
    switch (section) {
    case 0: // id
      return tr("Customer ID");

    case 1: // Einkäufe
      return tr("Purchases");

    case 2: // Vertrauenswürdigkeit
      return tr("Trustworthiness");

    case 3: // Kunde gesperrt?
      return tr("Customer locked?");

    case 4: // Vollständiger Name des Kunden
      return tr("Customer's full name");

    case 5: // Firma oder Person?
      return tr("Company or Person?");

    case 6: // Kundendaten erstellt am...
      return tr("Customer data created on...");

    case 7: // Kundenadresse
      return tr("Customer address");

    default:
      return QVariant();
    }
  }

  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0: // id
      return setHeaderTitel(tr("ID"));

    case 1: // c_purchases
      return QVariant();

    case 2: // c_trusted
      return QVariant();

    case 3: // c_locked
      return QVariant();

    case 4: // shurename
      return setHeaderTitel(tr("Fullname"));

    case 5: // company
      return setHeaderTitel(tr("Commercially"));

    case 6: // since
      return setHeaderTitel(tr("Since"));

    case 7: // location
      return setHeaderTitel(tr("Location"));

    default:
      return QString("%1").arg(section);
    }
  }
  return QString("%1").arg(section);
}
