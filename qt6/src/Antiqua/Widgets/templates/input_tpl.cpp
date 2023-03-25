// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "@INCLUDE@.h"

#include <QDebug>

namespace AntiquaCRM {

@CLASSNAME@::@CLASSNAME@(QWidget *parent) : AntiquaCRM::AbstractInput{parent} {
  // m_edit = new AntiquaCRM::(this);
  // layout->addWidget(m_edit);
  // initData();
  // connect(m_edit, SIGNAL(), SLOT(valueChanged()));
}

void @CLASSNAME@::valueChanged() {
  if (isValid())
    setWindowModified(true);
}

void @CLASSNAME@::initData() {
  QSqlField _f;
  _f.setMetaType(QMetaType(QMetaType::Int));
  _f.setRequiredStatus(QSqlField::Required);
  _f.setDefaultValue(0);
  if (!objectName().isEmpty())
    _f.setName(objectName());

  setRestrictions(_f);
  setWindowModified(false);
}

void @CLASSNAME@::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  switch (_type.id()) {
  case (QMetaType::Int):
  case (QMetaType::Long):
  case (QMetaType::LongLong):
    // m_edit->setValue(value.toInt());
    break;

  default:
    qWarning("Invalid given Data Type in @CLASSNAME@.");
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "@CLASSNAME@ Requires type int but get:" << value;
#endif
    break;
  };
}

void @CLASSNAME@::setFocus() { m_edit->setFocus(); }

void @CLASSNAME@::reset() {
  setWindowModified(false);
  // m_edit->setValue(m_edit->minimum());
}

void @CLASSNAME@::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void @CLASSNAME@::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void @CLASSNAME@::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool @CLASSNAME@::isValid() {
//if (isRequired() && m_edit->value() < 0)
//  return false;

  return true;
}

const QVariant @CLASSNAME@::getValue() { return 0; //m_edit->value(); }

const QString @CLASSNAME@::popUpHints() {
  return tr("__TODO__");
}

const QString @CLASSNAME@::statusHints() {
  return tr("__TODO__");
}

} // namespace AntiquaCRM
