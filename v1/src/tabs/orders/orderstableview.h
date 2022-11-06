// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERSTABLEVIEW_H
#define ANTIQUACRM_ORDERSTABLEVIEW_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

class OrdersTableModel;

class OrdersTableView final : public InventoryTable {
  Q_OBJECT

private:
  OrdersTableModel *m_model;
  QString where_clause;
  QModelIndex p_modelIndex;

  qint64 getTableID(const QModelIndex &index) override;
  bool sqlQueryTable(const QString &query) override;
  void contextMenuEvent(QContextMenuEvent *) override;

private Q_SLOTS:
  void setSortByColumn(int column, Qt::SortOrder order) override;
  void getSelectedItem(const QModelIndex &index) override;
  void createOpenEntry() override;
  void createCopyClipboard() override;
  void createOrderSignal() override;

public Q_SLOTS:
  void setReloadView() override;

public:
  explicit OrdersTableView(QWidget *parent = nullptr);
  int rowCount() override;
  bool setQuery(const QString &clause = QString()) override;
  static const QString defaultWClause();
};

#endif // ANTIQUACRM_ORDERSTABLEVIEW_H
