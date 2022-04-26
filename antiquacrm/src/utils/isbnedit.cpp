// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "isbnedit.h"
#include "version.h"
#include <QtCore/QDebug>
#include <QtCore/QRegularExpression>
#include <QtCore/QRegularExpressionMatch>
#include <QtCore/QStringList>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QSizePolicy>

/*
#include <QtCore/QDir>
#include <QtCore/QFile>
static QStringList isbn_patterns() {
  QStringList l;
  QFile fp(QDir::homePath() + "/.cache/isbn.log");
  if (!fp.open(QIODevice::ReadOnly | QIODevice::Text))
    return l;

  while (!fp.atEnd()) {
    l.append(fp.readLine());
  }
  fp.close();

  return l;
}
// foreach (QString isbn, isbn_patterns()) { patternMatching(isbn.trimmed()); }
*/

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

  QRegExp pattern("^([\\d]{10,13})$");
  m_validator = new QRegExpValidator(pattern, m_isbn);
  m_isbn->setValidator(m_validator);

  QStringList list("978");
  list << "979";

  m_completer = new QCompleter(list, m_isbn);
  m_completer->setCaseSensitivity(Qt::CaseInsensitive);
  m_isbn->setCompleter(m_completer);

  layout->addStretch(1);
  setLayout(layout);

  connect(m_isbn, SIGNAL(textChanged(const QString &)), this,
          SLOT(isbnChanged(const QString &)));
  connect(m_query, SIGNAL(clicked()), this, SIGNAL(clicked()));
  connect(this, SIGNAL(isbnIsValid(bool)), this, SLOT(feedbackIcon(bool)));
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

bool IsbnEdit::patternMatching(const QString &isbn) {
  /** ISBN/EAN 13 */
  QRegularExpression re("^(97[89]{1})([\\d]{10})$");
  QRegularExpressionMatch check13 = re.match(isbn);
  if (check13.hasMatch()) {
    return true;
  }
  /** ISBN 10 */
  re.setPattern("^(?!97[89])([\\d]{10})$");
  QRegularExpressionMatch check10 = re.match(isbn);
  if (check10.hasMatch()) {
    return true;
  }
  return false;
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
  int len = s.length();
  setButtonText(len);

  switch (len) {
  case 0: {
    emit isbnIsValid(false);
    break;
  }

  case 10: {
    if (patternMatching(s))
      emit isbnIsValid(true);

    break;
  }

  case 13: {
    if (patternMatching(s))
      emit isbnIsValid(true);

    break;
  }

  default:
    emit isbnIsValid(false);
    break;
  }
  setModified(true);
}

void IsbnEdit::setValue(const QVariant &val) {
  qulonglong i = val.toLongLong();
  if (i == 0)
    return;

  m_isbn->setText(QString::number(i));
}

void IsbnEdit::reset() {
  m_isbn->clear();
  setModified(false);
}

bool IsbnEdit::isValid() {
  int len = m_isbn->text().trimmed().length();
  if ((len == 10) || (len == 13)) {
    return true;
  }
  return false;
}

const QVariant IsbnEdit::value() {
  QString txt = m_isbn->text().trimmed();
  int len = txt.length();
  if ((len == 10) || (len == 13)) {
    // qInfo("ISBN:%s Lenght:%s", qPrintable(txt),
    // qPrintable(QString::number(len)));
    bool b;
    qulonglong isbn = txt.toLong(&b);
    if (b)
      return isbn;
  }
  return 0;
}

const QString IsbnEdit::notes() {
  return tr(
      "ISBN is set to required and must be a valid 10 or 13 digit number.");
}
