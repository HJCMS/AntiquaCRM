// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "lineedit.h"

#include <QtWidgets/QHBoxLayout>

LineEdit::LineEdit(QWidget *parent) : UtilsMain{parent} {
  if (objectName().isEmpty())
    setObjectName("LineEdit");

  setRequired(false);
  setModified(false);

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_label = new QLabel(this);
  m_label->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
  layout->addWidget(m_label);

  m_edit = new QLineEdit(this);
  m_edit->setMaxLength(80);
  m_edit->setMaximumWidth(220);
  layout->addWidget(m_edit);

  setLayout(layout);

  connect(m_edit, SIGNAL(textChanged(const QString &)), this,
          SLOT(inputChanged(const QString &)));

  connect(m_edit, SIGNAL(returnPressed()), this, SLOT(skipReturnPressed()));
}

void LineEdit::inputChanged(const QString &str) {
  Q_UNUSED(str)
  setModified(true);
}

void LineEdit::setValue(const QVariant &val) {
  p_value = val;
  m_edit->setText(val.toString());
}

void LineEdit::reset() {
  p_value.clear();
  m_edit->clear();
}

void LineEdit::addCompleter(const QStringList &list) {
  m_completer = new QCompleter(list, m_edit);
  m_completer->setCompletionMode(QCompleter::UnfilteredPopupCompletion);
  m_edit->setCompleter(m_completer);
  emit  m_completer->activated(list.last());
}

void LineEdit::setInfo(const QString &info) {
  QString label(info);
  label.append(":");
  m_label->setText(label);
  m_edit->setToolTip(info);
}

const QString LineEdit::info() { return m_label->text(); }

void LineEdit::setPlaceholderText(const QString &txt) {
  m_edit->setPlaceholderText(txt);
}

const QVariant LineEdit::value() { return m_edit->text(); }

bool LineEdit::isValid() {
  if (isRequired() && m_edit->text().isEmpty())
    return false;

  return true;
}

const QString LineEdit::notes() { return tr("a Valid Url must edit."); }
