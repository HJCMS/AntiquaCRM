// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordlineedit.h"
#include "keywordlabellist.h"

#include <AntiquaCRM>
#include <QtCore>
#include <QtWidgets>

KeywordLineEdit::KeywordLineEdit(QWidget *parent) : InputEdit{parent} {
  m_keywordList = new KeywordLabelList(this);
  m_keywordList->setToolTip(tr("add 3 Keywords for a better search!"));
  m_layout->addWidget(m_keywordList);
  m_layout->addStretch(1);
  QToolButton *ac_clear = new QToolButton(this);
  ac_clear->setIcon(QIcon(":icons/action_remove.png"));
  ac_clear->setToolTip(tr("This button reset the Keyword field."));
  m_layout->addWidget(ac_clear);
  QToolButton *ac_add = new QToolButton(this);
  ac_add->setIcon(QIcon(":icons/action_add.png"));
  ac_add->setToolTip(tr("Keyword add"));
  m_layout->addWidget(ac_add);
  m_lineEdit = new QLineEdit(this);
  m_lineEdit->setPlaceholderText(tr("Search, add to"));
  m_lineEdit->setToolTip(
      tr("To add keywords, paste them here and press Enter."));
  m_lineEdit->setDragEnabled(false);
  m_lineEdit->setClearButtonEnabled(true);
  m_lineEdit->setMinimumWidth(200);
  m_layout->addWidget(m_lineEdit);
  m_validator = new QRegExpValidator(validatePattern, m_lineEdit);
  m_lineEdit->setValidator(m_validator);
  setRequired(true);
  setModified(false);
  connect(m_keywordList, SIGNAL(sendModified(bool)), this,
          SLOT(setModified(bool)));
  connect(m_lineEdit, SIGNAL(returnPressed()), this, SLOT(finalize()));
  connect(ac_clear, SIGNAL(clicked()), this, SLOT(clearKeywords()));
  connect(ac_add, SIGNAL(clicked()), this, SLOT(finalize()));
}

void KeywordLineEdit::loadDataset() {
  QStringList list;
  AntiquaCRM::ACompleterData keywords("keywords");
  list << keywords.completition("name");

  if (list.size() < 1) {
    qWarning("Keyword editor: fallback to SQL query!");
    AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
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
  }

  m_completer = new QCompleter(list, m_lineEdit);
  m_completer->setCompletionMode(QCompleter::PopupCompletion);
  m_completer->setFilterMode(Qt::MatchContains);
  m_completer->setCaseSensitivity(Qt::CaseInsensitive);
  m_lineEdit->setCompleter(m_completer);
}

void KeywordLineEdit::clearKeywords() {
  m_keywordList->clear();
  setModified(true);
}

void KeywordLineEdit::finalize() {
  if (m_lineEdit->text().length() < minLength)
    return;

  QString buffer = m_lineEdit->text().trimmed();
  buffer.replace(stripPattern, "");
  m_keywordList->addKeyword(buffer);

  setModified(true);
}

void KeywordLineEdit::setValue(const QVariant &str) {
  QString keywords = str.toString().trimmed();
  keywords.replace(" ", "");
  if (keywords.isEmpty())
    return;

  setModified(true);
  m_keywordList->addKeywords(keywords.split(p_delimiter));
}

void KeywordLineEdit::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

const QVariant KeywordLineEdit::value() {
  QStringList list = m_keywordList->keywords();
  if (list.count() < 1) {
    qWarning("Empty Keywords list!");
    return QString();
  }

  return list.join(p_delimiter);
}

bool KeywordLineEdit::isValid() {
  if (isRequired() && m_keywordList->keywords().count() < 1)
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
