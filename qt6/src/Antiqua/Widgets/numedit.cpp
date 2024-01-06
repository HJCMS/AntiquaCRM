// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "numedit.h"

#include <QDebug>

namespace AntiquaCRM {

NumEdit::NumEdit(QWidget *parent) : AntiquaCRM::AInputWidget{parent} {
  m_edit = new AntiquaCRM::ASpinBox(this);
  m_edit->setMinimum(0);
  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(valueChanged(int)), SLOT(valueChanged(int)));
}

void NumEdit::valueChanged(int) {
  if (!isValid())
    return;

  setWindowModified(true);
  emit sendInputChanged();
}

void NumEdit::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Required);
  _f.setDefaultValue(0);
  setRestrictions(_f);
  setWindowModified(false);
}

void NumEdit::setValue(const QVariant &value) {
  int _num = value.toInt();
  switch (value.metaType().id()) {
  case (QMetaType::Int):
  case (QMetaType::Double):
  case (QMetaType::Long):
  case (QMetaType::ULong):
  case (QMetaType::LongLong):
    m_edit->setValue(_num);
    return;

  default: {
    if (_num > 0) {
      m_edit->setValue(_num);
      return;
    }
  } break;
  };
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << "NumEdit Invalid:" << value;
#endif
}

void NumEdit::setFocus() { m_edit->setFocus(); }

void NumEdit::reset() {
  m_edit->setValue(m_edit->minimum());
  setWindowModified(false);
}

void NumEdit::setReadOnly(bool b) {
  m_edit->setReadOnly(b);
  if (!b)
    m_edit->setButtonSymbols(QAbstractSpinBox::UpDownArrows);
  else
    m_edit->setButtonSymbols(QAbstractSpinBox::NoButtons);
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

const QMetaType NumEdit::getType() const { return QMetaType(QMetaType::Int); }

const QVariant NumEdit::getValue() { return m_edit->value(); }

const QString NumEdit::popUpHints() {
  return tr("A Numeric Input is required!");
}

const QString NumEdit::statusHints() { return popUpHints(); }

} // namespace AntiquaCRM
