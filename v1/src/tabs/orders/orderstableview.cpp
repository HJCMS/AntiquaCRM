// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderstableview.h"
#include "orderstablemodel.h"

#include <QMenu>

OrdersTableView::OrdersTableView(QWidget *parent) : InventoryTable{parent} {
  setEnableTableViewSorting(true);
  m_model = new OrdersTableModel(this);

  QString status = QString::number(AntiquaCRM::COMPLETED);
  where_clause = QString("o_order_status<" + status);

  connect(m_model, SIGNAL(sqlErrorMessage(const QString &, const QString &)),
          this, SLOT(getSqlModelError(const QString &, const QString &)));

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(getSelectedItem(const QModelIndex &)));
}

qint64 OrdersTableView::getTableID(const QModelIndex &index) {
  QModelIndex id(index);
  if (m_model->data(id.sibling(id.row(), 0), Qt::EditRole).toInt() >= 1) {
    return m_model->data(id.sibling(id.row(), 0), Qt::EditRole).toInt();
  }
  return -1;
}

bool OrdersTableView::sqlQueryTable(const QString &query) {
  // qDebug() << Q_FUNC_INFO << query << Qt::endl;
  if (m_model->querySelect(query)) {
    QueryHistory = query;
    setModel(m_model);
    QString result = tr("Query finished with '%1' Rows.")
                         .arg(QString::number(m_model->rowCount()));
    emit sendQueryReport(result);
    emit sendResultExists((m_model->rowCount() > 0));
    return true;
  }
  emit sendQueryReport(tr("Query without result"));
  emit sendResultExists((m_model->rowCount() > 0));
  return false;
}

void OrdersTableView::contextMenuEvent(QContextMenuEvent *event) {
  p_modelIndex = indexAt(event->pos());
  bool enable_action = p_modelIndex.isValid();
  bool enable_create = (m_model->rowCount() > 0);

  QMenu *m = new QMenu(this);
  // Eintrag öffnen  Bestellung anlegen

  QAction *ac_copy = m->addAction(cellIcon("db_comit"), tr("Copy Order Id"));
  ac_copy->setObjectName("ac_context_copy_order");
  ac_copy->setEnabled(enable_action);
  connect(ac_copy, SIGNAL(triggered()), SLOT(createCopyClipboard()));

  QAction *ac_refresh = m->addAction(cellIcon("action_reload"), tr("Reload"));
  ac_refresh->setObjectName("ac_context_refresh_orders");
  connect(ac_refresh, SIGNAL(triggered()), SLOT(setReloadView()));

  m->exec(event->globalPos());
  delete m;
}

void OrdersTableView::setSortByColumn(int column, Qt::SortOrder order) {
  if (column < 0)
    return;

  QString order_by = m_model->fieldName(column);
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
  qint64 id = getTableID(index);
  if (id >= 1)
    emit sendOpenEntry(id);
}

void OrdersTableView::createOpenEntry() {
  qint64 id = getTableID(p_modelIndex);
  if (id >= 1)
    emit sendOpenEntry(id);
}

void OrdersTableView::createCopyClipboard() {
  qint64 id = getTableID(p_modelIndex);
  if (id >= 1)
    emit sendCopyToClibboard(QString::number(id));
}

void OrdersTableView::createOrderSignal() {
  qint64 id = getTableID(p_modelIndex);
  if (id >= 1)
    qDebug() << Q_FUNC_INFO << id;
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
    query.setOrderBy("o_since");
    query.setSorting(Qt::DescendingOrder);
    query.setLimits(getQueryLimit());
    return sqlQueryTable(query.getQueryContent());
  }
  return false;
}
