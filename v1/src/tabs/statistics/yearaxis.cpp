// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "yearaxis.h"

YearAxis::YearAxis(QObject *parent)
    : QtCharts::QDateTimeAxis{parent} {
  setFormat("dd.MM.yy");
  setTickCount(12);
}

void YearAxis::setStartDate(const QDate &d) {
  QDateTime dt(d, QTime(0, 0, 0));
  setMin(dt);
}

void YearAxis::setEndDate(const QDate &d) {
  QDateTime dt(d, QTime(23, 59, 59));
  setMax(dt);
}
