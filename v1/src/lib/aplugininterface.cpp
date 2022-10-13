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
  config.username = cfg.value("api_use", QString()).toString();
  config.apiKey = cfg.value("api_key", QString()).toString();
  config.port = cfg.value("api_port", 443).toInt();
  config.history = cfg.value("api_history_call", -7).toInt();
  cfg.endGroup();
  return config;
}

}; // namespace AntiquaCRM
