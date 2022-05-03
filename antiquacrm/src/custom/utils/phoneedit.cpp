// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "phoneedit.h"

#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QHBoxLayout>

PhoneEdit::PhoneEdit(QWidget *parent) : UtilsMain{parent} {
  if (objectName().isEmpty())
    setObjectName("PhoneEdit");

  setModified(false);
  setRequired(false);

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_label = new QLabel(this);
  m_label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  layout->addWidget(m_label);

  id_phone = new QLineEdit(this);
  id_phone->setMaxLength(30);
  id_phone->setMaximumWidth(300);
  layout->addWidget(id_phone);

  QRegExp reg("([\\d\\s]+)");
  m_validator = new QRegExpValidator(reg, id_phone);
  id_phone->setValidator(m_validator);

  setLayout(layout);

  connect(id_phone, SIGNAL(textChanged(const QString &)), this,
          SLOT(inputChanged(const QString &)));

  connect(id_phone, SIGNAL(returnPressed()), this, SLOT(skipReturnPressed()));
}

void PhoneEdit::setValue(const QVariant &id) {
  QString tel = id.toString();
  if (tel.length() > 10) {
    tel.insert(3, " ");
    if (objectName().contains("c_mobil_")) {
      tel.insert(7, " ");
    } else {
      tel.insert(8, " ");
    }
  }
  id_phone->setText(tel);
}

void PhoneEdit::inputChanged(const QString &str) {
  QRegularExpressionMatch match = regexp.match(str.trimmed());
  if (match.hasMatch()) {
    id_phone->setStyleSheet("");
  } else {
    id_phone->setStyleSheet("color: red;");
  }
  setModified(true);
}

void PhoneEdit::reset() {
  id_phone->clear();
  setModified(false);
}

void PhoneEdit::setFocus() { id_phone->setFocus(); }

const QVariant PhoneEdit::value() {
  QString data = id_phone->text().trimmed();
  QRegExp reg("\\D+");
  return data.replace(reg, "");
}

bool PhoneEdit::isValid() {
  QString check(id_phone->text());
  if (isRequired() && check.isEmpty())
    return false;

  if (check.length() < 10)
    return false;

  return true;
}

void PhoneEdit::setInfo(const QString &txt) {
  QString info(txt);
  info.append(":");
  m_label->setText(info);
  m_label->setToolTip(txt);
  if (objectName().contains("c_mobil_")) {
    id_phone->setPlaceholderText("049 152 12345678");
    id_phone->setToolTip("049 152 12345678");
  } else {
    id_phone->setPlaceholderText("049 7632 1234567");
    id_phone->setToolTip("049 7632 1234567");
  }
}

const QString PhoneEdit::info() { return m_label->text(); }

const QString PhoneEdit::notes() {
  return tr("The Phone Number is required and can not empty.");
}
