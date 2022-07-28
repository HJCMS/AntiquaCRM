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
#include <QHeaderView>
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
  setSortingEnabled(false);
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
  QHeaderView *tHeader = horizontalHeader();
  tHeader->setDefaultAlignment(Qt::AlignCenter);
  tHeader->setStretchLastSection(true);

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(queryCustomerID(const QModelIndex &)));
}

bool CustomerTableView::sqlExecQuery(const QString &statement) {
  if (!statement.contains("SELECT"))
    return false;

  QSqlDatabase db(m_sql->db());
  if (db.open()) {
    if (SHOW_SQL_QUERIES) {
      qDebug() << Q_FUNC_INFO << statement;
    }
    QMutex mutex(QMutex::NonRecursive);
    mutex.lock();
    QTime time = QTime::currentTime();
    m_tableModel->setQuery(statement, db);
    if (m_tableModel->lastError().isValid()) {
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
  QAction *ac_open = m->addAction(myIcon("edit_group"), tr("Edit customer"));
  ac_open->setObjectName("ac_context_open_customer");
  ac_open->setEnabled(b);
  connect(ac_open, SIGNAL(triggered()), this, SLOT(openByContext()));

  QAction *ac_create = m->addAction(myIcon("add_user"), tr("Create customer"));
  ac_create->setObjectName("ac_context_create_customer");
  ac_create->setEnabled(true);
  connect(ac_create, SIGNAL(triggered()), this, SLOT(createByContext()));

  QAction *ac_order =
      m->addAction(myIcon("autostart"), tr("Create order from customer"));
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
  if (sqlExecQuery(p_historyQuery)) {
    resizeRowsToContents();
    resizeColumnsToContents();
  }
}

void CustomerTableView::queryHistory(const QString &history) {
  QString q = c_sqlTableQueryBody();
  if (history.contains("#today")) {
    q.append("DATE(c_changed)=(DATE(now()))");
  } else if (history.contains("#yesterday")) {
    q.append("DATE(c_changed)=(DATE(now() - INTERVAL '1 day'))");
  } else if (history.contains("#last7days")) {
    q.append("DATE(c_changed)>=(DATE(now() - INTERVAL '7 days'))");
  } else if (history.contains("#thismonth")) {
    q.append("EXTRACT(MONTH FROM c_changed)=(EXTRACT(MONTH FROM now()))");
  } else if (history.contains("#thisyear")) {
    q.append("EXTRACT(ISOYEAR FROM c_changed)=(EXTRACT(YEAR FROM now()))");
  } else {
    return;
  }
  q.append(" ORDER BY c_id DESC LIMIT ");
  q.append(QString::number(maxRowCount));
  q.append(";");

  if (sqlExecQuery(q)) {
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
  q.append(" ORDER BY c_since DESC;");

  if (sqlExecQuery(q)) {
    resizeRowsToContents();
    resizeColumnsToContents();
    p_historyQuery = q;
  }
}

int CustomerTableView::rowCount() { return m_tableModel->rowCount(); }
