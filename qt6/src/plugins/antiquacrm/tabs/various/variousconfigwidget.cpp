// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "variousconfigwidget.h"
#include "variousconfig.h"

VariousConfigWidget::VariousConfigWidget(QWidget *parent)
    : AntiquaCRM::PluginConfigWidget{"Various", parent} {
  setContentsMargins(0, 0, 0, 0);
}

void VariousConfigWidget::loadSectionConfig() {
  qDebug() << Q_FUNC_INFO << "TODO" << VARIOUS_CONFIG_POINTER;
}

void VariousConfigWidget::saveSectionConfig() {
  qDebug() << Q_FUNC_INFO << "TODO" << VARIOUS_CONFIG_POINTER;
}

AntiquaCRM::ConfigType VariousConfigWidget::getType() const {
  return AntiquaCRM::ConfigType::CONFIG_DATABASE;
}

const QJsonObject VariousConfigWidget::getMenuEntry() const {
  QJsonObject _o;
  _o.insert("icon", "code-variable");
  _o.insert("title", tr("Various"));
  _o.insert("tooltip", tr("Additional Varioustab settings."));
  return _o;
}
