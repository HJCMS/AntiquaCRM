// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabstatistics.h"
#include "statisticsconfigwidget.h"
#include "statisticsindex.h"

TabStatistics::TabStatistics(QObject *parent)
    : AntiquaCRM::TabsInterface{parent} {
  setObjectName("statistics_plugin");
}

const QString TabStatistics::displayName() const { return tr("Statistics"); }

const QString TabStatistics::interfaceName() const {
  return QString("statistics");
}

const QString TabStatistics::sqlTableName() const { return QString(); }

const QString TabStatistics::sqlFieldPrefix() const { return QString(); }

AntiquaCRM::PluginConfigWidget *
TabStatistics::configWidget(QWidget *parent) const {
  StatisticsConfigWidget *_w = new StatisticsConfigWidget(parent);
  return _w;
}

bool TabStatistics::addIndexOnInit() const { return false; }

const QJsonObject TabStatistics::menuEntry() const {
  QJsonObject _jo;
  _jo.insert("title", displayName());
  _jo.insert("id", "statistics_tab");
  _jo.insert("tip", tr("Database Statistics"));
  _jo.insert("icon", "office-chart-area");
  return _jo;
}

AntiquaCRM::TabsIndex *TabStatistics::indexWidget(QWidget *parent) const {
  StatisticsIndex *_w = new StatisticsIndex(parent);
  _w->setWindowTitle(displayName() + "[*]");
  return _w;
}

bool TabStatistics::createInterface(QObject *parent) {
  return (parent != nullptr);
}
