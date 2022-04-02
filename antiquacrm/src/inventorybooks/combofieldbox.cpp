// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "combofieldbox.h"
#include "version.h"
#include <QtCore/QDebug>

ComboFieldBox::ComboFieldBox(QWidget *parent) : QComboBox{parent} {
  setObjectName("ComboFieldBox");
  /**
   \note Wenn hier etwas geändert wird!
    Dann muss SearchLineEdit::updatePlaceHolder(int)
    auch geändert werden!
  */
  insertItem(0, tr("Book Title"), "title");
  insertItem(1, tr("Article ID"), "id");
  insertItem(2, tr("ISBN"), "isbn");
  insertItem(3, tr("Author"), "author");
  setCurrentIndex(0);
}

void ComboFieldBox::setIndex(int i) { setCurrentIndex(i); }

void ComboFieldBox::setIndex(const QString &str) {
  int i = findText(str, Qt::MatchContains);
  if (i >= 0)
    setCurrentIndex(i);
}
