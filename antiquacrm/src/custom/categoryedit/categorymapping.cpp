// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "categorymapping.h"

CategoryMapping::CategoryMapping() {
  p_subid = -1;
  p_sub = QString();
  p_words = QStringList();
}

void CategoryMapping::setSubId(int subId) { p_subid = subId; }

int CategoryMapping::getSubId() { return p_subid; }

void CategoryMapping::setSub(const QString &str) { p_sub = str; }

const QString CategoryMapping::getSub() { return p_sub; }

void CategoryMapping::setKeywords(const QStringList &list) { p_words = list; }

const QStringList CategoryMapping::getKeywords() { return p_words; }
