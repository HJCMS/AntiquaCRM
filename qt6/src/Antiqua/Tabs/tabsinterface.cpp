// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabsinterface.h"

#ifndef ANTIQUACRM_TABS_INDEX_H
#include "tabsindex.h"
#endif

#ifndef ANTIQUACRM_PLUGIN_CONFIGWIDGET_H
#include "apluginconfigwidget.h"
#endif

#include <QDebug>

namespace AntiquaCRM {

TabsInterface::TabsInterface(QObject *parent) : QObject{parent} {}

bool TabsInterface::createInterface(QObject *parent) {
  Q_UNUSED(parent);
  return false;
}

} // namespace AntiquaCRM
