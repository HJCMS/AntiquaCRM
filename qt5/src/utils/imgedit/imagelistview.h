// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUA_IMAGELISTVIEW_H
#define ANTIQUA_IMAGELISTVIEW_H

#include <QDir>
#include <QModelIndex>
#include <QFileSystemModel>
#include <QTreeView>
#include <QHeaderView>
#include <QObject>
#include <QWidget>

#include "sourceinfo.h"

/**
 * @ingroup _imgedit
 * @brief Image Tree list view
 */
class ImageListView final : public QTreeView {
  Q_OBJECT

private:
  QDir p_directory;
  QString p_selection;
  QModelIndex p_rootIndex;
  QHeaderView *m_headerView;
  QFileSystemModel *m_model;
  void currentChanged(const QModelIndex &current, const QModelIndex &previous) override;

private Q_SLOTS:
  void targetChanged(const QString &path);
  void folderChanged(const QModelIndex &index);

Q_SIGNALS:
  void sendTargetChanged(const QString &path);
  void imageSelected(const SourceInfo &image);

public Q_SLOTS:
  void expandTopLevel();
  void setSourceImage(const SourceInfo &image);

public:
  explicit ImageListView(QWidget *parent = nullptr);
  bool setDirectory(const QDir &dir);
  static const QStringList viewFilter();
};

#endif // ANTIQUA_IMAGELISTVIEW_H
