// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiqualineedit.h"

AntiquaLineEdit::AntiquaLineEdit(QWidget *parent) : QLineEdit{parent} {
  QSizePolicy sp(QSizePolicy::MinimumExpanding, /* klein halten */
                 QSizePolicy::Fixed, QSizePolicy::LineEdit);
  setMaxLength(80);
  setMinimumWidth(30);
  setSizePolicy(sp);
}

void AntiquaLineEdit::focusInEvent(QFocusEvent *event) {
  if (event->type() == QEvent::FocusIn) {
    if (completer() != nullptr && text().isEmpty() &&
        completer()->completionCount() > 1) {
      completer()->complete();
    }
  }
  QLineEdit::focusInEvent(event);
}
