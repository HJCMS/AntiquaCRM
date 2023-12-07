// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderstableoverview.h"
#include "ordersconfig.h"
#include "orderstableoverviewmodel.h"

#include <AntiquaCRM>

OrdersTableOverView::OrdersTableOverView(QWidget *parent)
    : AntiquaCRM::TableView{parent} {
  setEnableTableViewSorting(true);
  m_model = new OrdersTableOverViewModel(this);
  where_clause = defaultWhereClause();
  connect(m_model, SIGNAL(sqlErrorMessage(const QString &, const QString &)),
          this, SLOT(sqlModelError(const QString &, const QString &)));

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(getSelectedItem(const QModelIndex &)));
}

const QStringList OrdersTableOverView::sortOrder() const {
  return QStringList({"o_since", "o_modified", "o_id"});
}

qint64 OrdersTableOverView::getTableID(const QModelIndex &index, int column) {
  const QModelIndex _sibling = index.sibling(index.row(), column);
  if (m_model->data(_sibling, Qt::EditRole).toInt() >= 1) {
    return m_model->data(_sibling, Qt::EditRole).toInt();
  }
  return -1;
}

bool OrdersTableOverView::sqlModelQuery(const QString &query) {
  if (m_model->querySelect(query)) {
    QueryHistory = query;
    setModel(m_model);
    emit sendQueryReport(m_model->queryResultInfo());
    emit sendResultExists((m_model->rowCount() > 0));
    // Table Record und NICHT QueryRecord abfragen!
    p_tableRecord = m_model->tableRecord();
    return true;
  }
  emit sendQueryReport(m_model->queryResultInfo());
  bool _status = (m_model->rowCount() > 0);
  emit sendResultExists(_status);
  return _status;
}

void OrdersTableOverView::contextMenuEvent(QContextMenuEvent *event) {
  QModelIndex index = indexAt(event->pos());
  qint64 rows = m_model->rowCount();
  AntiquaCRM::TableContextMenu *m_menu =
      new AntiquaCRM::TableContextMenu(index, rows, this);
  m_menu->addOpenAction(tr("Open order"));
  m_menu->addCopyAction(tr("Copy Order Id"));
  QAction *ac_customer = m_menu->addOrderAction(tr("View Customer"));
  ac_customer->setIcon(AntiquaCRM::antiquaIcon("system-users"));
  m_menu->addRefundAction(tr("Create refund"));
  m_menu->addReloadAction(tr("Update"));
  connect(m_menu,
          SIGNAL(sendAction(AntiquaCRM::TableContextMenu::Actions,
                            const QModelIndex &)),
          SLOT(contextMenuAction(AntiquaCRM::TableContextMenu::Actions,
                                 const QModelIndex &)));

  connect(m_menu, SIGNAL(sendRefresh()), SLOT(setReloadView()));
  m_menu->exec(event->globalPos());
  m_menu->deleteLater();
}

void OrdersTableOverView::contextMenuAction(
    AntiquaCRM::TableContextMenu::Actions ac, const QModelIndex &index) {
  qint64 _oid = getTableID(index);
  if (_oid < 1)
    return;

  switch (ac) {
  case (AntiquaCRM::TableContextMenu::Actions::Open):
    emit sendOpenEntry(_oid);
    break;

  case (AntiquaCRM::TableContextMenu::Actions::Delete):
    emit sendDeleteEntry(_oid);
    break;

  case (AntiquaCRM::TableContextMenu::Actions::Order):
    createSocketOperation(index);
    break;

  case (AntiquaCRM::TableContextMenu::Actions::Copy):
    emit sendCopyToClibboard(QString::number(_oid));
    break;

  case (AntiquaCRM::TableContextMenu::Actions::Refund):
    emit sendStartRefund(_oid);
    break;

  case (AntiquaCRM::TableContextMenu::Actions::Undo):
    break; // Unused in this section

  default:
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << "Unknown OrderView::ContextMenu request:" << _oid;
#endif
    break;
  };
}

void OrdersTableOverView::setSortByColumn(int column, Qt::SortOrder order) {
  if (column < 0)
    return;

  /**
   * @warning Bei Alias basierenden SELECT abfragen!
   * ORDER BY "Multisort" Abfragen können nicht mit Aliases gemischt werden!
   */
  QString _order_by;
  if (!p_tableRecord.isEmpty()) {
    QStringList _fields;
    const QString _column_sort = m_model->fieldName(column);
    for (int i = 0; i < p_tableRecord.count(); i++) {
      _fields << p_tableRecord.field(i).name();
    }
    QStringList _sort = sortOrder();
    if (_fields.contains(_column_sort)) {
      if (!_sort.contains(_column_sort))
        _sort.prepend(_column_sort);
    }
    _order_by.append("(" + _sort.join(",") + ")");
  }

  AntiquaCRM::ASqlFiles query("query_tab_orders_main");
  if (query.openTemplate()) {
    query.setWhereClause(where_clause);
    query.setOrderBy(_order_by);
    query.setSorting(order);
    query.setLimits(getQueryLimit());
  }
  sqlModelQuery(query.getQueryContent());
}

void OrdersTableOverView::getSelectedItem(const QModelIndex &index) {
  qint64 _oid = getTableID(index);
  if (_oid >= 1)
    emit sendOpenEntry(_oid);
}

void OrdersTableOverView::createSocketOperation(const QModelIndex &index) {
  qint64 _oid = getTableID(index);
  if (_oid < 1)
    return;

  qint64 _cid = -1;
  QString _sql("SELECT o_customer_id FROM " + ORDERS_SQL_TABLE_NAME);
  _sql.append(" WHERE o_id=" + QString::number(_oid) + ";");
  QSqlQuery _query = AntiquaCRM::ASqlCore(this).query(_sql);
  if (_query.size() > 0) {
    _query.next();
    _cid = _query.value("o_customer_id").toLongLong();
  }
  _query.clear();

  if (_cid < 1)
    return;

  QJsonObject _obj;
  _obj.insert("ACTION", "open_customer");
  _obj.insert("TARGET", "customers_tab");
  _obj.insert("VALUE", QJsonValue(_cid));
  emit sendSocketOperation(_obj);
}

void OrdersTableOverView::setReloadView() {
  sqlModelQuery(m_model->query().lastQuery());
}

int OrdersTableOverView::rowCount() { return m_model->rowCount(); }

bool OrdersTableOverView::setQuery(const QString &clause) {
  AntiquaCRM::ASqlFiles query("query_tab_orders_main");
  if (query.openTemplate()) {
    where_clause = (clause.isEmpty() ? where_clause : clause);
    query.setWhereClause(where_clause);
    query.setOrderBy("(" + sortOrder().join(",") + ")");
    query.setSorting(Qt::DescendingOrder);
    query.setLimits(getQueryLimit());
  }
  return sqlModelQuery(query.getQueryContent());
}

const QString OrdersTableOverView::defaultWhereClause() {
  QStringList _l(QString::number(AntiquaCRM::OrderStatus::DELIVERED));
  _l.append(QString::number(AntiquaCRM::OrderStatus::CANCELED));
  QString _sql("o_order_status NOT IN (" + _l.join(",") + ")");
  _sql.append(" AND o_since BETWEEN ");
  _sql.append("(CURRENT_TIMESTAMP - justify_interval(interval '12 months'))");
  _sql.append(" AND CURRENT_TIMESTAMP");
  return _sql;
}
