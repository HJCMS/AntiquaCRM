// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklookerconfigwidget.h"

#include <QLayout>

BookLookerConfigWidget::BookLookerConfigWidget(QWidget *parent)
    : AntiquaCRM::PluginConfigWidget{"providers", "booklooker", parent} {
  setObjectName("config_booklooker");

  QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom, this);

  setLayout(layout);
}

void BookLookerConfigWidget::loadSectionConfig() { qDebug() << Q_FUNC_INFO; }

void BookLookerConfigWidget::saveSectionConfig() { qDebug() << Q_FUNC_INFO; }

AntiquaCRM::PluginConfigWidget::ConfigType
BookLookerConfigWidget::getType() const {
  return AntiquaCRM::PluginConfigWidget::ConfigType::CONFIG_DATABASE;
}

const QJsonObject BookLookerConfigWidget::getMenuEntry() const {
  QJsonObject _jo;
  _jo.insert("title", tr("BookLooker"));
  _jo.insert("id", tr("booklooker_tab"));
  _jo.insert("icon", tr("antiquacrm"));
  return _jo;
}
