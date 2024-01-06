// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooks.h"
#include "abebooksactionsdialog.h"
#include "abebooksconfigwidget.h"

#include <AntiquaWidgets>
#include <QDebug>

AbeBooks::AbeBooks(QObject *parent) : AntiquaCRM::ProviderInterface{parent} {
  setObjectName("provider_plugin_abebooks");
}

const QString AbeBooks::displayName() const { return QString("AbeBooks"); }

const QString AbeBooks::interfaceName() const { return QString("AbeBooks"); }

AntiquaCRM::PluginConfigWidget *AbeBooks::configWidget(QWidget *parent) const {
  return (new AbeBooksConfigWidget(parent));
}

bool AbeBooks::operationWidget(QWidget *parent,
                               const QJsonObject &order) const {
  AbeBooksActionsDialog *d = new AbeBooksActionsDialog(parent);
  if (d->exec(order) == QDialog::Rejected)
    return false;

  return true;
}

const QJsonObject AbeBooks::menuEntry() const {
  QJsonObject _jo;
  _jo.insert("title", "AbeBooks");
  _jo.insert("id", "abebooks_tab");
  _jo.insert("icon", "antiquacrm");
  return _jo;
}

bool AbeBooks::createInterface(QObject *parent) {
  Q_UNUSED(parent);
  return true;
}
