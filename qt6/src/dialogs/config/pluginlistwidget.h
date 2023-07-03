// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_LISTVIEW_H
#define ANTIQUACRM_PLUGIN_LISTVIEW_H

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QJsonObject>
#include <QList>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMouseEvent>
#include <QString>
#include <QWidget>

class PluginListWidget;

class PluginListWidget final : public QListWidget {
  Q_OBJECT

private:
  QPoint p_moveStartPosi;
  void mousePressEvent(QMouseEvent *event) override;
  void dragEnterEvent(QDragEnterEvent *event) override;
  void dropEvent(QDropEvent *event) override;

private Q_SLOTS:
  void setPluginSort(const QModelIndexList &);

public Q_SLOTS:
  void insertPlugin(const QJsonObject &jso);

public:
  explicit PluginListWidget(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_PLUGIN_LISTVIEW_H
