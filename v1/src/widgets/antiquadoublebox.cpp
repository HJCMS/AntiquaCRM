// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquadoublebox.h"

AntiquaDoubleBox::AntiquaDoubleBox(QWidget *parent) : QDoubleSpinBox{parent} {}

void AntiquaDoubleBox::wheelEvent(QWheelEvent *e) {
  if (mouseWheel) {
    QDoubleSpinBox::wheelEvent(e);
  }
}

void AntiquaDoubleBox::setMouseWheelEnabled(bool b) {
  mouseWheel = b;
  emit sendMouseWheelChanged();
}

bool AntiquaDoubleBox::isMouseWheelEnabled() { return mouseWheel; }
