// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "serialid.h"

#include <QDebug>
#include <QSizePolicy>

namespace AntiquaCRM {

SerialId::SerialId(QWidget *parent) : AntiquaCRM::AInputWidget{parent} {
  m_edit = new ASpinBox(this);
  m_edit->setReadOnly(true);
  m_edit->setMinimum(0);
  m_edit->setMaximum(9999999);
  m_edit->setButtonSymbols(QAbstractSpinBox::NoButtons);
  m_edit->setStyleSheet("QSpinBox {border:none;background:transparent;}");
  layout->addWidget(m_edit);
  initData();
}

void SerialId::valueChanged() {
  setWindowModified(true);
  emit sendInputChanged();
}

void SerialId::initData() { setWindowModified(false); }

void SerialId::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  switch (_type.id()) {
  case (QMetaType::Int):
  case (QMetaType::Long):
  case (QMetaType::Double):
  case (QMetaType::LongLong):
    m_edit->setValue(value.toLongLong());
    break;

  default:
    qWarning("Invalid given Data Type in SerialId.");
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "SerialId Requires type int but get:" << value;
#endif
    m_edit->setValue(value.toLongLong());
    break;
  };
}

void SerialId::setFocus() { m_edit->setFocus(); }

void SerialId::reset() {
  m_edit->clear();
  setWindowModified(false);
}

void SerialId::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void SerialId::setInputToolTip(const QString &tip) { m_edit->setToolTip(tip); }

void SerialId::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool SerialId::isValid() {
  if (isRequired() && m_edit->value() < 1)
    return false;

  return true;
}

const QMetaType SerialId::getType() const {
  return QMetaType(QMetaType::LongLong);
}

const QVariant SerialId::getValue() {
  quint64 _value = m_edit->value();
  /**< @warning Es darf keine 0 zurück gegeben werden */
  if (_value < 1)
    return QVariant();

  return _value;
}

const QString SerialId::popUpHints() {
  return tr("A valid Serial Id is important!");
}

const QString SerialId::statusHints() {
  return tr("is required and can not empty.");
}

} // namespace AntiquaCRM
