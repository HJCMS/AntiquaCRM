// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statsutils.h"

#include <QCalendar>

const QPair<QTime, QTime> StatsUtils::startEndDayTime() {
  QPair<QTime, QTime> p;
  p.first = QTime(0, 0, 0);
  p.second = QTime(23, 59, 59);
  return p;
}

const QMap<qint64, int> StatsUtils::daysInYearRange(int year, int sdoy) {
  QMap<qint64, int> m;
  QCalendar calendar;
  int year_days = calendar.daysInYear(year);
  QDate d = QDate(year, 1, 1).addDays(sdoy);
  QPair<QTime, QTime> at = startEndDayTime();
  for (int i = 1; i <= year_days; i++) {
    QTime _t = (i < year_days) ? at.first : at.second;
    QDateTime dt = QDateTime(d.addDays(i), _t);
    m.insert(dt.toMSecsSinceEpoch(), -1);
  }
  return m;
}

const QDateTime StatsUtils::startDateTime(const QDate &d) {
  QDateTime dt(d, QTime(0, 0, 0));
  return dt;
}

const QDateTime StatsUtils::endDateTime(const QDate &d) {
  QDateTime dt(d, QTime(23, 59, 59));
  return dt;
}
