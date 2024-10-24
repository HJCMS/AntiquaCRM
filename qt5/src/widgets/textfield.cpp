// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "textfield.h"

#include <QDebug>
#include <QTextDocument>

TextField::TextField(QWidget *parent) : InputEdit{parent} {
  m_edit = new QTextEdit(this);
  m_edit->setObjectName("TextField");
  m_edit->setAcceptDrops(false);
  m_edit->setAcceptRichText(false);
  m_edit->setTextInteractionFlags(Qt::TextEditorInteraction);
  m_edit->setAutoFormatting(QTextEdit::AutoNone);
  m_layout->addWidget(m_edit);

  setRequired(false);

  connect(m_edit, SIGNAL(textChanged()), SLOT(dataChanged()));
}

void TextField::loadDataset() { /* unused */
}

void TextField::dataChanged() { setModified(true); }

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

void TextField::setEditable(bool b) { m_edit->setReadOnly(!b); }

void TextField::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant TextField::value() {
  QString txt = m_edit->toPlainText();
  return stripString(txt);
}

bool TextField::isValid() {
  QString _buffer = value().toString();
  if (isRequired() && _buffer.length() < 1)
    return false;

  return true;
}

void TextField::setInfo(const QString &info) { m_edit->setToolTip(info); }

const QString TextField::info() { return m_edit->toolTip(); }

const QString TextField::notes() {
  QString str(info() + " ");
  str.append(tr("requires a valid input."));
  return str;
}
