// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderpaymentstatusselecter.h"

#include <QDebug>

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
  int c = 0;
  m_box->clear();

  m_box->insertItem(c, tr("Not paid"), AntiquaCRM::OrderPayment::NOTPAID);
  m_box->setItemData(c, tr("This order is currently not paid!"),
                     Qt::ToolTipRole);
  m_box->setItemData(c++, QIcon("://icons/warning.png"), Qt::DecorationRole);

  m_box->insertItem(c, tr("Payed"), AntiquaCRM::OrderPayment::PAYED);
  m_box->setItemData(c, tr("Order paid and completed!"), Qt::ToolTipRole);
  m_box->setItemData(c++, QIcon("://icons/action_ok.png"), Qt::DecorationRole);

  m_box->insertItem(c, tr("Remindet"), AntiquaCRM::OrderPayment::REMIND);
  m_box->setItemData(c, tr("Buyer was reminded."), Qt::ToolTipRole);
  m_box->setItemData(c++, QIcon("://icons/flag-yellow.png"),
                     Qt::DecorationRole);

  m_box->insertItem(c, tr("Admonished"), AntiquaCRM::OrderPayment::ADMONISH);
  m_box->setItemData(c, tr("The Buyer was admonished!"), Qt::ToolTipRole);
  m_box->setItemData(c++, QIcon("://icons/flag-yellow.png"),
                     Qt::DecorationRole);

  m_box->insertItem(c, tr("Returning"), AntiquaCRM::OrderPayment::RETURN);
  m_box->setItemData(c, tr("Returning Process"), Qt::ToolTipRole);
  m_box->setItemData(c++, QIcon("://icons/action_undo.png"),
                     Qt::DecorationRole);

  m_box->insertItem(c, tr("Collection procedures"),
                    AntiquaCRM::OrderPayment::COLLPROC);
  m_box->setItemData(c, tr("A Payment Collection procedure is done"),
                     Qt::ToolTipRole);
  m_box->setItemData(c++, QIcon("://icons/flag-red.png"),
                     Qt::DecorationRole);
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
