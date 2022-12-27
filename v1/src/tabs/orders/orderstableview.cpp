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
  p_modelIndex = indexAt(event->pos());
  bool enable_action = p_modelIndex.isValid();

  QMenu *m = new QMenu(this);

  QAction *ac_edit = m->addAction(cellIcon("spreadsheet"), tr("Open order"));
  ac_edit->setEnabled(enable_action);
  connect(ac_edit, SIGNAL(triggered()), SLOT(createOpenEntry()));

  QAction *ac_copy = m->addAction(cellIcon("edit"), tr("Copy Order Id"));
  ac_copy->setEnabled(enable_action);
  connect(ac_copy, SIGNAL(triggered()), SLOT(createCopyClipboard()));

  QAction *ac_customer =
      m->addAction(cellIcon("user_group"), tr("View Customer"));
  ac_customer->setEnabled(enable_action);
  connect(ac_customer, SIGNAL(triggered()), SLOT(createSocketOperation()));

  QAction *ac_returning = m->addAction(cellIcon("action_undo"),
                                       tr("Create a return from this order."));
  ac_returning->setEnabled(enable_action);
  connect(ac_returning, SIGNAL(triggered()), SLOT(createOrderReturning()));

  QAction *ac_refresh = m->addAction(cellIcon("action_reload"), tr("Reload"));
  connect(ac_refresh, SIGNAL(triggered()), SLOT(setReloadView()));

  m->exec(event->globalPos());
  delete m;
}

void OrdersTableView::createOrderReturning() {
  qint64 oid = getTableID(p_modelIndex);
  if (oid < 1)
    return;

  ReturnOrder *d = new ReturnOrder(this);
  if (d->exec(oid) == QDialog::Accepted) {
    setReloadView();
  }
  d->deleteLater();
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

void OrdersTableView::createOpenEntry() {
  qint64 oid = getTableID(p_modelIndex);
  if (oid < 1)
    return;

  emit sendOpenEntry(oid);
}

void OrdersTableView::createCopyClipboard() {
  qint64 oid = getTableID(p_modelIndex);
  if (oid < 1)
    return;

  emit sendCopyToClibboard(QString::number(oid));
}

void OrdersTableView::createSocketOperation() {
  qint64 oid = getTableID(p_modelIndex);
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
