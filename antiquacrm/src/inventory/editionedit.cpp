// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "editionedit.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QMapIterator>
#include <QtCore/QObject>

static const QMap<int, QString> editionTypes() {
  QMap<int, QString> m;
  m.insert(0, QObject::tr("Unknown"));
  m.insert(1, QObject::tr("First edition"));
  m.insert(2, QObject::tr("Second edition"));
  m.insert(3, QObject::tr("Further edition"));
  return m;
}

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
  int index = val.toInt();
  if (index < 0) {
    setCurrentIndex(0);
    return;
  }
  setCurrentIndex(index);
}

const QVariant EditionEdit::value() {
  QVariant val;
  val.setValue(currentIndex());
  return val;
}
