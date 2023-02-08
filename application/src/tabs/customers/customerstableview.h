// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_CUSTOMERSTABLEVIEW_H
#define ANTIQUACRM_CUSTOMERSTABLEVIEW_H

#include <QContextMenuEvent>
#include <QIcon>
#include <QObject>
#include <QSqlRecord>
#include <QWidget>

#ifndef ANTIQUACRM_INVENTORYTABLE_H
#include "inventorytable.h"
#endif

class CustomersTableModel;

class CustomersTableView final : public InventoryTable {
  Q_OBJECT

private:
  CustomersTableModel *m_model;
  QString where_clause;
  QSqlRecord p_tableRecord;

  qint64 getTableID(const QModelIndex &index, int column = 0) override;
  bool sqlQueryTable(const QString &query) override;
  void contextMenuEvent(QContextMenuEvent *) override;

private Q_SLOTS:
  void contextMenuAction(TableContextMenu::Actions,
                         const QModelIndex &) override;
  void setSortByColumn(int column, Qt::SortOrder order) override;
  void getSelectedItem(const QModelIndex &) override;
  void createSocketOperation(const QModelIndex &) override{/* unused */};

public Q_SLOTS:
  void setReloadView() override;

Q_SIGNALS:
  void sendDeleteEntry(qint64 costumerId);

public:
  explicit CustomersTableView(QWidget *parent = nullptr);
  int rowCount() override;
  bool setQuery(const QString &clause = QString()) override;
  const QString defaultWhereClause() override;
};

#endif // ANTIQUACRM_CUSTOMERSTABLEVIEW_H
