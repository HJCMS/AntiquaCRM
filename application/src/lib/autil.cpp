// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "autil.h"

#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QStringList>
#include <QSysInfo>
#include <QTextCodec>
#include <QTextEncoder>

namespace AntiquaCRM {

const QString AUtil::socketName() {
  QString name(ANTIQUACRM_CONNECTION_DOMAIN);
  name.append(".");
  name.append(QString::fromLocal8Bit(QSysInfo::machineUniqueId()));
  QString userName;
#ifdef Q_OS_LINUX
  userName = qEnvironmentVariable("USER").trimmed().replace(" ", "");
  if (!userName.isEmpty()) {
    name.append(".");
    name.append(userName);
  }
#endif
  return name;
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

const QRegExp AUtil::mailPattern() {
  QRegExp reg;
  reg.setCaseSensitivity(Qt::CaseInsensitive);
  reg.setPattern("^([\\d\\w\\-\\.]{3,})@([\\d\\w\\-\\.]{3,})\\.([a-z]{2,6})$");
  return reg;
}

bool AUtil::checkMail(const QString &mail) {
  QRegularExpression expr(mailPattern().pattern());
  QString str = mail.trimmed().toLower();
  QRegularExpressionMatch match = expr.match(str);
  return match.hasMatch();
}

const QRegExp AUtil::phonePattern() {
  QRegExp reg;
  reg.setCaseSensitivity(Qt::CaseSensitive);
  reg.setPattern("^([\\d]{2,3}\\s?[\\d]{2,4}[\\s?\\d]+)$");
  return reg;
}

bool AUtil::checkPhone(const QString &phone) {
  QRegularExpression expr(phonePattern().pattern());
  QString str = phone.trimmed().toLower();
  QRegularExpressionMatch match = expr.match(str);
  return match.hasMatch();
}

const QString AUtil::toISO88591(const QString &str) {
  QTextCodec *codec = QTextCodec::codecForLocale();
  QTextEncoder encoder(codec);
  return QString(encoder.fromUnicode(str));
}

const QString AUtil::fileNumber(qint64 number, int length) {
  return QString::number(number).rightJustified(length, '0');
}

}; // namespace AntiquaCRM
