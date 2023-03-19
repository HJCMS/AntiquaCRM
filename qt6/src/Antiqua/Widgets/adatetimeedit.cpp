// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "adatetimeedit.h"

#include <QAbstractSpinBox>

namespace AntiquaCRM {

ADateTimeEdit::ADateTimeEdit(QWidget *parent, bool mouseEvents)
    : QDateTimeEdit{parent}, wheel_support{mouseEvents} {
  setDisplayFormat(ANTIQUACRM_DATETIME_DISPLAY);
}

void ADateTimeEdit::wheelEvent(QWheelEvent *e) {
  if (wheel_support) {
    QDateTimeEdit::wheelEvent(e);
  }
}

void ADateTimeEdit::setReadOnlyMode() {
  setInputMethodHints(Qt::ImhNone);
  setButtonSymbols(QAbstractSpinBox::NoButtons);
  setReadOnly(true);
  setStyleSheet("QDateTimeEdit {border:none;background:transparent;}");
  update();
}

} // namespace AntiquaCRM
