// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectorderstatus.h"
#include "antiquaicon.h"

namespace AntiquaCRM {

SelectOrderStatus::SelectOrderStatus(QWidget *parent)
    : AntiquaCRM::AInputWidget{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  m_edit->setMinimumContentsLength(m_edit->withoutDisclosures().length());
  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void SelectOrderStatus::valueChanged(int) {
  setWindowModified(true);
  emit sendInputChanged();
}

void SelectOrderStatus::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setDefaultValue(AntiquaCRM::OrderStatus::OPEN);
  setRestrictions(_f);

  m_edit->setWithoutDisclosures(-1);

  int _c = 1;
  // Offen
  m_edit->insertItem(_c, tr("Open"), AntiquaCRM::OrderStatus::OPEN);
  m_edit->setItemData(_c, tr("Status open"), Qt::ToolTipRole);
  m_edit->setItemIcon(_c++, AntiquaCRM::antiquaIcon("dialog-warning"));

  // Auftrag angenommen
  m_edit->insertItem(_c, tr("Created"), AntiquaCRM::OrderStatus::STARTED);
  m_edit->setItemIcon(_c++, AntiquaCRM::antiquaIcon("action-edit-document"));

  // Bereit zur Abholung
  m_edit->insertItem(_c, tr("Fetchet"), AntiquaCRM::OrderStatus::FETCHET);
  m_edit->setItemIcon(_c++, QIcon("://icons/package-created.png"));

  // Unterwegs
  m_edit->insertItem(_c, tr("Delivery"), AntiquaCRM::OrderStatus::DELIVERY);
  m_edit->setItemIcon(_c++, QIcon("://icons/package-deliver.png"));

  // Geliefert
  m_edit->insertItem(_c, tr("Delivered"), AntiquaCRM::OrderStatus::DELIVERED);
  m_edit->setItemIcon(_c++, AntiquaCRM::antiquaIcon("dialog-ok-apply"));

  // Storniert
  m_edit->insertItem(_c, tr("Canceled"), AntiquaCRM::OrderStatus::CANCELED);
  m_edit->setItemIcon(_c++, AntiquaCRM::antiquaIcon("edit-delete"));

  setWindowModified(false);
}

void SelectOrderStatus::setValue(const QVariant &value) {
  if (value.metaType().id() != QMetaType::Int) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Reject:" << value;
#endif
    return;
  }
  int _index = m_edit->findData(value.toInt(), Qt::UserRole);
  if (_index > 0)
    m_edit->setCurrentIndex(_index);
}

void SelectOrderStatus::setFocus() { m_edit->setFocus(); }

void SelectOrderStatus::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SelectOrderStatus::setReadOnly(bool b) { m_edit->setEnabled(!b); }

void SelectOrderStatus::setRestrictions(const QSqlField &field) {
  setRequired((field.requiredStatus() == QSqlField::Required));
}

void SelectOrderStatus::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void SelectOrderStatus::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SelectOrderStatus::isValid() {
  if (isRequired() && m_edit->currentIndex() == 0)
    return false;

  return true;
}

const QMetaType SelectOrderStatus::getType() const {
  return QMetaType(QMetaType::Int);
}

const QVariant SelectOrderStatus::getValue() {
  int _index = m_edit->currentIndex();
  int _status = m_edit->itemData(_index, Qt::UserRole).toInt();
  return static_cast<AntiquaCRM::OrderStatus>(_status);
}

const QString SelectOrderStatus::popUpHints() {
  return tr("a valid Order status is required.");
}

const QString SelectOrderStatus::statusHints() { return popUpHints(); }

} // namespace AntiquaCRM
