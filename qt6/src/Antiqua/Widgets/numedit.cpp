// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "numedit.h"

#include <QDebug>

namespace AntiquaCRM {

NumEdit::NumEdit(QWidget *parent) : AntiquaCRM::AbstractInput{parent} {
  m_edit = new AntiquaCRM::ASpinBox(this);
  m_edit->setMinimum(0);
  layout->addWidget(m_edit);
  initData();
  // connect(m_edit, SIGNAL(), SLOT(valueChanged()));
}

void NumEdit::valueChanged() {
  if (isValid())
    setWindowModified(true);
}

void NumEdit::initData() {
  QSqlField _f;
  _f.setMetaType(QMetaType(QMetaType::Int));
  _f.setRequiredStatus(QSqlField::Required);
  _f.setDefaultValue(0);
  setRestrictions(_f);
  setWindowModified(false);
}

void NumEdit::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  switch (_type.id()) {
  case (QMetaType::Int):
  case (QMetaType::Double):
  case (QMetaType::Long):
  case (QMetaType::ULong):
  case (QMetaType::LongLong):
    m_edit->setValue(value.toInt());
    break;

  default:
    qWarning("Invalid given Data Type in NumEdit.");
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "NumEdit Requires type int but get:" << value;
#endif
    break;
  };
}

void NumEdit::setFocus() { m_edit->setFocus(); }

void NumEdit::reset() {
  setWindowModified(false);
  m_edit->setValue(m_edit->minimum());
}

void NumEdit::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void NumEdit::setRange(int min, int max) { m_edit->setRange(min, max); }

void NumEdit::setSingleStep(int step) { m_edit->setSingleStep(step); }

void NumEdit::setPrefix(const QString &prefix) { m_edit->setPrefix(prefix); }

void NumEdit::setSuffix(const QString &suffix) { m_edit->setSuffix(suffix); }

void NumEdit::setInputToolTip(const QString &tip) { m_edit->setToolTip(tip); }

void NumEdit::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool NumEdit::isValid() {
  if (isRequired() && getValue().isNull())
    return false;

  return true;
}

const QVariant NumEdit::getValue() { return m_edit->value(); }

const QString NumEdit::popUpHints() {
  return tr("A Numeric Input is requierd");
}

const QString NumEdit::statusHints() { return popUpHints(); }

} // namespace AntiquaCRM
