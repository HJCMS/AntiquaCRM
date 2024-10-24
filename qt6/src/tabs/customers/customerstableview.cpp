// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customerstableview.h"
#include "customerstablemodel.h"

CustomersTableView::CustomersTableView(QWidget *parent)
    : AntiquaCRM::TableView{parent} {
  setEnableTableViewSorting(true);
  m_model = new CustomersTableModel(this);
  where_clause = defaultWhereClause();
  connect(m_model, SIGNAL(sqlErrorMessage(const QString &, const QString &)),
          SLOT(sqlModelError(const QString &, const QString &)));

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)),
          SLOT(getSelectedItem(const QModelIndex &)));
}

qint64 CustomersTableView::getTableID(const QModelIndex &index, int column) {
  QModelIndex id(index);
  if (m_model->data(id.sibling(id.row(), column), Qt::EditRole).toInt() >= 1) {
    return m_model->data(id.sibling(id.row(), column), Qt::EditRole).toInt();
  }
  return -1;
}

int CustomersTableView::getArticleCount(const QModelIndex &index) {
  Q_UNUSED(index);
#ifdef ANTIQUA_DEVELOPMENT
  qDebug() << Q_FUNC_INFO << "unused - always returns -1!";
#endif
  return -1;
}

bool CustomersTableView::sqlModelQuery(const QString &query) {
  if (m_model->querySelect(query)) {
    QueryHistory = query;
    setModel(m_model);
    // Table Record und NICHT QueryRecord abfragen!
    // Siehe: setSortByColumn
    p_tableRecord = m_model->tableRecord();
    emit sendQueryReport(m_model->queryResultInfo());
    queryFinished(m_model->rowCount() > 0);
    return true;
  }
  return false;
}

void CustomersTableView::contextMenuEvent(QContextMenuEvent *event) {
  QModelIndex index = indexAt(event->pos());
  qint64 rows = m_model->rowCount();
  AntiquaCRM::TableContextMenu *m_menu =
      new AntiquaCRM::TableContextMenu(index, rows, this);
  m_menu->addOpenAction(tr("Open entry"));
  m_menu->addCreateAction(tr("Create entry"));
  m_menu->addDeleteAction(tr("Delete selected Customer"));
  m_menu->addCopyAction(tr("Copy Article Id"));
  m_menu->addOrderAction(tr("Create new Order for this Customer"));
  m_menu->addReloadAction(tr("Update"));

  connect(m_menu,
          SIGNAL(sendAction(AntiquaCRM::TableContextMenu::Actions,
                            const QModelIndex &)),
          SLOT(contextMenuAction(AntiquaCRM::TableContextMenu::Actions,
                                 const QModelIndex &)));

  connect(m_menu, SIGNAL(sendCreate()), SIGNAL(sendCreateNewEntry()));
  connect(m_menu, SIGNAL(sendRefresh()), SLOT(setReloadView()));

  m_menu->exec(event->globalPos());
  m_menu->deleteLater();
}

void CustomersTableView::contextMenuAction(
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

void CustomersTableView::setSortByColumn(int column, Qt::SortOrder order) {
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
      order_by.append(",c_changed)");
    }
  }

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
  sqlModelQuery(query.getQueryContent());
}

void CustomersTableView::getSelectedItem(const QModelIndex &index) {
  qint64 aid = getTableID(index);
  if (aid >= 1)
    emit sendOpenEntry(aid);
}

void CustomersTableView::createSocketOperation(const QModelIndex &index) {
  qint64 cid = getTableID(index);
  if (cid >= 1) {
    QJsonObject obj;
    obj.insert("ACTION", "create_order");
    obj.insert("TARGET", "orders_tab");
    obj.insert("VALUE", QJsonValue(cid));
    emit sendSocketOperation(obj);
  } else {
    qInfo("Socket operation ignored (%lld)!", cid);
  }
}

void CustomersTableView::setReloadView() {
  sqlModelQuery(m_model->query().lastQuery());
}

int CustomersTableView::rowCount() { return m_model->rowCount(); }

bool CustomersTableView::setQuery(const QString &clause) {
  AntiquaCRM::ASqlFiles query("query_tab_customers_main");
  if (query.openTemplate()) {
    where_clause = (clause.isEmpty() ? where_clause : clause);
    query.setWhereClause(where_clause);
    query.setOrderBy("(c_changed,c_id)");
    query.setSorting(Qt::DescendingOrder);
    query.setLimits(getQueryLimit());
  }
  return sqlModelQuery(query.getQueryContent());
}

const QString CustomersTableView::defaultWhereClause() {
  int _year = QDate::currentDate().year();
  QString _sql("(");
  _sql.append("DATE_PART('YEAR', c_changed)=" + QString::number(_year));
  _sql.append(" AND ");
  _sql.append("DATE_PART('WEEK', c_changed)=DATE_PART('WEEK', CURRENT_DATE)");
  _sql.append(")");
  return _sql;
}
