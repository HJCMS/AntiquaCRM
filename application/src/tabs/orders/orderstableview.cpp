// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderstableview.h"
#include "orderstablemodel.h"
#include "returnorder.h"

#include <QMenu>

OrdersTableView::OrdersTableView(QWidget *parent) : InventoryTable{parent} {
  setEnableTableViewSorting(true);
  m_model = new OrdersTableModel(this);

  where_clause = defaultWhereClause();

  connect(m_model, SIGNAL(sqlErrorMessage(const QString &, const QString &)),
          this, SLOT(getSqlModelError(const QString &, const QString &)));

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(getSelectedItem(const QModelIndex &)));
}

qint64 OrdersTableView::getTableID(const QModelIndex &index, int column) {
  QModelIndex id(index);
  if (m_model->data(id.sibling(id.row(), column), Qt::EditRole).toInt() >= 1) {
    return m_model->data(id.sibling(id.row(), column), Qt::EditRole).toInt();
  }
  return -1;
}

bool OrdersTableView::sqlQueryTable(const QString &query) {
  // qDebug() << Q_FUNC_INFO << query << Qt::endl;
  if (m_model->querySelect(query)) {
    QueryHistory = query;
    setModel(m_model);
    emit sendQueryReport(m_model->queryResultInfo());
    emit sendResultExists((m_model->rowCount() > 0));
    // Table Record und NICHT QueryRecord abfragen!
    // Siehe: setSortByColumn
    p_tableRecord = m_model->tableRecord();
    return true;
  }
  emit sendQueryReport(m_model->queryResultInfo());
  emit sendResultExists((m_model->rowCount() > 0));
  return false;
}

void OrdersTableView::contextMenuEvent(QContextMenuEvent *event) {
  QModelIndex index = indexAt(event->pos());
  qint64 rows = m_model->rowCount();
  TableContextMenu *m_menu = new TableContextMenu(index, rows, this);
  m_menu->addOpenAction(tr("Open order"));
  m_menu->addCopyAction(tr("Copy Order Id"));
  QAction *ac_customer = m_menu->addOrderAction(tr("View Customer"));
  ac_customer->setIcon(cellIcon("user_group"));
  m_menu->addUndoAction(tr("Create a return from this order."));
  m_menu->addReloadAction(tr("Reload"));

  connect(
      m_menu,
      SIGNAL(sendAction(TableContextMenu::Actions, const QModelIndex &)),
      SLOT(contextMenuAction(TableContextMenu::Actions, const QModelIndex &)));

  connect(m_menu, SIGNAL(sendRefresh()), SLOT(setReloadView()));

  m_menu->exec(event->globalPos());
  delete m_menu;
}

void OrdersTableView::createOrderReturning(qint64 oid) {
  ReturnOrder *d = new ReturnOrder(this);
  if (d->exec(oid) == QDialog::Accepted) {
    setReloadView();
  }
  d->deleteLater();
}

void OrdersTableView::contextMenuAction(TableContextMenu::Actions ac,
                                        const QModelIndex &index) {

  qint64 oid = getTableID(index);
  if (oid < 1)
    return;

  switch (ac) {
  case (TableContextMenu::Actions::Open):
    emit sendOpenEntry(oid);
    break;

  case (TableContextMenu::Actions::Order):
    createSocketOperation(index);
    break;

  case (TableContextMenu::Actions::Copy):
    emit sendCopyToClibboard(QString::number(oid));
    break;

  case (TableContextMenu::Actions::Undo):
    createOrderReturning(oid);
    break;

  default:
    qWarning("Unknown %s", Q_FUNC_INFO);
    return;
  };
}

void OrdersTableView::setSortByColumn(int column, Qt::SortOrder order) {
  if (column < 0)
    return;

  QString order_by = m_model->fieldName(column);
  /**
   * @warning Bei Alias basierenden SELECT abfragen!
   * Können ORDER BY "Multisort" Abfragen nicht mit Aliases gemischt werden!
   */
  if (!p_tableRecord.isEmpty()) {
    QStringList fieldList;
    for (int i = 0; i < p_tableRecord.count(); i++) {
      fieldList << p_tableRecord.field(i).name();
    }
    // Standard ist o_id
    if (fieldList.contains(order_by)) {
      order_by.prepend("(");
      order_by.append((order_by == "o_id") ? ",o_since" : "");
      order_by.append(")");
    }
  }

  // NOTE Muss hier umgedreht werden!
  Qt::SortOrder sort = Qt::AscendingOrder;
  if (order == Qt::AscendingOrder)
    sort = Qt::DescendingOrder;

  AntiquaCRM::ASqlFiles query("query_tab_orders_main");
  if (query.openTemplate()) {
    query.setWhereClause(where_clause);
    query.setOrderBy(order_by);
    query.setSorting(sort);
    query.setLimits(getQueryLimit());
  }
  sqlQueryTable(query.getQueryContent());
}

void OrdersTableView::getSelectedItem(const QModelIndex &index) {
  qint64 oid = getTableID(index);
  if (oid < 1)
    return;

  emit sendOpenEntry(oid);
}

void OrdersTableView::createSocketOperation(const QModelIndex &index) {
  qint64 oid = getTableID(index);
  if (oid < 1)
    return;

  QJsonObject obj;
  obj.insert("window_operation", "order_customer");
  obj.insert("tab", "customers_tab");
  obj.insert("order_customer", oid);
  emit sendSocketOperation(obj);
}

void OrdersTableView::setReloadView() {
  sqlQueryTable(m_model->query().lastQuery());
}

int OrdersTableView::rowCount() { return m_model->rowCount(); }

bool OrdersTableView::setQuery(const QString &clause) {
  AntiquaCRM::ASqlFiles query("query_tab_orders_main");
  if (query.openTemplate()) {
    where_clause = (clause.isEmpty() ? where_clause : clause);
    query.setWhereClause(where_clause);
    query.setOrderBy("o_id");
    query.setSorting(Qt::DescendingOrder);
    query.setLimits(getQueryLimit());
    return sqlQueryTable(query.getQueryContent());
  }
  return false;
}

const QString OrdersTableView::defaultWhereClause() {
  QString status = QString::number(AntiquaCRM::OrderStatus::DELIVERED);
  QString sql = QString("o_order_status<" + status);
  return sql;
}
