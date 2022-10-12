// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklooker.h"

#ifndef PLUGIN_NAME
#define PLUGIN_NAME "Booklooker"
#endif

Booklooker::Booklooker(QObject *parent) : AntiquaCRM::APluginInterface{parent} {
  setObjectName(PLUGIN_NAME);
}

void Booklooker::prepareJsonResponse(const QJsonDocument &) {}

void Booklooker::queryOrders() {}

const QString Booklooker::configProvider() const {
  return QString(PLUGIN_NAME).toLower();
}

const QString Booklooker::displayName() const {
  return QString(PLUGIN_NAME).trimmed();
}

bool Booklooker::createInterface(QObject *parent) { return true; }
