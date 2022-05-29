// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customertableview.h"
#include "customerstablemodel.h"
#include "customerstatements.h"
#include "myicontheme.h"
#include "searchbar.h"
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
  setEditTriggers(QAbstractItemView::DoubleClicked);
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
      qDebug() << Q_FUNC_INFO << "{SQL Query} " << statement << "{SQL Error} "
               << m_tableModel->lastError() << Qt::endl
               << m_sql->fetchErrors() << Qt::endl;
      return false;
    }
    mutex.unlock();
    QString m = QString(tr("Rows: %1, Time: %2 msec."))
                    .arg(QString::asprintf("%d", m_tableModel->rowCount()),
                         QString::asprintf("%d", time.msec()));
    emit s_reportQuery(m);
    return true;
  } else {
    qWarning("No SQL Connection for Customers query.");
  }
  return false;
}

bool CustomerTableView::queryCustomerID(const QModelIndex &index,
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
  }
  return false;
}

void CustomerTableView::queryCustomerID(const QModelIndex &index) {
  queryCustomerID(index, QueryType::Update);
}

void CustomerTableView::openByContext() {
  queryCustomerID(p_modelIndex, QueryType::Update);
}

void CustomerTableView::createByContext() {
  // queryCustomerID(p_modelIndex, QueryType::Create);
  emit s_insertCustomer();
}

void CustomerTableView::orderByContext() {
  queryCustomerID(p_modelIndex, QueryType::Order);
}

void CustomerTableView::contextMenuEvent(QContextMenuEvent *ev) {
  p_modelIndex = indexAt(ev->pos());
  // Aktiviere/Deaktivieren der Einträge
  bool b = p_modelIndex.isValid();

  QMenu *m = new QMenu("Actions", this);
  // Eintrag öffnen  Bestellung anlegen
  QAction *ac_open = m->addAction(myIcon("spreadsheet"), tr("Open entry"));
  ac_open->setObjectName("ac_context_open_customer");
  ac_open->setEnabled(b);
  connect(ac_open, SIGNAL(triggered()), this, SLOT(openByContext()));

  QAction *ac_create = m->addAction(myIcon("db_add"), tr("Create entry"));
  ac_create->setObjectName("ac_context_create_customer");
  ac_create->setEnabled(true);
  connect(ac_create, SIGNAL(triggered()), this, SLOT(createByContext()));

  QAction *ac_order = m->addAction(myIcon("autostart"), tr("Create order"));
  ac_order->setObjectName("ac_context_order_customer");
  connect(ac_order, SIGNAL(triggered()), this, SLOT(orderByContext()));
  ac_order->setEnabled(b);

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
  if (!isVisible())
    return;

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

  if (SHOW_SQL_QUERIES)
    qDebug() << Q_FUNC_INFO << q;

  if (sqlExecQuery(q)) {
    resizeRowsToContents();
    resizeColumnsToContents();
    p_historyQuery = q;
  }
}

void CustomerTableView::queryStatement(const SearchStatement &search) {
  QString str(search.SearchString);
  QRegExp reg("\\s+");
  QStringList list = str.split(reg);
  QStringList clause;
  foreach (QString s, list) {
    QString buffer;
    buffer.append("(c_firstname ILIKE '" + s + "%' ");
    buffer.append("OR c_lastname ILIKE '" + s + "%')");
    clause.append(buffer);
    clause.append("(c_company_name ILIKE '%" + s + "%')");
  }

  QString q = c_sqlTableQueryBody();
  q.append(clause.join(" OR "));
  q.append(" ORDER BY " + search.SearchField);
  q.append(" ASC;");

  if (sqlExecQuery(q)) {
    resizeRowsToContents();
    resizeColumnsToContents();
    p_historyQuery = q;
  }
}