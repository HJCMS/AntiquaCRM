// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "emailedit.h"

#include <QtCore/QDebug>
#include <QtWidgets/QHBoxLayout>

EMailEdit::EMailEdit(QWidget *parent) : UtilsMain{parent} {
  if (objectName().isEmpty())
    setObjectName("EMailEdit");

  setModified(false);
  setRequired(false);

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_label = new QLabel(this);
  m_label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  layout->addWidget(m_label);

  m_mail = new QLineEdit(this);
  m_mail->setMaxLength(30);
  m_mail->setMaximumWidth(220);
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
  reg.setPattern("^[\\da-z]+([\\da-z][-._+])[\\da-z]+@[\\da-z]+([-.][\\da-z]+)("
                 "[\\da-z][.])[a-z]{2,6}$");
  return reg;
}

void EMailEdit::setValue(const QVariant &id) {
  m_mail->setText(id.toString());
  setModified(true);
}

void EMailEdit::inputChanged(const QString &str) {
  Q_UNUSED(str);
  setModified(true);
}

void EMailEdit::skipReturnPressed() { setModified(true); }

void EMailEdit::reset() {
  m_mail->clear();
  setModified(false);
}

void EMailEdit::setModified(bool b) { modified = b; }

void EMailEdit::setRequired(bool b) { required = b; }

bool EMailEdit::isRequired() { return required; }

bool EMailEdit::hasModified() { return modified; }

const QString EMailEdit::text() { return m_mail->text(); }

const QVariant EMailEdit::value() {
  qDebug() << Q_FUNC_INFO << "Todo Parser";
  QVariant data = QVariant(m_mail->text());
  return data;
}

void EMailEdit::setInfoText(const QString &txt) {
  QString info(txt);
  info.append(":");
  m_label->setText(info);
}

bool EMailEdit::isValid() {
  if (required && m_mail->text().isEmpty())
    return false;

  if (QVariant(m_mail->text()).toULongLong() < 1)
    return false;

  return true;
}

const QString EMailEdit::notes() {
  return tr("a E-Mail Address is required and can not empty.");
}
