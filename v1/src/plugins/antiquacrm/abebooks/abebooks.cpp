// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooks.h"

#ifndef PLUGIN_NAME
#define PLUGIN_NAME "AbeBooks"
#endif

#include <QDebug>

Abebooks::Abebooks(QObject *parent) : AntiquaCRM::APluginInterface{parent} {
  setObjectName(PLUGIN_NAME);
}

void Abebooks::prepareJsonResponse(const QJsonDocument &) {}

void Abebooks::queryOrders() {
  qDebug() << Q_FUNC_INFO << "TODO";
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

bool Abebooks::createInterface(QObject *parent) { return true; }
