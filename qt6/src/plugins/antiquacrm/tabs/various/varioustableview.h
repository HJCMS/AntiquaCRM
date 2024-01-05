// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_VARIOUSTABLEVIEW_H
#define ANTIQUACRM_PLUGIN_VARIOUSTABLEVIEW_H

#include <AntiquaTabs>
#include <QObject>
#include <QWidget>

class VariousTableModel;

class ANTIQUACRM_LIBRARY VariousTableView final : public AntiquaCRM::TableView {
  Q_OBJECT

private:
  VariousTableModel *m_model;
  QString where_clause;
  QSqlRecord p_tableRecord;
  qint64 getTableID(const QModelIndex &index, int column = 0) override;
  bool sqlModelQuery(const QString &query) override;
  void contextMenuEvent(QContextMenuEvent *) override;

private Q_SLOTS:
  void contextMenuAction(AntiquaCRM::TableContextMenu::Actions,
                         const QModelIndex &) override;
  void setSortByColumn(int column, Qt::SortOrder order) override;
  void getSelectedItem(const QModelIndex &) override;
  void createSocketOperation(const QModelIndex &) override;

public Q_SLOTS:
  void setReloadView() override;

public:
  explicit VariousTableView(QWidget *parent = nullptr);
  int rowCount() override;
  bool setQuery(const QString &clause = QString()) override;
  const QString defaultWhereClause() override;
};

#endif // ANTIQUACRM_PLUGIN_VARIOUSTABLEVIEW_H
