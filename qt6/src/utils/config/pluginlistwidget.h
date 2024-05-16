// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_LISTVIEW_H
#define ANTIQUACRM_PLUGIN_LISTVIEW_H

#include <QJsonObject>
#include <QListWidget>
#include <QMap>
#include <QModelIndex>
#include <QString>
#include <QWidget>

class PluginListWidgetItem;

class PluginListWidget final : public QListWidget {
  Q_OBJECT

private:
  PluginListWidgetItem *rowItem(int r);
  const QString itemTip() const;

private Q_SLOTS:
  void changeState(const QModelIndex &index);

public Q_SLOTS:
  void addListItem(const QJsonObject &meta);

public:
  explicit PluginListWidget(QWidget *parent = nullptr);

  /**
   * @brief set Plugins CheckStateRole with SerialId
   * @param map - <SerialId,Status>
   */
  bool setStatus(const QMap<QString, bool> &map);

  /**
   * @brief get Plugins with SerialId and isChecked
   */
  const QMap<QString, bool> getStatus();

  /**
   * @brief get Plugins sortings with SerialId
   */
  const QMap<int, QString> getSort();
};

#endif // ANTIQUACRM_PLUGIN_LISTVIEW_H
