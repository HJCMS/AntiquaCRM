// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "opacityslider.h"

#include <QDebug>

OpacitySlider::OpacitySlider(QWidget *parent) : InputEdit{parent} {
  m_slider = new QSlider(Qt::Horizontal, this);
  m_slider->setRange(1, 10);
  m_slider->setValue(5);
  m_layout->addWidget(m_slider);
  lb_info = new QLabel(this);
  m_layout->addWidget(lb_info);
  connect(m_slider, SIGNAL(valueChanged(int)), SLOT(dataChanged(int)));
}

void OpacitySlider::dataChanged(int i) {
  if (i == m_slider->minimum()) {
    lb_info->setText("0.1");
  } else if (i == m_slider->maximum()) {
    lb_info->setText("1.0");
  } else {
    lb_info->setText(QString::asprintf("0.%d", i));
  }
}

void OpacitySlider::reset() { setModified(false); }

void OpacitySlider::setValue(const QVariant &val) {
  QString r = val.toString();
  for(int i = 1; i < 10; ++i) {
    if(QString::asprintf("0.%d", i) == r) {
      m_slider->setValue(i);
      lb_info->setText(r);
      break;
    }
  }
}

void OpacitySlider::setFocus() { m_slider->setFocus(); }

void OpacitySlider::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant OpacitySlider::value() {
  qreal out;
  int i = m_slider->value();
  if (i == m_slider->maximum()) {
    out = 1.0;
  } else if (i == m_slider->minimum()) {
    out = 0.1;
  } else {
    QString str = QString::asprintf("0.%d", i);
    out = str.toDouble();
  }
  // qDebug() << Q_FUNC_INFO << out;
  return QVariant(out);
}

bool OpacitySlider::isValid() {
  if (isRequired() && value().isNull())
    return false;

  return true;
}

void OpacitySlider::setInfo(const QString &info) {
  m_slider->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString OpacitySlider::info() { return m_slider->toolTip(); }

const QString OpacitySlider::notes() {
  return tr("This Slider input is required.");
}
