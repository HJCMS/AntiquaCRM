// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUA_IMAGETREEVIEW_H
#define ANTIQUA_IMAGETREEVIEW_H

#include <QDir>
#include <QFileSystemModel>
#include <QHeaderView>
#include <QModelIndex>
#include <QObject>
#include <QTreeView>
#include <QWidget>

#include "sourceinfo.h"

/**
 * @class ImageTreeView
 * @brief Image Tree list view
 * @ingroup _imgedit
 */
class ImageTreeView final : public QTreeView {
  Q_OBJECT

private:
  QDir p_directory;
  QString p_selection;
  QModelIndex p_rootIndex;
  QHeaderView *m_headerView;
  QFileSystemModel *m_model;
  void currentChanged(const QModelIndex &current,
                      const QModelIndex &previous) override;

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
  explicit ImageTreeView(QWidget *parent = nullptr);
  bool setDirectory(const QDir &dir);
  const QStringList filter() const;
  const QDir directory() const;
};

#endif // ANTIQUA_IMAGETREEVIEW_H
