// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "isbnedit.h"

#include <QHBoxLayout>
#include <QIcon>
#include <QMetaType>
#include <QPixmap>
#include <QRegularExpressionMatch>
#include <QStringList>

// https://www.activebarcode.de/codes/upca_upce
IsbnEdit::IsbnEdit(QWidget *parent, IsbnEdit::CodeType ctype)
    : InputEdit{parent}, p_codeType{ctype} {
  setObjectName("IsbnEdit");
  m_lineEdit = new QLineEdit(this);
  m_lineEdit->setMaxLength(13);
  m_lineEdit->setMinimumWidth(200);
  m_lineEdit->setClearButtonEnabled(true);
  m_layout->addWidget(m_lineEdit);

  m_status = new QLabel(this);
  m_layout->addWidget(m_status);

  m_validator = new QRegExpValidator(simplePattern, m_lineEdit);
  m_lineEdit->setValidator(m_validator);

  // Autovervollständigung nur bei ISBN Buchpräfixe einsetzen!
  if (p_codeType == IsbnEdit::CodeType::ISBNEAN) {
    m_lineEdit->setWhatsThis(tr("It must consist of 10 or 13 digits."));
    m_lineEdit->setPlaceholderText("ISBN/EAN 10/13 (ISO 2108)");
    QStringList list({"978", "979"});
    QCompleter *m_completer = new QCompleter(list, m_lineEdit);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->setFilterMode(Qt::MatchStartsWith);
    m_lineEdit->setCompleter(m_completer);
  } else {
    m_lineEdit->setWhatsThis(tr("It must consist of 13 digits."));
    m_lineEdit->setPlaceholderText("EAN13/GTIN13 Code");
  }

  m_layout->addStretch(1);

  connect(m_lineEdit, SIGNAL(textChanged(const QString &)),
          SLOT(inputChanged(const QString &)));
}

bool IsbnEdit::isISBN10(const QString &isbn) const {
  if (p_codeType == IsbnEdit::CodeType::GTIN)
    return false;

  QRegularExpressionMatch c = p10.match(isbn);
  return c.hasMatch();
}

bool IsbnEdit::validateGTIN(const QString &ean, int type) const {
  if (ean.size() != type)
    return false;

  int apd = 0; // A: Produktnummer (product digit)
  int brd = 0; // B: Aufgerundet   (roundet digit)
  int ccd = 0; // C: Prüfnummer    (checksum digit)
  int pos = 0; // Start position
  for (int i = type; i > 0; --i) {
    int num = ean.at(i - 1).digitValue();
    if (i == type) {
      ccd = num;
      continue;
    }
    apd += (pos % 2 & 1) ? (num * 1) : (num * 3);
    pos++;
  }
  // qDebug() << "GTIN:" << type << "Code:" << ean << "Check:" << ccd;
  if (ccd == 0)
    return (apd % 10 == 0);

  brd = apd;
  for (int r = 0; r <= 9; ++r) {
    if (++brd % 10 == 0)
      break;
  }
  return ((brd - apd) == ccd);
}

bool IsbnEdit::isISBN13(const QString &isbn) const {
  if (p_codeType == IsbnEdit::CodeType::GTIN)
    return isGTIN(isbn);

  QRegularExpressionMatch c = p13.match(isbn);
  return c.hasMatch();
}

bool IsbnEdit::isGTIN(const QString &ean) const {
  int l = ean.length();
  if (l < 12)
    return false;

  QRegularExpressionMatch c = gtin.match(ean);
  if (!c.hasMatch())
    return false;

  return validateGTIN(ean, l);

  return false;
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

void IsbnEdit::inputChanged(const QString &s) {
  bool valid = false;
  int len = s.trimmed().length();
  if (len == 10 && isISBN10(s)) {
    valid = true;
  } else if (len >= 12 && isISBN13(s)) {
    valid = true;
  }
  setValidationIcon(valid);
  emit isbnIsValid(valid);
  setModified(true);
}

void IsbnEdit::setValue(const QVariant &val) {
  // GTIN
  if (p_codeType == IsbnEdit::CodeType::GTIN) {
    QString gtin = val.toString().trimmed();
    if (isGTIN(gtin)) {
      m_lineEdit->setText(gtin);
      return;
    }
  }
  // ISBN
  QString s;
  switch (val.type()) {
  case QVariant::ULongLong:
  case QVariant::LongLong:
    s = QString::number(val.toLongLong());
    break;

  default:
    s = val.toString().trimmed();
  }

  // ISBN check
  int len = s.length();
  if (len < 10 || len > 13)
    return; // ignore

  if (len == 10 && isISBN10(s)) {
    m_lineEdit->setText(s);
  } else if (len == 13 && isISBN13(s)) {
    m_lineEdit->setText(s);
  }
}

void IsbnEdit::reset() {
  m_lineEdit->clear();
  setModified(false);
}

void IsbnEdit::setFocus() { m_lineEdit->setFocus(); }

bool IsbnEdit::isValid() {
  QString code = m_lineEdit->text().trimmed();
  if (p_codeType == IsbnEdit::CodeType::GTIN)
    return isGTIN(code);

  int l = code.length();
  if (l == 10 && isISBN10(code))
    return true;

  if (l == 13 && isISBN13(code))
    return true;

  return false;
}

qint64 IsbnEdit::number() {
  QString txt = m_lineEdit->text().trimmed();
  bool b = true;
  qint64 isbn = 0;
  isbn = txt.toLongLong(&b);
  if (!b) {
    qWarning("Current EAN:'%s' convert failed.", qPrintable(txt));
  }
  return isbn;
}

const QVariant IsbnEdit::value() {
  if (!isValid())
    return 0;

  if (p_codeType == IsbnEdit::CodeType::GTIN) {
    QString gtin = m_lineEdit->text().trimmed();
    return isGTIN(gtin) ? gtin : QString();
  } else {
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

const QString IsbnEdit::info() { return m_lineEdit->toolTip(); }

const QString IsbnEdit::notes() {
  QString info(tr("Invalid ISBN/EAN/UPC detected."));
  info.append(" ");
  info.append(m_lineEdit->toolTip());
  return info;
}
