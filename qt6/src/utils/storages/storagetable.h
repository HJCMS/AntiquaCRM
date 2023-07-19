// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_STORAGETABLE_H
#define ANTIQUACRM_UTILS_STORAGETABLE_H

#include <AntiquaWidgets>
#include <QModelIndex>
#include <QTableView>
#include <QWidget>

#include "storageutil.h"

class StorageTableModel;

class StorageTable final : public QTableView {
  Q_OBJECT

private:
  AntiquaCRM::TableHeader *m_header;
  StorageTableModel *m_model;

private Q_SLOTS:
  void itemClicked(const QModelIndex &);

Q_SIGNALS:
  void queryMessages(const QString &);
  void itemChanged(const StorageItemData &);

public Q_SLOTS:
  void postQuery(const QString &query);
  void findColumn(const QString &);

public:
  explicit StorageTable(QWidget *parent = nullptr);
  bool initTable();
};

#endif // ANTIQUACRM_UTILS_STORAGETABLE_H
