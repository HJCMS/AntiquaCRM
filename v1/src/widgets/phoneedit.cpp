// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "phoneedit.h"

#include <QDebug>

PhoneEdit::PhoneEdit(QWidget *parent) : InputEdit{parent} {
  m_edit = new AntiquaLineEdit(this);
  m_edit->setToolTip(tr("phone edit"));
  m_layout->addWidget(m_edit);

  m_validator = new QRegExpValidator(rePattern(), m_edit);
  m_edit->setValidator(m_validator);

  setRequired(true);
  setModified(false);

  connect(m_edit, SIGNAL(textChanged(const QString &)),
          SLOT(dataChanged(const QString &)));
}

void PhoneEdit::dataChanged(const QString &) { setModified(true); }

void PhoneEdit::reset() {
  m_edit->clear();
  setModified(false);
}

void PhoneEdit::setValue(const QVariant &val) {
  QString phone = val.toString().trimmed();
  m_edit->setText(phone);
}

void PhoneEdit::setFocus() { m_edit->setFocus(); }

const QRegExp PhoneEdit::rePattern() {
  QRegExp reg;
  reg.setPattern("^(0[\\d]{2}\\s*[\\d]{2,4}[\\s*\\d]+)$");
  return reg;
}

void PhoneEdit::setProperties(const QSqlField &field) {
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

const QVariant PhoneEdit::value() {
  QString phone = m_edit->text();
  if (isValid())
    return phone.replace(QRegExp("\\D+"), "");
  else
    return QString();
}

bool PhoneEdit::isValid() {
  if (isRequired() && m_edit->text().isEmpty())
    return false;

  QString phone = m_edit->text();
  QRegularExpression r(rePattern().pattern());
  QRegularExpressionMatch m = r.match(phone.trimmed());
  return m.hasMatch();
}

void PhoneEdit::setInfo(const QString &info) {
  m_edit->setToolTip(info);
  if (info.length() > 2) {
    m_label->setVisible(true);
    m_label->setText(info + ":");
  }
}

const QString PhoneEdit::info() { return m_edit->toolTip(); }

const QString PhoneEdit::notes() { return tr("phone input is required."); }
