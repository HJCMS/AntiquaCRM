// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printsstitchessearchline.h"

PrintsStitchesSearchLine::PrintsStitchesSearchLine(QWidget *parent)
    : SearchLineEdit{parent}, stripPattern{"[\\t\\*\\<\\>]+"} {
  setValidation(SearchLineEdit::Strings);
}

const QString PrintsStitchesSearchLine::getSearch() {
  QString txt = text().trimmed();
  txt.replace("'", "’");
  txt.replace(stripPattern, " ");
  return txt.trimmed();
}

bool PrintsStitchesSearchLine::isValid(int min) {
  QString str = getSearch();
  if (str.isEmpty())
    return false;

  return (str.length() < min) ? false : true;
}
