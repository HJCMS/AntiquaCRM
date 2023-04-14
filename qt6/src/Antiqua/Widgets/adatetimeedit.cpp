// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "adatetimeedit.h"

#include <QAbstractSpinBox>

namespace AntiquaCRM {

#ifndef FALLBACK_DATE
#define FALLBACK_DATE QDate(1900, 01, 01)
#endif

ADateTimeEdit::ADateTimeEdit(QWidget *parent, bool mouseEvents)
    : QDateTimeEdit{parent}, wheel_support{mouseEvents} {
  setDisplayFormat(ANTIQUACRM_DATETIME_DISPLAY);
}

void ADateTimeEdit::wheelEvent(QWheelEvent *e) {
  if (wheel_support) {
    QDateTimeEdit::wheelEvent(e);
  }
}

void ADateTimeEdit::setSqlTimestamp(const QString &d) {
  QString _str = d.trimmed();
  if (_str.isEmpty()) {
    setDate(FALLBACK_DATE);
    return;
  }

  QDateTime _dt = QDateTime::fromString(_str, ANTIQUACRM_TIMESTAMP_IMPORT);
  if (_dt.isValid())
    setDateTime(_dt);
  else
    setDate(FALLBACK_DATE);
}

void ADateTimeEdit::setReadOnlyMode() {
  setInputMethodHints(Qt::ImhNone);
  setButtonSymbols(QAbstractSpinBox::NoButtons);
  setReadOnly(true);
  setStyleSheet("QDateTimeEdit {border:none;background:transparent;}");
  update();
}

} // namespace AntiquaCRM
