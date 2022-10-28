// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquadoublebox.h"
#include <ASettings>

AntiquaDoubleBox::AntiquaDoubleBox(QWidget *parent) : QDoubleSpinBox{parent} {
  AntiquaCRM::ASettings cfg(this);
  mouseWheel = cfg.value("mouse_wheel_actions", false).toBool();
}

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
