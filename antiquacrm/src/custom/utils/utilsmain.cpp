// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "utilsmain.h"

UtilsMain::UtilsMain(QWidget *parent) : QFrame{parent} {
  if (objectName().isEmpty())
    setObjectName("UtilsTemplate");

  modified = false;
}

Qt::Alignment UtilsMain::labelAlignment()
{
    return (Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);
}

void UtilsMain::skipReturnPressed() { setModified(true); }

void UtilsMain::focusOutEvent(QFocusEvent *e) {
  if (e->type() == QEvent::FocusOut)
    emit editingFinished();

  QFrame::focusOutEvent(e);
}

void UtilsMain::setModified(bool b) {
  emit modifiedChanged();
  emit hasModified(true);
  modified = b;
}

void UtilsMain::setRequired(bool b) { required = b; }

bool UtilsMain::isRequired() {
  emit requireChanged();
  return required;
}

bool UtilsMain::isModified() { return modified; }
