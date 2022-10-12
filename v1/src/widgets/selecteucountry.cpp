// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "selecteucountry.h"
#include "aeuropeancountries.h"

SelectEuCountry::SelectEuCountry(QWidget *parent) : InputEdit{parent} {
  m_box = new AntiquaComboBox(this);
  m_box->setToolTip(tr("European Countires"));
  m_box->addItem(tr("Without disclosures"), QString());
  m_layout->addWidget(m_box);
  loadDataset();
  setRequired(false);

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

void SelectEuCountry::dataChanged(int index) { setModified(true); }

void SelectEuCountry::reset() { setModified(false); }

void SelectEuCountry::setValue(const QVariant &val) {
  int index = -1;
  if (val.type() != QVariant::String)
    return;

  index = m_box->findData(val.toString(), Qt::UserRole);
  if (index < 1)
    return;

  m_box->setCurrentIndex(index);
}

void SelectEuCountry::setFocus() {
  m_box->setFocus();
  m_box->showPopup();
}

const QVariant SelectEuCountry::value() {
  return m_box->itemData(m_box->currentIndex(), Qt::UserRole);
}

bool SelectEuCountry::isValid() {
  if (isRequired() && value().isNull())
    return false;

  return (m_box->currentIndex() > 0);
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
  return tr("A valid country selection is required.");
}
