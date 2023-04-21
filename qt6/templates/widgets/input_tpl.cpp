// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "@INCLUDE@.h"

#include <QDebug>

namespace AntiquaCRM {

@CLASSNAME@::@CLASSNAME@(QWidget *parent) : AntiquaCRM::AbstractInput{parent} {
  m_edit = new AntiquaCRM::ALineEdit(this);
  layout->addWidget(m_edit);
  initData();
  // connect(m_edit, SIGNAL(), SLOT(valueChanged()));
}

void @CLASSNAME@::valueChanged() {
  //
  setWindowModified(true);
}

void @CLASSNAME@::initData() {
  QSqlField _f;
  _f.setMetaType(QMetaType(QMetaType::Int));
  // Required | Optional
  _f.setRequiredStatus(QSqlField::Required);
  // _f.setDefaultValue(0);
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
  // Some clear options
  // m_edit->clear();
  // m_edit->setValue(m_edit->minimum());

  // The last line
  setWindowModified(false);
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
  if (isRequired() && getValue().isNull())
    return false;

  return true;
}

const QVariant @CLASSNAME@::getValue() {
  // Final Return value for database
  return QVariant(); //m_edit->value();
}

const QString @CLASSNAME@::popUpHints() {
  // MessageBox Notifications
  return tr("__TODO__");
}

const QString @CLASSNAME@::statusHints() {
  // StatusBar Notifications
  return tr("__TODO__");
}

} // namespace AntiquaCRM
