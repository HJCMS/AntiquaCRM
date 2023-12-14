// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statisticsconfigwidget.h"

StatisticsConfigWidget::StatisticsConfigWidget(QWidget *parent)
    : AntiquaCRM::PluginConfigWidget{"CONFIG_STATISTICS_TAB", parent} {
  setContentsMargins(0, 0, 0, 0);
}

void StatisticsConfigWidget::loadSectionConfig() {}

void StatisticsConfigWidget::saveSectionConfig() {}

AntiquaCRM::ConfigType StatisticsConfigWidget::getType() const {
  return AntiquaCRM::ConfigType::CONFIG_SYSTEM;
}

const QJsonObject StatisticsConfigWidget::getMenuEntry() const {
  return QJsonObject();
}
