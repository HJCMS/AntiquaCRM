// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "textfield.h"

#include <QDebug>
#include <QRegularExpression>

namespace AntiquaCRM {

TextField::TextField(QWidget *parent) : AntiquaCRM::AbstractInput{parent} {
  m_edit = new QTextEdit(this);
  m_edit->setAcceptDrops(false);
  m_edit->setAcceptRichText(false);
  m_edit->setTextInteractionFlags(Qt::TextEditorInteraction);
  m_edit->setAutoFormatting(QTextEdit::AutoNone);
  layout->addWidget(m_edit);
  connect(m_edit, SIGNAL(textChanged()), SLOT(valueChanged()));
  initData();
}

const QString TextField::stripString(const QString &str) const {
  QString buf = str.trimmed();
  buf.replace("'", "’");
  const QRegularExpression pattern("^\\s+");
  buf.replace(pattern, " ");
  return buf.trimmed();
}

void TextField::valueChanged() {}

void TextField::initData() {
  QSqlField _f;
  _f.setMetaType(QMetaType(QMetaType::QString));
  _f.setRequiredStatus(QSqlField::Optional);
  setRestrictions(_f);
  setWindowModified(false);
}

void TextField::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  if (_type.id() == QMetaType::QString) {
    m_edit->setPlainText(value.toString().trimmed());
    return;
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << "TextField Requires type int but get:" << value;
#endif
}

void TextField::setFocus() { m_edit->setFocus(); }

void TextField::reset() {
  m_edit->setPlainText(QString());
  m_edit->clear();
  setWindowModified(false);
}

void TextField::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void TextField::setInputToolTip(const QString &tip) { m_edit->setToolTip(tip); }

void TextField::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool TextField::isValid() {
  if (isRequired() && getValue().isNull())
    return false;

  return true;
}

const QVariant TextField::getValue() {
  return stripString(m_edit->toPlainText());
}

const QString TextField::popUpHints() {
  return tr("This Textfield requires a valid input.");
}

const QString TextField::statusHints() { return popUpHints(); }

} // namespace AntiquaCRM
