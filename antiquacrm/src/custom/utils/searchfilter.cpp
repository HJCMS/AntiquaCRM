// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "searchfilter.h"

#include <QDebug>

SearchFilter::SearchFilter() {
  p_title = QString();
  p_fieldset = QStringList();
  p_search = QString();
  p_type = SearchType::STRINGS;
  p_cleanInput = defaultPattern();
}

SearchFilter::SearchFilter(const SearchFilter &other) {
  p_title = other.p_title;
  p_fieldset = other.p_fieldset;
  p_search = other.p_search;
  p_type = other.p_type;
  p_cleanInput = other.p_cleanInput;
}

const QRegExp SearchFilter::defaultPattern() const {
  return QRegExp("^([\\/\\'\\´\\`]+)$");
}

const QRegularExpression SearchFilter::pattern() const {
  return QRegularExpression("^(\\w+)([\\s\\w])+");
}

void SearchFilter::setTitle(const QString &title) { p_title = title; }

QString SearchFilter::getTitle() { return p_title; }

void SearchFilter::setFields(const QString &f) { p_fieldset = f.split(","); }

void SearchFilter::setFields(const QStringList &f) { p_fieldset = f; }

QStringList SearchFilter::getFields() { return p_fieldset; }

void SearchFilter::setSearch(const QString &s) {
  QString buffer(s.trimmed());
  QRegularExpressionMatch matches = pattern().match(buffer);
  if (matches.isValid()) {
    p_search = buffer;
    return;
  }
  qWarning("Strip Search Input");
  p_search = buffer.replace(p_cleanInput, "");
}

QString SearchFilter::getSearch() { return p_search; }

void SearchFilter::setType(SearchFilter::SearchType t) { p_type = t; }

bool SearchFilter::setType(int t) {
  if (t == SearchType::STRINGS) {
    p_type = SearchType::STRINGS;
    return true;
  }
  if (t == SearchType::NUMERIC) {
    p_type = SearchType::NUMERIC;
    return true;
  }
  if (t == SearchType::REFERENCES) {
    p_type = SearchType::REFERENCES;
    return true;
  }
  return false;
}

SearchFilter::SearchType SearchFilter::getType() {
  switch (p_type) {
  case NUMERIC:
    return NUMERIC;

  case REFERENCES:
    return REFERENCES;

  default:
    return STRINGS;
  };
}

QJsonObject SearchFilter::getFilter() {
  QJsonObject obj;
  obj.insert("title", getTitle());
  obj.insert("search", getSearch());
  obj.insert("filter", getFields().join(","));
  obj.insert("type", getType());
  return obj;
}
