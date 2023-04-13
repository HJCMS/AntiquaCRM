// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PRIVATE_IMAGETREEVIEW_H
#define ANTIQUACRM_PRIVATE_IMAGETREEVIEW_H

#include <AGlobal>
#include <QDir>
#include <QFileSystemModel>
#include <QModelIndex>
#include <QObject>
#include <QTreeView>
#include <QWidget>

namespace AntiquaCRM {

class ImageFileSource;

class ANTIQUACRM_LIBRARY ImageTreeView final : public QTreeView {
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
  void sendPathChanged(const QDir &);
  void sendSelected(const AntiquaCRM::ImageFileSource &);

public Q_SLOTS:
  void expandTopLevel();
  void setSourceImage(const AntiquaCRM::ImageFileSource &image);
  void setChangeRoot(const QDir &);

public:
  explicit ImageTreeView(QWidget *parent = nullptr);
  bool setDirectory(const QDir &dir);
  static const QStringList viewFilter();
};

} // namespace AntiquaCRM

#endif // ANTIQUACRM_PRIVATE_IMAGETREEVIEW_H
