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
#include <AChartView>
#include <QFont>
#include <QObject>
#include <QWidget>

class ANTIQUACRM_LIBRARY DailyAverage final : public AntiquaCRM::AChartView {
  Q_OBJECT

private:
  const QDate p_date;
  QChart *m_chart;

public:
  explicit DailyAverage(QWidget *parent = nullptr);
  bool initialChartView(int year = -1) override;
};

#endif // ANTIQUACRM_STATISTICS_DAILYAVERAGE_H
