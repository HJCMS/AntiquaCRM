// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooksconfigwidget.h"

#include <QLayout>

AbeBooksConfigWidget::AbeBooksConfigWidget(QWidget *parent)
    : AntiquaCRM::PluginConfigWidget{"providers", "abebooks", parent} {
  setObjectName("config_abebooks");

  QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom, this);

  setLayout(layout);
}

void AbeBooksConfigWidget::loadSectionConfig() { qDebug() << Q_FUNC_INFO; }

void AbeBooksConfigWidget::saveSectionConfig() { qDebug() << Q_FUNC_INFO; }

AntiquaCRM::PluginConfigWidget::ConfigType
AbeBooksConfigWidget::getType() const {
  return AntiquaCRM::PluginConfigWidget::ConfigType::CONFIG_DATABASE;
}

const QJsonObject AbeBooksConfigWidget::getMenuEntry() const {
  QJsonObject _jo;
  _jo.insert("title", tr("AbeBooks"));
  _jo.insert("id", tr("abebooks_tab"));
  _jo.insert("icon", tr("antiquacrm"));
  return _jo;
}
