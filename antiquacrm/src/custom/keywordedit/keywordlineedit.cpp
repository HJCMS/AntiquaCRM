// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordlineedit.h"
#include "myicontheme.h"

#include <QtCore>
#include <QtWidgets>
#include <SqlCore>

KeywordLineEdit::KeywordLineEdit(QWidget *parent) : UtilsMain{parent} {
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  p_keywords = QString();

  m_keywordList = new QLabel(this);
  layout->addWidget(m_keywordList); // 0

  QToolButton *ac_clear = new QToolButton(this);
  ac_clear->setIcon(myIcon("clear_left"));
  ac_clear->setToolTip(tr("This button reset the Keyword field."));
  layout->addWidget(ac_clear); // 1

  m_lineEdit = new QLineEdit(this);
  m_lineEdit->setPlaceholderText(tr("Search, add to"));
  m_lineEdit->setToolTip(tr("To add keywords, paste them here and press Enter."));
  m_lineEdit->setDragEnabled(false);
  m_lineEdit->setClearButtonEnabled(true);
  layout->addWidget(m_lineEdit); // 2

  m_validator = new QRegExpValidator(validatePattern, m_lineEdit);
  m_lineEdit->setValidator(m_validator);

  setRequired(true);
  setModified(false);
  layout->setStretch(0, 70);
  layout->setStretch(2, 25);
  setLayout(layout);

  connect(m_lineEdit, SIGNAL(editingFinished()), this, SLOT(finalize()));
  connect(m_lineEdit, SIGNAL(returnPressed()), this, SLOT(finalize()));
  connect(ac_clear, SIGNAL(clicked()), this, SLOT(clearKeywords()));
}

void KeywordLineEdit::clearKeywords() {
  p_keywords.clear();
  m_keywordList->clear();
}

void KeywordLineEdit::finalize() {
  if (m_lineEdit->text().length() < minLength)
    return;

  QString buffer = m_lineEdit->text().trimmed();
  buffer.replace(stripPattern, "");

  QStringList list;
  if (!p_keywords.isEmpty())
    list = p_keywords.split(p_delimiter);

  list << buffer;
  list.sort();
  list.removeDuplicates();
  p_keywords = list.join(p_delimiter);
  // qDebug() << Q_FUNC_INFO << p_keywords;
  m_keywordList->setText(p_keywords);
  setModified(true);
  list.clear();
}

void KeywordLineEdit::setValue(const QVariant &str) {
  QString keywords = str.toString().trimmed();
  keywords.replace(" ", "");
  if (keywords.isEmpty())
    return;

  setModified(true);
  p_keywords = keywords;
  m_keywordList->setText(keywords);
}

void KeywordLineEdit::loadKeywords() {
  QStringList list;
  HJCMS::SqlCore *m_sql = new HJCMS::SqlCore(this);
  QString sql("SELECT ci_name FROM categories_intern;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    while (q.next()) {
      list << q.value("ci_name").toString().trimmed();
    }
  } else {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
    return;
  }
  m_completer = new QCompleter(list, m_lineEdit);
  m_completer->setCompletionMode(QCompleter::PopupCompletion);
  m_completer->setCaseSensitivity(Qt::CaseInsensitive);
  m_completer->setFilterMode(Qt::MatchContains);
  m_lineEdit->setCompleter(m_completer);
}

const QVariant KeywordLineEdit::value() {
  QString text = m_keywordList->text().trimmed();
  return text;
}

bool KeywordLineEdit::isValid() {
  if (isRequired() && m_keywordList->text().length() < 1)
    return false;

  return true;
}

void KeywordLineEdit::reset() {
  m_keywordList->clear();
  m_lineEdit->clear();
  setModified(false);
}

void KeywordLineEdit::setFocus() { m_lineEdit->setFocus(); }

void KeywordLineEdit::setInfo(const QString &info) { setToolTip(info); }

const QString KeywordLineEdit::info() { return toolTip(); }

const QString KeywordLineEdit::notes() {
  return tr("Keywords: Requires minimum one Keyword!");
}
