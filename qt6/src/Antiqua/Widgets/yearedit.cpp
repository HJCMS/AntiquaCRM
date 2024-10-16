// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "yearedit.h"

#include <QDate>
#include <QDebug>

namespace AntiquaCRM
{

YearEdit::YearEdit(QWidget* parent) : AntiquaCRM::AInputWidget{parent} {
  m_edit = new AntiquaCRM::ASpinBox(this);
  m_edit->setMinimum(1400);
  m_edit->setMaximum(QDate::currentDate().year() + 1);
  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(valueChanged(int)), SLOT(valueChanged(int)));
}

void YearEdit::valueChanged(int y) {
  if (y < m_edit->minimum())
    return;

  setWindowModified(true);
  emit sendInputChanged();
}

void YearEdit::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Required);
  _f.setDefaultValue(1400);
  setRestrictions(_f);
  setWindowModified(false);
}

void YearEdit::setValue(const QVariant& value) {
  QMetaType _type = value.metaType();
  switch (_type.id()) {
    case (QMetaType::Int):
    case (QMetaType::Long):
    case (QMetaType::Double):
    case (QMetaType::LongLong):
      m_edit->setValue(value.toDouble());
      break;

    default:
      qWarning("Invalid given Data Type in YearEdit.");
      m_edit->setValue(value.toDouble());
      break;
  };
}

void YearEdit::setFocus() {
  m_edit->setFocus();
}

void YearEdit::reset() {
  m_edit->setValue(m_edit->minimum());
  setWindowModified(false);
}

void YearEdit::setRestrictions(const QSqlField& field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);

  qint64 _sy = field.defaultValue().toInt();
  if (_sy > 1000 && _sy != m_edit->minimum())
    m_edit->setMinimum(_sy);
}

void YearEdit::setInputToolTip(const QString& tip) {
  m_edit->setToolTip(tip);
}

void YearEdit::setBuddyLabel(const QString& text) {
  if (text.isEmpty())
    return;

  ALabel* m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool YearEdit::isValid() {
  if (isRequired() && m_edit->value() == m_edit->minimum())
    return false;

  return true;
}

const QMetaType YearEdit::getType() const {
  return QMetaType(QMetaType::Double);
}

const QVariant YearEdit::getValue() {
  return QVariant(m_edit->value()).toDouble();
}

const QString YearEdit::popUpHints() {
  return tr("A valid Year must set!");
}

const QString YearEdit::statusHints() {
  return popUpHints();
}

} // namespace AntiquaCRM
