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
  p_onload_status = AntiquaCRM::OrderPayment::NOTPAID;
  initData();
  connect(m_edit, SIGNAL(currentIndexChanged(int)), SLOT(valueChanged(int)));
}

void SelectOrderPayment::valueChanged(int index) {
  int _id = m_edit->itemData(index, Qt::UserRole).toInt();
  if (_id >= 0) {
    setWindowModified(true);
    emit sendStatusChanged(static_cast<AntiquaCRM::OrderPayment>(_id));
  }
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
  m_edit->setItemData(_c, tr("Waiting for payment."), Qt::ToolTipRole);
  m_edit->setItemIcon(_c++, AntiquaCRM::antiquaIcon("dialog-warning"));

  // Bezahlstatus
  m_edit->insertItem(_c, tr("Payed"), AntiquaCRM::OrderPayment::PAYED);
  m_edit->setItemData(_c, tr("Order is already paid."), Qt::ToolTipRole);
  m_edit->setItemIcon(_c++, AntiquaCRM::antiquaIcon("dialog-ok-apply"));

  // Erinnerung
  m_edit->insertItem(_c, tr("Reminded"), AntiquaCRM::OrderPayment::REMIND);
  m_edit->setItemData(_c, tr("The recipient was reminded."), Qt::ToolTipRole);
  m_edit->setItemIcon(_c++, AntiquaCRM::antiquaIcon("flag-yellow"));

  // Ermahnung
  m_edit->insertItem(_c, tr("Admonished"), AntiquaCRM::OrderPayment::ADMONISH);
  m_edit->setItemData(_c, tr("The recipient has been admonished."),
                      Qt::ToolTipRole);
  m_edit->setItemIcon(_c++, AntiquaCRM::antiquaIcon("flag-yellow"));

  // Rücksendung
  m_edit->insertItem(_c, tr("Refunding"), AntiquaCRM::OrderPayment::RETURN);
  m_edit->setItemData(_c, tr("Creates a refunding for this order."),
                      Qt::ToolTipRole);
  m_edit->setItemIcon(_c++, AntiquaCRM::antiquaIcon("action-redo"));

  // Inkasso
  m_edit->insertItem(_c, tr("Collection process"),
                     AntiquaCRM::OrderPayment::COLLPROC);
  m_edit->setItemData(_c, tr("This order containing a collection transaction."),
                      Qt::ToolTipRole);
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
  if (_index > 0) {
    m_edit->setCurrentIndex(_index);
    p_onload_status = static_cast<AntiquaCRM::OrderPayment>(value.toInt());
  }
}

void SelectOrderPayment::setFocus() { m_edit->setFocus(); }

void SelectOrderPayment::reset() {
  m_edit->setCurrentIndex(0);
  setWindowModified(false);
}

void SelectOrderPayment::setReadOnly(bool b) { m_edit->setEnabled(!b); }

void SelectOrderPayment::setReject() { setValue(p_onload_status); }

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
