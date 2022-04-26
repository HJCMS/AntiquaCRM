// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "phoneedit.h"

#include <QtCore/QDebug>
#include <QtWidgets/QHBoxLayout>

PhoneEdit::PhoneEdit(QWidget *parent) : UtilsMain{parent} {
  if (objectName().isEmpty())
    setObjectName("PhoneEdit");

  setModified(false);
  setRequired(false);

  QHBoxLayout *layout = new QHBoxLayout(this);

  m_label = new QLabel(this);
  m_label->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
  layout->addWidget(m_label);

  id_phone = new QLineEdit(this);
  id_phone->setMaxLength(30);
  id_phone->setMaximumWidth(220);
  layout->addWidget(id_phone);

  m_validator = new QRegExpValidator(pcre(), id_phone);
  id_phone->setValidator(m_validator);

  setLayout(layout);

  connect(id_phone, SIGNAL(textChanged(const QString &)), this,
          SLOT(inputChanged(const QString &)));

  connect(id_phone, SIGNAL(returnPressed()), this, SLOT(skipReturnPressed()));
}

void PhoneEdit::setValue(const QVariant &id) {
  id_phone->setText(id.toString());
  setModified(true);
}

void PhoneEdit::inputChanged(const QString &str) {
  int pos = 0;
  QRegExp reg(pcre());
  if (reg.indexIn(str, pos) == -1) {
    setStyleSheet("color: red;");
  } else {
    setStyleSheet("");
  }
  setModified(true);
}

void PhoneEdit::reset() {
  id_phone->clear();
  setModified(false);
}

const QVariant PhoneEdit::value() {
  qDebug() << Q_FUNC_INFO << "Todo Parser";
  QVariant data = QVariant(id_phone->text());
  return data;
}

bool PhoneEdit::isValid() {
  if (isRequired() && id_phone->text().isEmpty())
    return false;

  if (QVariant(id_phone->text()).toULongLong() < 1)
    return false;

  return true;
}

void PhoneEdit::setInfo(const QString &txt) {
  QString info(txt);
  info.append(":");
  m_label->setText(info);
  m_label->setToolTip(txt);
  if (objectName().contains("c_mobil_")) {
    id_phone->setPlaceholderText("049 152 1234 1234 123");
    id_phone->setToolTip("049 152 1234 1234 123");
  } else {
    id_phone->setPlaceholderText("049 7632 4565 456");
    id_phone->setToolTip("049 7632 4565 456");
  }
}

const QString PhoneEdit::info() { return m_label->text(); }

const QString PhoneEdit::notes() {
  return tr("The Phone Number is required and can not empty.");
}
