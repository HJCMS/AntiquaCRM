// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_COUNTAXIS_H
#define ANTIQUACRM_COUNTAXIS_H

#include <QObject>
#include <QtCharts/QValueAxis>

/**
 * @brief The CountAxis class
 * @ingroup _chartstat
 */
class CountAxis final : public QtCharts::QValueAxis {
  Q_OBJECT

private:
  int p_count = 5;

public:
  explicit CountAxis(int count, QObject *parent = nullptr);
};

#endif // ANTIQUACRM_COUNTAXIS_H
