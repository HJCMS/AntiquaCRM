// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "pluginlistwidgetitem.h"
#include "antiquaicon.h"

#include <QIcon>
#include <QJsonValue>

PluginListWidgetItem::PluginListWidgetItem(const QJsonObject &meta,
                                           QListWidget *parent)
    : QListWidgetItem{parent, QListWidgetItem::UserType}, p_metaData{meta} {
  QString _title = option("Title").toString();
  _title.append(" - ");
  _title.append(option("Description").toString());
  setData(Qt::DisplayRole, _title);
  setData(Qt::DecorationRole, AntiquaCRM::antiquaIcon("bookmark"));
  setData(Qt::CheckStateRole, Qt::Unchecked);
  setFlags(Qt::ItemIsSelectable | Qt::ItemNeverHasChildren);
  if (canAddOnStart()) {
    setFlags(flags() | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable);
  }
}

void PluginListWidgetItem::toggleState() {
  if (checkState() == Qt::Checked) {
    setData(Qt::CheckStateRole, Qt::Unchecked);
  } else {
    setData(Qt::CheckStateRole, Qt::Checked);
  }
}

const QJsonObject PluginListWidgetItem::metaData() const { return p_metaData; }

const QVariant PluginListWidgetItem::option(const QString &param) const {
  return p_metaData.value(param).toVariant();
}

bool PluginListWidgetItem::canAddOnStart() {
  return p_metaData.value("Selectable").toBool(false);
}

void PluginListWidgetItem::setChecked(bool b) {
  setData(Qt::CheckStateRole, ((b) ? Qt::Checked : Qt::Unchecked));
}

bool PluginListWidgetItem::getChecked() {
  // enum Qt::CheckState
  return (data(Qt::CheckStateRole).toInt() == Qt::Checked);
}
