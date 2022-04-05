// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "version.h"
#include "priceedit.h"

#include <QtCore/QDebug>

PriceEdit::PriceEdit(QWidget *parent) : QDoubleSpinBox{parent} {
  setObjectName("PriceEdit");
  setSuffix(tr("\342\202\254"));
}

