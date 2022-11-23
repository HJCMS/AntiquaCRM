// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "paymentstatusselecter.h"

#include <QDebug>

PaymentStatusSelecter::PaymentStatusSelecter(QWidget *parent)
    : InputEdit{parent} {
  m_box = new AntiquaComboBox(this);
  m_layout->addWidget(m_box);
  loadDataset();
  setModified(false);
  setRequired(false);
  connect(m_box, SIGNAL(currentIndexChanged(int)), SLOT(dataChanged(int)));
}

void PaymentStatusSelecter::loadDataset() {
  QMapIterator<AntiquaCRM::PaymentStatus, QString> it(getStatus());
  int c = 0;
  while (it.hasNext()) {
    it.next();
    m_box->insertItem(c++, it.value(), it.key());
  }
}

void PaymentStatusSelecter::dataChanged(int index) {
  if (index > 0)
    setModified(true);

  int status = m_box->itemData(index, Qt::UserRole).toInt();
  AntiquaCRM::PaymentStatus st = static_cast<AntiquaCRM::PaymentStatus>(status);
  emit sendPaymentStatusChanged(st);
}

void PaymentStatusSelecter::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void PaymentStatusSelecter::setValue(const QVariant &val) {
  AntiquaCRM::PaymentStatus status =
      static_cast<AntiquaCRM::PaymentStatus>(val.toInt());
  int index = m_box->findData(status, Qt::UserRole);
  if (index >= 0)
    m_box->setCurrentIndex(index);
}

void PaymentStatusSelecter::setFocus() {
  m_box->setFocus();
  m_box->showPopup();
}

void PaymentStatusSelecter::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant PaymentStatusSelecter::value() {
  int index = m_box->currentIndex();
  int status = m_box->itemData(index, Qt::UserRole).toInt();
  return static_cast<AntiquaCRM::PaymentStatus>(status);
}

bool PaymentStatusSelecter::isValid() {
  if (isRequired() && m_box->currentIndex() > 0)
    return false;

  return true;
}

void PaymentStatusSelecter::setInfo(const QString &info) {
  m_box->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString PaymentStatusSelecter::info() { return m_box->toolTip(); }

const QString PaymentStatusSelecter::notes() {
  return tr("a valid Payment status is required.");
}

const QMap<AntiquaCRM::PaymentStatus, QString>
PaymentStatusSelecter::getStatus() {
  QMap<AntiquaCRM::PaymentStatus, QString> map;
  map.insert(AntiquaCRM::STATUS_NOT_SET, tr("No status set"));
  map.insert(AntiquaCRM::WAIT_FOR_PAYMENT, tr("Waiting for payment"));
  map.insert(AntiquaCRM::SHIPMENT_CREATED, tr("Ready for shipping"));
  map.insert(AntiquaCRM::SHIPPED_WAIT_FOR_PAYMENT,
             tr("Delivered waiting for payment"));
  map.insert(AntiquaCRM::SHIPPED_AND_PAID, tr("Paid and shipped"));
  map.insert(AntiquaCRM::BUYER_NO_REACTION, tr("No reaction from buyer"));
  map.insert(AntiquaCRM::ORDER_CANCELED, tr("order canceled"));
  return map;
}
