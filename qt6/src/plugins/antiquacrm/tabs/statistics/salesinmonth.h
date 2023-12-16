// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_STATISTICS_SALESINMONTH_H
#define ANTIQUACRM_STATISTICS_SALESINMONTH_H

#include <AntiquaCRM>
#include <QBarCategoryAxis>
#include <QBarSet>
#include <QChartView>
#include <QDateTime>
#include <QMap>
#include <QObject>
#include <QWidget>

class HorizontalBarSeries;

class ANTIQUACRM_LIBRARY SalesInMonth final : public QChartView {
  Q_OBJECT

private:
  QString p_currency;
  mutable QMap<int, QMap<int, qint64>> p_voluMap;
  mutable QMap<int, QMap<int, double>> p_soldMap;
  AntiquaCRM::ASqlCore *m_sql;
  QChart *m_chart;
  QBarCategoryAxis *m_label;
  HorizontalBarSeries *m_numsBar;
  HorizontalBarSeries *m_paidBar;
  bool initMaps();
  QBarSet *createBarset(const QString &title);
  inline const QDateTime fsepoch(qint64) const;
  bool initialChartView();

public:
  explicit SalesInMonth(QWidget *parent = nullptr);
  virtual ~SalesInMonth();
};

#endif // ANTIQUACRM_STATISTICS_SALESINMONTH_H
