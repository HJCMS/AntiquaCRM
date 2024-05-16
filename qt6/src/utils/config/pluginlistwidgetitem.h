// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_LISTWIDGETITEM_H
#define ANTIQUACRM_PLUGIN_LISTWIDGETITEM_H

#include <QJsonObject>
#include <QListWidget>
#include <QListWidgetItem>
#include <QVariant>

class PluginListWidgetItem final : public QListWidgetItem {

private:
  const QJsonObject p_metaData;

public:
  explicit PluginListWidgetItem(const QJsonObject &meta, QListWidget *parent);
  void toggleState();
  const QJsonObject metaData() const;
  const QVariant option(const QString &param = QString("SerialId")) const;
  bool canAddOnStart();
  void setChecked(bool b = false);
  bool getChecked();
};

#endif // ANTIQUACRM_PLUGIN_LISTWIDGETITEM_H
