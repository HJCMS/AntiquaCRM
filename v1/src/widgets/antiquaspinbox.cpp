// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquaspinbox.h"

AntiquaSpinBox::AntiquaSpinBox(QWidget *parent) : QSpinBox{parent} {}

void AntiquaSpinBox::wheelEvent(QWheelEvent *e) {
  if (mouseWheel) {
    QSpinBox::wheelEvent(e);
  }
}

void AntiquaSpinBox::setMouseWheelEnabled(bool b) {
  mouseWheel = b;
  emit sendMouseWheelChanged();
}

bool AntiquaSpinBox::isMouseWheelEnabled() { return mouseWheel; }
