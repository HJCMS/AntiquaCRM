// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquaspinbox.h"
#include <ASettings>

AntiquaSpinBox::AntiquaSpinBox(QWidget *parent) : QSpinBox{parent} {
  AntiquaCRM::ASettings cfg(this);
  mouseWheel = cfg.value("mouse_wheel_actions", false).toBool();
}

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
