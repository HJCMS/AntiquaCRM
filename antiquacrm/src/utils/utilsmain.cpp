// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "utilsmain.h"

UtilsMain::UtilsMain(QWidget *parent) : QFrame{parent} {
  if (objectName().isEmpty())
    setObjectName("UtilsTemplate");

  modified = false;
}

void UtilsMain::setModified(bool b) {
  emit modifiedChanged();
  modified = b;
}

void UtilsMain::setRequired(bool b) { required = b; }

bool UtilsMain::isRequired() {
  emit requireChanged();
  return required;
}

bool UtilsMain::isModified() { return modified; }
