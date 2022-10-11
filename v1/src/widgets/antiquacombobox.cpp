// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquacombobox.h"

AntiquaComboBox::AntiquaComboBox(QWidget *parent) : QComboBox{parent} {
  setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
  setInsertPolicy(QComboBox::NoInsert);
  setEditable(false);
}

void AntiquaComboBox::wheelEvent(QWheelEvent *e) {
  if (mouseWheel) {
    QComboBox::wheelEvent(e);
  }
}

void AntiquaComboBox::setMouseWheelEnabled(bool b) {
  mouseWheel = b;
  emit sendMouseWheelChanged();
}

bool AntiquaComboBox::isMouseWheelEnabled() { return mouseWheel; }
