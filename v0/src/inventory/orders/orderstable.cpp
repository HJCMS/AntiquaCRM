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
    QMutex mutex;
    mutex.lock();
    QTime time = QTime::currentTime();
    m_queryModel->setQuery(statement, db);
    if (m_queryModel->lastError().isValid()) {
#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << Q_FUNC_INFO << "{SQL Query} " << statement << "{SQL Error} "
               << m_queryModel->lastError() << Qt::endl
               << m_sql->fetchErrors() << Qt::endl;
#endif
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

  // Kundendaten aufrufen
  QAction *ac_customer =
      m->addAction(myIcon("group"), tr("View Customer data"));
  ac_customer->setObjectName("ac_context_open_customer");
  ac_customer->setEnabled(b);
  connect(ac_customer, SIGNAL(triggered()), this, SLOT(openCustomer()));

  m->exec(ev->globalPos());
  delete m;
}

void OrdersTable::setCustomQuery(const QString &from) {
  if (from.isEmpty())
    return;

  sqlExecQuery(advancedOrdersQuery(from));
}

void OrdersTable::refreshView() { initOrders(); }

void OrdersTable::initOrders() { sqlExecQuery(defaultOrdersQuery()); }

void OrdersTable::openCustomer() {
  QModelIndex im(p_modelIndex);
  if (m_queryModel->data(im.sibling(im.row(), 0), Qt::EditRole).toInt() >= 1) {
    int i = m_queryModel->data(im.sibling(im.row(), 0), Qt::EditRole).toInt();
    if (i < 1)
      return;

    QString sql = queryCustomerByOrderId(i);
    QSqlQuery q = m_sql->query(sql);
    if (q.size() > 0) {
      q.next();
      int c_id = q.value("c_id").toInt();
      if (c_id > 0)
        emit s_openCustomer(c_id);
    }
  }
}

void OrdersTable::searchHighlight(const QString &txt, int cell) {
  QModelIndex im = indexAt(QPoint(1, 1));
  for (int r = 0; r < m_queryModel->rowCount(); r++) {
    QString content;
    QVariant val = m_queryModel->data(im.sibling(r, cell), Qt::DisplayRole);
    if (val.type() == QVariant::Int) {
      content = QString::number(val.toInt());
    } else {
      content = val.toString();
    }
    if (content.contains(txt, Qt::CaseInsensitive)) {
      selectRow(r);
      return;
    }
    content.clear();
    selectRow(0);
  }
}
