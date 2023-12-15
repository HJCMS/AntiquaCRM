// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "countaxis.h"

CountAxis::CountAxis(int count, QObject *parent)
    : QValueAxis{parent}, p_count(count) {
  setLabelFormat("%d");
  setRange(0, (p_count % 2 & 1) ? (p_count + 1) : p_count);
  setMinorTickCount(1);
}
