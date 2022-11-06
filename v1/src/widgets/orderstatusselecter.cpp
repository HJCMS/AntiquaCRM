// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderstatusselecter.h"

#include <QDebug>

OrderStatusSelecter::OrderStatusSelecter(QWidget *parent) : InputEdit{parent} {
  m_box = new AntiquaComboBox(this);
  m_layout->addWidget(m_box);
  loadDataset();
  setModified(false);
  setRequired(false);
  connect(m_box, SIGNAL(currentIndexChanged(int)), SLOT(dataChanged(int)));
}

void OrderStatusSelecter::loadDataset() {
  int c = 0;
  p_map.clear();
  p_map.insert(c++, QPair(tr("Open"), AntiquaCRM::OPEN));
  p_map.insert(c++, QPair(tr("Order accepted"), AntiquaCRM::STARTED));
  p_map.insert(c++, QPair(tr("Ready for pickup"), AntiquaCRM::FETCHET));
  p_map.insert(c++, QPair(tr("Delivered"), AntiquaCRM::DELIVERED));
  p_map.insert(c++, QPair(tr("Reminder sent"), AntiquaCRM::REMINDET));
  p_map.insert(c++, QPair(tr("Closed"), AntiquaCRM::COMPLETED));
  p_map.insert(c++, QPair(tr("Canceled"), AntiquaCRM::CANCELED));
  p_map.insert(c++, QPair(tr("Back"), AntiquaCRM::RETURNING));
  for (int i = 0; i < p_map.size(); i++) {
    QPair<QString, AntiquaCRM::OrderStatus> st = p_map[i];
    m_box->insertItem(i, st.first, st.second);
  }
}

void OrderStatusSelecter::dataChanged(int index) {
  if (index > 0)
    setModified(true);

  int status = m_box->itemData(index, Qt::UserRole).toInt();
  AntiquaCRM::OrderStatus st = static_cast<AntiquaCRM::OrderStatus>(status);
  emit sendOrderStatusChanged(st);
}

void OrderStatusSelecter::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void OrderStatusSelecter::setValue(const QVariant &val) {
  AntiquaCRM::OrderStatus status = static_cast<AntiquaCRM::OrderStatus>(val.toInt());
  int index = m_box->findData(status, Qt::UserRole);
  if (index >= 0)
    m_box->setCurrentIndex(index);
}

void OrderStatusSelecter::setFocus() {
  m_box->setFocus();
  m_box->showPopup();
}

void OrderStatusSelecter::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant OrderStatusSelecter::value() {
  int index = m_box->currentIndex();
  int status = m_box->itemData(index, Qt::UserRole).toInt();
  return static_cast<AntiquaCRM::OrderStatus>(status);
}

bool OrderStatusSelecter::isValid() {
  if (isRequired() && m_box->currentIndex() > 0)
    return false;

  return true;
}

void OrderStatusSelecter::setInfo(const QString &info) {
  m_box->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString OrderStatusSelecter::info() { return m_box->toolTip(); }

const QString OrderStatusSelecter::notes() {
  return tr("a valid Order status is required.");
}
