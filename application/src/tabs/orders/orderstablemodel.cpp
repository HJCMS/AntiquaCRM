// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderstablemodel.h"

#include <QIcon>

#ifndef ORDERS_TABLENAME
#define ORDERS_TABLENAME "inventory_orders"
#endif

OrdersTableModel::OrdersTableModel(QObject *parent)
    : AntiquaCRM::ASqlQueryModel{ORDERS_TABLENAME, parent} {
  setObjectName("orders_table_model");
}

const QIcon OrdersTableModel::getHeaderIcon(int column) const {
  QMap<int, QIcon> map;
  map.insert(0, QIcon(":icons/user_identity.png"));
  map.insert(1, QIcon(":icons/view_log.png"));
  map.insert(2, QIcon(":icons/view_info.png"));
  map.insert(3, QIcon(":icons/warning.png"));
  return map.value(column);
}

const QIcon OrdersTableModel::getPaymentIcon(int status) const {
  switch (static_cast<AntiquaCRM::OrderPayment>(status)) {
  case AntiquaCRM::OrderPayment::PAYED: /**< Bezahlt */
    return QIcon("://icons/action_ok.png");

  case AntiquaCRM::OrderPayment::REMIND: /**< Erinnert */
    return QIcon("://icons/flag-yellow.png");

  case AntiquaCRM::OrderPayment::ADMONISH: /**< Mahnen */
    return QIcon("://icons/flag-yellow.png");

  case AntiquaCRM::OrderPayment::RETURN: /**< Retour */
    return QIcon("://icons/action_undo.png");

  case AntiquaCRM::OrderPayment::COLLPROC: /**< Inkasso */
    return QIcon("://icons/flag-red.png");

  default: /**< Nicht bezahlt */
    return QIcon("://icons/warning.png");
  }
}

const QString OrdersTableModel::getPaymentStatus(int status) const {
  switch (static_cast<AntiquaCRM::OrderPayment>(status)) {
  case AntiquaCRM::OrderPayment::NOTPAID: /**< Warte auf Zahlung */
    return tr("Not paid");

  case AntiquaCRM::OrderPayment::PAYED: /**< Bezahlt */
    return tr("Payed");

  case AntiquaCRM::OrderPayment::REMIND: /**< Erinnert */
    return tr("Reminded");

  case AntiquaCRM::OrderPayment::ADMONISH: /**< Mahnen */
    return tr("Admonished");

  case AntiquaCRM::OrderPayment::RETURN: /**< Retour */
    return tr("Returned");

  case AntiquaCRM::OrderPayment::COLLPROC: /**< Inkasso */
    return tr("Payment Collection");

  default:
    return tr("Not paid");
  };
}

const QString OrdersTableModel::getOrderStatus(int status) const {
  switch (static_cast<AntiquaCRM::OrderStatus>(status)) {
  case (AntiquaCRM::STARTED): /**< Auftrag erstellt */
    return tr("Order created");

  case (AntiquaCRM::FETCHET): /**< Bereit zur Abholung */
    return tr("Ready for pickup");

  case (AntiquaCRM::DELIVERY): /**< Auslieferung */
    return tr("Delivery");

  case (AntiquaCRM::DELIVERED): /**< Geliefert */
  case (AntiquaCRM::COMPLETED): /**< @deprecated */
    return tr("Delivered");

  case (AntiquaCRM::CANCELED): /**< Storniert */
    return tr("Canceled");

  default:
    return tr("Open");
  }
}

const QIcon OrdersTableModel::getOrderStatusIcon(int status) const {
  switch (static_cast<AntiquaCRM::OrderStatus>(status)) {
  case (AntiquaCRM::STARTED): /**< Auftrag angenommen */
    return QIcon("://icons/edit.png");

  case (AntiquaCRM::FETCHET): /**< Bereit zur Abholung */
    return QIcon("://icons/action_ready_pickup.png");

  case (AntiquaCRM::DELIVERY): /**< Unterwegs */
    return QIcon("://icons/delivery48.png");

  case (AntiquaCRM::DELIVERED): /**< Geliefert */
    return QIcon("://icons/action_ok.png");

  case (AntiquaCRM::CANCELED): /**< Storniert */
    return QIcon("://icons/action_cancel.png");

  default:
    return QIcon("://icons/warning.png");
  }
}

const QString OrdersTableModel::getRunTime(const qint64 &seconds) const {
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

const QString OrdersTableModel::getToolTip(int column) const {
  QMap<int, QString> map;
  map.insert(0, tr("Order Id"));
  map.insert(1, tr("Order Since"));
  map.insert(2, tr("Order status"));
  map.insert(3, tr("Payment status"));
  map.insert(4, tr("Customer"));
  map.insert(5, tr("Delivery Service"));
  map.insert(6, tr("Provider info"));
  map.insert(7, tr("Order lifetime"));
  map.insert(8, tr("Order finished at"));
  return map.value(column);
}

const QMap<int, QString> OrdersTableModel::headerList() const {
  QMap<int, QString> map;
  map.insert(0, tr("Id"));
  map.insert(1, tr("Since"));
  map.insert(2, tr("Status"));
  map.insert(3, tr("Payed"));
  map.insert(4, tr("Customer"));
  map.insert(5, tr("Service"));
  map.insert(6, tr("Provider"));
  map.insert(7, tr("Runtime"));
  map.insert(8, tr("Finished"));
  return map;
}

QVariant OrdersTableModel::headerData(int section, Qt::Orientation orientation,
                                      int role) const {
  if (orientation == Qt::Vertical && role == Qt::DisplayRole)
    return verticalHeader(section, role);

  QMap<int, QString> map = headerList();
  if (orientation == Qt::Horizontal) {
    if (section > map.size())
      return QVariant();

    if (role == Qt::DecorationRole && map.value(section).isEmpty()) {
      return getHeaderIcon(section);
    }

    if (role == Qt::ToolTipRole) {
      return getToolTip(section);
    }
  }

  if (role != Qt::DisplayRole)
    return AntiquaCRM::ASqlQueryModel::headerData(section, orientation, role);

  if (section > map.size())
    return QVariant();

  return map.value(section);
}

QVariant OrdersTableModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid() && (role != (Qt::DisplayRole & Qt::DecorationRole)))
    return AntiquaCRM::ASqlQueryModel::data(index, role);

  int column = index.column();
  if (role == Qt::TextAlignmentRole) {
    if (column == 7) {
      return (int)(Qt::AlignRight | Qt::AlignVCenter);
    } else {
      return (int)(Qt::AlignLeft | Qt::AlignVCenter);
    }
  }

  QVariant item = AntiquaCRM::ASqlQueryModel::data(index, role);
  if (role == Qt::DisplayRole && column == 2) {
    return getOrderStatus(item.toInt());
  }

  if (role == Qt::DisplayRole && column == 3) {
    return getPaymentStatus(item.toInt());
  }

  if (role == Qt::DisplayRole && column == 7) {
    return getRunTime(item.toInt());
  }

  if (role == Qt::DecorationRole && column == 2) {
    QVariant status = AntiquaCRM::ASqlQueryModel::data(index, Qt::EditRole);
    return getOrderStatusIcon(status.toInt());
  }

  if (role == Qt::DecorationRole && column == 3) {
    QVariant sub = AntiquaCRM::ASqlQueryModel::data(index, Qt::EditRole);
    return getPaymentIcon(sub.toInt());
  }

  return item;
}
