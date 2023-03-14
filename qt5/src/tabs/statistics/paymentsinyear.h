// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_PAYMENTSINYEAR_H
#define ANTIQUACRM_PAYMENTSINYEAR_H

#include <AGlobal>
#include <QDate>
#include <QObject>
#include <QScrollArea>
#include <QWidget>
#include <QtCharts/QChartView>

/**
 * @brief The PaymentsInYear class
 * @ingroup _chartstat
 */
class PaymentsInYear final : public QScrollArea {
  Q_OBJECT

private:
  const QDate p_date;
  QtCharts::QChartView *m_view;

public:
  explicit PaymentsInYear(const QDate &date, QWidget *parent = nullptr);
  ~PaymentsInYear();
};

#endif // ANTIQUACRM_PAYMENTSINYEAR_H
