// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "emailedit.h"
#include "autil.h"

#include <QApplication>
#include <QPalette>
#include <QRegExp>
#include <QRegularExpressionMatch>
#include <QUrl>

EMailEdit::EMailEdit(QWidget *parent) : InputEdit{parent} {
  m_edit = new AntiquaLineEdit(this);
  m_edit->setToolTip(tr("eMail edit"));
  m_edit->setPlaceholderText(tr("usage.example@example.com"));
  m_layout->addWidget(m_edit);
  QRegExp pattern = AntiquaCRM::AUtil::emailRegExp();
  m_validator = new QRegExpValidator(pattern, m_edit);
  m_edit->setValidator(m_validator);
  setRequired(false);
  setModified(false);
  connect(m_edit, SIGNAL(textChanged(const QString &)),
          SLOT(dataChanged(const QString &)));
}

bool EMailEdit::validate(const QString &mail) const {
  bool _check = AntiquaCRM::AUtil::checkMail(mail);
  if (_check) {
    QStringList _l = mail.split("@");
    QUrl _url;
    _url.setScheme("mailto");
    _url.setUserInfo(_l.first());
    _url.setHost(_l.last());
    _check = _url.isValid();
  }
  // display errors
  if (_check) {
    m_edit->setStyleSheet(QString());
  } else {
    m_edit->setStyleSheet("QLineEdit {selection-background-color: red;}");
  }
  return _check;
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
  QString str = val.toString().trimmed().toLower();
  if (AntiquaCRM::AUtil::checkMail(str))
    m_edit->setText(str);
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
