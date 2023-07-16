// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordersconfigwidget.h"
#include "ordersconfig.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QLayout>

OrdersConfigWidget::OrdersConfigWidget(QWidget *parent)
    : AntiquaCRM::PluginConfigWidget{ORDERS_INTERFACE_NAME, parent} {
  setContentsMargins(0, 0, 0, 0);

  QJsonObject _jobj = getMenuEntry();
  setWindowTitle(_jobj.value("title").toString() + " [*]");
  setToolTip(_jobj.value("tooltip").toString());

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(5, 5, 5, 5);

  layout->addStretch(1);
  setLayout(layout);
}

void OrdersConfigWidget::loadSectionConfig() {}

void OrdersConfigWidget::saveSectionConfig() {}

AntiquaCRM::ConfigType OrdersConfigWidget::getType() const {
  return AntiquaCRM::ConfigType::CONFIG_SYSTEM;
}

const QJsonObject OrdersConfigWidget::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "view-financial-transfer");
  _o.insert("title", tr("Orders"));
  _o.insert("tooltip", tr("Additional Orders settings."));
  return _o;
}
