// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "priceedit.h"

#include <ASettings>

PriceEdit::PriceEdit(double minimum, double maximum, QWidget *parent)
    : InputEdit{parent} {
  m_box = new AntiquaDoubleBox(this);
  m_box->setMinimum(minimum);
  m_box->setMaximum(maximum);

  AntiquaCRM::ASettings cfg(this);
  QString currency = cfg.value("payment/currency", "\342\202\254").toString();
  m_box->setSuffix(currency);

  m_layout->addWidget(m_box);
  m_layout->addStretch(1);

  connect(m_box, SIGNAL(valueChanged(double)), SLOT(itemChanged(double)));
}

PriceEdit::PriceEdit(QWidget *parent) : PriceEdit{0.00, 99999.99, parent} {}

void PriceEdit::loadDataset() {}

void PriceEdit::itemChanged(double) { setModified(true); }

void PriceEdit::setMinimum(double min) { m_box->setMinimum(min); }

void PriceEdit::setMaximum(double max) { m_box->setMaximum(max); }

void PriceEdit::setRange(double min, double max) {
  m_box->setMinimum(min);
  m_box->setMaximum(max);
}

void PriceEdit::setValue(const QVariant &val) {
  m_box->setValue(val.toDouble());
  setModified(false);
}

void PriceEdit::reset() {
  m_box->setValue(0);
  setModified(false);
}

void PriceEdit::setFocus() { m_box->setFocus(); }

void PriceEdit::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);

  if (!field.defaultValue().isNull() && m_box->value() == 0.00) {
    setValue(field.defaultValue());
  }
}

const QVariant PriceEdit::value() { return QVariant(m_box->value()); }

bool PriceEdit::isValid() {
  if (isRequired() && (m_box->value() <= m_box->minimum()))
    return false;

  if (m_box->value() <= m_box->minimum())
    return false;

  return true;
}

void PriceEdit::setInfo(const QString &info) {
  QString txt(info);
  txt.append(":");
  m_label->setVisible(true);
  m_label->setText(txt);
  m_box->setToolTip(info);
}

const QString PriceEdit::info() { return m_box->toolTip(); }

const QString PriceEdit::notes() {
  return tr("A valid Price is required and must set.");
}
