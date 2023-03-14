// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "editinfolabel.h"

EditInfoLabel::EditInfoLabel(QWidget *parent) : InputEdit{parent} {
  m_label->setVisible(true);
  m_label->setTextInteractionFlags(Qt::TextSelectableByKeyboard |
                                   Qt::TextSelectableByMouse);
  m_layout->addStretch(1);
  setRequired(false);
  setModified(false);
}

void EditInfoLabel::loadDataset() {}

void EditInfoLabel::reset() {
  m_label->clear();
  setModified(false);
}

void EditInfoLabel::setValue(const QVariant &val) {
  QString txt = val.toString().trimmed();
  if (txt.isEmpty())
    return;

  m_label->setText(txt);
}

void EditInfoLabel::setFocus() { m_label->setFocus(); }

void EditInfoLabel::setProperties(const QSqlField &field) {
  setRequired(false);
  if (!field.defaultValue().isNull()) {
    if (m_label->text().isEmpty() && field.type() == QVariant::String)
      setValue(field.defaultValue());
  }
}

const QVariant EditInfoLabel::value() { return m_label->text(); }

bool EditInfoLabel::isValid() { return true; }

void EditInfoLabel::setInfo(const QString &info) { m_label->setToolTip(info); }

const QString EditInfoLabel::info() { return m_label->toolTip(); }

const QString EditInfoLabel::notes() { return QString(); }
