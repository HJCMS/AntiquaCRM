// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "textfield.h"

#include <QDebug>
#include <QRegExp>
#include <QVBoxLayout>

TextField::TextField(QWidget *parent) : UtilsMain{parent} {
  if (objectName().isEmpty())
    setObjectName("TextField");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_edit = new QTextEdit(this);
  m_edit->setTextInteractionFlags(Qt::TextEditorInteraction);
  m_edit->setAcceptRichText(false); // Qt::PlainText
  layout->addWidget(m_edit);

  setModified(false);
  setRequired(false);
  setLayout(layout);

  connect(m_edit, SIGNAL(textChanged()), this, SLOT(dataChanged()));
}

void TextField::dataChanged() { setModified(true); }

void TextField::setValue(const QVariant &val) {
  QString data = val.toString();
  m_edit->setPlainText(data);
}

void TextField::reset() {
  m_edit->clear();
  setModified(false);
}

void TextField::setFocus() { m_edit->setFocus(); }

const QVariant TextField::value() {
  QString data = m_edit->toPlainText();
  QRegExp reg("[\\']+");
  return data.replace(reg, "");
}

bool TextField::isValid() {
  if (isRequired() && m_edit->toPlainText().isEmpty())
    return false;

  return true;
}

void TextField::setInfo(const QString &info) { m_edit->setToolTip(info); }

const QString TextField::info() { return m_edit->toolTip(); }

const QString TextField::notes() {
  QString msg(m_edit->toolTip());
  if (msg.isEmpty()) {
    msg.append(" " + objectName() + " ");
  }
  return tr("a '%1' required and can not empty.").arg(msg);
}
