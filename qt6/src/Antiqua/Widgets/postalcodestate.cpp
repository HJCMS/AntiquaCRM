// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "postalcodestate.h"

#include <QDebug>

namespace AntiquaCRM {

PostalCodeState::PostalCodeState(QWidget *parent)
    : AntiquaCRM::AInputWidget{parent} {
  m_edit = new ALineEdit(this);
  layout->addWidget(m_edit);
}

void PostalCodeState::initData() {
  // unused in this
}

void PostalCodeState::setCountry(const AntiquaCRM::PostalCode &code) {
  if (code.state.isEmpty())
    return;

  m_edit->setText(code.state);
}

void PostalCodeState::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  if (_type.id() == QMetaType::QString) {
    m_edit->setText(value.toString());
    return;
  }
  QString str = QString::number(value.toInt());
  m_edit->setText(str);
}

void PostalCodeState::setFocus() { m_edit->setFocus(); }

void PostalCodeState::reset() {
  m_edit->clear();
  setWindowModified(false);
}

void PostalCodeState::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);

  if (field.metaType().id() != getType().id())
    return;

  m_edit->setLineEditProperties(field);
}

bool PostalCodeState::isValid() {
  if (isRequired() && m_edit->text().isEmpty())
    return false;

  return true;
}

void PostalCodeState::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void PostalCodeState::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

const QMetaType PostalCodeState::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant PostalCodeState::getValue() { return m_edit->text().trimmed(); }

const QString PostalCodeState::popUpHints() {
  return tr("Missing Country/State for Postalcode!");
}

const QString PostalCodeState::statusHints() {
  return tr("Missing Country/State for Postalcode!");
}

} // namespace AntiquaCRM
