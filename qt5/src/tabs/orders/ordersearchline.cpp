// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordersearchline.h"

OrderSearchLine::OrderSearchLine(QWidget *parent)
    : SearchLineEdit{parent} {
  setValidation(SearchLineEdit::Strings);
}

const QString OrderSearchLine::getSearch() {
  QString txt = text().trimmed();
  txt.replace("'", "");
  txt.replace("\"", "");
  return txt;
}
