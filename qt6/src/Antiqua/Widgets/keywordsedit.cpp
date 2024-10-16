// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordsedit.h"
#include "antiquaicon.h"
#include "keywordlistview_p.h"

#include <AntiquaCRM>
#include <QCompleter>
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QToolButton>
#include <QToolTip>

namespace AntiquaCRM {

KeywordsEdit::KeywordsEdit(QWidget *parent) : AntiquaCRM::AInputWidget{parent} {
  // ColumnList
  m_keywords = new KeywordListView(this);
  m_keywords->setFocusPolicy(Qt::NoFocus);
  layout->addWidget(m_keywords);
  layout->addStretch();

  // Buttons
  QToolButton *ac_clear = new QToolButton(this);
  ac_clear->setIcon(AntiquaCRM::antiquaIcon("action-remove"));
  ac_clear->setToolTip(tr("This button reset the Keyword field."));
  ac_clear->setFocusPolicy(Qt::NoFocus);
  layout->addWidget(ac_clear);
  QToolButton *ac_add = new QToolButton(this);
  ac_add->setIcon(AntiquaCRM::antiquaIcon("action-add"));
  ac_add->setToolTip(tr("Keyword add"));
  ac_add->setFocusPolicy(Qt::NoFocus);
  layout->addWidget(ac_add);

  // LineEdit
  m_edit = new AntiquaCRM::ALineEdit(this);
  m_edit->setPlaceholderText(tr("Search, add to"));
  m_edit->setToolTip(tr("To add keywords, paste them here and press Enter."));
  m_edit->setDragEnabled(false);
  m_edit->setClearButtonEnabled(true);
  m_edit->setMinimumWidth(150);
  m_edit->setMaximumWidth(200);
  layout->addWidget(m_edit);

  QRegularExpressionValidator *m_validator = new QRegularExpressionValidator(
      AntiquaCRM::AUtil::keywordRegExp(), m_edit);
  m_edit->setValidator(m_validator);

  initData();

  connect(ac_clear, SIGNAL(clicked()), SLOT(clearKeywords()));
  connect(ac_add, SIGNAL(clicked()), SLOT(insertKeyword()));
  connect(m_edit, SIGNAL(returnPressed()), SLOT(insertKeyword()));
  connect(m_keywords, SIGNAL(keywordsChanged()), SLOT(valueChanged()));
}

void KeywordsEdit::initData() {
  QSqlField _f;
  _f.setMetaType(getType());
  _f.setRequiredStatus(QSqlField::Required);
  _f.setLength(m_keywords->maxLength());
  setRestrictions(_f);
  setWindowModified(false);
}

void KeywordsEdit::valueChanged() {
  setWindowModified(true);
  emit sendInputChanged();
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
  if (_keyword.length() <= m_edit->getMinLength()) {
    int _timeout = 1500;
    m_edit->setFocus();
    m_edit->setVisualFeedback(_timeout);
    QPoint _p = mapToGlobal(m_edit->pos());
    _p.setY(_p.y() - (m_edit->rect().height() * 2));
    QString _txt = tr("The Keyword length is to small!");
    QToolTip::showText(_p, _txt, m_edit, m_edit->rect(), _timeout);
    return;
  }
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
  m_keywords->clearKeywords();
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

void KeywordsEdit::setCompleterList(const QStringList &list) {
  if (list.isEmpty())
    return;

  QCompleter *_completer = new QCompleter(list, this);
  _completer->setCompletionMode(QCompleter::PopupCompletion);
  _completer->setFilterMode(Qt::MatchStartsWith);
  _completer->setCaseSensitivity(Qt::CaseInsensitive);
  _completer->setMaxVisibleItems(15);
  m_edit->setCompleter(_completer);
}

bool KeywordsEdit::isValid() {
  if (isRequired() && !m_keywords->isValid())
    return false;

  return true;
}

const QMetaType KeywordsEdit::getType() const {
  return QMetaType(QMetaType::QString);
}

const QVariant KeywordsEdit::getValue() { return m_keywords->getKeywords(); }

const QString KeywordsEdit::popUpHints() {
  QStringList txt(statusHints() + " ");
  txt << tr("And Restricted to a maximum %1 Characters length.")
             .arg(m_keywords->maxLength());
  return txt.join("\n");
}

const QString KeywordsEdit::statusHints() {
  return tr("Minimum one Keyword is required for this field!");
}

} // namespace AntiquaCRM
