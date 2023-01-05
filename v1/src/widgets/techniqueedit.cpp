// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "techniqueedit.h"

#include <QIcon>

TechniqueEdit::TechniqueEdit(QWidget *parent) : InputEdit{parent} {
  m_comboBox = new AntiquaComboBox(this);
  m_comboBox->setInsertPolicy(QComboBox::NoInsert);
  m_comboBox->setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
  m_comboBox->setMaxVisibleItems(20);
  m_comboBox->setEditable(false);
  m_layout->addWidget(m_comboBox);

  m_search = new AntiquaLineEdit(this);
  m_search->setPlaceholderText(tr("Search"));
  m_search->setToolTip(tr("Search starts with more than 3 chars."));
  m_layout->addWidget(m_search);

  setModified(false);
  setRequired(false);
  loadDataset();

  connect(m_search, SIGNAL(returnPressed()), SLOT(setCompleted()));
  connect(m_comboBox, SIGNAL(currentIndexChanged(int)),
          SLOT(setDataChanged(int)));
}

void TechniqueEdit::setDataChanged(int) { setModified(true); }

void TechniqueEdit::setCompleted() {
  QString search = m_search->text().trimmed();
  int index = m_comboBox->findText(search, Qt::MatchExactly);
  if (index > 0) {
    m_comboBox->setCurrentIndex(index);
    setModified(true);
  }
}

void TechniqueEdit::reset() {
  m_search->clear();
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
    QStringList list;
    while (q.next()) {
      int index = q.value("rpt_id").toInt();
      QString txt = q.value("rpt_type").toString();
      if (index > 0)
        list.append(txt);

      m_comboBox->insertItem(index, txt);
    }
    m_comboBox->setMaxCount(m_comboBox->count());
    m_comboBox->setCurrentIndex(0);

    m_completer = new QCompleter(list, m_search);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_search->setCompleter(m_completer);
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
  if (isRequired() && m_comboBox->currentIndex() == 0)
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
