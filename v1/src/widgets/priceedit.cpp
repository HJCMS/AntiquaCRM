// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "priceedit.h"

PriceEdit::PriceEdit(QWidget *parent) : InputEdit{parent} {
  m_box = new AntiquaDoubleBox(this);
  m_box->setSuffix(tr("\342\202\254")); /**< Setze € Suffix */
  m_box->setMinimum(0.00);
  m_box->setMaximum(99999.99);
  m_layout->addWidget(m_box);
  connect(m_box, SIGNAL(valueChanged(double)), this, SLOT(itemChanged(double)));
}

void PriceEdit::loadDataset() {}

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

void PriceEdit::setFocus() { m_box->setFocus(); }

void PriceEdit::setProperties(const QSqlField &field) {
  qInfo("TODO PriceEdit::setProperties");
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
