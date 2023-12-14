// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_STATISTICS_UTILS_H
#define ANTIQUACRM_STATISTICS_UTILS_H

#include <QDate>
#include <QDateTime>
#include <QMap>
#include <QPair>
#include <QTime>

/**
 * @brief The StatsUtils class
 * @ingroup _chartstat
 */
class StatsUtils final {
public:
  /**
   * @brief Start end DayTime: QPair<00:00:00,23:59:59>
   */
  static const QPair<QTime, QTime> startEndDayTime();

  /**
   * Create a Days in Year Range and returning a Milliseconds Epoch as key
   * @param year - Year
   */
  static const QMap<qint64, int> dayRangeFromYear(int);

  /**
   * Create a Days in Range and returning a Milliseconds Epoch as key
   * @param from Date
   * @param to Date
   */
  static const QMap<qint64, int> dayRangeFromDate(const QDate &, const QDate &);

  /**
   * @brief DateTime from Date with Time 00:00:00
   */
  static const QDateTime startDateTime(const QDate &);

  /**
   * @brief SQL TIMESTAMP from Date with Time 00:00:00
   */
  static const QString startTimeStamp(const QDate &);

  /**
   * @brief DateTime from Date with Time 23:59:59
   */
  static const QDateTime endDateTime(const QDate &);

  /**
   * @brief SQL TIMESTAMP from Date with Time 23:59:59
   */
  static const QString endTimeStamp(const QDate &);
};

#endif // ANTIQUACRM_STATISTICS_UTILS_H
