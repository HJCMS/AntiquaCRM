// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "articleid.h"
#include "version.h"

#include <QtCore/QDebug>

ArticleID::ArticleID(QWidget *parent) : QLineEdit{parent} {
  setObjectName("ArticleID");
  setWindowTitle(tr("Article ID"));
  setMaxLength(12);
  setReadOnly(true);
  setModified(false);
  setPlaceholderText(tr("Not editable"));
}

void ArticleID::setValue(const QVariant &id) {
  setText(id.toString());
  setModified(true);
}

void ArticleID::reset() {
  clear();
  setModified(false);
}

void ArticleID::setModified(bool b) { QLineEdit::setModified(b); }

void ArticleID::setRequired(bool b) { required = b; }

bool ArticleID::isRequired() { return required; }

bool ArticleID::hasModified() { return isModified(); }

const QVariant ArticleID::value() {
  QVariant data = QVariant(text()).toULongLong();
  return data;
}

bool ArticleID::isValid() {
  if (required && text().isEmpty())
    return false;

  if (QVariant(text()).toULongLong() < 1)
    return false;

  return true;
}

const QString ArticleID::notes() {
  return tr("The Arcticle ID is required and can not empty.");
}
