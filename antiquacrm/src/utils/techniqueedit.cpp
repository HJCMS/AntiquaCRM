// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "techniqueedit.h"
#include "sqlcore.h"

#include <QtCore/QDebug>
#include <QtWidgets/QHBoxLayout>

TechniqueEdit::TechniqueEdit(QWidget *parent) : UtilsMain{parent} {
  setObjectName("TechniqueEdit");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_comboBox = new QComboBox(this);
  m_comboBox->setInsertPolicy(QComboBox::NoInsert);
  m_comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
  m_comboBox->setEditable(false);
  layout->addWidget(m_comboBox);

  setModified(false);
  setRequired(false);
  setLayout(layout);

  connect(m_comboBox, SIGNAL(currentIndexChanged(int)), this,
          SLOT(itemChanged(int)));
}

void TechniqueEdit::laodData() {
  HJCMS::SqlCore *m_sql = new HJCMS::SqlCore(this);

  QString sql("SELECT rpt_id,rpt_type FROM");
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

void TechniqueEdit::itemChanged(int) { setModified(true); }

void TechniqueEdit::loadDataset() { laodData(); }

void TechniqueEdit::setValue(const QVariant &v) {
  m_comboBox->setCurrentIndex(v.toInt());
}

void TechniqueEdit::reset() {
  m_comboBox->setCurrentIndex(0);
  setModified(false);
}

void TechniqueEdit::setFocus() { m_comboBox->setFocus(); }

const QVariant TechniqueEdit::value() { return m_comboBox->currentIndex(); }

bool TechniqueEdit::isValid() { return true; }

const QString TechniqueEdit::notes() {
  return tr("Technique field is required and must set.");
}
