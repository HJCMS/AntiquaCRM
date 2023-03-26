// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "emailedit.h"

#include <AntiquaCRM>
#include <QFontMetrics>
#include <QSize>

namespace AntiquaCRM {

EMailEdit::EMailEdit(const QString &name, QWidget *parent)
    : AntiquaCRM::AbstractInput{parent} {
  setObjectName(name);
  m_edit = new AntiquaCRM::ALineEdit(this);
  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(textChanged(const QString &)),
          SLOT(valueChanged(const QString &)));
}

EMailEdit::EMailEdit(QWidget *parent) : EMailEdit{"email_edit", parent} {}

bool EMailEdit::validate(const QString &mail) const {
  bool check = AntiquaCRM::AUtil::checkMail(mail);
  m_edit->isValidContent(check);
  return check;
}

void EMailEdit::initData() {
  QSqlField _f;
  _f.setMetaType(QMetaType(QMetaType::QString));
  _f.setRequiredStatus(QSqlField::Required);
  _f.setLength(80);
  if (!objectName().isEmpty())
    _f.setName(objectName());

  setRestrictions(_f);
  setWindowModified(false);
}

void EMailEdit::valueChanged(const QString &email) {
  validate(email);
  setWindowModified(true);
}

void EMailEdit::setValue(const QVariant &value) {
  if (value.metaType().id() != QMetaType::QString)
    return;

  QString _email = value.toString();
  validate(_email);
  m_edit->setText(_email);
}

void EMailEdit::setFocus() { m_edit->setFocus(); }

void EMailEdit::reset() {
  m_edit->clear();
  setWindowModified(false);
}

void EMailEdit::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required) {
    setRequired(true);
    m_edit->setClearButtonEnabled(false);
  }
  // Secundary eMail ...
  if (!objectName().contains("mail_0")) {
    setRequired(false);
    m_edit->setClearButtonEnabled(true);
  }

  QMetaType _type = field.metaType();
  if (_type.id() == QMetaType::QString && field.length() > 0) {
    m_edit->setMaxLength(field.length());
    QString _txt(tr("Max allowed length") + " ");
    _txt.append(QString::number(field.length()));
    _txt.append(".");
    m_edit->setPlaceholderText(_txt);
  }

  if (field.length() < 1)
    return;

  QString _mail;
  _mail.append(_mail.leftJustified(field.length(), QChar('@')));

  QFontMetrics _m(m_edit->font());
  qsizetype _w = _m.size(Qt::TextSingleLine, _mail).width();
  m_edit->setMaximumWidth(((_w < 80) ? _w : (_w / 2)));
}

void EMailEdit::setInputToolTip(const QString &tip) { m_edit->setToolTip(tip); }

void EMailEdit::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool EMailEdit::isValid() {
  if (isRequired() && m_edit->text().isEmpty())
    return false;

  return validate(m_edit->text());
}

const QVariant EMailEdit::getValue() { return m_edit->text(); }

const QString EMailEdit::popUpHints() {
  return tr("Please enter a valid EMail.");
}

const QString EMailEdit::statusHints() { return tr("Missing EMail ..."); }

} // namespace AntiquaCRM
