// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "priceedit.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtWidgets/QHBoxLayout>

PriceEdit::PriceEdit(QWidget *parent) : UtilsMain{parent} {
  setObjectName("PriceEdit");
  setWindowTitle("Price");

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_info = new QLabel(this);
  m_info->setAlignment(labelAlignment());
  layout->addWidget(m_info);

  m_box = new QDoubleSpinBox(this);
  m_box->setObjectName("PriceDoubleSpinBox");
  // Setze € Suffix
  m_box->setSuffix(tr("\342\202\254"));
  m_box->setMinimum(0.00);
  m_box->setMaximum(99999.99);
  layout->addWidget(m_box);

  setLayout(layout);

  connect(m_box, SIGNAL(valueChanged(double)), this, SLOT(itemChanged(double)));
}

void PriceEdit::itemChanged(double) { setModified(true); }

void PriceEdit::setMinimum(double min) { m_box->setMinimum(min); }

void PriceEdit::setValue(const QVariant &val) {
  m_box->setValue(val.toDouble());
  setModified(false);
}

void PriceEdit::reset() {
  m_box->setValue(0);
  setModified(false);
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
  m_info->setText(txt);
  m_box->setToolTip(info);
}

const QString PriceEdit::info() { return m_info->text(); }

const QString PriceEdit::notes() {
  return tr("A valid Price is required and must set.");
}
