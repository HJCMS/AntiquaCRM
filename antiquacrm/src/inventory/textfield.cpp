// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "textfield.h"

#include <QtCore/QDebug>
#include <QtCore/QRegExp>

TextField::TextField(QWidget *parent) : QTextEdit{parent} {
  if (objectName().isEmpty())
    setObjectName("TextField");

  setWindowTitle(tr("Textfield"));
  setTextInteractionFlags(Qt::TextEditorInteraction);
  setAcceptRichText(false); // Qt::PlainText
}

void TextField::dataChanged() { setModified(true); }

void TextField::setModified(bool b) { modified = b; }

void TextField::setRequired(bool b) { required = b; }

bool TextField::isRequired() { return required; }

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

bool TextField::isValid() {
  if (required && toPlainText().isEmpty())
    return false;

  return true;
}

const QString TextField::notes() {
  QString msg(tr("The field"));
  if (windowTitle().isEmpty()) {
    msg.append(" " + objectName() + " ");
  } else {
    msg.append(" " + windowTitle() + " ");
  }
  msg.append(tr("is required and can not empty."));
  return msg;
}
