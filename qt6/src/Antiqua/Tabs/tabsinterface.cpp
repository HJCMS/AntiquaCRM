// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabsinterface.h"

#include <QDebug>

namespace AntiquaCRM {

TabsInterface::TabsInterface(QObject *parent) : QObject{parent} {
}

bool TabsInterface::createInterface(QObject *parent) {
  Q_UNUSED(parent);
  return false;
}

} // namespace AntiquaCRM
