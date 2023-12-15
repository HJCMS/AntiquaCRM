// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "yearaxis.h"

YearAxis::YearAxis(QObject *parent) : QDateTimeAxis{parent} {
  setFormat("dd.MM.yy");
}

YearAxis::YearAxis(int year, QObject *parent) : YearAxis{parent} {
  setFormat("dd.MM.yy");
  setYear(year);
}

void YearAxis::setStartDate(const QDate &d) {
  QDateTime dt(d, QTime(0, 0, 0));
  setMin(dt);
}

void YearAxis::setEndDate(const QDate &d) {
  QDateTime dt(d, QTime(23, 59, 59));
  setMax(dt);
}

void YearAxis::setYear(int y) {
  setStartDate(QDate(y, 1, 1));
  setEndDate(QDate(y, 12, 31));
}
