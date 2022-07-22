// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordlineedit.h"

#include <QtCore>
#include <QtWidgets>
#include <SqlCore>

KeywordLineEdit::KeywordLineEdit(QWidget *parent) : UtilsMain{parent} {
  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);

  p_keywords = QString();
  m_completer = nullptr;

  m_lineEdit = new QLineEdit(this);
  m_lineEdit->setPlaceholderText(tr("Category keywords"));
  m_lineEdit->setToolTip(tr("In this area, enter the keywords separated by commas."));
  layout->addWidget(m_lineEdit);

  m_validator = new QRegExpValidator(validatePattern, m_lineEdit);
  m_lineEdit->setValidator(m_validator);

  setRequired(true);
  setModified(false);
  setLayout(layout);

  connect(m_lineEdit, SIGNAL(editingFinished()), this, SLOT(finalize()));
  connect(m_lineEdit, SIGNAL(returnPressed()), this, SLOT(finalize()));
  connect(m_lineEdit, SIGNAL(textChanged(QString)), this,
          SLOT(keywordChanged(QString)));
}

void KeywordLineEdit::finalize() {
  if (m_lineEdit->text().length() < minLength)
    return;

  if (p_keywords.length() < minLength)
    return;

  m_completer->blockSignals(true);
  QString buffer(p_keywords);
  buffer.append(p_delimiter);
  if(p_keywords != m_lineEdit->text())
    buffer.append(m_lineEdit->text());

  buffer = buffer.trimmed();
  if(buffer != p_keywords)
    p_keywords = buffer;

  setModified(true);
  m_lineEdit->setText(p_keywords);
  m_completer->blockSignals(false);
  buffer.clear();
}

void KeywordLineEdit::keywordChanged(const QString &str) {
  if (!str.contains(p_delimiter))
    return; // keine Aktion notwendig

  if (m_completer == nullptr) {
    qWarning("No Completer for KeywordLineEdit loaded!");
    return;
  }

  int to = str.lastIndexOf(p_delimiter);
  if (to < minLength)
    return; // Zeichenlänge zu niedrig

  QString buffer(str); // bereinigen
  buffer.replace(stripPattern, "");

  // puffer Aktualisieren
  p_keywords = buffer.left(to);
  // Neue Suchzeichenkette senden
  QString search = buffer.section(p_delimiter, -1, to).trimmed();
  if (search.length() > minLength) {
    m_completer->setCompletionPrefix(p_keywords);
    emit m_completer->highlighted(search);
  }
}

void KeywordLineEdit::setValue(const QVariant &str) {
  QString keywords = str.toString().trimmed();
  keywords.replace(" ", "");
  if (keywords.isEmpty())
    return;

  setModified(true);
  p_keywords = keywords;
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
  m_completer = new QCompleter(list, m_lineEdit);
  m_completer->setCompletionMode(QCompleter::PopupCompletion);
  m_completer->setCaseSensitivity(Qt::CaseInsensitive);
  m_completer->setFilterMode(Qt::MatchContains);
  m_lineEdit->setCompleter(m_completer);
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
  setToolTip(info);
}

const QString KeywordLineEdit::info() { return toolTip(); }

const QString KeywordLineEdit::notes() {
  return tr("Keywords: Requires minimum one Keyword!");
}
