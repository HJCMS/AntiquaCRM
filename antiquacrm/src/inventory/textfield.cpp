// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "textfield.h"

#include <QtCore/QDebug>
#include <QtCore/QRegExp>

TextField::TextField(QWidget *parent) : QTextEdit{parent} {
  setObjectName("TextField");
  setTextInteractionFlags(Qt::TextEditorInteraction);
  setAcceptRichText(false); // Qt::PlainText
}

void TextField::dataChanged() { setModified(true); }

void TextField::setModified(bool b) { modified = b; }

void TextField::setValue(const QVariant &val) {
  QString data = val.toString();
  setPlainText(data);
  setModified(false);
}

void TextField::reset() {
  clear();
  setModified(false);
}

bool TextField::hasModified() { return modified; }

const QVariant TextField::value() {
  QString data = toPlainText();
  QRegExp reg("[\\']+");
  return QVariant(data.replace(reg, ""));
}
