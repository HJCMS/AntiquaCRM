// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectpaymentstatus.h"

#include <QDebug>
#include <QIcon>

namespace AntiquaCRM {

SelectPaymentStatus::SelectPaymentStatus(QWidget *parent)
    : AntiquaCRM::AbstractInput{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void SelectPaymentStatus::valueChanged(int index) {
  if (index < 1)
    return;

  int _status = m_edit->itemData(index, Qt::UserRole).toInt();
  emit sendPaymentStatusChanged(
      static_cast<AntiquaCRM::ProviderPaymentStatus>(_status));
}

void SelectPaymentStatus::initData() {
  QSqlField _f;
  _f.setMetaType(QMetaType(QMetaType::Int));
  _f.setDefaultValue(AntiquaCRM::STATUS_NOT_SET);
  setRestrictions(_f);

  int c = 1;
  m_edit->setWithoutDisclosures(AntiquaCRM::STATUS_NOT_SET);
  m_edit->insertItem(c++, tr("Waiting for payment"),
                     AntiquaCRM::WAIT_FOR_PAYMENT);
  m_edit->insertItem(c++, tr("Ready for shipping"),
                     AntiquaCRM::SHIPMENT_CREATED);
  m_edit->insertItem(c++, tr("Delivered waiting for payment"),
                     AntiquaCRM::SHIPPED_WAIT_FOR_PAYMENT);
  m_edit->insertItem(c++, tr("Paid and shipped"), AntiquaCRM::SHIPPED_AND_PAID);
  m_edit->insertItem(c++, tr("No reaction from buyer"),
                     AntiquaCRM::BUYER_NO_REACTION);
  m_edit->insertItem(c++, tr("order canceled"), AntiquaCRM::ORDER_CANCELED);

  const QIcon _icon = AntiquaApplIcon("view-loan-asset");
  for (int i = 1; i < m_edit->count(); i++) {
    m_edit->setItemIcon(i, _icon);
  }

  setWindowModified(false);
}

void SelectPaymentStatus::setValue(const QVariant &value) {
  if (value.metaType().id() != QMetaType::Int) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Reject:" << value;
#endif
    return;
  }

  AntiquaCRM::ProviderPaymentStatus status =
      static_cast<AntiquaCRM::ProviderPaymentStatus>(value.toInt());
  int index = m_edit->findData(status, Qt::UserRole);
  if (index > 0)
    m_edit->setCurrentIndex(index);
}

void SelectPaymentStatus::setFocus() { m_edit->setFocus(); }

void SelectPaymentStatus::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SelectPaymentStatus::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void SelectPaymentStatus::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void SelectPaymentStatus::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SelectPaymentStatus::isValid() {
  if (isRequired() && m_edit->currentIndex() == 0)
    return false;

  return true;
}

const QVariant SelectPaymentStatus::getValue() {
  int _index = m_edit->currentIndex();
  int _status = m_edit->itemData(_index, Qt::UserRole).toInt();
  return static_cast<AntiquaCRM::ProviderPaymentStatus>(_status);
}

const QString SelectPaymentStatus::popUpHints() {
  return tr("a valid Payment status is required.");
}

const QString SelectPaymentStatus::statusHints() { return popUpHints(); }

} // namespace AntiquaCRM
