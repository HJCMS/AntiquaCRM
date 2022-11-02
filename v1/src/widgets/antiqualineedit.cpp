// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiqualineedit.h"

AntiquaLineEdit::AntiquaLineEdit(QWidget *parent)
    : QLineEdit{parent}, keyEnterEvent{false} {
  QSizePolicy sp(QSizePolicy::MinimumExpanding, /* klein halten */
                 QSizePolicy::Fixed, QSizePolicy::LineEdit);
  setClearButtonEnabled(true);
  setMinimumWidth(30);
  setSizePolicy(sp);
  connect(this, SIGNAL(returnPressed()), SLOT(checkEnterEventPressed()));
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

void AntiquaLineEdit::checkEnterEventPressed() {
  if(keyEnterEvent) {
    // emit returnPressed();
    qInfo("Enter key event enabled!");
    return;
  }
  qInfo("Enter key event disabled!");
}

void AntiquaLineEdit::setEnableKeyEnterEvent(bool b) {
  keyEnterEvent = b;
  emit sendKeyEnterEventChanged();
}

bool AntiquaLineEdit::keyEnterEventEnabled() { return keyEnterEvent; }
