// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inputedit.h"
#include "antiquailabel.h"

#include <QDebug>
#include <QRegExp>

InputEdit::InputEdit(QWidget *parent) : QFrame{parent} {
  setContentsMargins(0, 0, 0, 0);
  m_layout = new QHBoxLayout(this);
  m_layout->setContentsMargins(0, 0, 0, 0);
  m_label = new AntiquaILabel(this);
  m_layout->addWidget(m_label);
  setLayout(m_layout);
  setRequired(false);
  setModified(false);
}

const QString InputEdit::stripString(const QString &str) const {
  QString buf = str.trimmed();
  buf.replace("'", "´");
  QRegExp pattern("^\\s+");
  buf.replace(pattern, "´");
  return buf.trimmed();
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
  emit sendHasModified(b);
}

bool InputEdit::isRequired() { return required; }

bool InputEdit::isModified() { return modified; }
