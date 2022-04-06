// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "articleid.h"
#include "version.h"

#include <QtCore/QDebug>

ArticleID::ArticleID(QWidget *parent) : QLineEdit{parent} {
  setObjectName("ArticleID");
  setMaxLength(12);
  setReadOnly(true);
}

void ArticleID::setValue(const QVariant &id) {
  // qDebug() << id;
  setText(id.toString());
}

const QVariant ArticleID::value() {
  QVariant data = QVariant(text()).toULongLong();
  return data;
}
