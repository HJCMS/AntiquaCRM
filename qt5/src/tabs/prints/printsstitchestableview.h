// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_PRINTSSTITCHES_TABLEVIEW_H
#define ANTIQUACRM_PRINTSSTITCHES_TABLEVIEW_H

#include <QContextMenuEvent>
#include <QIcon>
#include <QModelIndex>
#include <QObject>
#include <QWidget>

#ifndef ANTIQUACRM_INVENTORYTABLE_H
#include "inventorytable.h"
#endif

class PrintsStitchesModel;

/**
 * @brief Inventory Prints Table view
 * @ingroup _prints
 */
class PrintsStitchesTableView final : public InventoryTable {
  Q_OBJECT

private:
  PrintsStitchesModel *m_model;
  QString where_clause;
  QSqlRecord p_tableRecord;

  qint64 getTableID(const QModelIndex &index, int column = 0) override;
  int getArticleCount(const QModelIndex &index);
  bool sqlModelQuery(const QString &query) override;
  void contextMenuEvent(QContextMenuEvent *) override;

private Q_SLOTS:
  void contextMenuAction(TableContextMenu::Actions,
                         const QModelIndex &) override;
  void setSortByColumn(int column, Qt::SortOrder order) override;
  void getSelectedItem(const QModelIndex &) override;
  void createSocketOperation(const QModelIndex &) override;

public Q_SLOTS:
  void setReloadView() override;

public:
  explicit PrintsStitchesTableView(QWidget *parent = nullptr);
  int rowCount() override;
  bool setQuery(const QString &clause = QString()) override;
  const QString defaultWhereClause() override;
};

#endif // ANTIQUACRM_PRINTSSTITCHES_TABLEVIEW_H
