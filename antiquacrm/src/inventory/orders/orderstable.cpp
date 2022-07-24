// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderstable.h"
#include "myicontheme.h"
#include "orderstablemodel.h"
#include "orderstatements.h"

#include <QAction>
#include <QDebug>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QMenu>
#include <QMessageBox>
#include <QMutex>
#include <QPoint>
#include <QRegExp>
#include <QSignalMapper>
#include <QSqlTableModel>
#include <QTime>

OrdersTable::OrdersTable(QWidget *parent) : QTableView{parent} {
  setObjectName("AssigmentTableView");
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setCornerButtonEnabled(false);
  setSortingEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);
  setAlternatingRowColors(true);
  setSelectionBehavior(QAbstractItemView::SelectRows);

  m_sql = new HJCMS::SqlCore(this);

  m_queryModel = new OrdersTableModel(this);
  m_queryModel->setObjectName("orders_table_model");
  setModel(m_queryModel);

  /* Kopfzeilen anpassen */
  QHeaderView *tHeader = horizontalHeader();
  tHeader->setDefaultAlignment(Qt::AlignCenter);
  tHeader->setStretchLastSection(true);

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(queryOrder(const QModelIndex &)));
}

bool OrdersTable::sqlExecQuery(const QString &statement) {
  if (statement.isEmpty())
    return false;

  if (SHOW_ORDER_SQL_QUERIES) {
    qDebug() << Q_FUNC_INFO << statement;
  }

  QSqlDatabase db(m_sql->db());
  if (db.open()) {
    QMutex mutex(QMutex::NonRecursive);
    mutex.lock();
    QTime time = QTime::currentTime();
    m_queryModel->setQuery(statement, db);
    if (m_queryModel->lastError().isValid()) {
      qDebug() << Q_FUNC_INFO << "{SQL Query} " << statement << "{SQL Error} "
               << m_queryModel->lastError() << Qt::endl
               << m_sql->fetchErrors() << Qt::endl;
      return false;
    }
    mutex.unlock();

    QString m = QString(tr("Rows: %1, Time: %2 msec."))
                    .arg(QString::asprintf("%d", m_queryModel->rowCount()),
                         QString::asprintf("%d", time.msec()));
    emit s_reportQuery(m);

    if (statement.contains("SELECT")) {
      resizeRowsToContents();
      resizeColumnsToContents();
    }
    return true;
  } else {
    qWarning("No SQL Connection in Booktable");
  }
  return false;
}

void OrdersTable::queryOrder(const QModelIndex &index) {
  QModelIndex id(index);
  if (m_queryModel->data(id.sibling(id.row(), 0), Qt::EditRole).toInt() >= 1) {
    int i = m_queryModel->data(id.sibling(id.row(), 0), Qt::EditRole).toInt();
    if (i < 1)
      return;

    emit s_editOrder(i);
  }
}

void OrdersTable::openByContext() { queryOrder(p_modelIndex); }

void OrdersTable::contextMenuEvent(QContextMenuEvent *ev) {
  p_modelIndex = indexAt(ev->pos());
  // Aktiviere/Deaktivieren der Einträge
  bool b = p_modelIndex.isValid();

  QMenu *m = new QMenu("Actions", this);
  // Eintrag öffnen  Bestellung anlegen
  QAction *ac_open = m->addAction(myIcon("spreadsheet"), tr("Open entry"));
  ac_open->setObjectName("ac_context_open_order");
  ac_open->setEnabled(b);
  connect(ac_open, SIGNAL(triggered()), this, SLOT(openByContext()));

  m->exec(ev->globalPos());
  delete m;
}

void OrdersTable::refreshView() { initOrders(); }

void OrdersTable::initOrders() {
  sqlExecQuery(defaultOrdersQuery());
}
