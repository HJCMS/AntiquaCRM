// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "boolbox.h"

#include <QDebug>

namespace AntiquaCRM {

BoolBox::BoolBox(QWidget *parent) : AntiquaCRM::AbstractInput{parent} {
  m_edit = new QCheckBox(this);
  layout->addWidget(m_edit);
  initData();
  // connect(m_edit, SIGNAL(), SLOT(valueChanged()));
}

void BoolBox::valueChanged() {
  if (isValid())
    setWindowModified(true);
}

void BoolBox::initData() {
  QSqlField _f;
  _f.setMetaType(QMetaType(QMetaType::Bool));
  _f.setRequiredStatus(QSqlField::Required);
  _f.setDefaultValue(false);
  setRestrictions(_f);
  setWindowModified(false);
}

void BoolBox::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  switch (_type.id()) {
  case (QMetaType::Bool):
    m_edit->setChecked(value.toBool());
    break;

  default:
    qWarning("Invalid given Data Type in BoolBox.");
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "BoolBox Requires type int but get:" << value;
#endif
    break;
  };
}

void BoolBox::setFocus() { m_edit->setFocus(); }

void BoolBox::reset() {
  m_edit->setChecked(false);
  setWindowModified(false);
}

void BoolBox::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void BoolBox::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void BoolBox::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool BoolBox::isValid() {
  return true;
}

const QVariant BoolBox::getValue() {
  return m_edit->isChecked();
}

const QString BoolBox::popUpHints() {
  return tr("is required must selected.");
}

const QString BoolBox::statusHints() {
  return popUpHints();
}

} // namespace AntiquaCRM
