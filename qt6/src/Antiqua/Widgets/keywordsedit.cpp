// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordsedit.h"
#include "keywordeditor/keywordlistview.h"

#include <AntiquaCRM>
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpression>
#include <QToolButton>

namespace AntiquaCRM {

KeywordsEdit::KeywordsEdit(QWidget *parent)
    : AntiquaCRM::AbstractInput{parent} {
  // ColumnList
  m_keywords = new KeywordListView(this);
  layout->addWidget(m_keywords);
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
  m_edit = new AntiquaCRM::ALineEdit(this);
  m_edit->setPlaceholderText(tr("Search, add to"));
  m_edit->setToolTip(tr("To add keywords, paste them here and press Enter."));
  m_edit->setDragEnabled(false);
  m_edit->setClearButtonEnabled(true);
  m_edit->setMinimumWidth(150);
  m_edit->setMaximumWidth(200);
  m_edit->setValidation(AntiquaCRM::ALineEdit::STRINGS);
  layout->addWidget(m_edit);

  initData();
  connect(ac_clear, SIGNAL(clicked()), SLOT(clearKeywords()));
  connect(ac_add, SIGNAL(clicked()), SLOT(insertKeyword()));
  connect(m_edit, SIGNAL(editingFinished()), SLOT(insertKeyword()));
  connect(m_keywords, SIGNAL(valueChanged()), SLOT(valueChanged()));
}

void KeywordsEdit::valueChanged() {
  QString _keyword = m_edit->text().trimmed();
  if (_keyword.length() < minLength)
    return;

  setWindowModified(true);
}

void KeywordsEdit::clearKeywords() {
  QMessageBox::StandardButton ret = QMessageBox::warning(
      this, tr("Warning"), tr("You really want to delete all Keywords!"),
      (QMessageBox::Ok | QMessageBox::Cancel));
  if (ret == QMessageBox::Cancel)
    return;

  m_keywords->clearKeywords();
  setWindowModified(true);
}

void KeywordsEdit::insertKeyword() {
  QString _keyword = m_edit->text().trimmed();
  if (_keyword.length() < minLength)
    return;

  static const QRegularExpression strip("([^\\w\\d\\-]+)");
  _keyword.replace(strip, "");
  m_keywords->insertKeyword(_keyword);
}

void KeywordsEdit::setValue(const QVariant &value) {
  QMetaType _type = value.metaType();
  if (_type.id() == QMetaType::QString) {
    m_keywords->setKeywordList(value.toString().split(","));
  } else if (_type.id() == QMetaType::QStringList) {
    m_keywords->setKeywordList(value.toStringList());
  } else {
    qWarning("Invalid given Data Type in KeywordsEdit.");
  }
}

void KeywordsEdit::setFocus() { m_edit->setFocus(); }

void KeywordsEdit::reset() {
  m_edit->clear();
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
  m_edit->setToolTip(tip);
}

void KeywordsEdit::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool KeywordsEdit::isValid() {
  if (isRequired() && m_keywords->length() < 1)
    return false;

  return true;
}

const QVariant KeywordsEdit::getValue() { return m_keywords->getKeywords(); }

const QString KeywordsEdit::popUpHints() {
  QStringList txt(statusHints());
  txt << tr("And Restricted to a maximum %1 Characters length.").arg(60);
  return txt.join("\n");
}

const QString KeywordsEdit::statusHints() {
  return tr("It requires minimum one Keyword!");
}

} // namespace AntiquaCRM
