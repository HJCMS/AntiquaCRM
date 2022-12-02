// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "provider.h"
#include "networker.h"
#include "settings.h"
#include "sqlpsql.h"

Provider::Provider(QObject *parent) : QObject{parent} {
  m_config = new Settings(this);
  m_networker = new Networker(this);
  m_sql = new SqlPsql(this);
}

const QString Provider::ucFirst(const QString &str) {
  QStringList array = str.trimmed().split(" ", Qt::SkipEmptyParts);
  for (int i = 0; i < array.size(); i++) {
    array[i].replace(0, 1, array[i][0].toUpper());
  }
  return array.join(" ");
}

AntiquaCRM::Gender Provider::convertGender(const QString &from) const {
  QStringList female({"ms.", "mss", "mses", "madam", "frau", "freifrau"});
  QStringList male({"mr", "mister", "sir", "herr", "herrn", "freiherr"});
  if (female.contains(from, Qt::CaseInsensitive))
    return AntiquaCRM::Gender::FEMALE;
  else if (male.contains(from, Qt::CaseInsensitive))
    return AntiquaCRM::Gender::MALE;
  else if (from.contains("diverse", Qt::CaseInsensitive))
    return AntiquaCRM::Gender::VARIOUS;
  else
    return AntiquaCRM::Gender::NO_GENDER;
}

const QString Provider::bcp47Country(const QString &country) const {
  Q_UNUSED(country);
  return QString("DE");
}

const QString Provider::getCountry(const QString &bcp47) const {
  Q_UNUSED(bcp47);
  return QString("Germany");
}

const QDateTime Provider::getDateTime(const QString &dateString,
                                      const QString &timeString,
                                      Qt::TimeSpec spec) const {
  QDateTime dateTime;
  QDate d = QDate::fromString(dateString, "yyyy-MM-dd");
  dateTime.setDate(d);
  QTime t = QTime::fromString(timeString, "HH:mm:ss");
  dateTime.setTime(t);
  dateTime.setTimeSpec(spec);
  return dateTime;
}

const QDateTime Provider::getDateTime(const QString &dateTimeString,
                                      Qt::TimeSpec spec) const {
  QDateTime dateTime;
  dateTime = QDateTime::fromString(dateTimeString, "yyyy-MM-dd HH:mm:ss");
  dateTime.setTimeSpec(spec);
  return dateTime;
}

const QDateTime Provider::timeSpecDate(const QDateTime &dateTime,
                                       Qt::TimeSpec fromSpec) const {
  QDateTime dt(dateTime);
  dt.setTimeSpec(fromSpec);
  return dt;
}

double Provider::getPrice(const QJsonValue &value) const {
  if (value.type() == QJsonValue::Double)
    return value.toDouble();

  if (value.type() == QJsonValue::String) {
    QString str = value.toString();
    if (!str.isEmpty())
      return str.toDouble();
  }
  return 0.00;
}

qint64 Provider::getArticleId(const QJsonValue &value) const {
  if (value.type() == QJsonValue::String) {
    QString str = value.toString();
    if (!str.isEmpty())
      return str.toInt();
  }
  return value.toInt();
}
