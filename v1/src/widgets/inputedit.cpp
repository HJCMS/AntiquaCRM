// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inputedit.h"

InputEdit::InputEdit(QWidget *parent)
    : QFrame{parent},
      labelAlign(Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter) {
  setContentsMargins(0, 0, 0, 0);
  m_layout = new QHBoxLayout(this);

  setLayout(m_layout);
  setRequired(false);
  setModified(false);
}

void InputEdit::skipReturnPressed() { setModified(true); }

void InputEdit::focusOutEvent(QFocusEvent *e) {
  if (e->type() == QEvent::FocusOut)
    emit sendLeaveEditor();

  QFrame::focusOutEvent(e);
}

void InputEdit::setRequired(bool b) {
  required = b;
  emit sendRequireChanged();
}

void InputEdit::setModified(bool b) {
  modified = b;
  emit sendModifiedChanged();
}

bool InputEdit::isRequired() { return required; }

bool InputEdit::isModified() { return modified; }
