// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "isbnedit.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QStringList>

/*
  Siehe OpenLibrary https://openlibrary.org/dev/docs/api/books

  curl
  'http://openlibrary.org/api/books?bibkeys=ISBN:${_isbn}&jscmd=data&format=json'

  https://openlibrary.org/isbn/${_isbn}.json

  Alternativen isbnsearch.org oder die Google Books API
  Beispielseite für die Google Books API Nutzung:
    https://www.labnol.org/code/20020-query-book-by-isbn
*/
IsbnEdit::IsbnEdit(QWidget *parent) : QLineEdit{parent} {
  setObjectName("IsbnEdit");
  setMaxLength(13);
  setPlaceholderText(tr("ISBN/EAN 10/13 (ISO 2108)"));
  setClearButtonEnabled(true);
  setToolTip(tr("The ISBN number must consist of 10 or 13 digits."));

  QRegExp pattern("^\\d+");
  m_validator = new QRegExpValidator(pattern, this);
  setValidator(m_validator);

  QStringList list("978");
  list << "979";

  m_completer = new QCompleter(list, this);
  m_completer->setCaseSensitivity(Qt::CaseInsensitive);
  setCompleter(m_completer);

  connect(this, SIGNAL(textChanged(const QString &)), this,
          SLOT(isbnChanged(const QString &)));
}

void IsbnEdit::isbnChanged(const QString &s) {
  QRegExp r("^97[89]\\d+");
  switch (s.length()) {
  case 10: {
    if (r.exactMatch(s)) {
      setStyleSheet("color: red;");
    } else {
      setStyleSheet("");
      emit s_isbnIsValid(true);
    }
    return;
  }
  case 13:
    if (r.exactMatch(s)) {
      setStyleSheet("");
      emit s_isbnIsValid(true);
    } else {
      setStyleSheet("color: red;");
    }
    return;

  default:
    setStyleSheet("color: red;");
    emit s_isbnIsValid(false);
    return;
  }
}

void IsbnEdit::setValue(const QVariant &val) {
  qulonglong i = val.toLongLong();
  if (i == 0)
    return;

  setText(QString::number(i));
}

const QVariant IsbnEdit::value() {
  QString txt = text().trimmed();
  int l = txt.length();
  if (l > 13 && l != 10)
    return 0;
  else if (l != 13)
    return 0;

  bool b;
  qulonglong isbn = txt.toLong(&b);
  qDebug() << "" << txt << "isbn():" << b << isbn;
  if (b)
    return isbn;

  return 0;
}
