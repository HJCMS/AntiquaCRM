// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "taxlevelbox.h"

#include <ASettings>
#include <QDebug>

TaxLevelBox::TaxLevelBox(QWidget *parent) : InputEdit{parent} {
  m_box = new AntiquaComboBox(this);
  m_box->setToolTip(tr("Tax"));
  m_box->addItem(tr("without VAT"), 0);
  m_box->addItem(tr("with VAT"), 1);
  m_layout->addWidget(m_box);
  setModified(false);
  setRequired(true);
  connect(m_box, SIGNAL(currentIndexChanged(int)), SLOT(dataChanged(int)));
}

void TaxLevelBox::loadDataset() {}

void TaxLevelBox::dataChanged(int) { setModified(true); }

void TaxLevelBox::reset() {
  m_box->setCurrentIndex(2);
  setModified(false);
}

void TaxLevelBox::setValue(const QVariant &val) {
  int index = m_box->findData(val.toInt(), Qt::UserRole);
  m_box->setCurrentIndex((index > 0) ? 1 : 0);
}

void TaxLevelBox::setFocus() {
  m_box->setFocus();
  m_box->showPopup();
}

void TaxLevelBox::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant TaxLevelBox::value() {
  int index = m_box->currentIndex();
  return m_box->itemData(index, Qt::UserRole).toInt();
}

bool TaxLevelBox::isValid() {
  if (isRequired() && value().toInt() == -1)
    return false;

  return true;
}

void TaxLevelBox::setInfo(const QString &info) {
  m_box->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString TaxLevelBox::info() { return m_box->toolTip(); }

const QString TaxLevelBox::notes() {
  return tr("a Valid tax level is required.");
}
