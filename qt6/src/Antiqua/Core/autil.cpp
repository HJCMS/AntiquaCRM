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
  QStringList _arr = str.trimmed().split(" ", Qt::SkipEmptyParts);
  for (int i = 0; i < _arr.size(); i++) {
    _arr[i].replace(0, 1, _arr[i][0].toUpper());
  }
  return _arr.join(" ");
}

const QRegularExpression AUtil::emailRegExp() {
  QRegularExpression _reg;
  _reg.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
  _reg.setPattern("^([\\d\\w\\-\\.]{3,})@([\\d\\w\\-\\.]{2,})\\.([a-z]{2,6})$");
  return _reg;
}

bool AUtil::checkMail(const QString &mail) {
  QString _str = mail.trimmed().toLower();
  QRegularExpressionMatch _match = emailRegExp().match(_str);
  if (_match.hasMatch()) {
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
  QRegularExpression _reg;
  _reg.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
  _reg.setPattern("^(0[\\d]{2,3}\\s?[\\d]{2,4}[\\s?\\d]+)$");
  return _reg;
}

bool AUtil::checkPhone(const QString &phone) {
  QString _str = phone.trimmed().toLower();
  QRegularExpressionMatch _match = phoneRegExp().match(_str);
  return _match.hasMatch();
}

const QRegularExpression AUtil::keywordRegExp() {
  QRegularExpression _reg;
  _reg.setPatternOptions(QRegularExpression::UseUnicodePropertiesOption);
  _reg.setPattern("^([\\w\\d]+)$");
  return _reg;
}

const QString AUtil::zerofill(qint64 number, int length) {
  return QString::number(number).rightJustified(length, '0');
}

const QString AUtil::toMoney(double value,
                             QLocale::CurrencySymbolFormat format) {
  QLocale _lc = QLocale::system();
  QString _cs = _lc.currencySymbol(format);
  return _lc.toCurrencyString(value, _cs, 2);
}

}; // namespace AntiquaCRM
