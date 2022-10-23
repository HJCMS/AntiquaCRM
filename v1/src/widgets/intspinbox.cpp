// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "intspinbox.h"

IntSpinBox::IntSpinBox(QWidget *parent) : InputEdit{parent} { loadDataset(); }

IntSpinBox::IntSpinBox(int minimum, int maximum, QWidget *parent)
    : InputEdit{parent} {
  min = minimum;
  max = maximum;
  loadDataset();
}

void IntSpinBox::loadDataset() {
  m_spinBox = new AntiquaSpinBox(this);
  m_spinBox->setMinimum(min);
  m_spinBox->setMaximum(max);
  m_spinBox->setMaximumWidth(300);
  m_layout->addWidget(m_spinBox);
  m_layout->addStretch(1);
  setModified(false);
  setRequired(false);
  connect(m_spinBox, SIGNAL(valueChanged(int)), this, SLOT(itemChanged(int)));
}

void IntSpinBox::itemChanged(int) { setModified(true); }

void IntSpinBox::setRange(int minimum, int maximum) {
  min = minimum;
  m_spinBox->setMinimum(minimum);
  max = maximum;
  m_spinBox->setMaximum(maximum);
}

void IntSpinBox::setSingleStep(int i) { m_spinBox->setSingleStep(i); }

void IntSpinBox::setSuffix(const QString &s) { m_spinBox->setSuffix(s); }

void IntSpinBox::setPrefix(const QString &s) { m_spinBox->setPrefix(s); }

void IntSpinBox::setValue(const QVariant &val) {
  int value = val.toInt();
  m_spinBox->setValue(value);
  setModified(false);
}

void IntSpinBox::reset() {
  m_spinBox->setValue(0);
  setModified(false);
}

void IntSpinBox::setFocus() { m_spinBox->setFocus(); }

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

const QVariant IntSpinBox::value() { return m_spinBox->value(); }

bool IntSpinBox::isValid() {
  if (isRequired() && (m_spinBox->value() <= m_spinBox->minimum()))
    return false;

  return true;
}

void IntSpinBox::setInfo(const QString &info) {
  QString txt(info);
  txt.append(":");
  m_label->setVisible(true);
  m_label->setText(txt);
  m_spinBox->setToolTip(info);
}

const QString IntSpinBox::info() { return m_spinBox->toolTip(); }

const QString IntSpinBox::notes() {
  QString msg(m_spinBox->toolTip() + " ");
  msg.append(tr("is required and can not empty."));
  return msg;
}

IntSpinBox::~IntSpinBox() {}
