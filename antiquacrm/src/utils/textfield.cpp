// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "textfield.h"

#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtWidgets/QVBoxLayout>

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

const QString TextField::notes() {
  QString msg(tr("The field"));
  if (windowTitle().isEmpty()) {
    msg.append(" " + objectName() + " ");
  } else {
    msg.append(" " + windowTitle() + " ");
  }
  msg.append(tr("is required and can not empty."));
  return msg;
}
