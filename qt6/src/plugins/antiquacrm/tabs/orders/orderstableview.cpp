// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderstableview.h"
#include "orderstablemodel.h"

#include <AntiquaCRM>

OrdersTableView::OrdersTableView(QWidget *parent)
    : AntiquaCRM::TableView{parent} {
  setEnableTableViewSorting(true);
  m_model = new OrdersTableModel(this);
  where_clause = defaultWhereClause();
  connect(m_model, SIGNAL(sqlErrorMessage(const QString &, const QString &)),
          this, SLOT(sqlModelError(const QString &, const QString &)));

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

bool OrdersTableView::sqlModelQuery(const QString &query) {
  // qDebug() << Q_FUNC_INFO << query;
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
  bool status = (m_model->rowCount() > 0);
  emit sendResultExists(status);
  return status;
}

void OrdersTableView::contextMenuEvent(QContextMenuEvent *event) {
  QModelIndex index = indexAt(event->pos());
  qint64 rows = m_model->rowCount();
  AntiquaCRM::TableContextMenu *m_menu =
      new AntiquaCRM::TableContextMenu(index, rows, this);
  m_menu->addOpenAction(tr("Open entry"));
  //m_menu->addCreateAction(tr("Create entry"));
  m_menu->addReloadAction(tr("Update"));
  /*
  connect(m_menu,
          SIGNAL(sendAction(AntiquaCRM::TableContextMenu::Actions,
                            const QModelIndex &)),
          SLOT(contextMenuAction(AntiquaCRM::TableContextMenu::Actions,
                                 const QModelIndex &)));
  */

  //connect(m_menu, SIGNAL(sendCreate()), SIGNAL(sendCreateNewEntry()));
  connect(m_menu, SIGNAL(sendRefresh()), SLOT(setReloadView()));
  m_menu->exec(event->globalPos());
  delete m_menu;
}

void OrdersTableView::contextMenuAction(
    AntiquaCRM::TableContextMenu::Actions ac, const QModelIndex &index) {
  qint64 _id = getTableID(index);
  if (_id < 1)
    return;

  switch (ac) {
  case (AntiquaCRM::TableContextMenu::Actions::Open):
    emit sendOpenEntry(_id);
    break;

  case (AntiquaCRM::TableContextMenu::Actions::Delete):
    emit sendDeleteEntry(_id);
    break;

  case (AntiquaCRM::TableContextMenu::Actions::Order):
    createSocketOperation(index);
    break;

  case (AntiquaCRM::TableContextMenu::Actions::Copy):
    emit sendCopyToClibboard(QString::number(_id));
    break;

  default:
    qWarning("Unknown Menu context request!");
    return;
  };
}

void OrdersTableView::setSortByColumn(int column, Qt::SortOrder order) {
  if (column < 0)
    return;

  QString order_by = m_model->fieldName(column);
  /**
   * @warning Bei Alias basierenden SELECT abfragen!
   * ORDER BY "Multisort" Abfragen können nicht mit Aliases gemischt werden!
   */
  if (!p_tableRecord.isEmpty()) {
    QStringList fieldList;
    for (int i = 0; i < p_tableRecord.count(); i++) {
      fieldList << p_tableRecord.field(i).name();
    }
    if (fieldList.contains(order_by)) {
      order_by.prepend("(");
      order_by.append(",o_modified)");
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
  sqlModelQuery(query.getQueryContent());
}

void OrdersTableView::getSelectedItem(const QModelIndex &index) {
  qint64 _oid = getTableID(index);
  if (_oid >= 1)
    emit sendOpenEntry(_oid);
}

void OrdersTableView::createSocketOperation(const QModelIndex &index) {
  qint64 _oid = getTableID(index);
  qDebug() << Q_FUNC_INFO << _oid << "TODO";
}

void OrdersTableView::setReloadView() {
  sqlModelQuery(m_model->query().lastQuery());
}

int OrdersTableView::rowCount() { return m_model->rowCount(); }

bool OrdersTableView::setQuery(const QString &clause) {
  AntiquaCRM::ASqlFiles query("query_tab_orders_main");
  if (query.openTemplate()) {
    where_clause = (clause.isEmpty() ? where_clause : clause);
    query.setWhereClause(where_clause);
    query.setOrderBy("(o_modified,o_id)");
    query.setSorting(Qt::DescendingOrder);
    query.setLimits(getQueryLimit());
  }
  return sqlModelQuery(query.getQueryContent());
}

const QString OrdersTableView::defaultWhereClause() {
  QDate _d = QDate::currentDate();
  QString _sql("(");
  _sql.append("DATE_PART('YEAR', o_modified)=" + QString::number(_d.year()));
  //_sql.append(" AND ");
  //_sql.append("DATE_PART('MONTH', o_modified)=" + QString::number(_d.month()));
  _sql.append(")");
  return _sql;
}
