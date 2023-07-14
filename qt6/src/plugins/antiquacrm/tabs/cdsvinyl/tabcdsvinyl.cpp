// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabcdsvinyl.h"
#include "cdsvinylconfig.h"
#include "cdsvinylconfigwidget.h"
#include "cdsvinylwidget.h"

TabCDsVinyl::TabCDsVinyl(QObject *parent) : AntiquaCRM::TabsInterface{parent} {
  setObjectName("cdsvinyl_plugin");
}

const QString TabCDsVinyl::displayName() const { return tr("CD && Vinyl"); }

const QString TabCDsVinyl::interfaceName() const {
  return CDSVINYL_INTERFACE_NAME;
}

const QString TabCDsVinyl::sqlTableName() const { return CDSVINYL_TABLE_NAME; }

const QString TabCDsVinyl::sqlFieldPrefix() const { return QString("cv"); }

AntiquaCRM::PluginConfigWidget *
TabCDsVinyl::configWidget(QWidget *parent) const {
  CDsVinylConfigWidget *_w = new CDsVinylConfigWidget(parent);
  if (_w == nullptr)
    qFatal("Unable to load CDsVinylConfigWidget!");

  return _w;
}

bool TabCDsVinyl::addIndexOnInit() const { return false; }

const QJsonObject TabCDsVinyl::menuEntry() const {
  QJsonObject _jo;
  _jo.insert("id", QString(CDSVINYL_INVENTORY_TABID));
  _jo.insert("title", displayName());
  _jo.insert("tip", tr("Media Inventory, CD’s and Vinyl"));
  _jo.insert("icon", tr("media-optical-dvd-video"));
  return _jo;
}

AntiquaCRM::TabsIndex *TabCDsVinyl::indexWidget(QWidget *parent) const {
  CDsVinylWidget *_w = new CDsVinylWidget(parent);
  if (_w == nullptr)
    qFatal("Unable to load CDsVinylWidget!");

  return _w;
}

bool TabCDsVinyl::createInterface(QObject *parent) {
  // #ifdef Q_OS_LINUX
  //  if() {} TODO MEDIA CHECK
  // #endif
  return (parent != nullptr);
}
