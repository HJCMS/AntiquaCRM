// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aplugininterface.h"
#include "asettings.h"

#include <QHash>

namespace AntiquaCRM {

APluginInterface::APluginInterface(QObject *parent) : QObject{parent} {}

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

const QDateTime APluginInterface::getDateTime(const QString &dateString,
                                              const QString &timeString,
                                              Qt::TimeSpec spec) const {
  QDateTime dateTime;
  QDate d = QDate::fromString(dateString, "yyyy-MM-dd");
  dateTime.setDate(d);
  QTime t = QTime::fromString(timeString, "HH:mm:00");
  dateTime.setTime(t);
  dateTime.setTimeSpec(spec);
  return dateTime;
}

const QDateTime APluginInterface::timeSpecDate(const QDateTime &dateTime,
                                               Qt::TimeSpec fromSpec) const {
  QDateTime dt(dateTime);
  dt.setTimeSpec(fromSpec);
  return dt;
}

}; // namespace AntiquaCRM
