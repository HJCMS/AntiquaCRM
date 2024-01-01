// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "textline.h"

#include <QCompleter>
#include <QDebug>

namespace AntiquaCRM {

TextLine::TextLine(QWidget *parent) : AntiquaCRM::AInputWidget{parent} {
  m_edit = new AntiquaCRM::ALineEdit(this);
  m_edit->setValidation(ALineEdit::InputValidator::DEFAULT);
  layout->addWidget(m_edit);
  initData();

  connect(m_edit, SIGNAL(textChanged(const QString &)),
          SLOT(valueChanged(const QString &)));
  connect(m_edit, SIGNAL(returnPressed()), SIGNAL(signalEnterPressed()));
}

void TextLine::valueChanged(const QString &) {
  // if(m_edit->validator() != nullptr) {}
  setWindowModified(true);
  emit sendInputChanged();
}

void TextLine::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Optional);
  setRestrictions(_f);
  setWindowModified(false);
}

void TextLine::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  QString _data;
  switch (_type.id()) {
  case (QMetaType::QString):
    _data = value.toString().trimmed();
    break;

  case (QMetaType::QByteArray): // Password
    _data = QByteArray::fromBase64(value.toByteArray(),
                                   QByteArray::Base64UrlEncoding);
    break;

  default:
    qWarning("Invalid given Data Type in TextLine.");
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "TextLine Requires type int but get:" << value;
#endif
    break;
  };

  m_edit->setText(_data);
}

void TextLine::setFocus() {
  m_edit->setFocus();
  if (!isValid())
    m_edit->setVisualFeedback(2500);
}

void TextLine::reset() {
  m_edit->clear();
  setWindowModified(false);
}

void TextLine::setReadOnly(bool b) {
  if (b) {
    m_edit->setReadOnly(true);
    m_edit->setClearButtonEnabled(false);
    m_edit->setFrame(false);
    m_edit->setStyleSheet("QLineEdit {background:transparent;border:none;}");
  } else {
    m_edit->setReadOnly(false);
    m_edit->setFrame(true);
    m_edit->setStyleSheet(QString());
  }
}

void TextLine::setCompleterList(const QStringList &list) {
  QCompleter *_completer = new QCompleter(list, this);
  _completer->setCompletionMode(QCompleter::PopupCompletion);
  _completer->setFilterMode(Qt::MatchStartsWith);
  _completer->setMaxVisibleItems(15);
  m_edit->setCompleter(_completer);
}

void TextLine::setPasswordInput(bool b) {
  if (b) {
    m_edit->setEchoMode(QLineEdit::PasswordEchoOnEdit);
  } else {
    m_edit->setEchoMode(QLineEdit::Normal);
  }
}

void TextLine::setRestrictions(const QSqlField &field) {
  m_edit->setClearButtonEnabled(false);
  if (field.requiredStatus() == QSqlField::Required) {
    setRequired(true);
  } else if (!m_edit->isReadOnly()) {
    m_edit->setClearButtonEnabled(true);
  }

  QMetaType _type = field.metaType();
  if (_type.id() == QMetaType::QString && field.length() > 0) {
    m_edit->setMaxLength(field.length());
    QString _txt(tr("Max allowed length") + " ");
    _txt.append(QString::number(field.length()));
    _txt.append(".");
    m_edit->setPlaceholderText(_txt);
    m_edit->setEchoMode(QLineEdit::Normal);
  } else if (_type.id() == QMetaType::QByteArray && field.length() > 0) {
    m_edit->setMaxLength(field.length());
  }

  QString _default = field.defaultValue().toString();
  if (!_default.isEmpty() && m_edit->text().isEmpty())
    m_edit->setText(_default);
}

void TextLine::setInputToolTip(const QString &tip) {
  m_edit->setToolTip(tip);
  if (m_edit->placeholderText().isEmpty())
    m_edit->setPlaceholderText(tip);
}

void TextLine::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
  layout->setStretch(1, 1);
}

bool TextLine::isValid() {
  if (isRequired() && (m_edit->text().length() < 3))
    return false;

  return true;
}

const QMetaType TextLine::getType() const {
  if (m_edit->echoMode() == QLineEdit::PasswordEchoOnEdit)
    return QMetaType(QMetaType::QByteArray);

  return QMetaType(QMetaType::QString);
}

const QVariant TextLine::getValue() {
  QString _data = m_edit->text().trimmed();
  if (getType().id() == QMetaType::QByteArray) {
    QByteArray _pw = _data.toLocal8Bit().toBase64(QByteArray::Base64Encoding);
    return _pw;
  }
  return _data;
}

const QString TextLine::popUpHints() {
  return tr("This text line is required!");
}

const QString TextLine::statusHints() { return popUpHints(); }

} // namespace AntiquaCRM
