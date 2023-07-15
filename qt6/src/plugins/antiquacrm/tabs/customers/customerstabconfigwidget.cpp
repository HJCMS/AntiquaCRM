// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customerstabconfigwidget.h"
#include "customersconfig.h"

#include <QJsonObject>
#include <QLabel>
#include <QVBoxLayout>

CustomersTabConfigWidget::CustomersTabConfigWidget(QWidget *parent)
    : AntiquaCRM::PluginConfigWidget{CUSTOMERS_INTERFACE_NAME, parent} {
  setContentsMargins(0, 0, 0, 0);

  QJsonObject _jobj = getMenuEntry();
  setWindowTitle(_jobj.value("title").toString() + " [*]");
  setToolTip(_jobj.value("tooltip").toString());

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(5, 5, 5, 5);
  QString _txt = tr("This plugin don't have any configuration options.");
  QLabel *info = new QLabel(this);
  info->setText("<h4>" + _txt + "</h4>");
  layout->addWidget(info);

  layout->addStretch(1);
  setLayout(layout);
}

void CustomersTabConfigWidget::loadSectionConfig() { /* UNUSED */
}

void CustomersTabConfigWidget::saveSectionConfig() { /* UNUSED */
}

AntiquaCRM::ConfigType
CustomersTabConfigWidget::getType() const {
  return AntiquaCRM::ConfigType::CONFIG_DATABASE;
}

const QJsonObject CustomersTabConfigWidget::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "system-users");
  _o.insert("title", tr("Customers"));
  _o.insert("tooltip", tr("Additional Customers tab settings."));
  return _o;
}
