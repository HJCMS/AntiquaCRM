// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "taborders.h"
#include "ordersconfig.h"
#include "ordersconfigwidget.h"
#include "orderswidget.h"

TabOrders::TabOrders(QObject *parent) : AntiquaCRM::TabsInterface{parent} {
  setObjectName("static_tab_orders");
}

const QString TabOrders::displayName() const { return tr("Orders"); }

const QString TabOrders::interfaceName() const { return ORDERS_INTERFACE_NAME; }

const QString TabOrders::sqlTableName() const { return ORDERS_SQL_TABLE_NAME; }

const QString TabOrders::sqlFieldPrefix() const {
  return ORDERS_SQL_FIELD_PREFIX;
}

AntiquaCRM::PluginConfigWidget *TabOrders::configWidget(QWidget *parent) const {
  OrdersConfigWidget *_widget = new OrdersConfigWidget(parent);
  Q_CHECK_PTR(_widget);
  return _widget;
}

bool TabOrders::addIndexOnInit() const { return true; }

const QJsonObject TabOrders::menuEntry() const {
  QJsonObject _jo;
  _jo.insert("title", displayName());
  _jo.insert("id", QString(ORDERS_INTERFACE_TABID));
  _jo.insert("tip", tr("Inventory orders"));
  _jo.insert("icon", "view-financial-transfer");
  return _jo;
}

AntiquaCRM::TabsIndex *TabOrders::indexWidget(QWidget *parent) const {
  OrdersWidget *_w = new OrdersWidget(parent);
  _w->setWindowTitle(displayName() + "[*]");
  return _w;
}

bool TabOrders::createInterface(QObject *parent) {
  Q_UNUSED(parent);
  return true;
}
