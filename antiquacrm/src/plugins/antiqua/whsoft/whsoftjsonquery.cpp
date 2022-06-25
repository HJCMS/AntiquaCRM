// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "whsoftjsonquery.h"
#include "whsoftconfig.h"

#include <QByteArray>
#include <QDateTime>
#include <QRegExp>
#include <QTime>
#include <QUrl>

WHSoftJSonQuery::WHSoftJSonQuery(QObject *parent) : QObject{parent} {
  setObjectName("whsoft_json_query");
}

const QUrl WHSoftJSonQuery::apiQuery(const QString &operation) {
  QRegExp pattern("([\\/]{2,})");
  ApplSettings cfg;

  QUrl url;
  cfg.beginGroup(CONFIG_GROUP);
  url.setScheme(cfg.value("api_scheme").toString());
  url.setHost(cfg.value("api_host").toString());
  QString path(cfg.value("api_basepath").toString());
  path.append("/");
  path.append(cfg.value("api_key").toString());
  path.append("/" + operation);
  cfg.endGroup();
  url.setPath(path.replace(pattern, "/"));
  return url;
}

void WHSoftJSonQuery::queryList() {
  QDateTime curDt = QDateTime::currentDateTime();
  QTime t(curDt.time().hour(), curDt.time().minute(), 0);
  curDt.setTime(t);
  QDateTime from = curDt.addDays(-14);
  QJsonObject obj;
  obj.insert("datum_von", QJsonValue::fromVariant(from.toString(DATE_FORMAT)));
  obj.insert("datum_bis", QJsonValue::fromVariant(curDt.toString(DATE_FORMAT)));
  QJsonDocument doc(obj);
  QByteArray data = doc.toJson(QJsonDocument::Compact);
  QUrl url = apiQuery("bestellungen");
  Antiqua::CurlJson *prQuery = new Antiqua::CurlJson(this, PLUGIN_WHSOFT_DEBUG);
  prQuery->setObjectName("buchfreund_query_list");
  connect(prQuery, SIGNAL(responsed(const QJsonDocument &)), this,
          SIGNAL(listResponsed(const QJsonDocument &)));
  connect(prQuery, SIGNAL(finished()), prQuery, SLOT(deleteLater()));
  prQuery->sendPost(url, data);
}

void WHSoftJSonQuery::queryOrder(const QString &bfId) {
  QJsonObject obj;
  obj.insert("id", QJsonValue(bfId));
  QJsonDocument doc(obj);
  QByteArray data = doc.toJson(QJsonDocument::Compact);
  QUrl url = apiQuery("bestellung");
  Antiqua::CurlJson *prQuery = new Antiqua::CurlJson(this, PLUGIN_WHSOFT_DEBUG);
  prQuery->setObjectName("buchfreund_query_view");
  connect(prQuery, SIGNAL(responsed(const QJsonDocument &)), this,
          SIGNAL(orderResponsed(const QJsonDocument &)));
  connect(prQuery, SIGNAL(finished()), prQuery, SLOT(deleteLater()));
  prQuery->sendPost(url, data);
}

void WHSoftJSonQuery::customQuery(const QString &operation,
                                  const QJsonDocument &doc) {
  QByteArray data = doc.toJson(QJsonDocument::Compact);
  QUrl url = apiQuery(operation);
  Antiqua::CurlJson *prQuery = new Antiqua::CurlJson(this, PLUGIN_WHSOFT_DEBUG);
  prQuery->setObjectName("buchfreund_query_" + operation);
  connect(prQuery, SIGNAL(responsed(const QJsonDocument &)), this,
          SIGNAL(orderResponsed(const QJsonDocument &)));
  connect(prQuery, SIGNAL(finished()), prQuery, SLOT(deleteLater()));
  prQuery->sendPost(url, data);
}
