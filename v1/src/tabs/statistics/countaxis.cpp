// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "countaxis.h"

CountAxis::CountAxis(int count, QObject *parent)
    : QtCharts::QValueAxis{parent}, p_count(count) {
  setLabelFormat("%d");
  setRange(0, (p_count % 2 == 0) ? p_count : (p_count + 1));
}
