// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_STATISTICS_MONTHBARSET_H
#define ANTIQUACRM_STATISTICS_MONTHBARSET_H

#include "statisticsconfig.h"
#include <AGlobal>
#include <QBarSet>
#include <QChart>
#include <QObject>
#include <QWidget>

class ANTIQUACRM_STATISTICS_PLUGIN MonthBarSet final : public QBarSet {
  Q_OBJECT;

private:
  int p_year;
  QMap<int, double> p_sales;
  QWidget* m_chartWidget = nullptr;

private Q_SLOTS:
  void showToolTip(bool, int);

public Q_SLOTS:
  /**
   * @brief set Parent Widget
   *
   * We need a valid QWidget for QToolTip::showText and Timeout behavior.
   */
  void setParent(QWidget*);

  /**
   * @brief setSales
   * @param sales Map
   */
  void setSales(const QMap<int, double>& sales);

public:
  enum Type { Volume = 0, Sales = 1 };
  explicit MonthBarSet(int year, QChart* parent = nullptr,
                       MonthBarSet::Type type = MonthBarSet::Type::Volume);
  int year() const;
};

#endif // ANTIQUACRM_STATISTICS_MONTHBARSET_H
