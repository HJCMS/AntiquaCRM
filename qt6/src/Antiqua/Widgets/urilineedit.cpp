// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "urilineedit.h"
#include "antiquaicon.h"

#include <AntiquaCRM>
#include <QDesktopServices>
#include <QSize>

namespace AntiquaCRM {

URILineEdit::URILineEdit(const QString &name, QWidget *parent)
    : AntiquaCRM::AInputWidget{parent} {
  setObjectName(name);
  m_edit = new AntiquaCRM::ALineEdit(this);

  if (objectName().contains("email")) {
    ac_icon = m_edit->addAction(AntiquaCRM::antiquaIcon("edit-paste"),
                                QLineEdit::TrailingPosition);
    ac_icon->setObjectName("copy_action");
    ac_icon->setToolTip(tr("Copy eMail into system clipboard."));
    ac_icon->setVisible(false);
    connect(ac_icon, SIGNAL(triggered()), m_edit, SLOT(copyIntoClipboard()));
  } else {
    ac_icon = m_edit->addAction(AntiquaCRM::antiquaIcon("internet-web-browser"),
                                QLineEdit::TrailingPosition);
    ac_icon->setObjectName("weblink_action");
    ac_icon->setToolTip(tr("Open Weblink in external Webbrowser"));
    ac_icon->setVisible(false);
    connect(ac_icon, SIGNAL(triggered()), SLOT(openWeblink()));
  }

  layout->addWidget(m_edit);
  initData();

  connect(m_edit, SIGNAL(textChanged(const QString &)),
          SLOT(valueChanged(const QString &)));
}

URILineEdit::URILineEdit(QWidget *parent)
    : URILineEdit{"uri_line_edit", parent} {}

bool URILineEdit::validate(const QString &uri) const {
  if (uri.length() < 4)
    return false;

  const QString _uri = uri.toLower();
  if (_uri.startsWith("http")) {
    return AntiquaCRM::AUtil::checkUrl(QUrl(_uri));
  } else if (uri.contains("@")) {
    return AntiquaCRM::AUtil::checkMail(_uri);
  }
  return false;
}

void URILineEdit::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Required);
  _f.setLength(80);
  setRestrictions(_f);
  setWindowModified(false);
}

void URILineEdit::valueChanged(const QString &uri) {
  bool _b = validate(uri);
  m_edit->setValidContent(_b);
  ac_icon->setVisible(_b);

  if (isRequired())
    m_edit->setIconWarning(!_b);

  setWindowModified(true);
  emit sendInputChanged();
}

void URILineEdit::openWeblink() {
  const QString _l = m_edit->text().trimmed();
  if (_l.startsWith("http") && validate(_l))
    QDesktopServices::openUrl(QUrl(_l));
}

void URILineEdit::setValue(const QVariant &value) {
  if (value.metaType().id() != QMetaType::QString)
    return;

  QString _email = value.toString().trimmed();
  m_edit->setText(_email);
  setWindowModified(false);
}

void URILineEdit::setFocus() { m_edit->setFocus(); }

void URILineEdit::reset() {
  m_edit->clear();
  setWindowModified(false);
}

void URILineEdit::setRestrictions(const QSqlField &field) {
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

  if (objectName().contains("_email_")) {
    m_edit->setPlaceholderText(tr("a.name@domain.tld"));
  } else if (objectName().contains("website")) {
    m_edit->setPlaceholderText(tr("https://domain.tld"));
  }
}

void URILineEdit::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
}

void URILineEdit::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
  layout->setStretch(1, 1);
}

bool URILineEdit::isValid() {
  if (isRequired() && m_edit->text().isEmpty())
    return false;

  return validate(m_edit->text());
}

const QMetaType URILineEdit::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant URILineEdit::getValue() { return m_edit->text(); }

const QString URILineEdit::popUpHints() {
  return tr("Please enter a valid URI.");
}

const QString URILineEdit::statusHints() { return tr("Missing URI ..."); }

} // namespace AntiquaCRM
