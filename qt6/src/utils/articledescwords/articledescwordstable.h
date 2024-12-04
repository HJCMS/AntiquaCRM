// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_ARTICLEDESCWORDSTABLE_H
#define ANTIQUACRM_UTILS_ARTICLEDESCWORDSTABLE_H

#include <AntiquaWidgets>
#include <QContextMenuEvent>
#include <QHeaderView>
#include <QSqlRecord>

class DescWordsTableModel;

class ArticleDescWordsTable final : public AntiquaCRM::TableView {
  Q_OBJECT

private:
  QString p_wehreClause;
  QSqlRecord p_tableRecord;
  QHeaderView* m_header;
  DescWordsTableModel* m_model;

  qint64 getTableID(const QModelIndex& index, int column = 0) override;

private Q_SLOTS:
  void contextMenuAction(AntiquaCRM::TableContextMenu::Actions, const QModelIndex&) override;
  void contextMenuEvent(QContextMenuEvent*) override;
  void setSortByColumn(int column, Qt::SortOrder order) override;
  void getSelectedItem(const QModelIndex&) override;
  void createSocketOperation(const QModelIndex&) override;
  bool sqlModelQuery(const QString& query) override;

public Q_SLOTS:
  void setReloadView() override;

public:
  explicit ArticleDescWordsTable(QWidget* parent = nullptr);
  int rowCount() override;
  bool setQuery(const QString& clause = QString()) override;
  const QString defaultWhereClause() override;
  const QString tableName();
  qint64 getItemId(const QModelIndex&);
};

#endif // ANTIQUACRM_UTILS_ARTICLEDESCWORDSTABLE_H
