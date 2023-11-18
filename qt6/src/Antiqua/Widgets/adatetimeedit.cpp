// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "adatetimeedit.h"

#include <QAbstractSpinBox>

namespace AntiquaCRM {

ADateTimeEdit::ADateTimeEdit(QWidget *parent, bool mouseEvents)
    : QDateTimeEdit{parent}, wheel_support{mouseEvents} {
  setObjectName("ADateTimeEdit");
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
    setDate(ANTIQUACRM_DEFAULT_STARTDATE);
    return;
  }

  QDateTime _dt = QDateTime::fromString(_str, ANTIQUACRM_TIMESTAMP_IMPORT);
  if (_dt.isValid())
    setDateTime(_dt);
  else
    setDate(ANTIQUACRM_DEFAULT_STARTDATE);
}

void ADateTimeEdit::setReadOnlyMode() {
  setInputMethodHints(Qt::ImhNone);
  setButtonSymbols(QAbstractSpinBox::NoButtons);
  setReadOnly(true);
  setFrame(false);
  update();
}

} // namespace AntiquaCRM
