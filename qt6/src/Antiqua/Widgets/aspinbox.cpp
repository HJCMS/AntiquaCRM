// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aspinbox.h"

namespace AntiquaCRM {

ASpinBox::ASpinBox(QWidget *parent, bool mouseEvents)
    : QSpinBox{parent}, wheel_support{mouseEvents} {
  setObjectName("ASpinBox");
}

void ASpinBox::wheelEvent(QWheelEvent *e) {
  if (wheel_support) {
    QSpinBox::wheelEvent(e);
  }
}

} // namespace AntiquaCRM
