// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "intspinbox.h"

IntSpinBox::IntSpinBox(int minimum, int maximum, QWidget *parent)
    : InputEdit{parent} {
  m_box = new AntiquaSpinBox(this);
  m_box->setMinimum(minimum);
  m_box->setMaximum(maximum);
  m_layout->addWidget(m_box);
  m_layout->addStretch(1);
  setModified(false);
  setRequired(false);
  connect(m_box, SIGNAL(valueChanged(int)), SLOT(itemChanged(int)));
}

IntSpinBox::IntSpinBox(QWidget *parent) : IntSpinBox{0, 9999, parent} {}

void IntSpinBox::itemChanged(int) { setModified(true); }

void IntSpinBox::reset() {
  m_box->setValue(0);
  setModified(false);
}

void IntSpinBox::setFocus() { m_box->setFocus(); }

void IntSpinBox::setValue(const QVariant &val) {
  int value = val.toInt();
  m_box->setValue(value);
  setModified(false);
}

void IntSpinBox::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);

  switch (field.length()) {
  case 1:
    setRange(0, 9);
    break;

  case 2:
    setRange(0, 99);
    break;

  case 3:
    setRange(0, 999);
    break;

  case 4:
    setRange(0, 9999);
    break;

  default:
    setRange(0, 99999);
  }
}

void IntSpinBox::setRange(int minimum, int maximum) {
  m_box->setMinimum(minimum);
  m_box->setMaximum(maximum);
}

void IntSpinBox::setSingleStep(int i) { m_box->setSingleStep(i); }

void IntSpinBox::setSuffix(const QString &s) { m_box->setSuffix(s); }

void IntSpinBox::setPrefix(const QString &s) { m_box->setPrefix(s); }

void IntSpinBox::setShowButtons(bool b) {
  if (!b)
    m_box->setButtonSymbols(QAbstractSpinBox::NoButtons);
}

const QVariant IntSpinBox::value() { return m_box->value(); }

bool IntSpinBox::isValid() {
  if (isRequired() && (m_box->value() <= m_box->minimum()))
    return false;

  return true;
}

void IntSpinBox::setInfo(const QString &info) {
  QString txt(info);
  txt.append(":");
  m_label->setVisible(true);
  m_label->setText(txt);
  m_box->setToolTip(info);
}

const QString IntSpinBox::info() { return m_box->toolTip(); }

const QString IntSpinBox::notes() {
  QString msg(m_box->toolTip() + " ");
  msg.append(tr("is required and can not empty."));
  return msg;
}
