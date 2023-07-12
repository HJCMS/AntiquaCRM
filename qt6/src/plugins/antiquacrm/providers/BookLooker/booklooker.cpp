// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklooker.h"
#include "booklookerconfigwidget.h"

#include <AntiquaWidgets>
#include <QDebug>

BookLooker::BookLooker(QObject *parent) : AntiquaCRM::ProviderInterface{parent} {
  setObjectName("provider_plugin_booklooker");
}

const QString BookLooker::displayName() const { return QString("AbeBooks"); }

const QString BookLooker::interfaceName() const { return QString("AbeBooks"); }

AntiquaCRM::PluginConfigWidget *BookLooker::configWidget(QWidget *parent) const {
  return (new BookLookerConfigWidget(parent));
}

const QJsonObject BookLooker::menuEntry() const {
  QJsonObject _jo;
  _jo.insert("title", tr("BookLooker"));
  _jo.insert("id", tr("booklooker_tab"));
  _jo.insert("icon", tr("antiquacrm"));
  return _jo;
}

bool BookLooker::createInterface(QObject *parent) {
  Q_UNUSED(parent);
  return true;
}
