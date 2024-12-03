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
#include <QTableWidget>
#include <QWidget>

class DescWordsTableModel;

class ArticleDescWordsTable final : public AntiquaCRM::TableView {
  Q_OBJECT

private:
  QString p_wehreClause;
  QSqlRecord p_tableRecord;
  DescWordsTableModel* m_model;

  virtual qint64 getTableID(const QModelIndex& index, int column = 0);

private Q_SLOTS:
  virtual void contextMenuAction(AntiquaCRM::TableContextMenu::Actions, const QModelIndex&);
  virtual void setSortByColumn(int column, Qt::SortOrder order);
  virtual void getSelectedItem(const QModelIndex&);
  virtual void createSocketOperation(const QModelIndex&);
  virtual bool sqlModelQuery(const QString& query);

public Q_SLOTS:
  virtual void setReloadView();

public:
  explicit ArticleDescWordsTable(QWidget* parent = nullptr);
  virtual int rowCount();
  virtual bool setQuery(const QString& clause = QString());
  virtual const QString defaultWhereClause();
};

#endif // ANTIQUACRM_UTILS_ARTICLEDESCWORDSTABLE_H
