// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_STORAGETABLE_H
#define ANTIQUACRM_UTILS_STORAGETABLE_H

#include <AntiquaWidgets>
#include <QContextMenuEvent>
#include <QModelIndex>
#include <QSqlError>
#include <QSqlRecord>
#include <QWidget>

#include "storageutil.h"

class StorageTableModel;

class StorageTable final : public AntiquaCRM::TableView {
  Q_OBJECT

private:
  AntiquaCRM::TableHeader *m_header;
  StorageTableModel *m_model;
  qint64 getTableID(const QModelIndex &index, int column = 0) override;
  void contextMenuEvent(QContextMenuEvent *event) override;
  void contextMenuAction(AntiquaCRM::TableContextMenu::Actions ac,
                         const QModelIndex &index) override;
  void createSocketOperation(const QModelIndex &) override;
  bool sqlModelQuery(const QString &query) override;
  bool askBeforeDelete();

private Q_SLOTS:
  void setSortByColumn(int column, Qt::SortOrder order) override;
  void getSelectedItem(const QModelIndex &) override;

Q_SIGNALS:
  void sendCreateEntry();
  void queryMessages(const QString &);
  void itemChanged(const StorageItemData &);
  void tableRecordChanged(const QSqlRecord &);

public Q_SLOTS:
  void findColumn(const QString &);
  void setReloadView() override;

public:
  explicit StorageTable(QWidget *parent = nullptr);
  int rowCount() override;
  bool setQuery(const QString &clause = QString()) override;
  const QString defaultWhereClause() override;
  bool initTable();
};

#endif // ANTIQUACRM_UTILS_STORAGETABLE_H
