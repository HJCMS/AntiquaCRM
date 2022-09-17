// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "yearedit.h"

#include <QDate>
#include <QDebug>
#include <QHBoxLayout>

static const QDate currentDate() { return QDateTime::currentDateTime().date(); }

YearEdit::YearEdit(QWidget *parent)
    : UtilsMain{parent}, startDate(currentDate()) {
  setObjectName("YearEdit");

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_info = new QLabel(this);
  m_info->setAlignment(labelAlignment());
  layout->addWidget(m_info);

  m_year = new QDateEdit(this);
  m_year->setDisplayFormat("yyyy");
  m_year->setMaximumDate(startDate.addYears(+1));
  m_year->setDate(startDate.addYears(+1));
  layout->addWidget(m_year);

  setLayout(layout);

  setModified(false);
  setRequired(false);

  connect(m_year, SIGNAL(dateChanged(const QDate &)), this,
          SLOT(dataChanged(const QDate &)));
}

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

const QVariant YearEdit::value() { return m_year->date().year(); }

bool YearEdit::isValid() {
  if (isRequired() && m_year->date() == m_year->maximumDate())
    return false;

  return true;
}

void YearEdit::setInfo(const QString &info) {
  QString txt(info);
  txt.append(":");
  m_info->setText(txt);
  m_year->setToolTip(info);
}

const QString YearEdit::info() { return m_info->text(); }

const QString YearEdit::notes() {
  QString in =
      tr("The Year must contain a valid entry, it can not lie in the future.");
  return in;
}
