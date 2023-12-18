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
#include <QBarSet>
#include <QChart>
#include <QChartView>
#include <QFont>
#include <QObject>
#include <QWidget>

class HorizontalBarSeries;

class ANTIQUACRM_LIBRARY SalesCategories final : public QChartView {
  Q_OBJECT

private:
  AntiquaCRM::ASqlCore *m_sql;
  int p_year;
  QFont p_headerFont, p_barsFont;
  QString p_currency;
  QChart *m_chart;
  HorizontalBarSeries *m_doubleSeries, *m_countSeries;
  QBarSet *createBarSet(const QString &title, QChart *parent);
  double currencyRound(double) const;

private Q_SLOTS:
  void onHoverTip(bool, int);
  void updateHeight(int);

public:
  explicit SalesCategories(QWidget *parent = nullptr);
  virtual ~SalesCategories();
  bool initChartView(int year);
};

#endif // ANTIQUACRM_STATISTICS_SALESCATEGORIES_H
