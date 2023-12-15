// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_STATISTICS_SALESFROMPROVIDERS_H
#define ANTIQUACRM_STATISTICS_SALESFROMPROVIDERS_H

#include <AntiquaCRM>
#include <QChart>
#include <QChartView>
#include <QObject>
#include <QPieSeries>
#include <QWidget>

class ANTIQUACRM_LIBRARY SalesFromProviders final : public QWidget {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql;
  QChartView *m_averageView, *m_volumeView;
  QChart *m_averageChart, *m_volumeChart;
  QPieSeries *initSeries(QChart *chart);

private Q_SLOTS:
  void highlight(bool);

public:
  explicit SalesFromProviders(QWidget *parent = nullptr);
  virtual ~SalesFromProviders();
  bool initChartView(int year);
};

#endif // ANTIQUACRM_STATISTICS_SALESFROMPROVIDERS_H
