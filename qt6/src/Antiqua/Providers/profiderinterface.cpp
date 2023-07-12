// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "profiderinterface.h"

#ifndef ANTIQUACRM_PLUGIN_CONFIGWIDGET_H
#include "apluginconfigwidget.h"
#endif

namespace AntiquaCRM {

ProviderInterface::ProviderInterface(QObject *parent) : QObject{parent} {}

bool ProviderInterface::createInterface(QObject *parent) {
  Q_UNUSED(parent);
  return false;
}

} // namespace AntiquaCRM
