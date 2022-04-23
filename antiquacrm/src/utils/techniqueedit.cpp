// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "techniqueedit.h"
#include "sqlcore.h"

#include <QtCore/QDebug>
#include <QtWidgets>

TechniqueEdit::TechniqueEdit(QWidget *parent) : QComboBox{parent} {
  setObjectName("TechniqueEdit");
  setInsertPolicy(QComboBox::NoInsert);
  setSizeAdjustPolicy(QComboBox::AdjustToContentsOnFirstShow);
  setEditable(false);
}

void TechniqueEdit::laodData() {
  HJCMS::SqlCore *db = new HJCMS::SqlCore(this);
  QString sql("SELECT rpt_id,rpt_type FROM ref_print_technique ORDER BY rpt_id ASC;");
  QSqlQuery q = db->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      int index = q.value(0).toInt();
      insertItem(index, q.value(1).toString());
    }
    setMaxCount(count());
    setCurrentIndex(0);
  } else {
    qWarning("No SQL QueryResult in Technique.");
  }
}

void TechniqueEdit::itemChanged(int) { setModified(true); }

void TechniqueEdit::loadDataset() { laodData(); }

void TechniqueEdit::setValue(const QVariant &v) {
  setCurrentIndex(v.toInt());
  setModified(false);
}

void TechniqueEdit::setModified(bool b) { modified = b; }

void TechniqueEdit::setRequired(bool b) { required = b; }

bool TechniqueEdit::isRequired() { return required; }

bool TechniqueEdit::hasModified() { return modified; }

void TechniqueEdit::reset() {
  setCurrentIndex(0);
  setModified(false);
}

const QVariant TechniqueEdit::value() { return currentIndex(); }

bool TechniqueEdit::isValid() { return true; }

const QString TechniqueEdit::notes() {
  return tr("Technique field is required and must set.");
}
