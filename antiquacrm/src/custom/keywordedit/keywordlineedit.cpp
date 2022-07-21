// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordlineedit.h"
#include <QtCore>
#include <QtWidgets>
#include <SqlCore>

KeywordLineEdit::KeywordLineEdit(QWidget *parent) : UtilsMain{parent} {
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  m_lineEdit = new QLineEdit(this);
  layout->addWidget(m_lineEdit);

  m_validator = new QRegExpValidator(pattern, m_lineEdit);
  m_lineEdit->setValidator(m_validator);

  setRequired(true);
  setModified(false);
  setLayout(layout);
}

void KeywordLineEdit::setValue(const QVariant &str) {
  QString keywords = str.toString().trimmed();
  keywords.replace(" ", "");
  if (keywords.isEmpty())
    return;

  setModified(true);
  m_lineEdit->setText(keywords);
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

  if (list.count() > 1) {
    QCompleter *cpl = new QCompleter(list, m_lineEdit);
    cpl->setCompletionMode(QCompleter::PopupCompletion);
    cpl->setCaseSensitivity(Qt::CaseInsensitive);
    cpl->setFilterMode(Qt::MatchContains);
    cpl->setWrapAround(false);
    m_lineEdit->setCompleter(cpl);
  }
}

const QVariant KeywordLineEdit::value() {
  QString text = m_lineEdit->text().trimmed();
  return text;
}

bool KeywordLineEdit::isValid() {
  if (isRequired() && m_lineEdit->text().length() < 1)
    return false;

  return true;
}

void KeywordLineEdit::reset() {
  m_lineEdit->clear();
  setModified(false);
}

void KeywordLineEdit::setFocus() { m_lineEdit->setFocus(); }

void KeywordLineEdit::setInfo(const QString &info) {
  m_lineEdit->setToolTip(info);
}

const QString KeywordLineEdit::info() { return m_lineEdit->toolTip(); }

const QString KeywordLineEdit::notes() {
  return tr("Keywords: Requires minimum one Keyword!");
}
