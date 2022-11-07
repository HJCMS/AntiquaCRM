// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aplugininterface.h"
#include "asettings.h"

#include <QHash>

namespace AntiquaCRM {

APluginInterface::APluginInterface(QObject *parent) : QObject{parent} {
  setContentDecoder(QTextCodec::codecForLocale());
}

const APluginConfig APluginInterface::getConfig(const QString &providerName) {
  APluginConfig config;
  if (providerName.isEmpty())
    return config;

  ASettings cfg(this);
  cfg.beginGroup("provider/" + providerName);
  config.hostname = cfg.value("api_host", QString()).toString();
  config.querypath = cfg.value("api_path", QString()).toString();
  config.username = cfg.value("api_user", QString()).toString();
  config.apiKey = cfg.value("api_key", QString()).toString();
  config.port = cfg.value("api_port", 443).toInt();
  config.history = cfg.value("api_history_call", -7).toInt();
  cfg.endGroup();
  return config;
}

AntiquaCRM::Gender APluginInterface::convertGender(const QString &from) const {
  QStringList male({"mr", "mister", "sir", "herr", "herrn", "freiherr"});
  QStringList female({"ms.", "mss", "mses", "madam", "frau", "freifrau"});
  if (female.contains(from, Qt::CaseInsensitive))
    return AntiquaCRM::Gender::MALE;
  else if (male.contains(from, Qt::CaseInsensitive))
    return AntiquaCRM::Gender::MALE;
  else if (from.contains("diverse", Qt::CaseInsensitive))
    return AntiquaCRM::Gender::VARIOUS;
  else
    return AntiquaCRM::Gender::NO_GENDER;
}

const QString APluginInterface::bcp47Country(const QString &country) const {
  AntiquaCRM::ASettings cfg;
  QFileInfo info(cfg.getDataDir("json"), "iso_countrycodes.json");
  if (info.isReadable()) {
    QString code = country.trimmed().toLower();
    QJsonDocument js;
    QFile fp(info.filePath());
    if (fp.open(QIODevice::ReadOnly)) {
      QTextStream data(&fp);
      data.setCodec(ANTIQUACRM_TEXTCODEC);
      QByteArray buffer = data.readAll().toLocal8Bit();
      js = QJsonDocument::fromJson(buffer);
      fp.close();
      buffer.clear();
    }

    if (js.isEmpty())
      return QString("C");

    QJsonArray countries = js.object().value("countries").toArray();
    for (int c = 0; c < countries.size(); c++) {
      QJsonObject o = countries.at(c).toObject();
      if (o.value("country").toString().contains(code, Qt::CaseInsensitive))
        return o.value("iso2").toString();
    }
  }

  return QString("C");
}

const QString APluginInterface::getCountry(const QString &bcp47) const {
  AntiquaCRM::ASettings cfg;
  QFileInfo info(cfg.getDataDir("json"), "iso_countrycodes.json");
  if (info.isReadable()) {
    QString code = bcp47.trimmed().toLower();
    QJsonDocument js;
    QFile fp(info.filePath());
    if (fp.open(QIODevice::ReadOnly)) {
      QTextStream data(&fp);
      data.setCodec(ANTIQUACRM_TEXTCODEC);
      QByteArray buffer = data.readAll().toLocal8Bit();
      js = QJsonDocument::fromJson(buffer);
      fp.close();
      buffer.clear();
    }

    if (js.isEmpty())
      return QString("C");

    QJsonArray countries = js.object().value("countries").toArray();
    for (int c = 0; c < countries.size(); c++) {
      QJsonObject o = countries.at(c).toObject();
      if (o.value("iso2").toString().contains(code, Qt::CaseInsensitive))
        return o.value("country").toString();
    }
  }

  return QString("C");
}

const QDateTime APluginInterface::getDateTime(const QString &dateString,
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

const QDateTime APluginInterface::getDateTime(const QString &dateTimeString,
                                              Qt::TimeSpec spec) const {
  QDateTime dateTime;
  dateTime = QDateTime::fromString(dateTimeString, "yyyy-MM-dd HH:mm:ss");
  dateTime.setTimeSpec(spec);
  return dateTime;
}

const QDateTime APluginInterface::timeSpecDate(const QDateTime &dateTime,
                                               Qt::TimeSpec fromSpec) const {
  QDateTime dt(dateTime);
  dt.setTimeSpec(fromSpec);
  return dt;
}

void APluginInterface::setContentDecoder(QTextCodec *codec) {
  if (codec != nullptr)
    m_decodeFrom = codec;
}

}; // namespace AntiquaCRM
