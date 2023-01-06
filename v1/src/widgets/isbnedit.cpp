// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "isbnedit.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QMetaType>
#include <QPixmap>
#include <QRegularExpressionMatch>
#include <QStringList>

IsbnEdit::IsbnEdit(QWidget *parent) : InputEdit{parent} {
  setObjectName("IsbnEdit");
  m_isbn = new QLineEdit(this);
  m_isbn->setMaxLength(13);
  m_isbn->setMinimumWidth(200);
  m_isbn->setWhatsThis(tr("a ISBN number must consist of 10 or 13 digits."));
  m_isbn->setPlaceholderText(tr("ISBN/EAN 10/13 (ISO 2108)"));
  m_isbn->setClearButtonEnabled(true);
  m_layout->addWidget(m_isbn);

  m_status = new QLabel(this);
  m_layout->addWidget(m_status);

  m_validator = new QRegExpValidator(simplePattern, m_isbn);
  m_isbn->setValidator(m_validator);
  m_completer = new QCompleter(prefix, m_isbn);
  m_completer->setCompletionMode(QCompleter::PopupCompletion);
  m_completer->setFilterMode(Qt::MatchStartsWith);
  m_isbn->setCompleter(m_completer);
  m_layout->addStretch(1);
  connect(m_isbn, SIGNAL(textChanged(const QString &)),
          SLOT(isbnChanged(const QString &)));
}

bool IsbnEdit::isISBN10(const QString &isbn) const {
  QRegularExpressionMatch c = p10.match(isbn);
  return c.hasMatch();
}

bool IsbnEdit::isISBN13(const QString &isbn) const {
  QRegularExpressionMatch c = p13.match(isbn);
  return c.hasMatch();
}

void IsbnEdit::loadDataset() {}

void IsbnEdit::setValidationIcon(bool b) {
  QPixmap pixmap;
  int h = (m_status->rect().height() - 4);
  QSize size(h, h);
  if (b) {
    QIcon icon("://icons/action_ok.png");
    pixmap = icon.pixmap(size, QIcon::Disabled);
  } else {
    QIcon icon("://icons/warning.png");
    pixmap = icon.pixmap(size, QIcon::Disabled);
  }
  m_status->setPixmap(pixmap);
}

void IsbnEdit::isbnChanged(const QString &s) {
  bool valid = false;
  int len = s.trimmed().length();
  if (len == 10 && isISBN10(s)) {
    valid = true;
  } else if (len == 13 && isISBN13(s)) {
    valid = true;
  }
  setValidationIcon(valid);
  emit isbnIsValid(valid);
  setModified(true);
}

void IsbnEdit::setValue(const QVariant &val) {
  QString s;
  switch (val.type()) {
  case QVariant::ULongLong:
  case QVariant::LongLong:
    s = QString::number(val.toLongLong());
    break;

  default:
    s = val.toString().trimmed();
  }

  int len = s.length();
  if (len < 10 || len > 13)
    return; // ignore

  if (len == 10 && isISBN10(s)) {
    m_isbn->setText(s);
  } else if (len == 13 && isISBN13(s)) {
    m_isbn->setText(s);
  }
}

void IsbnEdit::reset() {
  m_isbn->clear();
  setModified(false);
}

void IsbnEdit::setFocus() { m_isbn->setFocus(); }

bool IsbnEdit::isValid() {
  QString isbn = m_isbn->text().trimmed();
  int len = isbn.length();
  if (len == 10 && isISBN10(isbn))
    return true;

  if (len == 13 && isISBN13(isbn))
    return true;

  return false;
}

qint64 IsbnEdit::number() {
  QString txt = m_isbn->text().trimmed();
  bool b = true;
  qint64 isbn = 0;
  isbn = txt.toLongLong(&b);
  if (!b) {
    qWarning("Current ISBN:'%s' convert failed.", qPrintable(txt));
  }
  return isbn;
}

const QVariant IsbnEdit::value() {
  if (isValid()) {
    QVariant ret(number());
    if (!ret.convert(QMetaType::LongLong))
      qWarning("No ISBN retrieved.");

    return ret;
  }
  return 0;
}

void IsbnEdit::setProperties(const QSqlField &field) {
  if (field.requiredStatus() == QSqlField::Required)
    setRequired(true);
}

void IsbnEdit::setInfo(const QString &info) { setToolTip(info); }

const QString IsbnEdit::info() { return m_isbn->toolTip(); }

const QString IsbnEdit::notes() {
  QString info(tr("Invalid ISBN detected."));
  info.append(" ");
  info.append(m_isbn->toolTip());
  return info;
}
