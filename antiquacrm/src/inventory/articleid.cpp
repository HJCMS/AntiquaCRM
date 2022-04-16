// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "articleid.h"
#include "version.h"

#include <QtCore/QDebug>

ArticleID::ArticleID(QWidget *parent) : QLineEdit{parent} {
  setObjectName("ArticleID");
  setMaxLength(12);
  setReadOnly(true);
  setModified(false);
  setPlaceholderText(tr("Not editable"));
}

void ArticleID::setValue(const QVariant &id) {
  setText(id.toString());
  setModified(false);
}

void ArticleID::reset() {
  clear();
  setModified(false);
}

bool ArticleID::hasModified() { return isModified(); }

const QVariant ArticleID::value() {
  QVariant data = QVariant(text()).toULongLong();
  return data;
}
