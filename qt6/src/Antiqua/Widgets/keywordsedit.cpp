// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordsedit.h"

#include <AntiquaCRM>
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpression>
#include <QToolButton>

namespace AntiquaCRM {

KeywordsEdit::KeywordsEdit(QWidget *parent)
    : AntiquaCRM::AbstractInput{parent} {
  // ColumnList
  m_keysList = new QWidget(this);
  m_keysList->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(m_keysList);
  layout->addStretch();

  // Buttons
  QToolButton *ac_clear = new QToolButton(this);
  ac_clear->setIcon(AntiquaApplIcon("action-remove"));
  ac_clear->setToolTip(tr("This button reset the Keyword field."));
  layout->addWidget(ac_clear);
  QToolButton *ac_add = new QToolButton(this);
  ac_add->setIcon(AntiquaApplIcon("action-add"));
  ac_add->setToolTip(tr("Keyword add"));
  layout->addWidget(ac_add);

  // LineEdit
  m_lineEdit = new AntiquaCRM::ALineEdit(this);
  m_lineEdit->setPlaceholderText(tr("Search, add to"));
  m_lineEdit->setToolTip(
      tr("To add keywords, paste them here and press Enter."));
  m_lineEdit->setDragEnabled(false);
  m_lineEdit->setClearButtonEnabled(true);
  m_lineEdit->setMaximumWidth(200);
  layout->addWidget(m_lineEdit);

  initData();
  // connect(m_edit, SIGNAL(), SLOT(valueChanged()));
}

void KeywordsEdit::valueChanged() {
  if (isValid())
    setWindowModified(true);
}

void KeywordsEdit::clearKeywords() {
  QMessageBox::StandardButton ret = QMessageBox::warning(
      this, tr("Warning"), tr("You really want to delete all Keywords!"),
      (QMessageBox::Ok | QMessageBox::Cancel));
  if (ret == QMessageBox::Cancel)
    return;

  // m_keysList->clear();
  setWindowModified(true);
}

void KeywordsEdit::finalize() {
  if (m_lineEdit->text().length() < minLength)
    return;

  static const QRegularExpression strip("([^\\w\\d\\-]+)");
  QString buffer = m_lineEdit->text().trimmed();
  buffer.replace(strip, "");
  qDebug() << Q_FUNC_INFO << "TODO::addKeyword" << buffer;
  // m_keysList->addKeyword(buffer);

  setWindowModified(true);
}

void KeywordsEdit::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  if (_type.id() != QMetaType::QString) {
    qWarning("Invalid given Data Type in KeywordsEdit.");
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "KeywordsEdit Requires type int but get:" << value;
#endif
    return;
  }
  qDebug() << Q_FUNC_INFO << "TODO" << value;
}

void KeywordsEdit::setFocus() { m_lineEdit->setFocus(); }

void KeywordsEdit::reset() {
  m_lineEdit->clear();
  setWindowModified(false);
}

void KeywordsEdit::initData() {
  QSqlField _f;
  _f.setMetaType(QMetaType(QMetaType::QString));
  _f.setRequiredStatus(QSqlField::Required);
  setRestrictions(_f);

  setWindowModified(false);
}

void KeywordsEdit::setRestrictions(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void KeywordsEdit::setInputToolTip(const QString &tip) {
  m_lineEdit->setToolTip(tip);
}

void KeywordsEdit::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_lineEdit);
}

bool KeywordsEdit::isValid() {
  if (isRequired() && getValue().isNull())
    return false;

  return true;
}

const QVariant KeywordsEdit::getValue() {
  qDebug() << Q_FUNC_INFO << "TODO";
  return QVariant(); // m_edit->value();
}

const QString KeywordsEdit::popUpHints() {
  QStringList txt(statusHints());
  txt << tr("And Restricted to a maximum %1 Characters length.").arg(60);
  return txt.join("\n");
}

const QString KeywordsEdit::statusHints() {
  return tr("It requires minimum one Keyword!");
}

} // namespace AntiquaCRM
