// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cdvtableview.h"
#include "cdvtablemodel.h"

#include <QDebug>
#include <QMenu>

#ifndef CDV_QUERY_TEMPLATE
#define CDV_QUERY_TEMPLATE "query_tab_cdv_main"
#endif

CDVTableView::CDVTableView(QWidget *parent) : InventoryTable{parent} {
  setEnableTableViewSorting(true);
  where_clause = defaultWhereClause();
  m_model = new CDVTableModel(this);
  connect(m_model, SIGNAL(sqlErrorMessage(const QString &, const QString &)),
          this, SLOT(sqlModelError(const QString &, const QString &)));

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(getSelectedItem(const QModelIndex &)));
}

qint64 CDVTableView::getTableID(const QModelIndex &index, int column) {
  QModelIndex id(index);
  if (m_model->data(id.sibling(id.row(), column), Qt::EditRole).toInt() >= 1) {
    return m_model->data(id.sibling(id.row(), column), Qt::EditRole).toInt();
  }
  return -1;
}

int CDVTableView::getArticleCount(const QModelIndex &index) {
  return getTableID(index, 1);
}

bool CDVTableView::sqlModelQuery(const QString &query) {
  // qDebug() << query;
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

void CDVTableView::contextMenuEvent(QContextMenuEvent *event) {
  QModelIndex index = indexAt(event->pos());
  qint64 rows = m_model->rowCount();
  TableContextMenu *m_menu = new TableContextMenu(index, rows, this);
  m_menu->addOpenAction(tr("Open entry"));
  m_menu->addCreateAction(tr("Create entry"));
  m_menu->addCopyAction(tr("Copy Article Id"));
  m_menu->addOrderAction(tr("Add Article to opened Order"));
  m_menu->addReloadAction(tr("Update"));

  connect(
      m_menu,
      SIGNAL(sendAction(TableContextMenu::Actions, const QModelIndex &)),
      SLOT(contextMenuAction(TableContextMenu::Actions, const QModelIndex &)));

  connect(m_menu, SIGNAL(sendCreate()), SIGNAL(sendCreateNewEntry()));
  connect(m_menu, SIGNAL(sendRefresh()), SLOT(setReloadView()));

  m_menu->exec(event->globalPos());
  delete m_menu;
}

void CDVTableView::contextMenuAction(TableContextMenu::Actions ac,
                                     const QModelIndex &index) {

  qint64 aid = getTableID(index);
  if (aid < 1)
    return;

  switch (ac) {
  case (TableContextMenu::Actions::Open):
    emit sendOpenEntry(aid);
    break;

  case (TableContextMenu::Actions::Order):
    createSocketOperation(index);
    break;

  case (TableContextMenu::Actions::Copy):
    emit sendCopyToClibboard(QString::number(aid));
    break;

  default:
    qWarning("Unknown %s", Q_FUNC_INFO);
    return;
  };
}

void CDVTableView::setSortByColumn(int column, Qt::SortOrder order) {
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
      order_by.append(",cv_changed)");
    }
  }

  // NOTE Muss hier umgedreht werden!
  Qt::SortOrder sort = Qt::AscendingOrder;
  if (order == Qt::AscendingOrder)
    sort = Qt::DescendingOrder;

  AntiquaCRM::ASqlFiles query(CDV_QUERY_TEMPLATE);
  if (query.openTemplate()) {
    query.setWhereClause(where_clause);
    query.setOrderBy(order_by);
    query.setSorting(sort);
    query.setLimits(getQueryLimit());
  }
  sqlModelQuery(query.getQueryContent());
}

void CDVTableView::getSelectedItem(const QModelIndex &index) {
  qint64 aid = getTableID(index);
  if (aid >= 1)
    emit sendOpenEntry(aid);
}

void CDVTableView::createSocketOperation(const QModelIndex &index) {
  qint64 aid = getTableID(index);
  if (aid >= 1 && getArticleCount(index) > 0) {
    QJsonObject obj;
    obj.insert("window_operation", "add_article");
    obj.insert("tab", "orders_tab");
    obj.insert("add_article", QJsonValue(aid));
    emit sendSocketOperation(obj);
  } else {
    qInfo("Socket operation ignored!");
  }
}

void CDVTableView::setReloadView() {
  sqlModelQuery(m_model->query().lastQuery());
}

int CDVTableView::rowCount() { return m_model->rowCount(); }

bool CDVTableView::setQuery(const QString &clause) {
  AntiquaCRM::ASqlFiles query(CDV_QUERY_TEMPLATE);
  if (query.openTemplate()) {
    where_clause = (clause.isEmpty() ? where_clause : clause);
    query.setWhereClause(where_clause);
    query.setOrderBy("(cv_count,cv_changed,cv_id)");
    query.setSorting(Qt::DescendingOrder);
    query.setLimits(getQueryLimit());
  }
  return sqlModelQuery(query.getQueryContent());
}

const QString CDVTableView::defaultWhereClause() {
  QString sql("DATE(cv_changed) BETWEEN (CURRENT_DATE - 1) AND CURRENT_DATE");
  return sql;
}
