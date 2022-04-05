// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "version.h"
#include "yearedit.h"

#include <QtCore/QDebug>
#include <QtCore/QDate>

YearEdit::YearEdit(QWidget *parent) : QDateEdit{parent} {
  setObjectName("YearEdit");
  setDisplayFormat("yyyy");
  setMaximumDate(QDate());
}

void YearEdit::setValue(double y)
{
    QDate d(y,1,1);
    setDate(d);
}

double YearEdit::getValue()
{
    return date().year();
}
