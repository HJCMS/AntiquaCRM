// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_LISTVIEW_H
#define ANTIQUACRM_PLUGIN_LISTVIEW_H

#include <QJsonObject>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QString>
#include <QWidget>

class PluginListWidgetItem final : public QListWidgetItem {
private:
  const QString p_id;

public:
  explicit PluginListWidgetItem(const QString id, QListWidget *parent);
  inline const QString id() { return p_id; }
  void setChecked(bool b = false);
  bool getChecked();
};

class PluginListWidget final : public QListWidget {
  Q_OBJECT

private:
  PluginListWidgetItem *rowItem(int r) const;

private Q_SLOTS:
  void switchItemState(QListWidgetItem *);

public Q_SLOTS:
  void addListWidgetItem(const QJsonObject &jso);

public:
  explicit PluginListWidget(QWidget *parent = nullptr);

  /**
   * @brief set Plugins CheckStateRole with SerialId
   * @param map - <SerialId,Status>
   */
  void setStatus(const QMap<QString, bool> &map);

  /**
   * @brief get Plugins with SerialId and isChecked
   */
  const QMap<QString, bool> getStatus();

  /**
   * @brief get Plugins sortings with SerialId
   */
  const QMap<QString, int> getSort();
};

#endif // ANTIQUACRM_PLUGIN_LISTVIEW_H
