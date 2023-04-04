// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "textline.h"

#include <QCompleter>
#include <QDebug>

namespace AntiquaCRM {

TextLine::TextLine(QWidget *parent) : AntiquaCRM::AbstractInput{parent} {
  m_edit = new AntiquaCRM::ALineEdit(this);
  layout->addWidget(m_edit);
  initData();
  // connect(m_edit, SIGNAL(), SLOT(valueChanged()));
}

void TextLine::valueChanged() {
  if (isValid())
    setWindowModified(true);
}

void TextLine::initData() {
  QSqlField _f;
  _f.setMetaType(QMetaType(QMetaType::QString));
  _f.setRequiredStatus(QSqlField::Required);
  setRestrictions(_f);
  setWindowModified(false);
}

void TextLine::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  switch (_type.id()) {
  case (QMetaType::QString):
    m_edit->setText(value.toString().trimmed());
    break;

  default:
    qWarning("Invalid given Data Type in TextLine.");
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "TextLine Requires type int but get:" << value;
#endif
    break;
  };
}

void TextLine::setFocus() { m_edit->setFocus(); }

void TextLine::reset() {
  m_edit->clear();
  setWindowModified(false);
}

void TextLine::setCompleterList(const QStringList &list) {
  QCompleter *_completer = new QCompleter(list, this);
  m_edit->setCompleter(_completer);
}

void TextLine::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required) {
    m_edit->setClearButtonEnabled(false);
    setRequired(true);
  } else {
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

  QString _default = field.defaultValue().toString();
  if (!_default.isEmpty() && m_edit->text().isEmpty())
    m_edit->setText(_default);
}

void TextLine::setInputToolTip(const QString &tip) { m_edit->setToolTip(tip); }

void TextLine::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool TextLine::isValid() {
  if (isRequired() && m_edit->text().isEmpty())
    return false;

  return true;
}

const QVariant TextLine::getValue() { return m_edit->text().trimmed(); }

const QString TextLine::popUpHints() {
  return tr("This text line is required!");
}

const QString TextLine::statusHints() { return popUpHints(); }

} // namespace AntiquaCRM
