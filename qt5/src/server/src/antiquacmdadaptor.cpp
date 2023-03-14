// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "antiquacmdadaptor.h"

#include <QCoreApplication>
#include <QDebug>

AntiquaCMDAdaptor::AntiquaCMDAdaptor(QObject *parent)
    : QDBusAbstractAdaptor{parent} {
  setObjectName(ANTIQUACRM_CONNECTION_DOMAIN);
  setAutoRelaySignals(false);
}

const QString AntiquaCMDAdaptor::help() {
  return QString("Print this help and exit");
}

const QString AntiquaCMDAdaptor::status() {
  QString info;
  info.append("AntiquaCMD is up!");
  info.append("\nWaiting for input ...");
  return info;
}

const QStringList AntiquaCMDAdaptor::providers() {
  QStringList l;
  l << "abebooks";
  l << "booklooker";
  return l;
}

const QStringList AntiquaCMDAdaptor::actions() {
  QStringList l;
  l << "get_orders";
  return l;
}

void AntiquaCMDAdaptor::quit() { QCoreApplication::quit(); }

void AntiquaCMDAdaptor::command(const QString &provider,
                                const QString &action) {
  QString _p = provider.trimmed().toLower();
  QString _a = action.trimmed().toLower();
  if (providers().contains(_p) && actions().contains(_a)) {
    emit sendExecution(_p, _a);
    return;
  }
  qInfo("ANTIQUACRM::IGNORE::command('%s','%s')", qPrintable(_p),
        qPrintable(_a));
}

void AntiquaCMDAdaptor::queryOrders() { emit sendQueryUpdate(); }

AntiquaCMDAdaptor::~AntiquaCMDAdaptor() {}
