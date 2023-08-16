// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookstableview.h"
#include "bookstablemodel.h"

BooksTableView::BooksTableView(QWidget *parent)
    : AntiquaCRM::TableView{parent} {
  setEnableTableViewSorting(true);
  m_model = new BooksTableModel(this);
  where_clause = defaultWhereClause();
  connect(m_model, SIGNAL(sqlErrorMessage(const QString &, const QString &)),
          this, SLOT(sqlModelError(const QString &, const QString &)));

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(getSelectedItem(const QModelIndex &)));
}

qint64 BooksTableView::getTableID(const QModelIndex &index, int column) {
  QModelIndex id(index);
  if (m_model->data(id.sibling(id.row(), column), Qt::EditRole).toInt() >= 1) {
    return m_model->data(id.sibling(id.row(), column), Qt::EditRole).toInt();
  }
  return -1;
}

int BooksTableView::getArticleCount(const QModelIndex &index) {
  return getTableID(index, 1);
}

bool BooksTableView::sqlModelQuery(const QString &query) {
  // #ifdef ANTIQUA_DEVELOPEMENT
  //   qDebug() << Q_FUNC_INFO << query;
  // #endif
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

void BooksTableView::contextMenuEvent(QContextMenuEvent *event) {
  QModelIndex index = indexAt(event->pos());
  qint64 rows = m_model->rowCount();
  AntiquaCRM::TableContextMenu *m_menu =
      new AntiquaCRM::TableContextMenu(index, rows, this);
  m_menu->addOpenAction(tr("Open entry"));
  m_menu->addCreateAction(tr("Create entry"));
  m_menu->addCopyAction(tr("Copy Article Id"));
  m_menu->addOrderAction(tr("Add Article to opened Order"));
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

void BooksTableView::contextMenuAction(AntiquaCRM::TableContextMenu::Actions ac,
                                       const QModelIndex &index) {

  qint64 aid = getTableID(index);
  if (aid < 1)
    return;

  switch (ac) {
  case (AntiquaCRM::TableContextMenu::Actions::Open):
    emit sendOpenEntry(aid);
    break;

  case (AntiquaCRM::TableContextMenu::Actions::Order):
    createSocketOperation(index);
    break;

  case (AntiquaCRM::TableContextMenu::Actions::Copy):
    emit sendCopyToClibboard(QString::number(aid));
    break;

  default:
    qWarning("Unknown Menu context request!");
    return;
  };
}

void BooksTableView::setSortByColumn(int column, Qt::SortOrder order) {
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
  sqlModelQuery(query.getQueryContent());
}

void BooksTableView::getSelectedItem(const QModelIndex &index) {
  qint64 aid = getTableID(index);
  if (aid >= 1)
    emit sendOpenEntry(aid);
}

void BooksTableView::createSocketOperation(const QModelIndex &index) {
  qint64 aid = getTableID(index);
  if (aid >= 1 && getArticleCount(index) > 0) {
    QJsonObject obj;
    obj.insert("ACTION", "add_article");
    obj.insert("TARGET", "orders_tab");
    obj.insert("VALUE", QJsonValue(aid));
    emit sendSocketOperation(obj);
  } else {
    qInfo("Socket operation ignored (%lld)!", aid);
  }
}

void BooksTableView::setReloadView() {
  sqlModelQuery(m_model->query().lastQuery());
}

int BooksTableView::rowCount() { return m_model->rowCount(); }

bool BooksTableView::setQuery(const QString &clause) {
  AntiquaCRM::ASqlFiles query("query_tab_books_main");
  if (query.openTemplate()) {
    where_clause = (clause.isEmpty() ? where_clause : clause);
    query.setWhereClause(where_clause);
    query.setOrderBy("(ib_count,ib_changed,ib_id)");
    query.setSorting(Qt::DescendingOrder);
    query.setLimits(getQueryLimit());
  }
  return sqlModelQuery(query.getQueryContent());
}

const QString BooksTableView::defaultWhereClause() {
  return QString("DATE(ib_changed)=CURRENT_DATE");
}
