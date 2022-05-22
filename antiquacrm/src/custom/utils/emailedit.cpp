// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "emailedit.h"

#include <QDebug>
#include <QHBoxLayout>

EMailEdit::EMailEdit(QWidget *parent) : UtilsMain{parent} {
  if (objectName().isEmpty())
    setObjectName("EMailEdit");

  setModified(false);
  setRequired(false);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_label = new QLabel(this);
  m_label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  layout->addWidget(m_label);

  m_mail = new QLineEdit(this);
  m_mail->setMaxLength(80);
  m_mail->setMaximumWidth(300);
  m_mail->setPlaceholderText(tr("usage.example@example.com"));
  layout->addWidget(m_mail);

  m_validator = new QRegExpValidator(pcre(), m_mail);
  m_mail->setValidator(m_validator);

  setLayout(layout);

  connect(m_mail, SIGNAL(textChanged(const QString &)), this,
          SLOT(inputChanged(const QString &)));

  connect(m_mail, SIGNAL(returnPressed()), this, SLOT(skipReturnPressed()));
}

const QRegExp EMailEdit::pcre() {
  QRegExp reg;
  reg.setCaseSensitivity(Qt::CaseInsensitive);
  reg.setPattern("^([\\d\\w\\-\\.]{3,})@([\\d\\w\\-\\.]{3,})\\.([a-z]{2,6})$");
  return reg;
}

void EMailEdit::setValue(const QVariant &id) {
  m_mail->setText(id.toString().toLower());
  setModified(true);
}

void EMailEdit::inputChanged(const QString &str) {
  QRegularExpression r(pcre().pattern());
  QRegularExpressionMatch m = r.match(str.trimmed());
  if (m.hasMatch()) {
    m_mail->setStyleSheet("");
  } else {
    m_mail->setStyleSheet("color: red;");
  }
  setModified(true);
}

void EMailEdit::reset() {
  m_mail->clear();
  setModified(false);
}

void EMailEdit::setFocus() { m_mail->setFocus(); }

const QString EMailEdit::info() { return m_label->text(); }

const QVariant EMailEdit::value() { return m_mail->text(); }

void EMailEdit::setInfo(const QString &info) {
  QString label(info);
  label.append(":");
  m_label->setText(label);
  m_mail->setToolTip(info);
}

bool EMailEdit::isValid() {
  if (isRequired() && m_mail->text().isEmpty())
    return false;

  if (QVariant(m_mail->text()).toULongLong() < 1)
    return false;

  return true;
}

const QString EMailEdit::notes() {
  return tr("a E-Mail Address is required and can not empty.");
}
