// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "textfield.h"

#include <QDebug>

TextField::TextField(QWidget *parent) : InputEdit{parent} {
  m_edit = new QTextEdit(this);
  m_layout->addWidget(m_edit);
  loadDataset();
  setRequired(true);
}

void TextField::loadDataset() {}

void TextField::dataChanged(const QString &) { setModified(true); }

void TextField::reset() {
  m_edit->setPlainText(QString());
  m_edit->clear();
  setModified(false);
}

void TextField::setValue(const QVariant &val) {
  QString txt = val.toString().trimmed();
  m_edit->setPlainText(txt);
}

void TextField::setFocus() { m_edit->setFocus(); }

void TextField::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant TextField::value() {
  QString txt = m_edit->toPlainText();
  return txt.trimmed();
}

bool TextField::isValid() {
  if (isRequired() && value().isNull())
    return false;

  return true;
}

void TextField::setInfo(const QString &info) {
  m_edit->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString TextField::info() { return m_edit->toolTip(); }

const QString TextField::notes() {
  QString str(info() + " ");
  str.append(tr("requires a valid input."));
  return str;
}
