// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_YEARAXIS_H
#define ANTIQUACRM_YEARAXIS_H

#include "statisticsconfig.h"
#include <AGlobal>
#include <QDate>
#include <QDateTime>
#include <QDateTimeAxis>
#include <QObject>

/**
 * @brief The YearAxis class
 * @ingroup _chartstat
 */
class ANTIQUACRM_STATISTICS_PLUGIN YearAxis final : public QDateTimeAxis {
  Q_OBJECT

public:
  explicit YearAxis(QObject* parent = nullptr);
  explicit YearAxis(int year, QObject* parent = nullptr);
  void setStartDate(const QDate&);
  void setEndDate(const QDate&);
  void setYear(int);
};

#endif // ANTIQUACRM_YEARAXIS_H
