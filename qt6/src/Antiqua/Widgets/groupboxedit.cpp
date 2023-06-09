// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "groupboxedit.h"

namespace AntiquaCRM {

GroupBoxEdit::GroupBoxEdit(QWidget *parent) : AntiquaCRM::AInputWidget{parent} {
  m_edit = new QGroupBox(this);
  m_edit->setCheckable(true);
  m_edit->setChecked(false);
  m_layout = new QBoxLayout(QBoxLayout::LeftToRight, m_edit);
  m_layout->setObjectName("antiqua_input_layout");
  m_layout->setContentsMargins(0, 0, 0, 0);
  m_edit->setLayout(m_layout);
  layout->addWidget(m_edit);

  connect(m_edit, SIGNAL(clicked(bool)), SLOT(valueChanged(bool)));
}

void GroupBoxEdit::valueChanged(bool) { setWindowModified(true); }

void GroupBoxEdit::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Optional);
  _f.setDefaultValue(false);
  setRestrictions(_f);
  setWindowModified(false);
}

void GroupBoxEdit::setValue(const QVariant &value) {
  if (value.metaType().id() == QMetaType::Bool)
    m_edit->setChecked(value.toBool());
  else
    m_edit->setChecked(false);
}

void GroupBoxEdit::setFocus() {}

void GroupBoxEdit::reset() { setWindowModified(false); }

QBoxLayout *GroupBoxEdit::boxLayout() { return m_layout; }

void GroupBoxEdit::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void GroupBoxEdit::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void GroupBoxEdit::setBuddyLabel(const QString &text) {
  m_edit->setTitle(text);
}

bool GroupBoxEdit::isValid() { return true; }

const QMetaType GroupBoxEdit::getType() const {
  return QMetaType(QMetaType::Bool);
}

const QVariant GroupBoxEdit::getValue() { return m_edit->isChecked(); }

const QString GroupBoxEdit::popUpHints() {
  return tr("This grouped input field requires a value specification.");
}

const QString GroupBoxEdit::statusHints() { return tr("No choice!"); }

} // namespace AntiquaCRM
