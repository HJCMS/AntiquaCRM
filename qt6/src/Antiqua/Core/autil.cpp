// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "autil.h"

#include <QLocale>
#include <QRegularExpressionMatch>
#include <QStringList>
#include <QTextStream>

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
  out.replace(QRegularExpression("[\\n\\r]+"), " ");
  out.replace(QRegularExpression("[\\s\\t]+"), " ");
  return out.trimmed();
}

const QString AUtil::ucFirst(const QString &str) {
  QStringList array = str.trimmed().split(" ", Qt::SkipEmptyParts);
  for (int i = 0; i < array.size(); i++) {
    array[i].replace(0, 1, array[i][0].toUpper());
  }
  return array.join(" ");
}

const QRegularExpression AUtil::emailRegExp() {
  QRegularExpression reg;
  reg.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
  reg.setPattern("^([\\d\\w\\-\\.]{3,})@([\\d\\w\\-\\.]{2,})\\.([a-z]{2,6})$");
  return reg;
}

bool AUtil::checkMail(const QString &mail) {
  QRegularExpression expr(emailRegExp().pattern());
  QString str = mail.trimmed().toLower();
  QRegularExpressionMatch match = expr.match(str);
  return match.hasMatch();
}

const QRegularExpression AUtil::phoneRegExp() {
  QRegularExpression reg;
  reg.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
  reg.setPattern("^(0[\\d]{2,3}\\s?[\\d]{2,4}[\\s?\\d]+)$");
  return reg;
}

bool AUtil::checkPhone(const QString &phone) {
  QRegularExpression expr(phoneRegExp().pattern());
  QString str = phone.trimmed().toLower();
  QRegularExpressionMatch match = expr.match(str);
  return match.hasMatch();
}

const QString AUtil::zerofill(qint64 number, int length) {
  return QString::number(number).rightJustified(length, '0');
}

const QString AUtil::toMoney(double value, QLocale::CurrencySymbolFormat format) {
  QLocale lc = QLocale::system();
  QString cs = lc.currencySymbol(format);
  return lc.toCurrencyString(value, cs, 2);
}

}; // namespace AntiquaCRM
