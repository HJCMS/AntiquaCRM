// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "configprovidersview.h"

#include <QLabel>
#include <QLayout>

ConfigProvidersView::ConfigProvidersView(QWidget *parent)
    : AntiquaCRM::TabsConfigWidget{"tabs", "config_providers", parent} {
  setWindowTitle(tr("Providers"));
  setObjectName("config_providers");
  setContentsMargins(5, 5, 5, 0);
  setAlignment(Qt::AlignTop | Qt::AlignLeft);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(5, 5, 5, 5);

  layout->addWidget(new QLabel("Config Providers", this));

  layout->addStretch(1);
  setLayout(layout);
}

void ConfigProvidersView::loadSectionConfig() {
  qDebug() << Q_FUNC_INFO << "TODO";
}

void ConfigProvidersView::saveSectionConfig() {
  qDebug() << Q_FUNC_INFO << "TODO";
}

AntiquaCRM::TabsConfigWidget::ConfigType ConfigProvidersView::getType() const {
  return AntiquaCRM::TabsConfigWidget::ConfigType::CONFIG_DATABASE;
}

const QJsonObject ConfigProvidersView::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "tab-detach");
  _o.insert("title", tr("Providers"));
  _o.insert("tooltip", tr("Configure Providers."));
  return _o;
}
