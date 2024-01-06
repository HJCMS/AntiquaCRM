// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "buchfreund.h"
#include "buchfreundconfigwidget.h"
#include "buchfreundoperations.h"

Buchfreund::Buchfreund(QObject *parent)
    : AntiquaCRM::ProviderInterface{parent} {
  setObjectName("provider_plugin_buchfreund");
}

const QString Buchfreund::displayName() const { return QString("Buchfreund"); }

const QString Buchfreund::interfaceName() const {
  return QString("buchfreund");
}

AntiquaCRM::PluginConfigWidget *
Buchfreund::configWidget(QWidget *parent) const {
  return (new BuchfreundConfigWidget(parent));
}

bool Buchfreund::operationWidget(QWidget *parent,
                                 const QJsonObject &order) const {
  BuchfreundOperations *d = new BuchfreundOperations(parent);
  if (d->exec(order) == QDialog::Rejected)
    return false;

  return true;
}

const QJsonObject Buchfreund::menuEntry() const {
  QJsonObject _jo;
  _jo.insert("title", "Buchfreund");
  _jo.insert("id", "booklooker_tab");
  _jo.insert("icon", "antiquacrm");
  return _jo;
}

bool Buchfreund::createInterface(QObject *parent) {
  Q_UNUSED(parent);
  return true;
}
