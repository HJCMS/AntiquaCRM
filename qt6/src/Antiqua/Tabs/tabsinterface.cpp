// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabsinterface.h"

#ifndef ANTIQUACRM_TABS_INDEX_H
#  include "tabsindex.h"
#endif

#ifndef ANTIQUACRM_PLUGIN_CONFIGWIDGET_H
#  include "apluginconfigwidget.h"
#endif

#include <QDebug>

namespace AntiquaCRM
{

TabsInterface::TabsInterface(QObject* parent) : QObject{parent}, serialId{"UNKNOWN_PLUGIN_SERIAL"} {
}

const QJsonObject TabsInterface::menuObject() {
  QJsonObject _jo;
  _jo.insert("id", QJsonValue(QJsonValue::String));
  _jo.insert("title", QJsonValue(QJsonValue::String));
  _jo.insert("tip", QJsonValue(QJsonValue::String));
  _jo.insert("icon", QJsonValue(QJsonValue::String));
  return _jo;
}

void TabsInterface::setSerialId(const QString& id) {
  serialId = id;
  emit sendSerialIdChanged(serialId);
}

const QString TabsInterface::getSerialId() {
  return serialId;
}

bool TabsInterface::createInterface(QObject* parent) {
  Q_UNUSED(parent);
  return false;
}

} // namespace AntiquaCRM
