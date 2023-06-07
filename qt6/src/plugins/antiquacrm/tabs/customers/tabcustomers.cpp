// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabcustomers.h"
#include "customersconfig.h"
#include "customerstabconfigwidget.h"
#include "customerswidget.h"

TabCustomers::TabCustomers(QObject *parent)
    : AntiquaCRM::TabsInterface{parent} {}

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

AntiquaCRM::TabsConfigWidget *
TabCustomers::configWidget(QWidget *parent) const {
  CustomersTabConfigWidget *_widget = new CustomersTabConfigWidget(parent);
  return _widget;
}

bool TabCustomers::addIndexOnInit() const {
  return CUSTOMERS_ALWAYS_ADD_ONLOAD;
}

AntiquaCRM::TabsIndex *TabCustomers::indexWidget(QWidget *parent) const {
  CustomersWidget *_widget = new CustomersWidget(parent);
  Q_CHECK_PTR(_widget);
  return _widget;
}

bool TabCustomers::createInterface(QObject *parent) {
  // TODO
  return (parent != nullptr);
}
