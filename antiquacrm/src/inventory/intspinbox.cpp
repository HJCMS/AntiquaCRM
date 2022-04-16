// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "intspinbox.h"
#include "version.h"

#include <QtCore/QDebug>

IntSpinBox::IntSpinBox(QWidget *parent) : QSpinBox{parent} {
  setObjectName("IntSpinBox");
  setModified(false);
  setMinimum(0);
}

void IntSpinBox::itemChanged(int) { setModified(true); }

void IntSpinBox::setModified(bool b) { modified = b; }

void IntSpinBox::setValue(const QVariant &val) {
  int value = val.toInt();
  QSpinBox::setValue(value);
  setModified(false);
}

void IntSpinBox::reset() {
  QSpinBox::setValue(0);
  setModified(false);
}

bool IntSpinBox::hasModified() { return modified; }

const QVariant IntSpinBox::value() { return QVariant(QSpinBox::value()); }
