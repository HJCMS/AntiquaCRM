// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklooker.h"
#include "booklookeractions.h"
#include "booklookerconfigwidget.h"

#include <AntiquaWidgets>
#include <QDebug>

BookLooker::BookLooker(QObject *parent)
    : AntiquaCRM::ProviderInterface{parent} {
  setObjectName("provider_plugin_booklooker");
}

const QString BookLooker::displayName() const { return QString("Booklooker"); }

const QString BookLooker::interfaceName() const { return QString("booklooker"); }

AntiquaCRM::PluginConfigWidget *
BookLooker::configWidget(QWidget *parent) const {
  return (new BookLookerConfigWidget(parent));
}

bool BookLooker::operationWidget(QWidget *parent,
                                 const QJsonObject &order) const {
  BookLookerActions *d = new BookLookerActions(parent);
  if (d->exec(order) == QDialog::Rejected)
    return false;

  return true;
}

const QJsonObject BookLooker::menuEntry() const {
  QJsonObject _jo;
  _jo.insert("title", tr("Booklooker"));
  _jo.insert("id", tr("booklooker_tab"));
  _jo.insert("icon", tr("antiquacrm"));
  return _jo;
}

bool BookLooker::createInterface(QObject *parent) {
  Q_UNUSED(parent);
  return true;
}
