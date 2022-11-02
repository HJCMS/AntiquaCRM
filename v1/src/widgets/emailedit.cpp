// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "emailedit.h"

#include <QDebug>
#include <QRegularExpressionMatch>

EMailEdit::EMailEdit(QWidget *parent) : InputEdit{parent} {
  m_edit = new AntiquaLineEdit(this);
  m_edit->setToolTip(tr("eMail edit"));
  m_edit->setPlaceholderText(tr("usage.example@example.com"));
  m_layout->addWidget(m_edit);

  m_validator = new QRegExpValidator(rePattern(), m_edit);
  m_edit->setValidator(m_validator);

  setRequired(true);
  setModified(false);

  connect(m_edit, SIGNAL(textChanged(const QString &)),
          SLOT(dataChanged(const QString &)));
}

void EMailEdit::dataChanged(const QString &) { setModified(true); }

void EMailEdit::reset() {
  m_edit->clear();
  setModified(false);
}

void EMailEdit::setValue(const QVariant &val) {
  QString email = val.toString().trimmed();
  m_edit->setText(email);
}

void EMailEdit::setFocus() { m_edit->setFocus(); }

const QRegExp EMailEdit::rePattern() {
  QRegExp reg;
  reg.setCaseSensitivity(Qt::CaseInsensitive);
  reg.setPattern("^([\\d\\w\\-\\.]{3,})@([\\d\\w\\-\\.]{3,})\\.([a-z]{2,6})$");
  return reg;
}

void EMailEdit::setProperties(const QSqlField &field) {
  if (!field.isValid())
    return;

  if (field.type() == QVariant::String && field.length() > 0) {
    m_edit->setMaxLength(field.length());
    // m_edit->setMaximumWidth(300);

    QString txt(tr("Max allowed length") + " ");
    txt.append(QString::number(field.length()));
    m_edit->setPlaceholderText(txt);
  }

  if (field.requiredStatus() == QSqlField::Required) {
    setRequired(true);
    m_edit->setClearButtonEnabled(false);
  }
}

const QVariant EMailEdit::value() {
  QString email = m_edit->text();
  return (isValid()) ? email : QString();
}

bool EMailEdit::isValid() {
  if (isRequired() && m_edit->text().isEmpty())
    return false;

  QString email = m_edit->text();
  QRegularExpression r(rePattern().pattern());
  QRegularExpressionMatch m = r.match(email.trimmed());
  return m.hasMatch();
}

void EMailEdit::setInfo(const QString &info) {
  m_edit->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString EMailEdit::info() { return m_edit->toolTip(); }

const QString EMailEdit::notes() { return tr("eMail input is required."); }
