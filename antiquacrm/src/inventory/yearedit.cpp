// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "yearedit.h"
#include "version.h"

#include <QtCore/QDate>
#include <QtCore/QDebug>

YearEdit::YearEdit(QWidget *parent) : QDateEdit{parent} {
  setObjectName("YearEdit");
  setDisplayFormat("yyyy");
  setMaximumDate(QDate());
}

void YearEdit::setValue(const QVariant &v) {
  double y = v.toDouble();
  QDate d(y, 1, 1);
  setDate(d);
}

const QVariant YearEdit::value() { return date().year(); }
