// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabsellers.h"
#include "sellersconfig.h"
#include "sellersconfigwidget.h"
#include "sellerswidget.h"

TabSellers::TabSellers(QObject *parent) : AntiquaCRM::TabsInterface{parent} {
  setObjectName("static_tab_sellers");
}

const QString TabSellers::displayName() const { return tr("Providers"); }

const QString TabSellers::interfaceName() const {
  return SELLERS_INTERFACE_NAME;
}

const QString TabSellers::sqlTableName() const {
  return SELLERS_SQL_TABLE_NAME;
}

const QString TabSellers::sqlFieldPrefix() const {
  return SELLERS_SQL_FIELD_PREFIX;
}

AntiquaCRM::PluginConfigWidget *
TabSellers::configWidget(QWidget *parent) const {
  SellersConfigWidget *_w = new SellersConfigWidget(parent);
  return _w;
}

bool TabSellers::addIndexOnInit() const { return true; }

const QJsonObject TabSellers::menuEntry() const {
  QJsonObject _jo;
  _jo.insert("title", displayName());
  _jo.insert("id", QString(SELLERS_INTERFACE_TABID));
  _jo.insert("tip", tr("Inventory Sellers"));
  _jo.insert("icon", "view-financial-transfer");
  return _jo;
}

AntiquaCRM::TabsIndex *TabSellers::indexWidget(QWidget *parent) const {
  SellersWidget *_w = new SellersWidget(parent);
  _w->setWindowTitle(displayName() + "[*]");
  return _w;
}

bool TabSellers::createInterface(QObject *parent) {
  Q_UNUSED(parent);
  return true;
}
