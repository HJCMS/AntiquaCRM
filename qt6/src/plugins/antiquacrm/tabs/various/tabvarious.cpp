// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabvarious.h"
#include "variousconfig.h"
#include "variousconfigwidget.h"
#include "variousindex.h"

TabVarious::TabVarious(QObject *parent) : AntiquaCRM::TabsInterface{parent} {
  setObjectName("various_plugin");
}

const QString TabVarious::displayName() const { return tr("Various"); }

const QString TabVarious::interfaceName() const {
  return QString(VARIOUS_INTERFACE_NAME);
}

const QString TabVarious::sqlTableName() const {
  return QString(VARIOUS_SQL_TABLE_NAME);
}

const QString TabVarious::sqlFieldPrefix() const {
  return QString(VARIOUS_SQL_FIELD_PREFIX);
}

AntiquaCRM::PluginConfigWidget *
TabVarious::configWidget(QWidget *parent) const {
  VariousConfigWidget *_w = new VariousConfigWidget(parent);
  _w->setWindowTitle(displayName() + "[*]");
  return _w;
}

bool TabVarious::addIndexOnInit() const { return false; }

const QJsonObject TabVarious::menuEntry() const {
  QJsonObject _jo;
  _jo.insert("id", QString(VARIOUS_INTERFACE_TABID));
  _jo.insert("title", displayName());
  _jo.insert("tip", tr("Inventory Various"));
  _jo.insert("icon", "applications-other");
  return _jo;
}

AntiquaCRM::TabsIndex *TabVarious::indexWidget(QWidget *parent) const {
  VariousIndex *_w = new VariousIndex(parent);
  _w->setWindowTitle(displayName());
  return _w;
}

bool TabVarious::createInterface(QObject *parent) {
  return (parent != nullptr);
}
