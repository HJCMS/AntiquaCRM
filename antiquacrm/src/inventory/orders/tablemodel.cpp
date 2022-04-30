// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tablemodel.h"
#include "antiqua_global.h"
#include "myicontheme.h"

#include <AntiquaCRM>

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

TableModel::TableModel(QObject *parent) : QSqlQueryModel{parent} {
  setObjectName("OrderTableModel");

  connect(this, SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
          this, SLOT(update(const QModelIndex &, const QModelIndex &)));
}

void TableModel::update(const QModelIndex &topLeft,
                        const QModelIndex &bottomRight) {
  if (topLeft.isValid() || bottomRight.isValid())
    emit dataUpdated(true);
}

QVariant TableModel::data(const QModelIndex &index, int role) const {
  const QVariant val;
  if (!index.isValid() || (role != Qt::DisplayRole && role != Qt::EditRole))
    return val;

  QVariant item = QSqlQueryModel::data(index, role);

  switch (index.column()) {
  case 0: // ib_id
    return item.toUInt();

  case 1: // o_since
    return item.toDateTime().date().toString(Qt::RFC2822Date);
    ;

  case 2: // o_order_status
  {
    OrderStatusList list;
    return list.title(item.toInt());
  }

  case 3: // o_article_id
    return item.toInt();

  case 4: // costumer
    return item.toString();

  case 5: // d_name
    return item.toString();

  case 6: // o_locked
    return item.toBool();

  case 7: // o_closed
    return item.toBool();

  case 8: // o_order_type ???
    return item;

  case 9: // age TODO
    return item.toTime();

  default: // nicht registrierter Datentype !!!
    return item;
  }
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation,
                                int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0: // ib_id
      return setHeaderTitel("ID");

    case 1: // o_since
      return setHeaderTitel(tr("Since"));

    case 2: // o_order_status
      return setHeaderTitel(tr("Status"));

    case 3: // o_article_id
      return setHeaderTitel(tr("Article ID"));

    case 4: // costumer
      return setHeaderTitel(tr("Costumer"));

    case 5: // d_name
      return setHeaderTitel(tr("Deliver"));

    case 6: // o_locked
      return setHeaderTitel(tr("Locked"));

    case 7: // o_closed
      return setHeaderTitel(tr("Closed"));

    case 8: // o_order_type
      return setHeaderTitel(tr("Order Type"));

    case 9: // age
      return setHeaderTitel(tr("Runtime"));

    default:
      return QString("%1").arg(section);
    }
  }
  return QString("%1").arg(section);
}
