// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "isbnedit.h"
#include "antiqua_global.h"
#include "myicontheme.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QMetaType>
#include <QRegularExpressionMatch>
#include <QSizePolicy>
#include <QStringList>

#ifndef ISBN_EDITOR_DEBUG
#define ISBN_EDITOR_DEBUG 1
#endif

IsbnEdit::IsbnEdit(QWidget *parent) : UtilsMain{parent} {
  setObjectName("IsbnEdit");

  QHBoxLayout *layout = new QHBoxLayout(this);
  layout->setContentsMargins(0, 1, 0, 1);
  layout->setAlignment(Qt::AlignLeft);

  QString toolTip(tr("ISBN/EAN 10/13 (ISO 2108)"));
  m_isbn = new QLineEdit(this);
  m_isbn->setObjectName("isbn_lineedit");
  m_isbn->setMaxLength(13);
  m_isbn->setMinimumWidth(200);
  m_isbn->setToolTip(tr("The ISBN number must consist of 10 or 13 digits."));
  m_isbn->setPlaceholderText(toolTip);
  m_isbn->setClearButtonEnabled(true);
  layout->addWidget(m_isbn);

  m_query = new QPushButton(this);
  m_query->setText("OpenLibrary.org");
  m_query->setToolTip(tr("ISBN request to openlibrary.org"));
  m_query->setIcon(myIcon("edit"));
  m_query->setEnabled(false);
  layout->addWidget(m_query);

  m_info = new QLabel(this);
  m_info->setText("ISBN/EAN");
  m_info->setToolTip(toolTip);
  layout->addWidget(m_info);

  m_validator = new QRegExpValidator(simplePattern, m_isbn);
  m_isbn->setValidator(m_validator);

  m_completer = new QCompleter(prefix, m_isbn);
  m_completer->setCompletionMode(QCompleter::PopupCompletion);
  m_completer->setFilterMode(Qt::MatchStartsWith);
  m_isbn->setCompleter(m_completer);

  layout->addStretch(1);
  setLayout(layout);

  connect(m_isbn, SIGNAL(textChanged(const QString &)), this,
          SLOT(isbnChanged(const QString &)));
  connect(m_query, SIGNAL(clicked()), this, SIGNAL(clicked()));
  connect(this, SIGNAL(isbnIsValid(bool)), this, SLOT(feedbackIcon(bool)));
}

const bool IsbnEdit::isISBN10(const QString &isbn) const {
  QRegularExpressionMatch c = p10.match(isbn);
  return c.hasMatch();
}

const bool IsbnEdit::isISBN13(const QString &isbn) const {
  QRegularExpressionMatch c = p13.match(isbn);
  return c.hasMatch();
}

void IsbnEdit::setButtonText(int i) {
  QString p("ISBN/EAN");
  if (i == 10 || i == 13) {
    QString t = QString("%1 %2 (ISO 2108)").arg(p, QString::number(i));
    m_info->setText(t);
  } else {
    m_info->setText(p + " " + tr("Required"));
  }
}

void IsbnEdit::feedbackIcon(bool b) {
  if (b) {
    m_query->setIcon(myIcon("button_ok"));
    m_query->setEnabled(true);
  } else {
    m_query->setIcon(myIcon("messagebox_warning"));
    m_query->setEnabled(false);
  }
}

void IsbnEdit::isbnChanged(const QString &s) {
  int len = s.trimmed().length();
  setButtonText(len);

  if (len == 10 && isISBN10(s)) {
    emit isbnIsValid(true);
    setModified(true);
    return;
  }

  if (len == 13 && isISBN13(s)) {
    emit isbnIsValid(true);
    setModified(true);
    return;
  }

  emit isbnIsValid(false);
}

void IsbnEdit::setValue(const QVariant &val) {
  QString isbn = val.toString().trimmed();
  int len = isbn.length();
  if (len < 10 && len > 13)
    return;

  if (len == 13 && isISBN13(isbn)) {
    m_isbn->setText(isbn);
    setModified(true);
  }

  if (len == 10 && isISBN10(isbn)) {
    m_isbn->setText(isbn);
    setModified(true);
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
  if (len == 10 && isISBN10(isbn)) {
    return true;
  }

  if (len == 13 && isISBN13(isbn)) {
    return true;
  }

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

void IsbnEdit::setInfo(const QString &info) { m_isbn->setToolTip(info); }

const QString IsbnEdit::info() { return m_isbn->toolTip(); }

const QString IsbnEdit::notes() {
  return tr(
      "ISBN is set to required and must be a valid 10 or 13 digit number.");
}
