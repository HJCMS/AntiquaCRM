// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderpaymentstatusselecter.h"

#include <QApplication>
#include <QPalette>

OrderPaymentStatusModel::OrderPaymentStatusModel(QWidget *parent)
    : QAbstractItemModel{parent} {
  statusList.append(createItem(AntiquaCRM::OrderPayment::NOTPAID));
  statusList.append(createItem(AntiquaCRM::OrderPayment::PAYED));
  statusList.append(createItem(AntiquaCRM::OrderPayment::REMIND));
  statusList.append(createItem(AntiquaCRM::OrderPayment::ADMONISH));
  statusList.append(createItem(AntiquaCRM::OrderPayment::RETURN));
  statusList.append(createItem(AntiquaCRM::OrderPayment::COLLPROC));
}

OrderPaymentStatusModel::StatusItem
OrderPaymentStatusModel::createItem(AntiquaCRM::OrderPayment status) {
  OrderPaymentStatusModel::StatusItem item;
  switch (status) {
  case (AntiquaCRM::OrderPayment::PAYED): {
    item.status = AntiquaCRM::OrderPayment::PAYED;
    item.title = tr("Payed");
    item.toolTip = tr("Order paid and completed!");
    item.decoration = QIcon("://icons/action_ok.png");
  } break;

  case (AntiquaCRM::OrderPayment::REMIND): {
    item.status = AntiquaCRM::OrderPayment::REMIND;
    item.title = tr("Remindet");
    item.toolTip = tr("Buyer was reminded.");
    item.decoration = QIcon("://icons/flag-yellow.png");
  } break;

  case (AntiquaCRM::OrderPayment::ADMONISH): {
    item.status = AntiquaCRM::OrderPayment::ADMONISH;
    item.title = tr("Admonished");
    item.toolTip = tr("The Buyer was admonished!");
    item.decoration = QIcon("://icons/flag-yellow.png");
  } break;

  case (AntiquaCRM::OrderPayment::RETURN): {
    item.status = AntiquaCRM::OrderPayment::RETURN;
    item.title = tr("Returning");
    item.toolTip = tr("Returning Process (Read Only)");
    item.decoration = QIcon("://icons/action_undo.png");
  } break;

  case (AntiquaCRM::OrderPayment::COLLPROC): {
    item.status = AntiquaCRM::OrderPayment::COLLPROC;
    item.title = tr("Collection procedures");
    item.toolTip = tr("A Payment Collection procedure is done");
    item.decoration = QIcon("://icons/flag-red.png");
  } break;

  default: {
    item.status = AntiquaCRM::OrderPayment::NOTPAID;
    item.title = tr("Not paid");
    item.toolTip = tr("This order is currently not paid!");
    item.decoration = QIcon("://icons/warning.png");
  } break;
  }
  return item;
}

QVariant OrderPaymentStatusModel::getRoleData(const StatusItem &item,
                                              int role) const {
  switch (role) {
  case Qt::DisplayRole:
    return item.title;

  case Qt::DecorationRole:
    return item.decoration;

  case Qt::EditRole:
  case Qt::UserRole:
    return item.status;

  case Qt::ToolTipRole:
  case Qt::StatusTipRole:
  case Qt::WhatsThisRole:
    return item.toolTip;

  case Qt::ForegroundRole: {
    QPalette p = qApp->palette();
    if (item.status == AntiquaCRM::OrderPayment::RETURN) {
      return QBrush(p.color(QPalette::Disabled, QPalette::ButtonText));
    }
    return QBrush(p.color(QPalette::Active, QPalette::ButtonText));
  } break;

  default: // Qt::*Role
    return QVariant();
  };
}

QModelIndex OrderPaymentStatusModel::parent(const QModelIndex &index) const {
  Q_UNUSED(index);
  return QModelIndex();
}

QModelIndex OrderPaymentStatusModel::index(int row, int column,
                                           const QModelIndex &parent) const {
  Q_UNUSED(row);
  Q_UNUSED(column);
  Q_UNUSED(parent);
  QListIterator<StatusItem> it(statusList);
  while (it.hasNext()) {
    StatusItem item = it.next();
    if (item.status == row)
      return createIndex(row, 0);
  }
  return QModelIndex();
}

int OrderPaymentStatusModel::rowCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return statusList.size();
}

int OrderPaymentStatusModel::columnCount(const QModelIndex &parent) const {
  Q_UNUSED(parent);
  return 1;
}

QVariant OrderPaymentStatusModel::data(const QModelIndex &index,
                                       int role) const {
  if (!index.isValid())
    return QVariant();

  QVariant buffer;
  QListIterator<StatusItem> it(statusList);
  while (it.hasNext()) {
    StatusItem item = it.next();
    if (item.status == index.row()) {
      buffer = getRoleData(item, role);
      break;
    }
  }
  return buffer;
}

Qt::ItemFlags OrderPaymentStatusModel::flags(const QModelIndex &index) const {
  if (!index.isValid())
    return Qt::ItemIsEnabled;

  // @note Not selectable in this Contexts
  if (index.row() == AntiquaCRM::OrderPayment::RETURN)
    return Qt::ItemIsEnabled;

  return (Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

OrderPaymentStatusSelecter::OrderPaymentStatusSelecter(QWidget *parent)
    : InputEdit{parent} {
  m_box = new AntiquaComboBox(this);
  m_layout->addWidget(m_box);
  loadDataset();
  setModified(false);
  setRequired(false);
  connect(m_box, SIGNAL(currentIndexChanged(int)), SLOT(dataChanged(int)));
}

void OrderPaymentStatusSelecter::loadDataset() {
  OrderPaymentStatusModel *model = new OrderPaymentStatusModel(this);
  m_box->setModel(model);
}

void OrderPaymentStatusSelecter::dataChanged(int) { setModified(true); }

void OrderPaymentStatusSelecter::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void OrderPaymentStatusSelecter::setValue(const QVariant &val) {
  AntiquaCRM::OrderPayment status =
      static_cast<AntiquaCRM::OrderPayment>(val.toInt());
  int index = m_box->findData(status, Qt::UserRole);
  if (index >= 0)
    m_box->setCurrentIndex(index);
}

void OrderPaymentStatusSelecter::setFocus() {
  m_box->setFocus();
  m_box->showPopup();
}

void OrderPaymentStatusSelecter::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant OrderPaymentStatusSelecter::value() {
  int index = m_box->currentIndex();
  int status = m_box->itemData(index, Qt::UserRole).toInt();
  return static_cast<AntiquaCRM::OrderPayment>(status);
}

bool OrderPaymentStatusSelecter::isValid() { return true; }

void OrderPaymentStatusSelecter::setInfo(const QString &info) {
  m_box->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString OrderPaymentStatusSelecter::info() { return m_box->toolTip(); }

const QString OrderPaymentStatusSelecter::notes() {
  return tr("a Payment status is required.");
}
