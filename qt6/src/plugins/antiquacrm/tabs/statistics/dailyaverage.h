// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_STATISTICS_DAILYAVERAGE_H
#define ANTIQUACRM_STATISTICS_DAILYAVERAGE_H

#include <AntiquaCRM>
#include <QChartView>
#include <QObject>
#include <QWidget>

class ANTIQUACRM_LIBRARY DailyAverage final : public QChartView {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql;
  const QDate p_date;
  QChart *m_chart;

public:
  explicit DailyAverage(QWidget *parent = nullptr);
  virtual ~DailyAverage();
  bool initChartView(int year);
};

#endif // ANTIQUACRM_STATISTICS_DAILYAVERAGE_H
