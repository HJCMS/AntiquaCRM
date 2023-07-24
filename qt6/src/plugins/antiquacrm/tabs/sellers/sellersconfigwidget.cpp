// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sellersconfigwidget.h"
#include "sellersconfig.h"

SellersConfigWidget::SellersConfigWidget(QWidget *parent)
    : AntiquaCRM::PluginConfigWidget{SELLERS_INTERFACE_NAME, parent} {
  setContentsMargins(0, 0, 0, 0);

  QJsonObject _jobj = getMenuEntry();
  setWindowTitle(_jobj.value("title").toString() + " [*]");
  setToolTip(_jobj.value("tooltip").toString());

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(5, 5, 5, 5);

  layout->addStretch(1);
  setLayout(layout);
}

void SellersConfigWidget::loadSectionConfig() {}

void SellersConfigWidget::saveSectionConfig() {}

AntiquaCRM::ConfigType SellersConfigWidget::getType() const {
  return AntiquaCRM::ConfigType::CONFIG_SYSTEM;
}

const QJsonObject SellersConfigWidget::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "view-financial-transfer");
  _o.insert("title", tr("Sales"));
  _o.insert("tooltip", tr("Additional Sales tab settings."));
  return _o;
}
