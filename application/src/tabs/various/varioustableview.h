// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_VARIOUSTABLEVIEW_H
#define ANTIQUACRM_VARIOUSTABLEVIEW_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

class VariousTableModel;

/**
 * @brief Inventory Various table view
 * @ingroup InventoryVarious
 */
class VariousTableView final : public InventoryTable {
  Q_OBJECT

private:
  VariousTableModel *m_model;
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
  explicit VariousTableView(QWidget *parent = nullptr);
  int rowCount() override;
  bool setQuery(const QString &clause = QString()) override;
  const QString defaultWhereClause() override;
};

#endif // ANTIQUACRM_VARIOUSTABLEVIEW_H
