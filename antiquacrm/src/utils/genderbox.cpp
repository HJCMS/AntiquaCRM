// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "genderbox.h"
#include "version.h"

#include <QtCore/QDebug>

Gender::Gender() {
  p_data.insert(0, QObject::tr("Without disclosures")); /**< Ohne Angabe */
  p_data.insert(1, QObject::tr("Male"));                /**< Männlich */
  p_data.insert(2, QObject::tr("Female"));              /**< Weiblich */
  p_data.insert(3, QObject::tr("Various"));             /**< Diverse */
}

int Gender::size() { return p_data.size(); }

const QString Gender::value(int index) { return p_data.value(index); }

const QStringList Gender::all() {
  QStringList list;
  QHash<int, QString>::iterator i;
  for (i = p_data.begin(); i != p_data.end(); ++i) {
    list.append(i.value());
  }
  return list;
}

GenderBox::GenderBox(QWidget *parent) : QComboBox{parent} {
  setObjectName("GenderBox");
  setToolTip(tr("Gender"));
  Gender gender;
  for (int i = 0; i < gender.size(); i++)
  {
      addItem(myIcon("group"),gender.value(i));
  }
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
