// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "buchfreund.h"
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

Buchfreund::Buchfreund(QObject *parent) : Provider{parent} {
  connect(m_networker, SIGNAL(sendResponse(const QByteArray &)),
          SLOT(responsed(const QByteArray &)));
}

void Buchfreund::initConfiguration() {
  m_config->beginGroup("provider/buchfreund");
  baseUrl.setScheme("https");
  baseUrl.setHost(m_config->value("api_host", "www.buchfreund.de").toString());
  apiKey = m_config->value("api_key", QString()).toString();
  apiPath = QString("/");
  m_config->endGroup();
}

void Buchfreund::queryOrders() {}
