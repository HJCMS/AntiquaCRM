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
  where_clause = defaultWhereClause();
  connect(m_model, SIGNAL(sqlErrorMessage(const QString &, const QString &)),
          this, SLOT(getSqlModelError(const QString &, const QString &)));

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(getSelectedItem(const QModelIndex &)));
}

qint64 CustomersTableView::getTableID(const QModelIndex &index, int column) {
  QModelIndex id(index);
  if (m_model->data(id.sibling(id.row(), column), Qt::EditRole).toInt() >= 1) {
    return m_model->data(id.sibling(id.row(), column), Qt::EditRole).toInt();
  }
  return -1;
}

bool CustomersTableView::sqlQueryTable(const QString &query) {
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
  emit sendResultExists(false);
  return false;
}

void CustomersTableView::contextMenuEvent(QContextMenuEvent *event) {
  QModelIndex index = indexAt(event->pos());
  qint64 rows = m_model->rowCount();
  TableContextMenu *m_menu = new TableContextMenu(index, rows, this);
  m_menu->addOpenAction(tr("Edit selected Customer"));
  m_menu->addCreateAction(tr("Create new Customer"));
  m_menu->addDeleteAction(tr("Delete selected Customer"));
  m_menu->addCopyAction(tr("Copy Customer Id"));
  m_menu->addOrderAction(tr("Create Order"));
  m_menu->addReloadAction(tr("Reload"));

  connect(
      m_menu,
      SIGNAL(sendAction(TableContextMenu::Actions, const QModelIndex &)),
      SLOT(contextMenuAction(TableContextMenu::Actions, const QModelIndex &)));

  connect(m_menu, SIGNAL(sendCreate()), SIGNAL(sendCreateNewEntry()));
  connect(m_menu, SIGNAL(sendRefresh()), SLOT(setReloadView()));

  m_menu->exec(event->globalPos());
  delete m_menu;
}

void CustomersTableView::contextMenuAction(TableContextMenu::Actions ac,
                                           const QModelIndex &index) {

  qint64 c_id = getTableID(index);
  if (c_id < 1)
    return;

  switch (ac) {
  case (TableContextMenu::Actions::Open):
    emit sendOpenEntry(c_id);
    break;

  case (TableContextMenu::Actions::Order):
    emit sendCurrentId(c_id);
    break;

  case (TableContextMenu::Actions::Delete):
    emit sendDeleteEntry(c_id);
    break;

  case (TableContextMenu::Actions::Copy):
    emit sendCopyToClibboard(QString::number(c_id));
    break;

  default:
    qWarning("Unknown %s", Q_FUNC_INFO);
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
  sqlQueryTable(query.getQueryContent());
}

void CustomersTableView::getSelectedItem(const QModelIndex &index) {
  qint64 id = getTableID(index);
  if (id >= 1)
    emit sendOpenEntry(id);
}

void CustomersTableView::setReloadView() {
  sqlQueryTable(m_model->query().lastQuery());
}

int CustomersTableView::rowCount() { return m_model->rowCount(); }

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

const QString CustomersTableView::defaultWhereClause() {
  QString today("DATE(c_changed)=CURRENT_DATE");
  QString yesterday("DATE(c_since)=(CURRENT_DATE -1)");
  return today + " OR " + yesterday;
}
