// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selecteucountry.h"
#include "aeuropeancountries.h"

SelectEuCountry::SelectEuCountry(QWidget *parent) : InputEdit{parent} {
  m_box = new AntiquaComboBox(this);
  m_box->setToolTip(tr("European Countries"));
  m_box->addItem(tr("Without disclosures"), QString());
  m_layout->addWidget(m_box);
  loadDataset();
  setRequired(true);

  connect(m_box, SIGNAL(currentIndexChanged(int)), this,
          SLOT(dataChanged(int)));
}

void SelectEuCountry::loadDataset() {
  AntiquaCRM::AEuropeanCountries hash;
  QHash<QString, QString>::const_iterator i = hash.constBegin();
  while (i != hash.constEnd()) {
    m_box->addItem(i.value(), i.key());
    ++i;
  }
}

void SelectEuCountry::dataChanged(int) { setModified(true); }

void SelectEuCountry::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void SelectEuCountry::setValue(const QVariant &val) {
  int index = -1;
  if (val.type() != QVariant::String)
    return;

  QString co = val.toString().toUpper();
  index = m_box->findData(co, Qt::UserRole);
  m_box->setCurrentIndex(index);
}

void SelectEuCountry::setFocus() { m_box->setFocus(); }

void SelectEuCountry::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant SelectEuCountry::value() {
  int index = m_box->currentIndex();
  QString co = m_box->itemData(index, Qt::UserRole).toString();
  return co.trimmed();
}

bool SelectEuCountry::isValid() {
  if (isRequired() && m_box->currentIndex() < 1)
    return false;

  return true;
}

void SelectEuCountry::setInfo(const QString &info) {
  setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString SelectEuCountry::info() { return toolTip(); }

const QString SelectEuCountry::notes() {
  return tr("A valid European Country selection is required.");
}
