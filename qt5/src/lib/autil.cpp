// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "autil.h"

#include <QLocale>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QStringList>
#include <QTextCodec>
#include <QTextEncoder>

namespace AntiquaCRM {

const QString AUtil::socketName() {
  QString sname(ANTIQUACRM_CONNECTION_DOMAIN);
  sname.append("-");
  sname.append(QString::fromLocal8Bit(HJCMS_CONFIG_APPID));
#ifdef Q_OS_LINUX
  QString userName = qEnvironmentVariable("USER").trimmed().replace(" ", "");
  if (!userName.isEmpty()) {
    sname.append("-");
    sname.append(userName);
  }
#endif
  return sname;
}

const QString AUtil::trim(const QString &str) {
  QString out = str;
  out.replace(QRegExp("[\\n\\r]+"), " ");
  out.replace(QRegExp("[\\s\\t]+"), " ");
  return out.trimmed();
}

const QString AUtil::ucFirst(const QString &str) {
  QStringList array = str.trimmed().split(" ", Qt::SkipEmptyParts);
  for (int i = 0; i < array.size(); i++) {
    array[i].replace(0, 1, array[i][0].toUpper());
  }
  return array.join(" ");
}

const QRegExp AUtil::emailRegExp() {
  const QString _recipient("([\\d\\w]+[\\-\\.\\_]?[\\d\\w]+)+");
  const QString _domain("([\\d\\w]+[\\-\\.]?[\\d\\w]+)+");
  const QString _tld("\\.([a-z]{2,8})");
  QRegExp _reg;
  _reg.setCaseSensitivity(Qt::CaseInsensitive);
  _reg.setPattern("^" + _recipient + "@" + _domain + _tld + "$");
  return _reg;
}

bool AUtil::checkMail(const QString &mail) {
  if(mail.isEmpty())
    return false;

  QRegularExpression expr(emailRegExp().pattern());
  QString _str = mail.toLower().trimmed();
  QRegularExpressionMatch match = expr.match(_str);
  return match.hasMatch();
}

const QRegExp AUtil::phoneRegExp() {
  QRegExp reg;
  reg.setCaseSensitivity(Qt::CaseSensitive);
  reg.setPattern("^([\\d]{2,3}\\s?[\\d]{2,4}[\\s?\\d]+)$");
  return reg;
}

bool AUtil::checkPhone(const QString &phone) {
  if(phone.isEmpty())
    return false;

  QRegularExpression expr(phoneRegExp().pattern());
  QString str = phone.trimmed().toLower();
  QRegularExpressionMatch match = expr.match(str);
  return match.hasMatch();
}

const QString AUtil::toISO88591(const QString &str) {
  QTextCodec *codec = QTextCodec::codecForLocale();
  QTextEncoder encoder(codec);
  return QString(encoder.fromUnicode(str));
}

const QString AUtil::zerofill(qint64 number, int length) {
  return QString::number(number).rightJustified(length, '0');
}

const QString AUtil::toMoney(double value,
                             QLocale::CurrencySymbolFormat format) {
  QLocale lc = QLocale::system();
  QString cs = lc.currencySymbol(format);
  return lc.toCurrencyString(value, cs, 2);
}

}; // namespace AntiquaCRM
