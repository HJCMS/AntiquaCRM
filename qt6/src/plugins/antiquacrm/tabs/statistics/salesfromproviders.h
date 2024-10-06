// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_STATISTICS_SALESFROMPROVIDERS_H
#define ANTIQUACRM_STATISTICS_SALESFROMPROVIDERS_H

#include "statisticsconfig.h"
#include <AChartView>
#include <AntiquaCRM>
#include <QChart>
#include <QFont>
#include <QObject>
#include <QSplitter>

class SoldCountChart;
class SalesVolumeChart;

class ANTIQUACRM_STATISTICS_PLUGIN SalesFromProviders final : public QSplitter {
  Q_OBJECT

private:
  SoldCountChart* m_soldCountChart;
  SalesVolumeChart* m_salesVolumeChart;

public:
  explicit SalesFromProviders(QWidget* parent = nullptr);
  bool initView(int year);
};

#endif // ANTIQUACRM_STATISTICS_SALESFROMPROVIDERS_H
