// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectpaymentstatus.h"
#include "antiquaicon.h"

#include <QDebug>
#include <QIcon>

namespace AntiquaCRM
{

SelectPaymentStatus::SelectPaymentStatus(QWidget* parent) : AntiquaCRM::AInputWidget{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  m_edit->setMinimumContentsLength(m_edit->withoutDisclosures().length());
  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void SelectPaymentStatus::valueChanged(int index) {
  if (index < 1)
    return;

  int _status = m_edit->itemData(index, Qt::UserRole).toInt();
  emit sendPaymentStatusChanged(static_cast<AntiquaCRM::ProviderPaymentStatus>(_status));
  emit sendInputChanged();
}

void SelectPaymentStatus::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setDefaultValue(AntiquaCRM::ProviderPaymentStatus::STATUS_NOT_SET);
  setRestrictions(_f);

  int _c = 1;
  m_edit->setWithoutDisclosures(AntiquaCRM::ProviderPaymentStatus::STATUS_NOT_SET);
  m_edit->insertItem(_c++, tr("Waiting for payment"),
                     AntiquaCRM::ProviderPaymentStatus::WAIT_FOR_PAYMENT);
  m_edit->insertItem(_c++, tr("Ready for shipping"),
                     AntiquaCRM::ProviderPaymentStatus::SHIPMENT_CREATED);
  m_edit->insertItem(_c++, tr("Delivered waiting for payment"),
                     AntiquaCRM::ProviderPaymentStatus::SHIPPED_WAIT_FOR_PAYMENT);
  m_edit->insertItem(_c++, tr("Paid and shipped"),
                     AntiquaCRM::ProviderPaymentStatus::SHIPPED_AND_PAID);
  m_edit->insertItem(_c++, tr("No reaction from buyer"),
                     AntiquaCRM::ProviderPaymentStatus::BUYER_NO_REACTION);
  m_edit->insertItem(_c++, tr("order canceled"), AntiquaCRM::ProviderPaymentStatus::ORDER_CANCELED);

  const QIcon _icon = AntiquaCRM::antiquaIcon("view-loan-asset");
  for (int i = 1; i < m_edit->count(); i++) {
    m_edit->setItemIcon(i, _icon);
  }

  setWindowModified(false);
}

void SelectPaymentStatus::setValue(const QVariant& value) {
  if (value.metaType().id() != QMetaType::Int) {
#ifdef ANTIQUA_DEVELOPMENT
    qDebug() << Q_FUNC_INFO << "Reject:" << value;
#endif
    return;
  }

  AntiquaCRM::ProviderPaymentStatus _status =
      static_cast<AntiquaCRM::ProviderPaymentStatus>(value.toInt());
  int _index = m_edit->findData(_status, Qt::UserRole);
  if (_index > 0)
    m_edit->setCurrentIndex(_index);
}

void SelectPaymentStatus::setFocus() {
  m_edit->setFocus();
}

void SelectPaymentStatus::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SelectPaymentStatus::setRestrictions(const QSqlField& field) {
  setRequired((field.requiredStatus() == QSqlField::Required));
}

void SelectPaymentStatus::setInputToolTip(const QString& tip) {
  m_edit->setToolTip(tip);
}

void SelectPaymentStatus::setBuddyLabel(const QString& text) {
  if (text.isEmpty())
    return;

  ALabel* m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SelectPaymentStatus::isValid() {
  if (isRequired() && m_edit->currentIndex() == 0)
    return false;

  return true;
}

const QMetaType SelectPaymentStatus::getType() const {
  return QMetaType(QMetaType::Int);
}

const QVariant SelectPaymentStatus::getValue() {
  int _index = m_edit->currentIndex();
  int _status = m_edit->itemData(_index, Qt::UserRole).toInt();
  return static_cast<AntiquaCRM::ProviderPaymentStatus>(_status);
}

const QString SelectPaymentStatus::popUpHints() {
  return tr("a valid Payment status is required.");
}

const QString SelectPaymentStatus::statusHints() {
  return popUpHints();
}

} // namespace AntiquaCRM
