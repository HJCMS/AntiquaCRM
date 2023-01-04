// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "techniqueedit.h"

#include <QDebug>

TechniqueEdit::TechniqueEdit(QWidget *parent) : InputEdit{parent} {
  m_comboBox = new AntiquaComboBox(this);
  m_comboBox->setInsertPolicy(QComboBox::NoInsert);
  m_comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
  m_comboBox->setEditable(false);
  m_layout->addWidget(m_comboBox);
  setModified(false);
  setRequired(false);
  loadDataset();

  connect(m_comboBox, SIGNAL(currentIndexChanged(int)), SLOT(dataChanged(int)));
}

void TechniqueEdit::dataChanged(int) { setModified(true); }

void TechniqueEdit::reset() {
  m_comboBox->setCurrentIndex(0);
  setModified(false);
}

void TechniqueEdit::setValue(const QVariant &v) {
  m_comboBox->setCurrentIndex(v.toInt());
}

void TechniqueEdit::setFocus() { m_comboBox->setFocus(); }

void TechniqueEdit::loadDataset() {
  AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
  QString sql("SELECT rpt_id, rpt_type FROM");
  sql.append(" ref_print_technique ORDER BY rpt_id ASC;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      int index = q.value(0).toInt();
      m_comboBox->insertItem(index, q.value(1).toString());
    }
    m_comboBox->setMaxCount(m_comboBox->count());
    m_comboBox->setCurrentIndex(0);
  } else {
    qWarning("No Database result for Technique.");
  }
}

void TechniqueEdit::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant TechniqueEdit::value() { return m_comboBox->currentIndex(); }

bool TechniqueEdit::isValid() {
  if (isRequired() && m_comboBox->currentIndex() != 0)
    return false;

  return true;
}

void TechniqueEdit::setInfo(const QString &info) {
  m_comboBox->setToolTip(info);
}

const QString TechniqueEdit::info() { return m_comboBox->toolTip(); }

const QString TechniqueEdit::notes() {
  return tr("Technique field is required and must set.");
}
