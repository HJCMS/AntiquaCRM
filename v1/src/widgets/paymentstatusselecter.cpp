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
  int c = 0;
  p_map.clear();
  p_map.insert(c++, QPair(tr("No status set"),
               AntiquaCRM::STATUS_NOT_SET));
  p_map.insert(c++, QPair(tr("Waiting for payment"),
               AntiquaCRM::WAIT_FOR_PAYMENT));
  p_map.insert(c++, QPair(tr("Ready for shipping"),
               AntiquaCRM::SHIPMENT_CREATED));
  p_map.insert(c++, QPair(tr("Delivered waiting for payment"),
               AntiquaCRM::SHIPPED_WAIT_FOR_PAYMENT));
  p_map.insert(c++, QPair(tr("Paid and shipped"),
               AntiquaCRM::SHIPPED_AND_PAID));
  p_map.insert(c++, QPair(tr("No reaction from buyer"),
               AntiquaCRM::BUYER_NO_REACTION));
  p_map.insert(c++, QPair(tr("order canceled"),
               AntiquaCRM::ORDER_CANCELED));

  for (int i = 0; i < p_map.size(); i++) {
    QPair<QString, AntiquaCRM::PaymentStatus> st = p_map[i];
    m_box->insertItem(i, st.first, st.second);
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
  AntiquaCRM::PaymentStatus status = static_cast<AntiquaCRM::PaymentStatus>(val.toInt());
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
