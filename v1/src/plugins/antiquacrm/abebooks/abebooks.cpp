// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooks.h"

/** @brief Wird für Menüeintrag und Gruppenzuweisung benötigt! */
#ifndef PLUGIN_NAME
#define PLUGIN_NAME "AbeBooks"
#endif

Abebooks::Abebooks(QObject *parent) : AntiquaCRM::APluginInterface{parent} {
  setObjectName(PLUGIN_NAME);
}

void Abebooks::prepareJsonResponse(const QJsonDocument &) {}

void Abebooks::queryOrders() {}

const QString Abebooks::configProvider() const {
  return QString(PLUGIN_NAME).toLower();
}

const QString Abebooks::displayName() const {
  return QString(PLUGIN_NAME).trimmed();
}

bool Abebooks::createInterface(QObject *parent) { return true; }
