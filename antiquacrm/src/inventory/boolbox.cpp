// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "boolbox.h"
#include "version.h"

#include <QtCore/QDebug>

BoolBox::BoolBox(QWidget *parent) : QCheckBox{parent} {
  connect(this, SIGNAL(stateChanged(int)), this, SLOT(itemChanged(int)));
}

void BoolBox::itemChanged(int) { setModified(true); }

void BoolBox::setModified(bool b) { modified = b; }

void BoolBox::setValue(const QVariant &val) {
  bool value = val.toBool();
  setChecked(value);
  setModified(false);
}

void BoolBox::reset() {
  setChecked(false);
  setModified(true);
}

bool BoolBox::hasModified() { return modified; }

const QVariant BoolBox::value() { return QVariant(isChecked()); }
