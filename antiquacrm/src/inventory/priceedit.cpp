// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "priceedit.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtWidgets/QVBoxLayout>

PriceEdit::PriceEdit(QWidget *parent) : QWidget{parent} {
  setObjectName("PriceEdit");
  modified = false;

  QVBoxLayout *mainlayout = new QVBoxLayout(this);
  mainlayout->setObjectName("PriceLayout");

  m_box = new QDoubleSpinBox(this);
  m_box->setObjectName("PriceDoubleSpinBox");
  // Setze € Suffix
  m_box->setSuffix(tr("\342\202\254"));

  m_box->setMinimum(0.00);
  m_box->setMaximum(99999.99);

  mainlayout->addWidget(m_box);
  setLayout(mainlayout);

  connect(m_box, SIGNAL(valueChanged(double)), this, SLOT(itemChanged(double)));
}

void PriceEdit::itemChanged(double) { setModified(true); }

void PriceEdit::setValue(const QVariant &val) {
  m_box->setValue(val.toDouble());
  setModified(false);
}

void PriceEdit::setModified(bool b) { modified = b; }

void PriceEdit::reset() {
  m_box->setValue(0);
  setModified(false);
}

bool PriceEdit::hasModified() { return modified; }

const QVariant PriceEdit::value() { return QVariant(m_box->value()); }
