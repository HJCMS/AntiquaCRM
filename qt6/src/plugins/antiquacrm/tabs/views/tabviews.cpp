// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabviews.h"
#include "viewsconfigwidget.h"
#include "viewsindex.h"

TabViews::TabViews(QObject *parent) : AntiquaCRM::TabsInterface{parent} {
  setObjectName("views_plugin");
}

const QString TabViews::displayName() const { return tr("Views"); }

const QString TabViews::interfaceName() const { return QString("views"); }

const QString TabViews::sqlTableName() const { return QString(); }

const QString TabViews::sqlFieldPrefix() const { return QString(); }

AntiquaCRM::PluginConfigWidget *TabViews::configWidget(QWidget *parent) const {
  ViewsConfigWidget *_w = new ViewsConfigWidget(parent);
  return _w;
}

bool TabViews::addIndexOnInit() const { return false; }

const QJsonObject TabViews::menuEntry() const {
  QJsonObject _jo;
  _jo.insert("title", displayName());
  _jo.insert("id", "views_tab");
  _jo.insert("tip", tr("Database Views"));
  _jo.insert("icon", "office-chart-area");
  return _jo;
}

AntiquaCRM::TabsIndex *TabViews::indexWidget(QWidget *parent) const {
  ViewsIndex *_w = new ViewsIndex(parent);
  _w->setWindowTitle(displayName() + "[*]");
  return _w;
}

bool TabViews::createInterface(QObject *parent) { return (parent != nullptr); }
