// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "autil.h"

#include <QCryptographicHash>
#include <QLocale>
#include <QRegularExpressionMatch>
#include <QStringList>
#include <QTextStream>
#include <QUrl>

namespace AntiquaCRM {

const QString AUtil::recipientPattern() {
  return QString("([\\d\\w\\-\\.\\_]+)");
}

const QString AUtil::domainPattern() {
  return QString("([\\d\\w]+[\\-\\.]?[\\d\\w]+)+");
}

const QString AUtil::tldPattern() { return QString(("\\.[a-z]{2,10}")); }

const QString AUtil::fqdnPattern() {
  QString _fqdn(domainPattern());
  _fqdn.append(tldPattern());
  return _fqdn;
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

const QRegularExpression AUtil::lineBreakRegExp() {
  return QRegularExpression("[\\n\\r]+", QRegularExpression::NoPatternOption);
}

const QString AUtil::trim(const QString &str) {
  QString _str = str.trimmed();

  _str.replace(lineBreakRegExp(), " ");

  static const QRegularExpression pcr2("(\\b[\\s\\t]+\\b)",
                                       QRegularExpression::NoPatternOption);
  _str.replace(pcr2, " ");

  _str.squeeze();

  return _str.trimmed();
}

const QString AUtil::trimBody(const QString &str) {
  QStringList _list;
  foreach (QString _line, str.split(lineBreakRegExp())) {
    _list << AUtil::trim(_line.trimmed());
  }
  return _list.join("\n");
}

const QString AUtil::ucFirst(const QString &str) {
  QStringList _arr = str.trimmed().split(" ", Qt::SkipEmptyParts);
  for (int i = 0; i < _arr.size(); i++) {
    _arr[i].replace(0, 1, _arr[i][0].toUpper());
  }
  return _arr.join(" ");
}

const QRegularExpression AUtil::emailRegExp() {
  const QString _recipient = recipientPattern();
  const QString _fqdn = fqdnPattern();
  QRegularExpression _reg;
  _reg.setPatternOptions(QRegularExpression::CaseInsensitiveOption);
  _reg.setPattern("^" + _recipient + "@" + _fqdn + "$");
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
  _reg.setPattern("^(0[\\d]{1,3}\\s?[\\d]{2,4}[\\s?\\d]+)$");
  return _reg;
}

bool AUtil::checkPhone(const QString &phone) {
  QString _str = phone.trimmed();
  QRegularExpressionMatch _match = phoneRegExp().match(_str);
  return _match.hasMatch();
}

bool AUtil::checkUrl(const QUrl &url) {
  QUrl _url(url.toString(), QUrl::StrictMode);
  if (!url.host().isEmpty()) {
    const QRegularExpression _reg(fqdnPattern(),
                                  QRegularExpression::CaseInsensitiveOption);
    QRegularExpressionMatch _match = _reg.match(url.host());
    if (!_match.hasMatch())
      return false;
  }
  return _url.isValid();
}

const QRegularExpression AUtil::keywordRegExp() {
  QRegularExpression _reg;
  _reg.setPatternOptions(QRegularExpression::UseUnicodePropertiesOption);
  _reg.setPattern("^([\\w\\d]+)$");
  return _reg;
}

const QRegularExpression AUtil::numericRegExp() {
  return QRegularExpression("^\\d+$");
}

const QRegularExpression AUtil::articleRegExp() {
  return QRegularExpression("^(\\d{1,9}[\\,]?)+$");
}

const QRegularExpression AUtil::spaceRegExp() {
  return QRegularExpression("[\\s\\t]+");
}

const QString AUtil::zerofill(qint64 number, int length) {
  return QString::number(number).rightJustified(length, '0');
}

const QString AUtil::md5sum(const QString &data) {
  const QByteArray _buff = data.trimmed().toLocal8Bit();
  QCryptographicHash _hash(QCryptographicHash::Md5);
  _hash.addData(_buff);
  return QString::fromLocal8Bit(_hash.result().toHex());
}

const QString AUtil::strEncode(const QString &query) {
  static const QRegularExpression spaces("[\\s\\t]+");
  QString _o = query.trimmed();
  _o.replace(".", "");
  _o.replace(",", "");
  _o.replace(":", " ");
  _o.replace("-", " ");
  _o.replace("'", "");
  _o.replace("´", "");
  _o.replace("`", "");
  _o.replace("&", " ");
  _o.replace("\"", "%22");
  _o.replace("=", "%3D");
  // finally replace spaces
  _o.replace(spaces, "+");
  return _o;
}

}; // namespace AntiquaCRM
