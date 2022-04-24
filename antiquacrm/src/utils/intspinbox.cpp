// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "intspinbox.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtWidgets/QHBoxLayout>

IntSpinBox::IntSpinBox(QWidget *parent) : UtilsMain{parent} {
  if (objectName().isEmpty())
    setObjectName("IntSpinBox");

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_info = new QLabel(this);
  m_info->setAlignment(labelAlignment());
  layout->addWidget(m_info);

  m_spinBox = new QSpinBox(this);
  m_spinBox->setMinimum(0);
  layout->addWidget(m_spinBox);

  setLayout(layout);

  setModified(false);
  setRequired(false);

  connect(m_spinBox, SIGNAL(valueChanged(int)), this, SLOT(itemChanged(int)));
}

void IntSpinBox::itemChanged(int) { setModified(true); }

void IntSpinBox::setRange(int min, int max) {
  m_spinBox->setMinimum(min);
  m_spinBox->setMaximum(max);
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

bool IntSpinBox::hasModified() { return isModified(); }

const QVariant IntSpinBox::value() { return m_spinBox->value(); }

bool IntSpinBox::isValid() {
  if (isRequired() && (m_spinBox->value() <= m_spinBox->minimum()))
    return false;

  return true;
}

void IntSpinBox::setInfo(const QString &info) {
  QString txt(info);
  txt.append(":");
  m_info->setText(txt);
  m_spinBox->setToolTip(info);
}

const QString IntSpinBox::info() { return m_info->text(); }

const QString IntSpinBox::notes() {
  QString msg(windowTitle() + " ");
  msg.append(tr("is required and can not empty."));
  return msg;
}
