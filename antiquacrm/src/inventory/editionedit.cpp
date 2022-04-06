// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "editionedit.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QMapIterator>
#include <QtCore/QObject>

/**
SELECT cell FROM information_schema.columns WHERE table_name = 'edition_t';
*/
EditionEdit::EditionEdit(QWidget *parent) : QComboBox{parent} {
  setObjectName("EditionEdit");

  QMapIterator<int, QString> it(editionTypes());
  while (it.hasNext()) {
    it.next();
    insertItem(it.key(), it.value());
  }
}

void EditionEdit::setValue(const QVariant &val) {
  if (val.toString().isEmpty()) {
    setCurrentIndex(0);
    return;
  }
  int index = findText(val.toString(), Qt::MatchExactly);
  setCurrentIndex(index);
}

const QVariant EditionEdit::value() {
  QVariant val;
  val.setValue(QString::number(currentIndex()));
  return val;
}
