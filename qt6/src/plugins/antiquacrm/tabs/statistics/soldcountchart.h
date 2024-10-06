// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_STATISTICS_SOLDCOUNTCHART_H
#define ANTIQUACRM_STATISTICS_SOLDCOUNTCHART_H

#include "statisticsconfig.h"
#include <AChartView>
#include <AntiquaCRM>
#include <QObject>
#include <QWidget>

class ANTIQUACRM_STATISTICS_PLUGIN SoldCountChart final : public AntiquaCRM::AChartView {
  Q_OBJECT

private:
  const QDate p_date;
  QChart* m_chart;

private Q_SLOTS:
  void highlight(bool);

public:
  explicit SoldCountChart(QWidget* parent = nullptr);
  bool initialChartView(int year = -1) override;
};

#endif // ANTIQUACRM_STATISTICS_SOLDCOUNTCHART_H
