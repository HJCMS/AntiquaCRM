// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "language.h"

#include <QDebug>
#include <QHBoxLayout>

Language::Language(QWidget *parent) : InputEdit{parent} {
  setObjectName("language_edit");
  m_comboBox = new AntiquaComboBox(this);
  loadDataset();
  m_layout->addWidget(m_comboBox);
  m_layout->addStretch(1);
  setRequired(false);
  setModified(false);
  connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this,
          SLOT(itemChanged(int)));
}

void Language::itemChanged(int) { setModified(true); }

void Language::setValue(const QVariant &v) {
  int index = m_comboBox->findData(v, Qt::UserRole, Qt::MatchExactly);

  m_comboBox->setCurrentIndex(index);
}

void Language::loadDataset() {
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
}

void Language::reset() {
  m_comboBox->setCurrentIndex(0);
  setModified(false);
}

void Language::setFocus() { m_comboBox->setFocus(); }

void Language::setProperties(const QSqlField &field) {
  if (!field.isValid())
    return;

  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant Language::value() {
  return m_comboBox->itemData(m_comboBox->currentIndex(), Qt::UserRole);
}

bool Language::isValid() { return true; }

void Language::setInfo(const QString &info) { m_comboBox->setToolTip(info); }

const QString Language::info() { return m_comboBox->toolTip(); }

const QString Language::notes() {
  return tr("Language field is required and must set.");
}
