// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "emailedit.h"
#include "antiquaicon.h"

#include <AntiquaCRM>
#include <QFontMetrics>
#include <QSize>

namespace AntiquaCRM {

EMailEdit::EMailEdit(const QString &name, QWidget *parent)
    : AntiquaCRM::AInputWidget{parent} {
  setObjectName(name);
  m_edit = new AntiquaCRM::ALineEdit(this);

  ac_copy = m_edit->addAction(AntiquaCRM::antiquaIcon("edit-copy"),
                              QLineEdit::TrailingPosition);
  ac_copy->setToolTip(tr("Copy eMail into system clipboard."));
  ac_copy->setVisible(false);

  layout->addWidget(m_edit);
  initData();
  connect(m_edit, SIGNAL(textChanged(const QString &)),
          SLOT(valueChanged(const QString &)));
  connect(ac_copy, SIGNAL(triggered()), m_edit, SLOT(copyIntoClipboard()));
}

EMailEdit::EMailEdit(QWidget *parent) : EMailEdit{"email_edit", parent} {}

bool EMailEdit::validate(const QString &mail) const {
  bool _b = (mail.length() > 3);
  if (_b)
    _b = AntiquaCRM::AUtil::checkMail(mail);

  return _b;
}

void EMailEdit::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Required);
  _f.setLength(80);
  setRestrictions(_f);
  setWindowModified(false);
}

void EMailEdit::valueChanged(const QString &email) {
  bool _b = validate(email);
  m_edit->setValidContent(_b);
  ac_copy->setVisible(_b);

  if (isRequired())
    m_edit->setIconWarning(!_b);

  setWindowModified(true);
  emit sendInputChanged();
}

void EMailEdit::setValue(const QVariant &value) {
  if (value.metaType().id() != QMetaType::QString)
    return;

  QString _email = value.toString().trimmed();
  m_edit->setText(_email);
  setWindowModified(false);
}

void EMailEdit::setFocus() { m_edit->setFocus(); }

void EMailEdit::reset() {
  m_edit->clear();
  setWindowModified(false);
}

void EMailEdit::setRestrictions(const QSqlField &field) {
  QMetaType _type = field.metaType();
  if (_type.id() == QMetaType::QString && field.length() > 0) {
    m_edit->setMaxLength(field.length());
  }

  if (field.requiredStatus() == QSqlField::Required) {
    setRequired(true);
    m_edit->setClearButtonEnabled(false);
  } else {
    setRequired(false);
    m_edit->setClearButtonEnabled(false);
  }

  m_edit->setPlaceholderText(tr("a.name@domain.tld"));
}

void EMailEdit::setInputToolTip(const QString &tip) { m_edit->setToolTip(tip); }

void EMailEdit::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
  layout->setStretch(1, 1);
}

bool EMailEdit::isValid() {
  if (isRequired() && m_edit->text().isEmpty())
    return false;

  return validate(m_edit->text());
}

const QMetaType EMailEdit::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant EMailEdit::getValue() { return m_edit->text(); }

const QString EMailEdit::popUpHints() {
  return tr("Please enter a valid EMail.");
}

const QString EMailEdit::statusHints() { return tr("Missing EMail ..."); }

} // namespace AntiquaCRM
