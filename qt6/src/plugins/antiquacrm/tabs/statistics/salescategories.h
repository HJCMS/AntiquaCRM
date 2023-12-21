// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_STATISTICS_SALESCATEGORIES_H
#define ANTIQUACRM_STATISTICS_SALESCATEGORIES_H

#include <AntiquaCRM>
#include <AChartView>
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QFont>
#include <QObject>
#include <QWidget>

class HorizontalBarSeries;

class ANTIQUACRM_LIBRARY SalesCategories final : public AntiquaCRM::AChartView {
  Q_OBJECT

private:
  int p_year;
  QChart *m_chart;
  HorizontalBarSeries *m_doubleSeries, *m_countSeries;
  QBarSet *createBarSet(const QString &title, QChart *parent);

private Q_SLOTS:
  void onHoverTip(bool, int);
  void updateHeight(int);

public:
  explicit SalesCategories(QWidget *parent = nullptr);
  bool initialChartView(int year = -1) override;
};

#endif // ANTIQUACRM_STATISTICS_SALESCATEGORIES_H
