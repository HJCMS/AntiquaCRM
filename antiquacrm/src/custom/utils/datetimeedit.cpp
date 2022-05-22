// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "datetimeedit.h"

#include <QDate>
#include <QHBoxLayout>
#include <QTimeZone>

#ifndef DISPLAY_FORMAT
#define DISPLAY_FORMAT "dddd dd. MMMM yyyy"
#endif

CalendarViewer::CalendarViewer(QWidget *parent) : QCalendarWidget{parent} {}

DateTimeEdit::DateTimeEdit(QWidget *parent) : UtilsMain{parent} {

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  QDate curDate = QDate::currentDate();

  m_info = new QLabel(this);
  m_info->setAlignment(labelAlignment());
  layout->addWidget(m_info);

  m_edit = new QDateTimeEdit(curDate);
  m_edit->setMaximumDate(curDate.addDays(365));
  m_edit->setDisplayFormat(DISPLAY_FORMAT);
  layout->addWidget(m_edit);

  m_popup = new CalendarViewer(m_edit);
  m_edit->setCalendarPopup(true);
  m_edit->setCalendarWidget(m_popup);

  setLayout(layout);

  setRequired(false);

  connect(m_edit, SIGNAL(dateChanged(const QDate &)), this,
          SLOT(dateChanged(const QDate &)));
  connect(m_edit, SIGNAL(dateTimeChanged(const QDateTime &)), this,
          SLOT(dateTimeChanged(const QDateTime &)));
  connect(m_edit, SIGNAL(timeChanged(const QTime &)), this,
          SLOT(timeChanged(const QTime &)));
}

void DateTimeEdit::dateChanged(const QDate &val) {
  QDateTime buffer = p_value;
  buffer.setDate(val);
  p_value = buffer;
  setModified(true);
}

void DateTimeEdit::dateTimeChanged(const QDateTime &val) {
  p_value = val;
  setModified(true);
}

void DateTimeEdit::timeChanged(const QTime &val) {
  QDateTime buffer = p_value;
  buffer.setTime(val);
  p_value = buffer;
  setModified(true);
}

const QDateTime DateTimeEdit::system() { return QDateTime::currentDateTime(); }

void DateTimeEdit::setDisplayFormat(const QString &format) {
  p_format = format;
  m_edit->setDisplayFormat(format);
  setModified(true);
}

void DateTimeEdit::setReadOnly(bool b) {
  m_edit->setReadOnly(b);
  m_popup->setDateEditEnabled(b);
  setModified(false);
}

void DateTimeEdit::setValue(const QVariant &val) {
  QDateTime dt = val.toDateTime();
  if (dt.isValid()) {
    p_value = dt;
    m_edit->setDateTime(p_value);
    setModified(true);
  } else {
    qWarning("invalid datetime");
  }
}

void DateTimeEdit::reset() {
  m_edit->setDateTime(system());
  setModified(false);
}

void DateTimeEdit::setFocus() { m_edit->setFocus(); }

const QVariant DateTimeEdit::value() { return m_edit->dateTime(); }

bool DateTimeEdit::isValid() {
  if (isRequired() && !m_edit->dateTime().isValid())
    return false;

  return true;
}

void DateTimeEdit::setInfo(const QString &info) {
  m_edit->setToolTip(info);
  m_info->setText(info + ":");
}

const QString DateTimeEdit::info() { return m_edit->toolTip(); }

const QString DateTimeEdit::notes() {
  return tr("a valid Datetime is needed.");
}
