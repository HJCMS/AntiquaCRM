// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "groupbox.h"

#include <QtCore>
#include <QtGui>
#include <QtWidgets>

GroupBox::GroupBox(QWidget *parent) : UtilsMain{parent} {
  if (objectName().isEmpty())
    setObjectName("GroupBox");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(1, 1, 1, 1);
  box = new QGroupBox(this);
  box->setCheckable(true);
  box->setFlat(true);
  layout->addWidget(box);

  setModified(false);
  setRequired(false);

  setLayout(layout);

  connect(box, SIGNAL(toggled(bool)), this, SLOT(setModified(bool)));
}

void GroupBox::setValue(const QVariant &val) {
  bool b = val.toBool();
  box->setChecked(b);
}

void GroupBox::reset() {
  box->setChecked(false);
  setModified(false);
}

const QVariant GroupBox::value() { return box->isChecked(); }

bool GroupBox::isValid() { return true; }

void GroupBox::setInfo(const QString &info) { return box->setTitle(info); }

const QString GroupBox::info() { return box->title(); }

const QString GroupBox::notes() { return QString("dummy"); }
