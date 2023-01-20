// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printsstitchessearchline.h"

PrintsStitchesSearchLine::PrintsStitchesSearchLine(QWidget *parent)
    : SearchLineEdit{parent} {
  setValidation(SearchLineEdit::Strings);
}

const QString PrintsStitchesSearchLine::getSearch() {
  QString txt = text().trimmed();
  txt.replace("'", "");
  return txt;
}
