// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customerstableview.h"
#include "customerstablemodel.h"

#include <AntiquaCRM>
#include <QAction>
#include <QDebug>
#include <QMenu>
#include <QtSql>

CustomersTableView::CustomersTableView(QWidget *parent)
    : InventoryTable{parent} {
  setEnableTableViewSorting(true);
  m_model = new CustomersTableModel(this);
  where_clause = QString("DATE(c_changed)=CURRENT_DATE");

  connect(m_model, SIGNAL(sqlErrorMessage(const QString &, const QString &)),
          this, SLOT(getSqlModelError(const QString &, const QString &)));

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(getSelectedItem(const QModelIndex &)));
}

qint64 CustomersTableView::getTableID(const QModelIndex &index) {
  QModelIndex id(index);
  if (m_model->data(id.sibling(id.row(), 0), Qt::EditRole).toInt() >= 1) {
    return m_model->data(id.sibling(id.row(), 0), Qt::EditRole).toInt();
  }
  return -1;
}

bool CustomersTableView::sqlQueryTable(const QString &query) {
  // qDebug() << Q_FUNC_INFO << query;
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

void CustomersTableView::contextMenuEvent(QContextMenuEvent *event) {
  p_modelIndex = indexAt(event->pos());
  bool enable_action = p_modelIndex.isValid();
  bool enable_create = (m_model->rowCount() > 0);

  QMenu *m = new QMenu(this);
  // Eintrag öffnen  Bestellung anlegen
  QAction *ac_open =
      m->addAction(cellIcon("database"), tr("Edit selected Customer"));
  ac_open->setObjectName("ac_context_open_customer");
  ac_open->setEnabled(enable_action);
  connect(ac_open, SIGNAL(triggered()), SLOT(createOpenEntry()));

  QAction *ac_create =
      m->addAction(cellIcon("db_add"), tr("Create new Customer"));
  ac_create->setObjectName("ac_context_create_customer");
  ac_create->setEnabled(enable_create);
  connect(ac_create, SIGNAL(triggered()), SIGNAL(sendCreateNewEntry()));

  QAction *ac_delete =
      m->addAction(cellIcon("db_remove"), tr("Delete selected Customer"));
  ac_delete->setObjectName("ac_context_delete_customer");
  ac_delete->setEnabled(enable_action);
  connect(ac_delete, SIGNAL(triggered()), SLOT(createDeleteRequest()));

  // BEGIN Einträge für Auftrag
  QAction *ac_copy = m->addAction(cellIcon("db_comit"), tr("Copy Customer Id"));
  ac_copy->setObjectName("ac_context_copy_customer");
  ac_copy->setEnabled(enable_action);
  connect(ac_copy, SIGNAL(triggered()), SLOT(createCopyClipboard()));

  QAction *ac_order =
      m->addAction(cellIcon("view_log"), tr("New Order for selected Customer"));
  ac_order->setObjectName("ac_context_customer_to_order");
  ac_order->setEnabled(enable_action);
  connect(ac_order, SIGNAL(triggered()), SLOT(createOrderSignal()));
  //  END

  QAction *ac_refresh = m->addAction(cellIcon("action_reload"), tr("Reload"));
  ac_refresh->setObjectName("ac_context_refresh_customers");
  connect(ac_refresh, SIGNAL(triggered()), SLOT(setReloadView()));

  m->exec(event->globalPos());
  delete m;
}

void CustomersTableView::setSortByColumn(int column, Qt::SortOrder order) {
  if (column < 0)
    return;

  QString order_by = m_model->fieldName(column);
  // NOTE Muss hier umgedreht werden!
  Qt::SortOrder sort = Qt::AscendingOrder;
  if (order == Qt::AscendingOrder)
    sort = Qt::DescendingOrder;

  AntiquaCRM::ASqlFiles query("query_tab_customers_main");
  if (query.openTemplate()) {
    query.setWhereClause(where_clause);
    query.setOrderBy(order_by);
    query.setSorting(sort);
    query.setLimits(getQueryLimit());
  }
  sqlQueryTable(query.getQueryContent());
}

void CustomersTableView::getSqlModelError(const QString &table,
                                          const QString &message) {
  qDebug() << Q_FUNC_INFO << "TODO" << table << message;
}

void CustomersTableView::getSelectedItem(const QModelIndex &index) {
  qint64 id = getTableID(index);
  if (id >= 1)
    emit sendOpenEntry(id);
}

void CustomersTableView::createOpenEntry() {
  qint64 id = getTableID(p_modelIndex);
  if (id >= 1)
    emit sendOpenEntry(id);
}

void CustomersTableView::createCopyClipboard() {
  qint64 id = getTableID(p_modelIndex);
  if (id >= 1)
    emit sendCopyToClibboard(QString::number(id));
}

void CustomersTableView::createOrderSignal() {
  qint64 id = getTableID(p_modelIndex);
  if (id >= 1)
    emit sendCurrentId(id);
}

void CustomersTableView::createDeleteRequest() {
  qint64 id = getTableID(p_modelIndex);
  if (id >= 1)
    emit sendDeleteEntry(id);
}

void CustomersTableView::setReloadView() {
  sqlQueryTable(m_model->query().lastQuery());
}

int CustomersTableView::rowCount() {
  return m_model->rowCount();
}

bool CustomersTableView::setQuery(const QString &clause) {
  AntiquaCRM::ASqlFiles query("query_tab_customers_main");
  if (query.openTemplate()) {
    where_clause = (clause.isEmpty() ? where_clause : clause);
    query.setWhereClause(where_clause);
    query.setOrderBy("c_id");
    query.setSorting(Qt::DescendingOrder);
    query.setLimits(getQueryLimit());
  }
  return sqlQueryTable(query.getQueryContent());
}
