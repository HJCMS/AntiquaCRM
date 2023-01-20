// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booksearchline.h"

BookSearchLine::BookSearchLine(QWidget *parent) : SearchLineEdit{parent} {
  setObjectName("book_search_line");
  // Default on load
  setValidation(SearchLineEdit::Strings);
}

const QString BookSearchLine::getSearch() {
  QString txt = text().trimmed();
  txt.replace("'","");
  return txt;
}
