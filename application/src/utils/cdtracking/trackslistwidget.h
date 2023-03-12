// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef CDTRACKING_TRACKSLISTWIDGET_H
#define CDTRACKING_TRACKSLISTWIDGET_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QJsonObject>
#include <QObject>
#include <QWidget>

#include "discinfo.h"

/**
 * @brief The TracksListWidget class
 * @ingroup _cdtracking
 */
class TracksListWidget final : public QTreeWidget {
  Q_OBJECT

private:
  void createEntry(const DiscInfo::Track &track);

public:
  explicit TracksListWidget(QWidget *parent = nullptr);
  void setTracks(const QList<DiscInfo::Track> &);
  const QJsonObject getTracks();
};

#endif // CDTRACKING_TRACKSLISTWIDGET_H
