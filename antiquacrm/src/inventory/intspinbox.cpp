// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "intspinbox.h"
#include "version.h"

#include <QtCore/QDebug>

IntSpinBox::IntSpinBox(QWidget *parent) : QSpinBox{parent} {
  setObjectName("IntSpinBox");
  setWindowTitle(tr("Number"));
  setModified(false);
  setMinimum(0);
}

void IntSpinBox::itemChanged(int) { setModified(true); }

void IntSpinBox::setModified(bool b) { modified = b; }

void IntSpinBox::setRequired(bool b) { required = b; }

bool IntSpinBox::isRequired() { return required; }

void IntSpinBox::setValue(const QVariant &val) {
  int value = val.toInt();
  QSpinBox::setValue(value);
  setModified(false);
}

void IntSpinBox::reset() {
  QSpinBox::setValue(0);
  setModified(false);
}

bool IntSpinBox::hasModified() { return modified; }

const QVariant IntSpinBox::value() { return QVariant(QSpinBox::value()); }

bool IntSpinBox::isValid() {
  if (required && (QSpinBox::value() <= minimum()))
    return false;

  return true;
}

const QString IntSpinBox::notes() {
  QString msg(windowTitle() + " ");
  msg.append(tr("is required and can not empty."));
  return msg;
}
