// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "groupbox.h"

#include <QDebug>

GroupBox::GroupBox(QWidget *parent) : InputEdit{parent} {
  box = new QGroupBox(this);
  box->setCheckable(true);
  box->setChecked(false);
  m_layout->addWidget(box);
  setRequired(false);
  setModified(false);
  connect(box, SIGNAL(toggled(bool)), this, SLOT(setModified(bool)));
}

void GroupBox::reset() {
  box->setChecked(false);
  setModified(false);
}

void GroupBox::setValue(const QVariant &val) {
  box->setChecked(val.toBool());
}

void GroupBox::setFocus() {}

void GroupBox::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant GroupBox::value() { return box->isChecked(); }

bool GroupBox::isValid() {
  if (isRequired() && !value().toBool())
    return false;

  return true;
}

void GroupBox::setInfo(const QString &info) {
  box->setTitle(info);
  box->setToolTip(info);
}

const QString GroupBox::info() { return box->toolTip(); }

const QString GroupBox::notes() { return tr("This GroupBox is required."); }
