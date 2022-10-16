// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooks.h"

#ifndef PLUGIN_NAME
#define PLUGIN_NAME "AbeBooks"
#endif

#include <QDebug>
#include <QTimer>

Abebooks::Abebooks(QObject *parent) : AntiquaCRM::APluginInterface{parent} {
  setObjectName(PLUGIN_NAME);
}

void Abebooks::prepareJsonResponse(const QJsonDocument &) {}

void Abebooks::queryOrders(int waitSecs) {
  QTimer::singleShot((waitSecs * 1000), this, SIGNAL(sendQueryFinished()));
}

const QString Abebooks::configProvider() const {
  return QString(PLUGIN_NAME).toLower();
}

const QString Abebooks::displayName() const {
  return QString(PLUGIN_NAME).trimmed();
}

const QJsonDocument Abebooks::getResponse() const {
  if (p_json.isEmpty())
    return QJsonDocument();

  return p_json;
}

const AntiquaCRM::AProviderOrders Abebooks::getOrders() const {
  AntiquaCRM::AProviderOrders booking;
  // AProviderOrder order(orderId);
  return booking;
}

bool Abebooks::createInterface(QObject *parent) { return true; }
