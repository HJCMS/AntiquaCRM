// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "emailedit.h"

#include <QApplication>
#include <QPalette>
#include <QRegularExpressionMatch>

EMailEdit::EMailEdit(QWidget *parent) : InputEdit{parent} {
  m_edit = new AntiquaLineEdit(this);
  m_edit->setToolTip(tr("eMail edit"));
  m_edit->setPlaceholderText(tr("usage.example@example.com"));
  m_layout->addWidget(m_edit);
  m_validator = new QRegExpValidator(mailPattern(), m_edit);
  m_edit->setValidator(m_validator);
  setRequired(false);
  setModified(false);
  connect(m_edit, SIGNAL(textChanged(const QString &)),
          SLOT(dataChanged(const QString &)));
}

const QRegExp EMailEdit::mailPattern() {
  QRegExp reg;
  reg.setCaseSensitivity(Qt::CaseInsensitive);
  reg.setPattern("^([\\d\\w\\-\\.]{3,})@([\\d\\w\\-\\.]{3,})\\.([a-z]{2,6})$");
  return reg;
}

bool EMailEdit::validate(const QString &mail) const {
  QRegularExpression r(mailPattern().pattern());
  QRegularExpressionMatch m = r.match(mail);
  if (mail.contains("@") && m.hasMatch()) {
    m_edit->setStyleSheet(QString());
    return true;
  }
  m_edit->setStyleSheet("QLineEdit {selection-background-color: red;}");
  return false;
}

void EMailEdit::dataChanged(const QString &email) {
  setModified(true);
  validate(email);
}

void EMailEdit::reset() {
  m_edit->clear();
  setModified(false);
}

void EMailEdit::setValue(const QVariant &val) {
  QString email = val.toString().trimmed();
  m_edit->setText(email.toLower());
}

void EMailEdit::setFocus() {
  if (isVisible()) {
    m_edit->setFocus();
  }
}

void EMailEdit::setProperties(const QSqlField &field) {
  if (!field.isValid())
    return;

  if (field.type() == QVariant::String && field.length() > 0) {
    m_edit->setMaxLength(field.length());
    QString txt(tr("Max allowed length") + " ");
    txt.append(QString::number(field.length()));
    m_edit->setPlaceholderText(txt);
  }

  if (field.requiredStatus() == QSqlField::Required) {
    setRequired((objectName().contains("mail_0")));
    m_edit->setClearButtonEnabled(false);
  }
}

const QVariant EMailEdit::value() {
  QString email = m_edit->text();
  return (isValid()) ? email : QString();
}

bool EMailEdit::isValid() {
  QString email = m_edit->text().trimmed();
  // Erforderlich und Leer
  if (isRequired() && email.isEmpty())
    return false;

  // Nicht Erforderlich und Leer
  if (email.isEmpty())
    return true;

  // Nicht leer, dann test mit Regulären ausdruck!
  return validate(email);
}

void EMailEdit::setInfo(const QString &info) {
  m_edit->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString EMailEdit::info() { return m_edit->toolTip(); }

const QString EMailEdit::notes() {
  QString txt = m_edit->toolTip();
  return tr("a valid input for '%1' is required.").arg(txt);
}
