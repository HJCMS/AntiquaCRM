// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aplugininterface.h"
#include "asettings.h"

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
  config.history = cfg.value("api_history_call", -3).toInt();
  cfg.endGroup();
  return config;
}

const QDateTime APluginInterface::getDateTime(const QString &dateString,
                                              const QString &timeString,
                                              Qt::TimeSpec to) const {
  QDateTime dateTime;
  QDate d = QDate::fromString(dateString, "yyyy-MM-dd");
  dateTime.setDate(d);
  QTime t = QTime::fromString(timeString, "HH:mm:ss");
  dateTime.setTime(t);
  dateTime.setTimeSpec(to);
  return dateTime;
}

const QDateTime APluginInterface::getDateTime(const QString &dateTimeString,
                                              Qt::TimeSpec to) const {
  QDateTime dateTime;
  dateTime = QDateTime::fromString(dateTimeString, "yyyy-MM-dd HH:mm:ss");
  dateTime.setTimeSpec(to);
  return dateTime;
}

const QDateTime APluginInterface::timeSpecDate(const QDateTime &dateTime,
                                               Qt::TimeSpec to) const {
  QDateTime dt(dateTime);
  dt.setTimeSpec(to);
  return dt;
}

double APluginInterface::getPrice(const QJsonValue &value) const {
  if (value.type() == QJsonValue::Double)
    return value.toDouble();

  if (value.type() == QJsonValue::String) {
    QString str = value.toString();
    if (!str.isEmpty())
      return str.toDouble();
  }
  return 0.00;
}

void APluginInterface::setContentDecoder(QTextCodec *codec) {
  if (codec != nullptr)
    m_decodeFrom = codec;
}

}; // namespace AntiquaCRM
