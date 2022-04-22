// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "salutationbox.h"
#include "version.h"

#include <QtCore/QDebug>
// #include <QtGui>
#include <QtWidgets>

static const QStringList salutations() {
  QStringList l;
  l << "Prof.";
  l << "Prof. Dr.";
  l << "Prof. Dr. Dr.";
  l << "Univ. Prof.";
  l << "Dr.";
  l << "Dr. Dr.";
  l << "Dr. med.";
  l << "Dr. phil.";
  l << "Dr. rer. nat.";
  l << "Dr. iur.";
  l << "Dr. oec. publ.";
  l << "Dres.";
  return l;
}

SalutationBox::SalutationBox(QWidget *parent) : QComboBox{parent} {
  setObjectName("SalutationBox");
  setToolTip(tr("Title or Salutation"));
  insertItem(0,tr("Without disclosures"));
  foreach (QString n, salutations()) {
    addItem(n);
  }

  m_edit = new QLineEdit(this);
  m_edit->setMaxLength(25);
  setLineEdit(m_edit);

  connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(itemChanged(int)));
}

void SalutationBox::itemChanged(int) { setModified(true); }

void SalutationBox::setValue(const QVariant &v) {
  int index = findData(v, Qt::DisplayRole, Qt::MatchExactly);
  setCurrentIndex(index);
  setModified(false);
}

void SalutationBox::setModified(bool b) { modified = b; }

void SalutationBox::setRequired(bool b) { required = b; }

bool SalutationBox::isRequired() { return required; }

bool SalutationBox::hasModified() { return modified; }

void SalutationBox::reset() {
  setCurrentIndex(0);
  setModified(false);
}

const QVariant SalutationBox::value() {
  if (currentIndex() == 0)
    return QString();

  return currentText();
}

bool SalutationBox::isValid() { return true; }

const QString SalutationBox::notes() { return tr("a Title is requierd"); }
