// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabcustomers.h"
#include "customersconfig.h"
#include "customerstabconfigwidget.h"
#include "customerswidget.h"

TabCustomers::TabCustomers(QObject *parent)
    : AntiquaCRM::TabsInterface{parent} {
  setObjectName("static_tab_customers");
}

const QString TabCustomers::displayName() const { return tr("Customers"); }

const QString TabCustomers::interfaceName() const {
  return CUSTOMERS_INTERFACE_NAME;
}

const QString TabCustomers::sqlTableName() const {
  return CUSTOMERS_SQL_TABLE_NAME;
}

const QString TabCustomers::sqlFieldPrefix() const {
  return CUSTOMERS_SQL_FIELD_PREFIX;
}

AntiquaCRM::PluginConfigWidget *
TabCustomers::configWidget(QWidget *parent) const {
  CustomersTabConfigWidget *_w = new CustomersTabConfigWidget(parent);
  return _w;
}

bool TabCustomers::addIndexOnInit() const {
  return CUSTOMERS_ALWAYS_ADD_ONLOAD;
}

const QJsonObject TabCustomers::menuEntry() const {
  QJsonObject _jo;
  _jo.insert("id", tr("customers_tab"));
  _jo.insert("title", tr("Customers"));
  _jo.insert("tip", tr("Inventory Customers"));
  _jo.insert("icon", tr("system-users"));
  return _jo;
}

AntiquaCRM::TabsIndex *TabCustomers::indexWidget(QWidget *parent) const {
  CustomersWidget *_w = new CustomersWidget(parent);
  return _w;
}

bool TabCustomers::createInterface(QObject *parent) {
  Q_UNUSED(parent);
  return true;
}
