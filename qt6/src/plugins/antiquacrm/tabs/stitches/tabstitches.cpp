// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabstitches.h"
#include "stitchesconfig.h"
#include "stitchesconfigwidget.h"
#include "stitcheswidget.h"

TabStitches::TabStitches(QObject *parent) : AntiquaCRM::TabsInterface{parent} {
  setObjectName("stitches_plugin");
}

const QString TabStitches::displayName() const {
  return tr("Prints & Stitches");
}

const QString TabStitches::interfaceName() const {
  return STITCHES_INTERFACE_NAME;
}

const QString TabStitches::sqlTableName() const {
  return QString("inventory_prints");
}

const QString TabStitches::sqlFieldPrefix() const { return QString("ip"); }

AntiquaCRM::TabsConfigWidget *TabStitches::configWidget(QWidget *parent) const {
  StitchesConfigWidget *_w = new StitchesConfigWidget(parent);
  if (_w == nullptr)
    qFatal("Unable to load StitchesConfigWidget!");

  return _w;
}

bool TabStitches::addIndexOnInit() const { return true; }

AntiquaCRM::TabsIndex *TabStitches::indexWidget(QWidget *parent) const {
  StitchesWidget *_w = new StitchesWidget(parent);
  if (_w == nullptr)
    qFatal("Unable to load StitchesWidget!");

  return _w;
}

bool TabStitches::createInterface(QObject *parent) {
  return (parent != nullptr);
}
