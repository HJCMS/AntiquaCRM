// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_BOOKTABLEVIEW_H
#define ANTIQUACRM_BOOKTABLEVIEW_H

#include <QContextMenuEvent>
#include <QIcon>
#include <QObject>
#include <QWidget>

#ifndef ANTIQUACRM_INVENTORYTABLE_H
#include "inventorytable.h"
#endif

class BooksTableModel;

class BookTableView final : public InventoryTable {
  Q_OBJECT

private:
  BooksTableModel *m_model;
  QString where_clause;
  QModelIndex p_modelIndex;
  qint64 getTableID(const QModelIndex &index) override;
  bool sqlQueryTable(const QString &query) override;
  void contextMenuEvent(QContextMenuEvent *) override;

private Q_SLOTS:
  void setSortByColumn(int column, Qt::SortOrder order) override;
  void getSqlModelError(const QString &table, const QString &message) override;
  void getSelectedItem(const QModelIndex &index) override;
  void createOpenEntry() override;
  void createCopyClipboard() override;
  void createOrderSignal() override;

public Q_SLOTS:
  void setReloadView() override;

public:
  explicit BookTableView(QWidget *parent = nullptr);
  int rowCount() override;
  bool setQuery(const QString &clause = QString()) override;
};

#endif // ANTIQUACRM_BOOKTABLEVIEW_H
