// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aslider.h"

namespace AntiquaCRM {

ASlider::ASlider(QWidget *parent, bool mouseEvents)
    : QSlider{Qt::Horizontal, parent}, wheel_support{mouseEvents} {
  setMinimum(0);
  setTickPosition(QSlider::TicksAbove);
}

void ASlider::wheelEvent(QWheelEvent *e) {
  if (wheel_support) {
    QSlider::wheelEvent(e);
  }
}

} // namespace AntiquaCRM
