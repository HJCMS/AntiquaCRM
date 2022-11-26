// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booktableview.h"
#include "bookstablemodel.h"

#include <AntiquaCRM>
#include <QAction>
#include <QDebug>
#include <QMenu>
#include <QtSql>

BookTableView::BookTableView(QWidget *parent) : InventoryTable{parent} {
  setEnableTableViewSorting(true);
  m_model = new BooksTableModel(this);
  where_clause = QString("DATE(ib_changed)=CURRENT_DATE");

  connect(m_model, SIGNAL(sqlErrorMessage(const QString &, const QString &)),
          this, SLOT(getSqlModelError(const QString &, const QString &)));

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(getSelectedItem(const QModelIndex &)));
}

qint64 BookTableView::getTableID(const QModelIndex &index) {
  QModelIndex id(index);
  if (m_model->data(id.sibling(id.row(), 0), Qt::EditRole).toInt() >= 1) {
    return m_model->data(id.sibling(id.row(), 0), Qt::EditRole).toInt();
  }
  return -1;
}

int BookTableView::getArticleCount(const QModelIndex &index) {
  QModelIndex id(index);
  if (m_model->data(id.sibling(id.row(), 1), Qt::EditRole).toInt() >= 1) {
    return m_model->data(id.sibling(id.row(), 1), Qt::EditRole).toInt();
  }
  return -1;
}

bool BookTableView::sqlQueryTable(const QString &query) {
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

void BookTableView::contextMenuEvent(QContextMenuEvent *event) {
  p_modelIndex = indexAt(event->pos());
  bool enable_action = p_modelIndex.isValid();
  bool enable_create = (m_model->rowCount() > 0);

  QMenu *m = new QMenu(this);
  // Eintrag öffnen  Bestellung anlegen
  QAction *ac_open = m->addAction(cellIcon("database"), tr("Open entry"));
  ac_open->setObjectName("ac_context_open_book");
  ac_open->setEnabled(enable_action);
  connect(ac_open, SIGNAL(triggered()), this, SLOT(createOpenEntry()));

  QAction *ac_create = m->addAction(cellIcon("db_add"), tr("Create entry"));
  ac_create->setObjectName("ac_context_create_book");
  ac_create->setEnabled(enable_create);
  connect(ac_create, SIGNAL(triggered()), this, SIGNAL(sendCreateNewEntry()));

  // BEGIN Einträge für Auftrag
  QAction *ac_copy = m->addAction(cellIcon("db_comit"), tr("Copy Article Id"));
  ac_copy->setObjectName("ac_context_copy_book");
  ac_copy->setEnabled(enable_action);
  connect(ac_copy, SIGNAL(triggered()), this, SLOT(createCopyClipboard()));

  QAction *ac_order =
      m->addAction(cellIcon("view_log"), tr("Add Article to opened Order"));
  ac_order->setObjectName("ac_context_book_to_order");
  ac_order->setEnabled(getArticleCount(p_modelIndex) > 0);
  connect(ac_order, SIGNAL(triggered()), this, SLOT(createOrderSignal()));
  //  END

  QAction *ac_refresh = m->addAction(cellIcon("action_reload"), tr("Update"));
  ac_refresh->setObjectName("ac_context_refresh_books");
  connect(ac_refresh, SIGNAL(triggered()), this, SLOT(setReloadView()));

  m->exec(event->globalPos());
  delete m;
}

void BookTableView::setSortByColumn(int column, Qt::SortOrder order) {
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
      order_by.append(",ib_changed)");
    }
  }

  // NOTE Muss hier umgedreht werden!
  Qt::SortOrder sort = Qt::AscendingOrder;
  if (order == Qt::AscendingOrder)
    sort = Qt::DescendingOrder;

  AntiquaCRM::ASqlFiles query("query_tab_books_main");
  if (query.openTemplate()) {
    query.setWhereClause(where_clause);
    query.setOrderBy(order_by);
    query.setSorting(sort);
    query.setLimits(getQueryLimit());
  }
  sqlQueryTable(query.getQueryContent());
}

void BookTableView::getSelectedItem(const QModelIndex &index) {
  qint64 aid = getTableID(index);
  if (aid >= 1)
    emit sendOpenEntry(aid);
}

void BookTableView::createOpenEntry() {
  qint64 aid = getTableID(p_modelIndex);
  if (aid >= 1)
    emit sendOpenEntry(aid);
}

void BookTableView::createCopyClipboard() {
  qint64 aid = getTableID(p_modelIndex);
  if (aid >= 1)
    emit sendCopyToClibboard(QString::number(aid));
}

void BookTableView::createOrderSignal() {
  qint64 aid = getTableID(p_modelIndex);
  if (aid >= 1 && getArticleCount(p_modelIndex) > 0) {
    QJsonObject obj;
    obj.insert("window_operation", "add_article");
    obj.insert("tab", "orders_tab");
    obj.insert("add_article", QJsonValue(aid));
    emit sendSocketOperation(obj);
  }
}

void BookTableView::setReloadView() {
  sqlQueryTable(m_model->query().lastQuery());
}

int BookTableView::rowCount() { return m_model->rowCount(); }

bool BookTableView::setQuery(const QString &clause) {
  AntiquaCRM::ASqlFiles query("query_tab_books_main");
  if (query.openTemplate()) {
    where_clause = (clause.isEmpty() ? where_clause : clause);
    query.setWhereClause(where_clause);
    query.setOrderBy("(ib_count,ib_changed,ib_id)");
    query.setSorting(Qt::DescendingOrder);
    query.setLimits(getQueryLimit());
  }
  return sqlQueryTable(query.getQueryContent());
}
