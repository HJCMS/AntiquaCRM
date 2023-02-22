// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "borderprintinput.h"

BorderPrintInput::BorderPrintInput(QWidget *parent) : InputEdit{parent} {
  m_box = new QDoubleSpinBox(this);
  m_box->setButtonSymbols(QAbstractSpinBox::NoButtons);
  m_box->setRange(0.0, 60.0);
  m_box->setSuffix(" px");
  m_layout->addWidget(m_box);

  m_slider = new QSlider(Qt::Horizontal, this);
  m_slider->setRange(0, 60);
  m_slider->setMinimumWidth(200);
  m_layout->addWidget(m_slider);

  m_layout->addStretch(1);
  setModified(false);
  setRequired(false);

  connect(m_slider, SIGNAL(valueChanged(int)), SLOT(borderChanged(int)));
}

void BorderPrintInput::loadDataset() {}

void BorderPrintInput::borderChanged(int i) {
  m_box->setValue(static_cast<double>(i));
}

void BorderPrintInput::reset() {
  m_box->setValue(0.00);
  m_slider->setValue(0);
}

void BorderPrintInput::setValue(const QVariant &val) {
  m_box->setValue(val.toDouble());
  m_slider->setValue(val.toInt());
}

void BorderPrintInput::setFocus() { m_slider->setFocus(); }

void BorderPrintInput::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);

  if (!field.defaultValue().isNull() && m_box->value() == 0.00) {
    setValue(field.defaultValue());
  }
}

const QVariant BorderPrintInput::value() { return m_box->value(); }

bool BorderPrintInput::isValid() { return true; }

void BorderPrintInput::setInfo(const QString &info) {
  if (!info.isEmpty()) {
    m_label->setVisible(true);
    m_label->setMinimumWidth(50);
    m_label->setText(info);
  }
  m_slider->setToolTip(info);
}

const QString BorderPrintInput::info() {
  if (m_label->isVisible())
    return m_label->text();
  else
    return m_slider->toolTip();
}

const QString BorderPrintInput::notes() {
  return tr("A valid border set is required!");
}
