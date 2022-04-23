// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "genderbox.h"
#include "version.h"

#include <QtCore/QDebug>

GenderBox::GenderBox(QWidget *parent) : QComboBox{parent} {
  setObjectName("GenderBox");
  setToolTip(tr("Gender"));
  addItem(myIcon("group"), tr("Without disclosures")); /**< Ohne Angabe */
  addItem(myIcon("edit_group"), tr("Male"));           /**< Männlich */
  addItem(myIcon("edit_group"), tr("Female"));         /**< Weiblich */
  addItem(myIcon("edit_group"), tr("Various"));        /**< Diverse */
  connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(itemChanged(int)));
}

void GenderBox::itemChanged(int) { setModified(true); }

void GenderBox::setValue(const QVariant &i) {
  setCurrentIndex(i.toInt());
  setModified(false);
}

void GenderBox::setModified(bool b) { modified = b; }

void GenderBox::setRequired(bool b) { required = b; }

bool GenderBox::isRequired() { return required; }

bool GenderBox::hasModified() { return modified; }

void GenderBox::reset() {
  setCurrentIndex(0);
  setModified(false);
}

const QVariant GenderBox::value() { return currentIndex(); }

bool GenderBox::isValid() { return true; }

const QString GenderBox::notes() { return QString(); }
