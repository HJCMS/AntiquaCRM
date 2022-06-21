// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "datetimeedit.h"

#include <QDate>
#include <QHBoxLayout>
#include <QTimeZone>

#ifndef DISPLAY_FORMAT
#define DISPLAY_FORMAT "dddd dd. MMMM yyyy"
#endif

/**
 * @example SELECT CURRENT_TIMESTAMP;
 */
#ifndef OUTPUT_FORMAT
#define OUTPUT_FORMAT "dd.MM.yyyy hh:mm:ss.zzz t"
#endif

CalendarViewer::CalendarViewer(QWidget *parent) : QCalendarWidget{parent} {}

DateTimeEdit::DateTimeEdit(QWidget *parent) : UtilsMain{parent} {

  p_displayFormat = QString(DISPLAY_FORMAT);
  p_outputFormat = QString(OUTPUT_FORMAT);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  QDateTime curDateTime = QDateTime::currentDateTime();

  m_info = new QLabel(this);
  m_info->setAlignment(labelAlignment());
  layout->addWidget(m_info);

  m_edit = new QDateTimeEdit(curDateTime);
  m_edit->setMinimumDate(curDateTime.date().addDays(-365));
  m_edit->setMaximumDate(curDateTime.date().addDays(365));
  m_edit->setDisplayFormat(p_displayFormat);
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
  p_displayFormat = format;
  m_edit->setDisplayFormat(p_displayFormat);
  setModified(true);
}

void DateTimeEdit::setOutputFormat(const QString &format) {
  if (format == "CURRENT_TIMESTAMP")
    p_outputFormat = QString(OUTPUT_FORMAT);
  else
    p_outputFormat = format;

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

const QVariant DateTimeEdit::value() {
  QDateTime dt(QDateTime::currentDateTime());
  dt.setDate(m_edit->dateTime().date());
  return dt.toString(p_outputFormat);
}

const QVariant DateTimeEdit::dateTime() {
  QDateTime dt(QDateTime::currentDateTime());
  dt.setDate(m_edit->dateTime().date());
  return dt;
}

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

DateTimeDisplay::DateTimeDisplay(QWidget *parent) : UtilsMain{parent} {
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  curDateTime = QDateTime::currentDateTime();

  m_edit = new QDateTimeEdit(this);
  m_edit->setDisplayFormat(DISPLAY_FORMAT);
  m_edit->setMaximumDateTime(curDateTime);
  m_edit->setButtonSymbols(QAbstractSpinBox::NoButtons);
  m_edit->setReadOnly(true);
  m_edit->setInputMethodHints(Qt::ImhNone);
  layout->addWidget(m_edit);

  setLayout(layout);
  setRequired(false);
  setModified(false);
}

void DateTimeDisplay::setValue(const QVariant &val) {
  QDateTime dt = val.toDateTime();
  if (dt.isValid()) {
    m_edit->setDateTime(dt);
  } else {
    m_edit->setDateTime(curDateTime);
  }
}

void DateTimeDisplay::reset() {
  m_edit->clear();
  setModified(false);
}

void DateTimeDisplay::setFocus() { m_edit->setFocus(); }

const QDate DateTimeDisplay::getDate()
{
  return m_edit->date();
}

const QVariant DateTimeDisplay::value() {
  QDateTime dt(QDateTime::currentDateTime());
  dt.setDate(m_edit->dateTime().date());
  return dt.toString(OUTPUT_FORMAT);
}

bool DateTimeDisplay::isValid() { return true; }

void DateTimeDisplay::setInfo(const QString &info) { m_edit->setToolTip(info); }

const QString DateTimeDisplay::info() { return m_edit->toolTip(); }

const QString DateTimeDisplay::notes() {
  return tr("a valid Datetime is needed.");
}
