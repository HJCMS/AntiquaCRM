// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "datetimeinfo.h"

#include <QDebug>

namespace AntiquaCRM {

DateTimeInfo::DateTimeInfo(QWidget *parent)
    : AntiquaCRM::AInputWidget{parent} {
  m_edit = new AntiquaCRM::ADateTimeEdit(this);
  m_edit->setReadOnlyMode();
  layout->addWidget(m_edit);
  initData();
}

void DateTimeInfo::valueChanged() {
  // Not used here
  // emit sendInputChanged();
}

void DateTimeInfo::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Optional);
  setRestrictions(_f);
  setWindowModified(false);
}

void DateTimeInfo::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  switch (_type.id()) {
  case (QMetaType::QString):
    m_edit->setSqlTimestamp(value.toString());
    break;

  case (QMetaType::QDate):
    m_edit->setDate(value.toDate());
    break;

  case (QMetaType::QDateTime):
    m_edit->setDateTime(value.toDateTime());
    break;

  default:
    qWarning("Invalid given Data Type in DateTimeInfo.");
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "DateTimeInfo::setValue " << value;
#endif
    break;
  };
}

void DateTimeInfo::setFocus() { m_edit->setFocus(); }

void DateTimeInfo::reset() {
  m_edit->setSqlTimestamp(QString());
  setWindowModified(false);
}

void DateTimeInfo::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void DateTimeInfo::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void DateTimeInfo::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool DateTimeInfo::isValid() {
  if (isRequired() && getValue().isNull())
    return false;

  return true;
}

const QMetaType DateTimeInfo::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant DateTimeInfo::getValue() { return m_edit->dateTime(); }

const QString DateTimeInfo::popUpHints() {
  return tr("Date Time information required!");
}

const QString DateTimeInfo::statusHints() { return popUpHints(); }

} // namespace AntiquaCRM
