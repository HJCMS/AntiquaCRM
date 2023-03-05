// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booksearchline.h"

BookSearchLine::BookSearchLine(QWidget *parent)
  : SearchLineEdit{parent}, stripPattern{"[\\t\\*\\<\\>]+"} {
  setObjectName("book_search_line");
  // Default on load
  setValidation(SearchLineEdit::Strings);
}

const QString BookSearchLine::getSearch() {
  QString txt = text().trimmed();
  txt.replace("'", "’");
  txt.replace(stripPattern, " ");
  return txt.trimmed();
}

bool BookSearchLine::isValid(int min) {
  QString str = getSearch();
  if (str.isEmpty())
    return false;

  return (str.length() < min) ? false : true;
}
