// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabstitches.h"
#include "stitchesconfigwidget.h"
#include "stitcheswidget.h"

TabStitches::TabStitches(QObject *parent) : AntiquaCRM::TabsInterface{parent} {
  setObjectName("stitches_plugin");
}

const QString TabStitches::displayName() const {
  return tr("Prints && Stitches");
}

const QString TabStitches::interfaceName() const {
  return STITCHES_INTERFACE_NAME;
}

const QString TabStitches::sqlTableName() const { return STITCHES_TABLE_NAME; }

const QString TabStitches::sqlFieldPrefix() const { return QString("ip"); }

AntiquaCRM::PluginConfigWidget *TabStitches::configWidget(QWidget *parent) const {
  StitchesConfigWidget *_w = new StitchesConfigWidget(parent);
  _w->setWindowTitle(displayName() + "[*]");
  return _w;
}

bool TabStitches::addIndexOnInit() const { return false; }

const QJsonObject TabStitches::menuEntry() const {
  QJsonObject _jo;
  _jo.insert("id", "stitches_tab");
  _jo.insert("title", tr("Prints && Stitches"));
  _jo.insert("tip", tr("Inventory Prints and Stitches"));
  _jo.insert("icon", "kjournal");
  return _jo;
}

AntiquaCRM::TabsIndex *TabStitches::indexWidget(QWidget *parent) const {
  StitchesWidget *_w = new StitchesWidget(parent);
  if (_w == nullptr)
    qFatal("Unable to load StitchesWidget!");

  return _w;
}

bool TabStitches::createInterface(QObject *parent) {
  return (parent != nullptr);
}
