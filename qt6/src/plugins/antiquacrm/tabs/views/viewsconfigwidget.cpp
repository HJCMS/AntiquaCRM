// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "viewsconfigwidget.h"

ViewsConfigWidget::ViewsConfigWidget(QWidget *parent)
    : AntiquaCRM::PluginConfigWidget{"views", parent} {
  setContentsMargins(0, 0, 0, 0);
  int row = 0;
  QGridLayout *layout = new QGridLayout(this);
  layout->setContentsMargins(contentsMargins());
  layout->setColumnStretch(0, 1);
  layout->setRowStretch(row, 1);
  setLayout(layout);
}

void ViewsConfigWidget::loadSectionConfig() {
  // Nothing todo settings
}

void ViewsConfigWidget::saveSectionConfig() {
  // Nothing todo settings
}

AntiquaCRM::ConfigType ViewsConfigWidget::getType() const {
  return AntiquaCRM::ConfigType::CONFIG_SYSTEM;
}

const QJsonObject ViewsConfigWidget::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "view-list-details");
  _o.insert("title", tr("Views"));
  _o.insert("tooltip", tr("Additional Views settings."));
  return _o;
}
