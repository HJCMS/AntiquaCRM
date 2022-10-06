// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "@CLASSNAME@.h"

@CLASSNAME@::@CLASSNAME@(QWidget *parent) : InputEdit{parent} {
}

void @CLASSNAME@::loadDataset() {
}

void @CLASSNAME@::reset() {
}

void @CLASSNAME@::setValue(const QVariant &) {
}

void @CLASSNAME@::setFocus() {
}

const QVariant @CLASSNAME@::value() {
  return QVariant();
}

bool @CLASSNAME@::isValid() {
  if (isRequired() && value().isNull())
    return false;

  return true;
}

void @CLASSNAME@::setInfo(const QString &info) {
  setToolTip(info);
}

const QString @CLASSNAME@::info() { return toolTip(); }

const QString @CLASSNAME@::notes() {
  return tr("This Input is required.");
}
