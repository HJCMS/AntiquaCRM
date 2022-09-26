// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customertableview.h"
#include "customerstablemodel.h"
#include "customerstatements.h"
#include "myicontheme.h"
#include "searchfilter.h"
#include "sqlcore.h"

#include <QAction>
#include <QDebug>
#include <QMenu>
#include <QMutex>
#include <QPoint>
#include <QRegExp>
#include <QSignalMapper>
#include <QStringList>
#include <QTime>

CustomerTableView::CustomerTableView(QWidget *parent) : QTableView{parent} {
  setObjectName("CustomerTableView");
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setCornerButtonEnabled(false);
  setSortingEnabled(true);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);
  setAlternatingRowColors(true);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);

  m_sql = new HJCMS::SqlCore(this);

  m_tableModel = new CustomersTableModel(this);
  setModel(m_tableModel);

  /* Kopfzeilen anpassen */
  m_headerView = horizontalHeader();
  m_headerView->setDefaultAlignment(Qt::AlignCenter);
  m_headerView->setStretchLastSection(true);

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(queryCustomerID(const QModelIndex &)));

  connect(m_headerView, SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this,
          SLOT(sortTableView(int, Qt::SortOrder)));
}

bool CustomerTableView::sqlExecQuery(const QString &statement) {
  if (!statement.contains("SELECT"))
    return false;

  QSqlDatabase db(m_sql->db());
  if (db.open()) {
    if (SHOW_SQL_QUERIES) {
      qDebug() << Q_FUNC_INFO << statement;
    }
    QMutex mutex;
    mutex.lock();
    QTime time = QTime::currentTime();
    m_tableModel->setQuery(statement, db);

    if (m_tableModel->lastError().isValid()) {
      mutex.unlock();
#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << Q_FUNC_INFO << "{SQL Query} " << statement << "{SQL Error} "
               << m_tableModel->lastError() << Qt::endl
               << m_sql->fetchErrors() << Qt::endl;
#endif
      emit s_reportQuery(tr("Broken Database query for Customers table."));
      return false;
    }
    mutex.unlock();
    QString m = QString(tr("Rows: %1, Time: %2 msec."))
                    .arg(QString::asprintf("%d", m_tableModel->rowCount()),
                         QString::asprintf("%d", time.msec()));
    emit s_reportQuery(m);
    return true;
  } else {
    emit s_reportQuery(tr("Missing Database connection for Customers."));
  }
  return false;
}

bool CustomerTableView::queryCustomerAction(const QModelIndex &index,
                                            QueryType type) {
  QModelIndex id(index);
  if (m_tableModel->data(id.sibling(id.row(), 0), Qt::EditRole).toInt() >= 1) {
    int i = m_tableModel->data(id.sibling(id.row(), 0), Qt::EditRole).toInt();
    if (i < 1)
      return false;

    if (type == Update) {
      emit s_updateCustomer(i);
      return true;
    }
    if (type == Order) {
      emit s_createOrder(i);
      return true;
    }
    if (type == Delete) {
      emit s_deleteCustomer(i);
      return true;
    }
  }
  return false;
}

void CustomerTableView::sortTableView(int cell, Qt::SortOrder sort) {
  p_orderBy = QString("c_since DESC");
  QString fieldName = m_tableModel->record().field(cell).name();
  if (fieldName.isEmpty())
    return;

  QString sortOrder = (sort == Qt::AscendingOrder) ? "DESC" : "ASC";
  p_orderBy = QString(fieldName + " " + sortOrder);
  refreshView();
}

void CustomerTableView::queryCustomerID(const QModelIndex &index) {
  queryCustomerAction(index, QueryType::Update);
}

void CustomerTableView::openByContext() {
  queryCustomerAction(p_modelIndex, QueryType::Update);
}

void CustomerTableView::createByContext() { emit s_insertCustomer(); }

void CustomerTableView::orderByContext() {
  queryCustomerAction(p_modelIndex, QueryType::Order);
}

void CustomerTableView::deleteUserContext() {
  queryCustomerAction(p_modelIndex, QueryType::Delete);
}

void CustomerTableView::contextMenuEvent(QContextMenuEvent *ev) {
  p_modelIndex = indexAt(ev->pos());
  // Aktiviere/Deaktivieren der Einträge
  bool b = p_modelIndex.isValid();

  QMenu *m = new QMenu("Actions", this);
  // Eintrag öffnen  Bestellung anlegen
  QAction *ac_open = m->addAction(myIcon("edit_group"), tr("Edit this Customer"));
  ac_open->setObjectName("ac_context_open_customer");
  ac_open->setEnabled(b);
  connect(ac_open, SIGNAL(triggered()), this, SLOT(openByContext()));

  QAction *ac_create = m->addAction(myIcon("add_user"), tr("Create a new Customer"));
  ac_create->setObjectName("ac_context_create_customer");
  ac_create->setEnabled(true);
  connect(ac_create, SIGNAL(triggered()), this, SLOT(createByContext()));

  QAction *ac_order =
      m->addAction(myIcon("autostart"), tr("Create a new order for this customer"));
  ac_order->setObjectName("ac_context_order_customer");
  connect(ac_order, SIGNAL(triggered()), this, SLOT(orderByContext()));
  ac_order->setEnabled(b);
  m->addSeparator();

  QAction *ac_delete =
      m->addAction(myIcon("delete_user"), tr("Delete customer"));
  ac_delete->setObjectName("ac_context_delete_customer");
  ac_delete->setEnabled(b);
  connect(ac_delete, SIGNAL(triggered()), this, SLOT(deleteUserContext()));

  m->exec(ev->globalPos());
  delete m;
}

void CustomerTableView::refreshView() {
  if (p_historyQuery.isEmpty())
    return;

  QString query(p_historyQuery);
  query.replace("@ORDER_BY@", p_orderBy);
  if (sqlExecQuery(query)) {
    resizeRowsToContents();
    resizeColumnsToContents();
  }
}

void CustomerTableView::queryHistory(const QString &query) {
  QString q = c_sqlTableQueryBody();
  q.append(" ");
  QString thisYear("date_part('year',c_since)=date_part('year',CURRENT_DATE)");
  if (query.contains("#today")) {
    q.append("DATE(c_changed)=CURRENT_DATE");
  } else if (query.contains("#yesterday")) {
    q.append("DATE(c_changed)=(CURRENT_DATE -1)");
  } else if (query.contains("#thisweek")) {
    q.append("date_part('week',c_since)=date_part('week',CURRENT_DATE)");
    q.append(" AND " + thisYear);
  } else if (query.contains("#lastweek")) {
    q.append("date_part('week',c_since)=date_part('week',CURRENT_DATE - 7)");
    q.append(" AND " + thisYear);
  } else if (query.contains("#thismonth")) {
    q.append("date_part('month', c_since)=date_part('month', CURRENT_DATE)");
    q.append(" AND " + thisYear);
  } else if(query.contains("#lastmonth")) {
    q.append("date_part('month', c_since)=date_part('month', CURRENT_DATE - 31)");
    q.append(" AND " + thisYear);
  } else if (query.contains("#thisyear")) {
    q.append(thisYear);
  } else {
    q.append("DATE(c_changed)=CURRENT_DATE");
  }
  q.append(" ORDER BY @ORDER_BY@ LIMIT ");
  q.append(QString::number(maxRowCount));
  q.append(";");

  QString sql(q);
  sql.replace("@ORDER_BY@", p_orderBy);
  if (sqlExecQuery(sql)) {
    resizeRowsToContents();
    resizeColumnsToContents();
    p_historyQuery = q;
  }
}

void CustomerTableView::queryStatement(const SearchFilter &cl) {
  SearchFilter search(cl);
  QString orderBy = search.getFields().join("").trimmed();
  QString str(search.getSearch());
  QRegExp reg("\\s+");
  QStringList list = str.split(reg);
  QStringList clause;
  if (search.getType() == SearchFilter::NUMERIC) {
    clause.append(orderBy + "=" + str);
  } else {
    foreach (QString s, list) {
      QString buffer;
      buffer.append("(c_firstname ILIKE '" + s + "%' ");
      buffer.append("OR c_lastname ILIKE '" + s + "%')");
      clause.append(buffer);
      clause.append("(c_company_name ILIKE '%" + s + "%')");
    }
  }
  QString q = c_sqlTableQueryBody();
  q.append(clause.join(" OR "));
  q.append(" ORDER BY @ORDER_BY@;");

  QString query(q);
  query.replace("@ORDER_BY@", p_orderBy);

  if (sqlExecQuery(query)) {
    resizeRowsToContents();
    resizeColumnsToContents();
    // qDebug() << q;
    p_historyQuery = q;
  }
}

int CustomerTableView::rowCount() { return m_tableModel->rowCount(); }
