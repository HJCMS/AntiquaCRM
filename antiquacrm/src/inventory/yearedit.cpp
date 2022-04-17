// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "yearedit.h"
#include "version.h"

#include <QtCore/QDate>
#include <QtCore/QDebug>

static const QDate currentDate()
{
    return QDateTime::currentDateTime().date();
}

YearEdit::YearEdit(QWidget *parent) : QDateEdit{parent}, startDate(currentDate()) {
  setObjectName("YearEdit");
  setDisplayFormat("yyyy");
  setMaximumDate(startDate);
  setModified(false);
  setDate(startDate);

  connect(this, SIGNAL(dateChanged(const QDate &)), this,
          SLOT(dataChanged(const QDate &)));
}

void YearEdit::dataChanged(const QDate &d) {
  Q_UNUSED(d);
  setModified(true);
}

void YearEdit::setModified(bool b) { modified = b; }

void YearEdit::setValue(const QVariant &v) {
  double y = v.toDouble();
  QDate d(y, 1, 1);
  setDate(d);
  setModified(false);
}

void YearEdit::reset() {
  setDate(QDate());
  setModified(false);
}

void YearEdit::setRequired(bool b) { required = b; }

bool YearEdit::isRequired() { return required; }

bool YearEdit::hasModified() { return modified; }

const QVariant YearEdit::value() { return date().year(); }

bool YearEdit::isValid() {
  if (required && date() == startDate)
    return false;

  return true;
}

const QString YearEdit::notes() {
  return tr("The Year must contain a valid entry.");
}
