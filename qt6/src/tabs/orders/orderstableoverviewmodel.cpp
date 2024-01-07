// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderstableoverviewmodel.h"
#include "ordersconfig.h"

OrdersTableOverViewModel::OrdersTableOverViewModel(QObject *parent)
    : AntiquaCRM::ASqlQueryModel{ORDERS_SQL_TABLE_NAME, parent} {
  setObjectName("orders_table_model");
}

const QIcon OrdersTableOverViewModel::getHeaderIcon(int column) const {
  QMap<int, QIcon> map;
  map.insert(0, AntiquaCRM::antiquaIcon("user-identity"));
  map.insert(1, AntiquaCRM::antiquaIcon("view-log"));
  map.insert(2, AntiquaCRM::antiquaIcon("view-info"));
  map.insert(3, AntiquaCRM::antiquaIcon("dialog-warning"));
  return map.value(column);
}

const QIcon OrdersTableOverViewModel::getPaymentIcon(int status) const {
  switch (static_cast<AntiquaCRM::OrderPayment>(status)) {
  case AntiquaCRM::OrderPayment::PAYED: /**< Bezahlt */
    return AntiquaCRM::antiquaIcon("dialog-ok-apply");

  case AntiquaCRM::OrderPayment::REMIND: /**< Erinnert */
    return AntiquaCRM::antiquaIcon("flag-yellow");

  case AntiquaCRM::OrderPayment::ADMONISH: /**< Mahnen */
    return AntiquaCRM::antiquaIcon("flag-yellow");

  case AntiquaCRM::OrderPayment::RETURN: /**< Retour */
    return AntiquaCRM::antiquaIcon("action-redo");

  case AntiquaCRM::OrderPayment::COLLPROC: /**< Inkasso */
    return AntiquaCRM::antiquaIcon("flag-red");

  default: /**< Nicht bezahlt */
    return AntiquaCRM::antiquaIcon("dialog-warning");
  }
}

const QString OrdersTableOverViewModel::getPaymentStatus(int status) const {
  switch (static_cast<AntiquaCRM::OrderPayment>(status)) {
  case AntiquaCRM::OrderPayment::NOTPAID: /**< Warte auf Zahlung */
    return tr("Not paid");

  case AntiquaCRM::OrderPayment::PAYED: /**< Bezahlt */
    return tr("Payed");

  case AntiquaCRM::OrderPayment::REMIND: /**< Erinnert */
    return tr("Reminded");

  case AntiquaCRM::OrderPayment::ADMONISH: /**< Mahnen */
    return tr("Admonished");

  case AntiquaCRM::OrderPayment::RETURN: /**< Zurückerstattet */
    return tr("Refunded");

  case AntiquaCRM::OrderPayment::COLLPROC: /**< Inkasso */
    return tr("Payment Collection");

  default:
    return tr("Not paid");
  };
}

const QString OrdersTableOverViewModel::getOrderStatus(int status) const {
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

const QIcon OrdersTableOverViewModel::getOrderStatusIcon(int status) const {
  switch (static_cast<AntiquaCRM::OrderStatus>(status)) {
  case (AntiquaCRM::STARTED): /**< Auftrag angenommen */
    return AntiquaCRM::antiquaIcon("action-edit-document");

  case (AntiquaCRM::FETCHET): /**< Bereit zur Abholung */
    return AntiquaCRM::qrcIcon("package-created");

  case (AntiquaCRM::DELIVERY): /**< Unterwegs */
    return AntiquaCRM::qrcIcon("package-deliver");

  case (AntiquaCRM::DELIVERED): /**< Geliefert */
    return AntiquaCRM::antiquaIcon("dialog-ok-apply");

  case (AntiquaCRM::CANCELED): /**< Storniert */
    return AntiquaCRM::antiquaIcon("edit-delete");

  default:
    return AntiquaCRM::antiquaIcon("dialog-warning");
  }
}

const QString
OrdersTableOverViewModel::getRunTime(const qint64 &seconds) const {
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

const QString OrdersTableOverViewModel::getToolTip(int column) const {
  QMap<int, QString> map;
  map.insert(0, tr("Order Id"));
  map.insert(1, tr("Order Since"));
  map.insert(2, tr("Order status"));
  map.insert(3, tr("Payment status"));
  map.insert(4, tr("Payment recipients"));
  map.insert(5, tr("Delivery Service"));
  map.insert(6, tr("Provider info"));
  map.insert(7, tr("Order lifetime"));
  map.insert(8, tr("Order finished at"));
  return map.value(column);
}

const QMap<int, QString> OrdersTableOverViewModel::headerList() const {
  QMap<int, QString> map;
  map.insert(0, tr("Id"));
  map.insert(1, tr("Since"));
  map.insert(2, tr("Status"));
  map.insert(3, tr("Payed"));
  map.insert(4, tr("Recipients"));
  map.insert(5, tr("Service"));
  map.insert(6, tr("Provider"));
  map.insert(7, tr("Runtime"));
  map.insert(8, tr("Finished"));
  return map;
}

QVariant OrdersTableOverViewModel::headerData(int section,
                                              Qt::Orientation orientation,
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

QVariant OrdersTableOverViewModel::data(const QModelIndex &index,
                                        int role) const {
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
