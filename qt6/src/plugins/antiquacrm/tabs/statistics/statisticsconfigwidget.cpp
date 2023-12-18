// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "statisticsconfigwidget.h"

#include <QLabel>

StatisticsConfigWidget::StatisticsConfigWidget(QWidget *parent)
    : AntiquaCRM::PluginConfigWidget{"statistics", parent} {
  setContentsMargins(0, 0, 0, 0);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(5, 5, 5, 5);
  QString _info = tr("This plugin does not need any configuration!");
  layout->addWidget(new QLabel(_info, this));
  layout->addStretch(1);
  setLayout(layout);
}

void StatisticsConfigWidget::loadSectionConfig() {
  // CONFIG_STATISTICS_TAB
}

void StatisticsConfigWidget::saveSectionConfig() {
  // CONFIG_STATISTICS_TAB
}

AntiquaCRM::ConfigType StatisticsConfigWidget::getType() const {
  return AntiquaCRM::ConfigType::CONFIG_DATABASE;
}

const QJsonObject StatisticsConfigWidget::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "office-chart-area");
  _o.insert("title", tr("Statistics"));
  _o.insert("tooltip", tr("Additional Statistics settings."));
  return _o;
}
