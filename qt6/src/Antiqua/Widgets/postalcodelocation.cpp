// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "postalcodelocation.h"
#include "postalcodeedit.h"

namespace AntiquaCRM {

PostalCodeLocation::PostalCodeLocation(QWidget *parent)
    : AntiquaCRM::AInputWidget{parent} {
  m_edit = new ALineEdit(this);
  layout->addWidget(m_edit);
}

void PostalCodeLocation::initData() {}

void PostalCodeLocation::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  if (_type.id() == QMetaType::QString) {
    m_edit->setText(value.toString());
    return;
  }
  QString str = QString::number(value.toInt());
  m_edit->setText(str);
}

void PostalCodeLocation::setFocus() { m_edit->setFocus(); }

void PostalCodeLocation::reset() {
  m_edit->clear();
  m_edit->setCompleter(nullptr);
  m_edit->setCompleterAction(false);
  setWindowModified(false);
}

bool PostalCodeLocation::setCompletion(AntiquaCRM::PostalCodeEdit *txobj,
                                       const AntiquaCRM::PostalCode &code) {
  if (code.location.isEmpty())
    return false;

  if (txobj == nullptr)
    return false;

  // WARNING - do not override it!
  QString _str = m_edit->text();
  if (_str.isEmpty())
    m_edit->setText(code.location);

  QCompleter *m_cpl = txobj->getCompleter(m_edit);
  if (m_cpl == nullptr)
    return false;

  m_edit->setClearButtonEnabled(true);
  m_edit->setCompleter(m_cpl);
  m_edit->setCompleterAction(true);
  return true;
}

void PostalCodeLocation::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);

  if (field.metaType().id() != QMetaType::QString)
    return;

  m_edit->setLineEditProperties(field);
}

bool PostalCodeLocation::isValid() {
  if (isRequired() && m_edit->text().isEmpty())
    return false;

  return true;
}

void PostalCodeLocation::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void PostalCodeLocation::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

const QMetaType PostalCodeLocation::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant PostalCodeLocation::getValue() {
  return m_edit->text().trimmed();
}

const QString PostalCodeLocation::popUpHints() {
  return tr("Missing Location for Postalcode!");
}

const QString PostalCodeLocation::statusHints() {
  return tr("Missing Location for Postalcode!");
}

} // namespace AntiquaCRM
