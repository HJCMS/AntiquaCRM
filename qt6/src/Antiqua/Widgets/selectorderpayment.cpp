// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selectorderpayment.h"
#include "antiquaicon.h"

namespace AntiquaCRM {

SelectOrderPayment::SelectOrderPayment(QWidget *parent)
    : AntiquaCRM::AInputWidget{parent} {
  m_edit = new AntiquaCRM::AComboBox(this);
  m_edit->setMinimumContentsLength(m_edit->withoutDisclosures().length());
  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void SelectOrderPayment::valueChanged(int) {
  setWindowModified(true);
  emit sendInputChanged();
}

void SelectOrderPayment::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setDefaultValue(AntiquaCRM::OrderPayment::NOTPAID);
  setRestrictions(_f);

  m_edit->setWithoutDisclosures(-1);

  int _c = 1;
  // Warte auf Zahlung
  m_edit->insertItem(_c, tr("Open"), AntiquaCRM::OrderPayment::NOTPAID);
  m_edit->setItemIcon(_c++, AntiquaCRM::antiquaIcon("dialog-warning"));

  // Bezahlt
  m_edit->insertItem(_c, tr("Payed"), AntiquaCRM::OrderPayment::PAYED);
  m_edit->setItemIcon(_c++, AntiquaCRM::antiquaIcon("dialog-ok-apply"));

  // Erinnert
  m_edit->insertItem(_c, tr("Remind"), AntiquaCRM::OrderPayment::REMIND);
  m_edit->setItemIcon(_c++, AntiquaCRM::antiquaIcon("flag-yellow"));

  // Ermahnt
  m_edit->insertItem(_c, tr("Admonish"), AntiquaCRM::OrderPayment::ADMONISH);
  m_edit->setItemIcon(_c++, AntiquaCRM::antiquaIcon("flag-yellow"));

  // Retour
  m_edit->insertItem(_c, tr("Returned"), AntiquaCRM::OrderPayment::RETURN);
  m_edit->setItemIcon(_c++, AntiquaCRM::antiquaIcon("action-redo"));

  // Inkasso
  m_edit->insertItem(_c, tr("Collproc"), AntiquaCRM::OrderPayment::COLLPROC);
  m_edit->setItemIcon(_c++, AntiquaCRM::antiquaIcon("flag-red"));

  setWindowModified(false);
}

void SelectOrderPayment::setValue(const QVariant &value) {
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

void SelectOrderPayment::setFocus() { m_edit->setFocus(); }

void SelectOrderPayment::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SelectOrderPayment::setReadOnly(bool b) {
  m_edit->setEnabled(!b);
}

void SelectOrderPayment::setRestrictions(const QSqlField &field) {
  setRequired((field.requiredStatus() == QSqlField::Required));
}

void SelectOrderPayment::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void SelectOrderPayment::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SelectOrderPayment::isValid() {
  if (isRequired() && m_edit->currentIndex() == 0)
    return false;

  return true;
}

const QMetaType SelectOrderPayment::getType() const {
  return QMetaType(QMetaType::Int);
}

const QVariant SelectOrderPayment::getValue() {
  int _index = m_edit->currentIndex();
  int _status = m_edit->itemData(_index, Qt::UserRole).toInt();
  return static_cast<AntiquaCRM::OrderPayment>(_status);
}

const QString SelectOrderPayment::popUpHints() {
  return tr("a valid Order status is required.");
}

const QString SelectOrderPayment::statusHints() { return popUpHints(); }

} // namespace AntiquaCRM
