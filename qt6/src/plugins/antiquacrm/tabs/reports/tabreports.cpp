// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabreports.h"
#include "reportsconfigwidget.h"
#include "reportsindex.h"

TabReports::TabReports(QObject *parent) : AntiquaCRM::TabsInterface{parent} {
  setObjectName("reports_plugin");
}

const QString TabReports::displayName() const { return tr("Monthly report"); }

const QString TabReports::interfaceName() const { return QString("reports"); }

const QString TabReports::sqlTableName() const { return QString(); }

const QString TabReports::sqlFieldPrefix() const { return QString(); }

AntiquaCRM::PluginConfigWidget *
TabReports::configWidget(QWidget *parent) const {
  ReportsConfigWidget *_w = new ReportsConfigWidget(parent);
  return _w;
}

bool TabReports::addIndexOnInit() const { return false; }

const QJsonObject TabReports::menuEntry() const {
  QJsonObject _jo;
  _jo.insert("title", displayName());
  _jo.insert("id", "reports_tab");
  _jo.insert("tip", tr("Monthly report"));
  _jo.insert("icon", "x-office-spreadsheet");
  return _jo;
}

AntiquaCRM::TabsIndex *TabReports::indexWidget(QWidget *parent) const {
  ReportsIndex *_w = new ReportsIndex(parent);
  _w->setWindowTitle(displayName() + "[*]");
  return _w;
}

bool TabReports::createInterface(QObject *parent) {
  return (parent != nullptr);
}
