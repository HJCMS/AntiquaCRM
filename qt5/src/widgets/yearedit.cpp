// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "yearedit.h"

#include <QDate>
#include <QDebug>

YearEdit::YearEdit(QWidget *parent)
    : InputEdit{parent}, startDate(currentDate()), minDate(QDate(1400, 1, 1)) {
  setObjectName("YearEdit");
  m_year = new QDateEdit(this);
  m_year->setDisplayFormat("yyyy");
  m_year->setMaximumDate(startDate.addYears(+1));
  m_year->setMinimumDate(minDate);
  m_year->setDate(startDate.addYears(+1));
  m_year->setWhatsThis(
      tr("Changes the Year for this Article. It can't set to the future!"));
  m_layout->addWidget(m_year);
  setModified(false);
  setRequired(false);
  connect(m_year, SIGNAL(dateChanged(const QDate &)), this,
          SLOT(dataChanged(const QDate &)));
}

void YearEdit::loadDataset() {}

void YearEdit::dataChanged(const QDate &d) {
  Q_UNUSED(d);
  setModified(true);
}

void YearEdit::setValue(const QVariant &v) {
  double y = v.toDouble();
  QDate d(y, 1, 1);
  m_year->setDate(d);
  setModified(true);
}

void YearEdit::reset() {
  m_year->setDate(startDate);
  setModified(false);
}

void YearEdit::setFocus() { m_year->setFocus(); }

void YearEdit::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);

  // Set minimum start date
  qint64 _y = field.defaultValue().toInt();
  if (_y > 1000)
      m_year->setMinimumDate(QDate(_y, 1, 1));
}

const QVariant YearEdit::value() {
    qint64 _y = m_year->date().year();
    return (_y > 1000) ? _y : currentYear();
}

bool YearEdit::isValid() {
  if (isRequired() && m_year->date() == m_year->maximumDate())
    return false;

  return true;
}

void YearEdit::setInfo(const QString &info) {
  QString txt(info);
  txt.append(":");
  m_label->setVisible(true);
  m_label->setText(txt);
  m_year->setToolTip(info);
}

const QString YearEdit::info() { return m_year->toolTip(); }

const QString YearEdit::notes() {
  QString in =
      tr("Year must contain a valid entry, it can not lie in the future.");
  return in;
}

const QDate YearEdit::currentDate() {
    return QDateTime::currentDateTime().date();
}

qint64 YearEdit::currentYear() { return currentDate().year(); }
