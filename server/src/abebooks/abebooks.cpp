// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooks.h"
#include "networker.h"
#include "networkrequest.h"
#include "settings.h"
#include "sqlpsql.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonValue>
#include <QNetworkReply>
#include <QUrlQuery>

AbeBooks::AbeBooks(QObject *parent) : Provider{parent} {}

void AbeBooks::initConfiguration() {
  m_config->beginGroup("provider/abebooks");
  baseUrl.setScheme("https");
  baseUrl.setHost(
      m_config->value("api_host", "orderupdate.abebooks.com").toString());
  apiKey = m_config->value("api_key", QString()).toString();
  apiPort = m_config->value("api_port", 10003).toInt();
  apiUser = m_config->value("api_user", QString()).toString();
  apiPath = QString("/");
  m_config->endGroup();
}

void AbeBooks::queryOrders() {}
