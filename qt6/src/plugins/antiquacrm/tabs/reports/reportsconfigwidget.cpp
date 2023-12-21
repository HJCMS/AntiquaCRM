// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "reportsconfigwidget.h"

ReportsConfigWidget::ReportsConfigWidget(QWidget *parent)
    : AntiquaCRM::PluginConfigWidget{"reports", parent} {
  setContentsMargins(0, 0, 0, 0);
}

void ReportsConfigWidget::loadSectionConfig() {}

void ReportsConfigWidget::saveSectionConfig() {}

AntiquaCRM::ConfigType ReportsConfigWidget::getType() const {
  return AntiquaCRM::ConfigType::CONFIG_SYSTEM;
}

const QJsonObject ReportsConfigWidget::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "x-office-spreadsheet");
  _o.insert("title", tr("Reports"));
  _o.insert("tooltip", tr("Monthly report settings."));
  return _o;
}
