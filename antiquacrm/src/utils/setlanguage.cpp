// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "setlanguage.h"

#include <QtCore/QDebug>
#include <QtWidgets/QHBoxLayout>

SetLanguage::SetLanguage(QWidget *parent) : UtilsMain{parent} {
  setObjectName("SetLanguage");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_comboBox = new QComboBox(this);
  m_comboBox->setToolTip(tr("Language"));
  m_comboBox->addItem(tr("German"), "de_DE");
  m_comboBox->addItem(tr("Europe"), "eu_EU");
  m_comboBox->addItem(tr("Czech"), "cs_CS");
  m_comboBox->addItem(tr("Danish"), "da_DA");
  m_comboBox->addItem(tr("English"), "en_EN");
  m_comboBox->addItem(tr("Spanish"), "es_ES");
  m_comboBox->addItem(tr("Finnish"), "fi_FI");
  m_comboBox->addItem(tr("French"), "fr_FR");
  m_comboBox->addItem(tr("Italian"), "it_IT");
  m_comboBox->addItem(tr("Dutch"), "nl_NL");
  m_comboBox->addItem(tr("Norwegian"), "nn_NN");
  m_comboBox->addItem(tr("Polish"), "pl_PL");
  m_comboBox->addItem(tr("Portuguese"), "pt_PT");
  m_comboBox->addItem(tr("Slovenian"), "sl_SL");
  m_comboBox->addItem(tr("Swedish"), "sv_SV");
  layout->addWidget(m_comboBox);

  layout->addStretch(1);

  setLayout(layout);
  setRequired(false);
  setModified(false);

  connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this,
          SLOT(itemChanged(int)));
}

void SetLanguage::itemChanged(int) { setModified(true); }

void SetLanguage::setValue(const QVariant &v) {
  int index = m_comboBox->findData(v, Qt::UserRole, Qt::MatchExactly);

  m_comboBox->setCurrentIndex(index);
}

void SetLanguage::reset() {
  m_comboBox->setCurrentIndex(0);
  setModified(false);
}

const QVariant SetLanguage::value() {
  return m_comboBox->itemData(m_comboBox->currentIndex(), Qt::UserRole);
}

bool SetLanguage::isValid() {
  if (isRequired() == m_comboBox->currentIndex() == 0)
    return false;

  return true;
}

const QString SetLanguage::notes() {
  return tr("Language field is required and must set.");
}
