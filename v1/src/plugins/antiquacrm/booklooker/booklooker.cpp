// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklooker.h"

#ifndef PLUGIN_NAME
#define PLUGIN_NAME "Booklooker"
#endif

#include <QDebug>
#include <QTimer>

Booklooker::Booklooker(QObject *parent) : AntiquaCRM::APluginInterface{parent} {
  setObjectName(PLUGIN_NAME);
}

void Booklooker::prepareJsonResponse(const QJsonDocument &) {}

void Booklooker::queryOrders(int waitSecs) {
  // TODO
  QTimer::singleShot((waitSecs * 1000), this, SIGNAL(sendQueryFinished()));
}

const QString Booklooker::configProvider() const {
  return QString(PLUGIN_NAME).toLower();
}

const QString Booklooker::displayName() const {
  return QString(PLUGIN_NAME).trimmed();
}

const QJsonDocument Booklooker::getResponse() const {
  if (p_json.isEmpty())
    return QJsonDocument();

  return p_json;
}

const AntiquaCRM::AProviderOrders Booklooker::getOrders() const {
  AntiquaCRM::AProviderOrders booking;
  // AProviderOrder order(orderId);
  return booking;
}

bool Booklooker::createInterface(QObject *parent) { return true; }
