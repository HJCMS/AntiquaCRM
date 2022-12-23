// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_STATS_UTILS_H
#define ANTIQUACRM_STATS_UTILS_H

#include <QDate>
#include <QDateTime>
#include <QMap>
#include <QPair>
#include <QTime>

class StatsUtils {
public:
  static const QPair<QTime, QTime> startEndDayTime();

  /**
   * Create a Days in Year Range an returning a Milliseconds Epoch as key
   * @param year - Year
   * @param sdoy - Start Day of Year
   */
  static const QMap<qint64, int> daysInYearRange(int year, int sdoy = -1);

  static const QDateTime startDateTime(const QDate &);

  static const QDateTime endDateTime(const QDate &);
};

#endif // ANTIQUACRM_STATS_UTILS_H
