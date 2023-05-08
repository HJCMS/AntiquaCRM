// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printersetborder.h"
#include "awhatsthisbutton.h"

PrinterSetBorder::PrinterSetBorder(const QString &info, QWidget *parent)
    : QFrame{parent} {
  setContentsMargins(0, 0, 0, 0);
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_value = new AntiquaCRM::ASpinBox(this);
  m_value->setButtonSymbols(QAbstractSpinBox::NoButtons);
  m_value->setRange(0, 50);
  m_value->setSuffix(" mm");
  m_value->setGroupSeparatorShown(true);
  layout->addWidget(m_value);

  m_slider = new AntiquaCRM::ASlider(this);
  m_slider->setRange(0, m_value->maximum());
  m_slider->setTickInterval(1);
  layout->addWidget(m_slider);

  layout->addWidget(new AntiquaCRM::AWhatsThisButton(info, this));

  setLayout(layout);
  connect(m_slider, SIGNAL(valueChanged(int)), SLOT(sliderChanged(int)));
}

void PrinterSetBorder::sliderChanged(int i) {
  m_value->setValue(i);
}

void PrinterSetBorder::setValue(int d) {
  m_slider->setValue(d);
  m_value->setValue(d);
}

double PrinterSetBorder::getValue() { return m_value->value(); }
