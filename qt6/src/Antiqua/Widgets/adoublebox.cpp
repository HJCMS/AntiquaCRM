// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "adoublebox.h"

namespace AntiquaCRM {

ADoubleBox::ADoubleBox(QWidget *parent, bool mouseEvents)
    : QDoubleSpinBox{parent}, wheel_support{mouseEvents} {
  setObjectName("ADoubleBox");
}

void ADoubleBox::wheelEvent(QWheelEvent *e) {
  if (wheel_support) {
    QDoubleSpinBox::wheelEvent(e);
  }
}

} // namespace AntiquaCRM
