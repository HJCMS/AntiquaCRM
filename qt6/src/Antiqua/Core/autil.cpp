// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "autil.h"

#include <QLocale>
#include <QRegularExpressionMatch>
#include <QStringList>
#include <QTextStream>
#include <QUrl>

namespace AntiquaCRM {

const QRegularExpression AUtil::strip_lineends() {
  return QRegularExpression("[\\n\\r]+");
}

const QRegularExpression AUtil::strip_spaces() {
  return QRegularExpression("[\\s\\t]+");
}

const QString AUtil::socketName() {
  QString _name(ANTIQUACRM_CONNECTION_DOMAIN);
  _name.append("-");
  _name.append(QString::fromLocal8Bit(HJCMS_CONFIG_APPID));
#ifdef Q_OS_LINUX
  QString _user = qEnvironmentVariable("USER").trimmed().replace(" ", "");
  if (!_user.isEmpty()) {
    _name.append("-");
    _name.append(_user);
  }
#endif
  return _name;
}

const QString AUtil::trim(const QString &str) {
  QString _str(str.trimmed());
  _str.replace(strip_lineends(), " ");
  _str.replace(strip_spaces(), " ");
  return _str.trimmed();
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
  QString str = mail.trimmed().toLower();
  QRegularExpressionMatch match = emailRegExp().match(str);
  if (match.hasMatch()) {
    QStringList _l = mail.split("@");
    QUrl _url;
    _url.setScheme("mailto");
    _url.setUserInfo(_l.first(), QUrl::StrictMode);
    _url.setHost(_l.last(), QUrl::StrictMode);
    return _url.isValid();
  }
  return false;
}

const QRegularExpression AUtil::phoneRegExp() {
  QRegularExpression reg;
  reg.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
  reg.setPattern("^(0[\\d]{2,3}\\s?[\\d]{2,4}[\\s?\\d]+)$");
  return reg;
}

bool AUtil::checkPhone(const QString &phone) {
  QString str = phone.trimmed().toLower();
  QRegularExpressionMatch match = phoneRegExp().match(str);
  return match.hasMatch();
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
