// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "conditionedit.h"

ConditionEdit::ConditionEdit(QWidget *parent) : InputEdit{parent} {
  m_box = new AntiquaComboBox(this);
  m_box->setToolTip(tr("Condition"));
  m_layout->addWidget(m_box);
  loadDataset();
  setRequired(true);
  connect(m_box, SIGNAL(currentIndexChanged(int)), this,
          SLOT(dataChanged(int)));
}

void ConditionEdit::loadDataset() {
  m_box->clear();
  m_box->addItem(tr("Without disclosures"), AntiquaCRM::NO_CONDITION);
  m_box->addItem(tr("Very good, almost new!"), AntiquaCRM::FINE);
  m_box->addItem(tr("Slight signs of wear."), AntiquaCRM::GOOD);
  m_box->addItem(tr("Significant signs of use."), AntiquaCRM::SATISFYING);
  m_box->addItem(tr("Heavily worn!"), AntiquaCRM::SUFFICIENT);
}

void ConditionEdit::dataChanged(int index) {
  Q_UNUSED(index);
  setModified(true);
}

void ConditionEdit::reset() {
  m_box->setCurrentIndex(0);
  setModified(false);
}

void ConditionEdit::setValue(const QVariant &val) {
  int index = -1;
  if (val.type() == QVariant::Int)
    index = m_box->findData(val, Qt::UserRole);
  else if (val.type() == QVariant::String)
    index = m_box->findData(val, Qt::DisplayRole);

  if (index < 1)
    return;

  m_box->setCurrentIndex(index);
}

void ConditionEdit::setFocus() {
  m_box->setFocus();
  m_box->showPopup();
}

void ConditionEdit::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant ConditionEdit::value() {
  return m_box->itemData(m_box->currentIndex(), Qt::UserRole);
}

bool ConditionEdit::isValid() {
  if (isRequired() && m_box->currentIndex() == 0)
    return false;

  return true;
}

void ConditionEdit::setInfo(const QString &info) {
  setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString ConditionEdit::info() { return toolTip(); }

const QString ConditionEdit::notes() {
  return tr("a valid Condition is required.");
}
