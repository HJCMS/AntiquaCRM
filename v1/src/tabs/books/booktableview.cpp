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

const QIcon BookTableView::pIcon(const QString &name) {
  return QIcon(":icons/" + name + ".png");
}

qint64 BookTableView::getArticleID(const QModelIndex &index) {
  QModelIndex id(index);
  if (m_model->data(id.sibling(id.row(), 0), Qt::EditRole).toInt() >= 1) {
    return m_model->data(id.sibling(id.row(), 0), Qt::EditRole).toInt();
  }
  return -1;
}

bool BookTableView::sqlQueryTable(const QString &query) {
  // qDebug() << Q_FUNC_INFO << query;
  if (m_model->querySelect(query)) {
    QueryHistory = query;
    setModel(m_model);
    QString result = tr("Query finished with '%1' Rows.")
                         .arg(QString::number(m_model->rowCount()));
    emit sendQueryReport(result);
    return true;
  } else {
    emit sendQueryReport(tr("Query without result"));
  }
  return false;
}

void BookTableView::contextMenuEvent(QContextMenuEvent *event) {
  p_modelIndex = indexAt(event->pos());
  bool enable_action = p_modelIndex.isValid();
  bool enable_create = (m_model->rowCount() > 0);

  QMenu *m = new QMenu(this);
  // Eintrag öffnen  Bestellung anlegen
  QAction *ac_open = m->addAction(pIcon("database"), tr("Open entry"));
  ac_open->setObjectName("ac_context_open_book");
  ac_open->setEnabled(enable_action);
  connect(ac_open, SIGNAL(triggered()), this, SLOT(createOpenBookEntry()));

  QAction *ac_create = m->addAction(pIcon("db_add"), tr("Create entry"));
  ac_create->setObjectName("ac_context_create_book");
  ac_create->setEnabled(enable_create);
  connect(ac_create, SIGNAL(triggered()), this, SIGNAL(sendCreateNewEntry()));

  // BEGIN Einträge für Auftrag
  QAction *ac_copy = m->addAction(pIcon("db_comit"), tr("Copy Article Id"));
  ac_copy->setObjectName("ac_context_copy_book");
  ac_copy->setEnabled(enable_action);
  connect(ac_copy, SIGNAL(triggered()), this, SLOT(createCopyClipboard()));

  QAction *ac_order =
      m->addAction(pIcon("view_log"), tr("Add Article to current open Order"));
  ac_order->setObjectName("ac_context_book_to_order");
  ac_order->setEnabled(enable_action);
  connect(ac_order, SIGNAL(triggered()), this, SLOT(createIdToOrderSignal()));
  //  END

  QAction *ac_refresh = m->addAction(pIcon("action_reload"), tr("Update"));
  ac_refresh->setObjectName("ac_context_refresh_books");
  connect(ac_refresh, SIGNAL(triggered()), this, SLOT(setReloadView()));

  m->exec(event->globalPos());
  delete m;
}

void BookTableView::setSortByColumn(int column, Qt::SortOrder order) {
  if (column < 0)
    return;

  QString order_by = m_model->fieldName(column);
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

void BookTableView::getSqlModelError(const QString &table,
                                     const QString &message) {
  qDebug() << Q_FUNC_INFO << "TODO" << table << message;
}

void BookTableView::getSelectedItem(const QModelIndex &index) {
  qint64 id = getArticleID(index);
  if (id >= 1)
    emit sendOpenEntry(id);
}

void BookTableView::createOpenBookEntry() {
  qint64 id = getArticleID(p_modelIndex);
  if (id >= 1)
    emit sendOpenEntry(id);
}

void BookTableView::createCopyClipboard() {
  qint64 id = getArticleID(p_modelIndex);
  if (id >= 1)
    emit sendCopyToClibboard(QString::number(id));
}

void BookTableView::createIdToOrderSignal() {
  qint64 id = getArticleID(p_modelIndex);
  if (id >= 1)
    emit sendArticleId(id);
}

void BookTableView::setReloadView() {
  sqlQueryTable(m_model->query().lastQuery());
}

bool BookTableView::setQuery(const QString &clause) {
  AntiquaCRM::ASqlFiles query("query_tab_books_main");
  if (query.openTemplate()) {
    where_clause = (clause.isEmpty() ? where_clause : clause);
    query.setWhereClause(where_clause);
    query.setOrderBy("ib_id");
    query.setSorting(Qt::AscendingOrder);
    query.setLimits(getQueryLimit());
  }
  return sqlQueryTable(query.getQueryContent());
}
