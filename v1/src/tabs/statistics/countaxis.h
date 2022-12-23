// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_COUNTAXIS_H
#define ANTIQUACRM_COUNTAXIS_H

#include <QObject>
#include <QtCharts/QValueAxis>

class CountAxis final : public QtCharts::QValueAxis {
  Q_OBJECT

private:
  int p_count = 5;

public:
  explicit CountAxis(int count, QObject *parent = nullptr);
};

#endif // ANTIQUACRM_COUNTAXIS_H
