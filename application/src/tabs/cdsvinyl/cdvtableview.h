// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_CDVTABLEVIEW_H
#define ANTIQUACRM_CDVTABLEVIEW_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

class CDVTableModel;

/**
 * @brief CD+Vinyl Table view
 * @ingroup _cdvinyl
 */
class CDVTableView final : public InventoryTable {
  Q_OBJECT

private:
  CDVTableModel *m_model;
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
  explicit CDVTableView(QWidget *parent = nullptr);
  int rowCount() override;
  bool setQuery(const QString &clause = QString()) override;
  const QString defaultWhereClause() override;
};

#endif // ANTIQUACRM_CDVTABLEVIEW_H
