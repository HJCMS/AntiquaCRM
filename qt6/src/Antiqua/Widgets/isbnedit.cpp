// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "isbnedit.h"

#include <QCompleter>
#include <QDebug>
#include <QFont>
#include <QFontMetrics>

namespace AntiquaCRM {

IsbnEdit::IsbnEdit(QWidget *parent, IsbnEdit::CodeType ctype)
    : AntiquaCRM::AInputWidget{parent}, p_codeType{ctype} {
  m_edit = new AntiquaCRM::ALineEdit(this);
  m_edit->setMaxLength(13);
  m_edit->setClearButtonEnabled(true);
  layout->addWidget(m_edit);

  const QRegularExpression basePattern("^([\\d]{10,13})$");
  m_validator = new QRegularExpressionValidator(basePattern, m_edit);
  m_edit->setValidator(m_validator);

  // Autovervollständigung nur bei ISBN Buchpräfixe einsetzen!
  if (p_codeType == IsbnEdit::CodeType::ISBN) {
    m_edit->setToolTip(tr("It must consist of 10 or 13 digits."));
    m_edit->setPlaceholderText("ISBN10/13 (ISO 2108)");
    QStringList list({"978", "979"});
    QCompleter *m_completer = new QCompleter(list, m_edit);
    m_completer->setCompletionMode(QCompleter::PopupCompletion);
    m_completer->setFilterMode(Qt::MatchStartsWith);
    m_edit->setCompleter(m_completer);
  } else {
    m_edit->setToolTip(tr("It must consist of 13 digits."));
    m_edit->setPlaceholderText("GTIN13 Barcode");
  }

  initData();

  connect(m_edit, SIGNAL(textChanged(const QString &)),
          SLOT(valueChanged(const QString &)));
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

  int _apd = 0; // A: Produktnummer (product digit)
  int _brd = 0; // B: Aufgerundet   (roundet digit)
  int _ccd = 0; // C: Prüfnummer    (checksum digit)
  int _pos = 0; // Start position
  for (int i = type; i > 0; --i) {
    int _num = ean.at(i - 1).digitValue();
    if (i == type) {
      _ccd = _num;
      continue;
    }
    _apd += (_pos % 2 & 1) ? (_num * 1) : (_num * 3);
    _pos++;
  }
  // qDebug() << "GTIN:" << type << "Code:" << ean << "Check:" << ccd;
  if (_ccd == 0)
    return (_apd % 10 == 0);

  _brd = _apd;
  for (int r = 0; r <= 9; ++r) {
    if (++_brd % 10 == 0)
      break;
  }
  return ((_brd - _apd) == _ccd);
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

void IsbnEdit::valueChanged(const QString &data) {
  bool valid = false;
  int len = data.trimmed().length();
  if (len == 10 && isISBN10(data)) {
    valid = true;
  } else if (len >= 12 && isISBN13(data)) {
    valid = true;
  } else if (len == 0) {
    m_edit->setValidContent(true);
    return;
  }
  // visual feedback
  m_edit->setValidContent(valid);

  if (valid) {
    setWindowModified(true);
    emit sendInputChanged();
  }
}

void IsbnEdit::initData() {
  QString _info =
      tr("EAN/GTIN-10 or EAN/GTIN-13.\n"
         "This Code only represent digits 0-9 with  a length of 10 or 13. For "
         "detailed description visit www.gs1.org. AntiquaCRM contains a calc "
         "based Validator to check the Input. Invalid EAN’s will not saved.");
  setWhatsThisText(_info);
  setWindowModified(false);
}

void IsbnEdit::setValue(const QVariant &value) {
  // GTIN
  if (p_codeType == IsbnEdit::CodeType::GTIN) {
    QString _gtin = value.toString().trimmed();
    if (isGTIN(_gtin)) {
      m_edit->setText(_gtin);
      return;
    }
  }

  // ISBN
  QString _buf;
  QMetaType _type = value.metaType();
  switch (_type.id()) {
  case QMetaType::ULongLong:
  case QMetaType::LongLong:
    _buf = QString::number(value.toLongLong());
    break;

  default:
    _buf = value.toString().trimmed();
  }

  // ISBN check
  int _len = _buf.length();
  if (_len < 10 || _len > 13)
    return; // ignore

  if (_len == 10 && isISBN10(_buf)) {
    m_edit->setText(_buf);
  } else if (_len == 13 && isISBN13(_buf)) {
    m_edit->setText(_buf);
  }
}

void IsbnEdit::setFocus() { m_edit->setFocus(); }

void IsbnEdit::reset() {
  m_edit->clear();
  setWindowModified(false);
}

void IsbnEdit::setRestrictions(const QSqlField &field) {
  setRequired((field.requiredStatus() == QSqlField::Required));
  QString _str;
  _str.append(_str.leftJustified(m_edit->maxLength(), QChar('0')));
  QFontMetrics _m(m_edit->font());
  qsizetype _w = _m.size(Qt::TextSingleLine, _str).width();
  m_edit->setMinimumWidth(_w + m_edit->height());
}

void IsbnEdit::setInputToolTip(const QString &tip) { m_edit->setToolTip(tip); }

void IsbnEdit::setBuddyLabel(const QString &text) {
  if (text.isEmpty())
    return;

  ALabel *m_lb = addTitleLabel(text + ":");
  m_lb->setBuddy(m_edit);
}

bool IsbnEdit::isValid() {
  QString code = m_edit->text().trimmed();
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
  QString txt = m_edit->text().trimmed();
  bool b = true;
  qint64 isbn = 0;
  isbn = txt.toLongLong(&b);
  if (!b) {
    qWarning("Current EAN:'%s' convert failed.", qPrintable(txt));
  }
  return isbn;
}

const QMetaType IsbnEdit::getType() const {
  return QMetaType(QMetaType::LongLong);
}

const QVariant IsbnEdit::getValue() {
  if (!isValid())
    return 0;

  if (p_codeType == IsbnEdit::CodeType::GTIN) {
    QString gtin = m_edit->text().trimmed();
    return isGTIN(gtin) ? gtin : QString();
  } else {
    QVariant ret(number());
    if (ret.metaType().id() != QMetaType::LongLong)
      qWarning("No ISBN retrieved.");

    return ret;
  }
  return 0;
}

const QString IsbnEdit::popUpHints() {
  return tr("Invalid ISBN/EAN/UPC detected.");
}

const QString IsbnEdit::statusHints() { return popUpHints(); }

} // namespace AntiquaCRM
