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

const QMap<qint64, int> StatsUtils::dayRangeFromYear(int year) {
  QMap<qint64, int> m;
  QCalendar calendar;
  int year_days = calendar.daysInYear(year);
  QDate d = QDate(year, 1, 1).addDays(-1);
  QPair<QTime, QTime> at = startEndDayTime();
  for (int i = 1; i <= year_days; i++) {
    QTime _t = (i < year_days) ? at.first : at.second;
    QDateTime dt = QDateTime(d.addDays(i), _t);
    m.insert(dt.toMSecsSinceEpoch(), -1);
  }
  return m;
}

const QMap<qint64, int> StatsUtils::dayRangeFromDate(const QDate &from,
                                                     const QDate &to) {
  QMap<qint64, int> m;
  QCalendar calendar;
  int to_day = to.dayOfYear();
  QDate d = QDate(from.year(), 1, 1).addDays(to_day);
  QPair<QTime, QTime> at = startEndDayTime();
  for (int i = 1; i <= to_day; i++) {
    QTime _t = (i < to_day) ? at.first : at.second;
    QDateTime dt = QDateTime(d.addDays(i), _t);
    m.insert(dt.toMSecsSinceEpoch(), -1);
  }
  return m;
}

const QDateTime StatsUtils::startDateTime(const QDate &d) {
  QDateTime dt(d, QTime(0, 0, 0));
  return dt;
}

const QString StatsUtils::startTimeStamp(const QDate &d) {
  return d.toString("dd.MM.yyyy") + "T00:00:00";
}

const QDateTime StatsUtils::endDateTime(const QDate &d) {
  QDateTime dt(d, QTime(23, 59, 59));
  return dt;
}

const QString StatsUtils::endTimeStamp(const QDate &d) {
  return d.toString("dd.MM.yyyy") + "T23:59:00";
}
