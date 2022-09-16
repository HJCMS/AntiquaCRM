// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderstablemodel.h"
#include "myicontheme.h"

#include <AntiquaCRM>

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

OrdersTableModel::OrdersTableModel(QObject *parent) : QSqlQueryModel{parent} {
  setObjectName("orders_table_model");

  connect(this, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
          this, SLOT(update(const QModelIndex &, const QModelIndex &)));
}

const QString OrdersTableModel::displayDate(const QVariant &value) const {
  QDateTime dt(value.toDateTime());
  return QLocale::system().toString(dt, "dd MMMM yyyy");
}

const QString OrdersTableModel::runTimeString(const qint64 &seconds) const {
  QString retval = tr("no time");
  // Sekunden pro Tag (24 * 60 * 60)
  const qint64 spt = 86400;
  qint64 d = (seconds / spt);
  QTime t = QTime(0, 0).addSecs(seconds);
  qint64 h = t.hour();
  qint64 m = t.minute();
  if (d > 0) {
    return tr("%1 days, %2 hours, %3 minutes").arg(d).arg(h).arg(m);
  } else if (h > 1) {
    return tr("%1 hours, %2 minutes").arg(h).arg(m);
  } else {
    return tr("%1 minutes").arg(m);
  }
  return retval;
}

void OrdersTableModel::update(const QModelIndex &topLeft,
                              const QModelIndex &bottomRight) {
  if (topLeft.isValid() || bottomRight.isValid())
    emit dataUpdated(true);
}

QVariant OrdersTableModel::data(const QModelIndex &index, int role) const {
  const QVariant val;
  if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
    return val;

  QVariant item = QSqlQueryModel::data(index, role);
  if (role == Qt::DisplayRole) {
    switch (index.column()) {
    case 1: // o_since
      return displayDate(item);

    case 2: // o_order_status
    {
      OrderStatusList list;
      return list.title(item.toInt());
    }

    case 3: // o_payment_status
    {
      return (item.toBool()) ? tr("received") : tr("waiting");
    }

    case 6: // o_provider_info
      return item;

    case 7: // age Siehe SQL Statement!
      return runTimeString(item.toInt());

    case 8: // Ausgeliefert am!
      return runTimeString(item.toInt());
    }
  }
  return item;
}

QVariant OrdersTableModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const {
  QVariant dummy;
  if (orientation != Qt::Horizontal)
    return dummy;

  if (role == Qt::DecorationRole) {
    return myIcon("autostart");
  }

  if (role != Qt::DisplayRole)
    return dummy;

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0: // o_id
      return setHeaderTitel("ID");

    case 1: // o_since
      return setHeaderTitel(tr("Since"));

    case 2: // o_order_status
      return setHeaderTitel(tr("Status"));

    case 3: // o_payment_status
      return setHeaderTitel(tr("Payment"));

    case 4: // customer
      return setHeaderTitel(tr("Customer"));

    case 5: // d_name
      return setHeaderTitel(tr("Deliver"));

    case 6: // o_provider_info
      return setHeaderTitel(tr("Provider"));

    case 7: // age
      return setHeaderTitel(tr("Runtime"));

    case 8: // Ausgeliefert am!
      return setHeaderTitel(tr("Delivered"));

    default:
      return QString("%1").arg(section);
    }
  }
  return QString("%1").arg(section);
}
