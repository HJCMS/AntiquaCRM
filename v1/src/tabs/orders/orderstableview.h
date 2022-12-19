// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERSTABLEVIEW_H
#define ANTIQUACRM_ORDERSTABLEVIEW_H

#include <AntiquaWidgets>
#include <QModelIndex>
#include <QObject>
#include <QSqlRecord>
#include <QWidget>

class OrdersTableModel;

class OrdersTableView final : public InventoryTable {
  Q_OBJECT

private:
  OrdersTableModel *m_model;
  QString where_clause;
  QSqlRecord p_tableRecord;
  QModelIndex p_modelIndex;

  qint64 getTableID(const QModelIndex &index, int column = 0) override;
  bool sqlQueryTable(const QString &query) override;
  void contextMenuEvent(QContextMenuEvent *) override;

private Q_SLOTS:
  void setSortByColumn(int column, Qt::SortOrder order) override;
  void getSelectedItem(const QModelIndex &index) override;
  void createOpenEntry() override;
  void createCopyClipboard() override;
  void createOrderSignal() override;
  void createOpenCustomer();

public Q_SLOTS:
  void setReloadView() override;

public:
  explicit OrdersTableView(QWidget *parent = nullptr);
  int rowCount() override;
  bool setQuery(const QString &clause = QString()) override;
  const QString defaultWhereClause() override;
};

#endif // ANTIQUACRM_ORDERSTABLEVIEW_H
