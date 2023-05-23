// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabstitches.h"
#include "stitchesconfig.h"
#include "stitchesconfigwidget.h"
#include "stitcheswidget.h"

TabStitches::TabStitches(QObject *parent) : AntiquaCRM::TabsInterface{parent} {}

const QString TabStitches::displayName() const {
  return tr("Prints & Stitches");
}

const QString TabStitches::interfaceName() const {
  return STITCHES_INTERFACE_NAME;
}

const QString TabStitches::sqlTableName() const {
  return QString("inventory_prints");
}

const QString TabStitches::sqlFieldPrefix() const {
  return STITCHES_SQL_FIELD_PREFIX;
}

AntiquaCRM::TabsConfigWidget *TabStitches::configWidget(QWidget *parent) const {
  StitchesConfigWidget *_widget = new StitchesConfigWidget(parent);
  return _widget;
}

bool TabStitches::addIndexOnInit() const { return STITCHES_ALWAYS_ADD_ONLOAD; }

AntiquaCRM::TabsIndex *TabStitches::indexWidget(QWidget *parent) const {
  StitchesWidget *_widget = new StitchesWidget(parent);
  Q_CHECK_PTR(_widget);
  return _widget;
}

bool TabStitches::createInterface(QObject *parent) {
  // TODO
  return (parent != nullptr);
}
