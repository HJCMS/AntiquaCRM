// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "aplugininterface.h"

namespace AntiquaCRM {

APluginInterface::APluginInterface(QObject *parent) : QObject{parent} {}

const QString APluginInterface::configPrefix() {
  return QString("provider/");
}

}; // namespace AntiquaCRM
