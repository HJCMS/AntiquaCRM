// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderstablemodel.h"
#include "antiqua_global.h"
#include "myicontheme.h"

#include <AntiquaCRM>

/* QtCore */
#include <QDebug>
#include <QLocale>
#include <QString>
#include <QTime>
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
  setObjectName("OrderTableModel");

  connect(this, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
          this, SLOT(update(const QModelIndex &, const QModelIndex &)));
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

  switch (index.column()) {
  case 0: // ib_id
    return item.toUInt();

  case 1: // o_since
    return item.toDateTime().date().toString(Qt::RFC2822Date);

  case 2: // o_order_status
  {
    OrderStatusList list;
    return list.title(item.toInt());
  }

  case 3: // o_payment_status
  {
    return (item.toBool()) ? tr("received") : tr("waiting");
  }

  case 4: // costumer
    return item.toString();

  case 5: // d_name
    return item.toString();

  case 6: // o_locked
    return (item.toBool()) ? tr("Yes") : tr("No");

  case 7: // o_closed
    return (item.toBool()) ? tr("Yes") : tr("No");

  case 8: // age Siehe SQL Statement!
    return item.toString();

  default: // nicht registrierter Datentype !!!
    return item;
  }
}

QVariant OrdersTableModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const {
  QVariant dummy;
  if (orientation != Qt::Horizontal)
    return dummy;

  if (role == Qt::DecorationRole) {
    switch (section) {
    case 2:
    case 3:
      return myIcon("autostart");

    case 4:
      return myIcon("edit_group");

    default:
      return dummy;
    };
  }

  if (role != Qt::DisplayRole)
    return dummy;

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0: // ib_id
      return setHeaderTitel("ID");

    case 1: // o_since
      return setHeaderTitel(tr("Since"));

    case 2: // o_order_status
      return setHeaderTitel(tr("Status"));

    case 3: // o_payment_status
      return setHeaderTitel(tr("Payment"));

    case 4: // costumer
      return setHeaderTitel(tr("Costumer"));

    case 5: // d_name
      return setHeaderTitel(tr("Deliver"));

    case 6: // o_locked
      return setHeaderTitel(tr("Locked"));

    case 7: // o_closed
      return setHeaderTitel(tr("Closed"));

    case 8: // age
      return setHeaderTitel(tr("Runtime"));

    default:
      return QString("%1").arg(section);
    }
  }
  return QString("%1").arg(section);
}
