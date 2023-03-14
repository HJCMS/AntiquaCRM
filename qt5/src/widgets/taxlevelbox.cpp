// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "taxlevelbox.h"

#include <ASettings>
#include <QIcon>

TaxLevelBox::TaxLevelBox(QWidget *parent) : InputEdit{parent} {
  m_box = new AntiquaComboBox(this);
  m_box->setToolTip(tr("Tax"));

  m_box->addItem(QIcon("://icons/db_remove.png"), tr("without sales tax"),
                 AntiquaCRM::SalesTax::TAX_NOT);

  m_box->addItem(QIcon("://icons/db_add.png"), tr("with sales tax"),
                 AntiquaCRM::SalesTax::TAX_WITH);

  m_box->addItem(QIcon("://icons/db_comit.png"), tr("including sales tax"),
                 AntiquaCRM::SalesTax::TAX_INCL);

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
  if (val.type() != QVariant::Int) {
    qWarning("Invalid sales tax value!");
    return;
  }

  for (int i = 0; i < m_box->count(); i++) {
    if (m_box->itemData(i, Qt::UserRole).toInt() == val.toInt()) {
      m_box->setCurrentIndex(i);
      break;
    }
  }
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
