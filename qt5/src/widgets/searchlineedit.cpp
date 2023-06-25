// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "searchlineedit.h"

SearchLineEdit::SearchLineEdit(QWidget *parent) : QLineEdit{parent} {
  setClearButtonEnabled(true);
  setPlaceholderText(tr("Search for"));
}

const QRegExpValidator *SearchLineEdit::setTextValidator() {
  static const QRegExp pattern("^\\S{2}.+");
  return (new QRegExpValidator(pattern, this));
}

const QRegExpValidator *SearchLineEdit::setNumericValidator() {
  static const QRegExp pattern("^[0-9]+$");
  return (new QRegExpValidator(pattern, this));
}

const QRegExpValidator *SearchLineEdit::setArticleValidator() {
  static const QRegExp pattern("^([0-9]{1,9}[\\,]?)+$");
  return (new QRegExpValidator(pattern, this));
}

void SearchLineEdit::setValidation(SearchLineEdit::Validator v) {
  if (v == SearchLineEdit::Article)
    setValidator(setArticleValidator());
  else if (v == SearchLineEdit::Numeric)
    setValidator(setNumericValidator());
  else
    setValidator(setTextValidator());
}

const QRegExp SearchLineEdit::spacePattern() { return QRegExp("[\\s\\t]+"); }

int SearchLineEdit::getLength() { return text().length(); }
