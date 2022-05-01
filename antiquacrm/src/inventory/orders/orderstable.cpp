// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderstable.h"
#include "antiqua_global.h"
#include "applsettings.h"
#include "myicontheme.h"
#include "orderspaymentbox.h"
#include "orderstablemodel.h"
#include "orderstatusbox.h"
#include "orderstatements.h"

#include <QtCore/QDebug>
#include <QtCore/QItemSelectionModel>
#include <QtCore/QMutex>
#include <QtCore/QPoint>
#include <QtCore/QRegExp>
#include <QtCore/QSignalMapper>
#include <QtCore/QTime>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>
#include <QtWidgets/QAction>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMessageBox>

OrdersTable::OrdersTable(QWidget *parent) : QTableView{parent} {
  setObjectName("AssigmentTableView");
  setEditTriggers(QAbstractItemView::DoubleClicked);
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

  if (SHOW_SQL_QUERIES) {
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

    if (i >= 1)
      emit s_editOrder(i);
  }
}

void OrdersTable::openByContext() { queryOrder(p_modelIndex); }

void OrdersTable::createByContext() { emit s_createOrder(); }

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

  QAction *ac_create = m->addAction(myIcon("db_add"), tr("Create order"));
  ac_create->setObjectName("ac_context_create_order");
  ac_create->setEnabled(true);
  connect(ac_create, SIGNAL(triggered()), this, SLOT(createByContext()));

  QAction *ac_status =
      m->addAction(myIcon("autostart"), tr("Update Progress status"));
  ac_status->setObjectName("ac_context_status_order");
  connect(ac_status, SIGNAL(triggered()), this, SLOT(updateOrderStatus()));
  ac_status->setEnabled(b);

  QAction *ac_payment =
      m->addAction(myIcon("autostart"), tr("Update Payment status"));
  ac_payment->setObjectName("ac_context_payment_order");
  connect(ac_payment, SIGNAL(triggered()), this, SLOT(updatePaymentStatus()));
  ac_payment->setEnabled(b);

  QAction *ac_close = m->addAction(myIcon("db_remove"), tr("Finish order"));
  ac_close->setObjectName("ac_context_payment_order");
  connect(ac_close, SIGNAL(triggered()), this, SLOT(closeInventoryOrder()));
  ac_close->setEnabled(b);

  m->exec(ev->globalPos());
  delete m;
}

void OrdersTable::refreshView() { initOrders(); }

void OrdersTable::initOrders() { sqlExecQuery(defaultQuery()); }

void OrdersTable::updateOrderStatus() {
  QModelIndexList list = selectedIndexes();
  int article_id = 0;
  for (int i = 0; i < list.size(); i++) {
    QModelIndex index = list.at(i);
    if (index.isValid() && (index.column() == 0))
      article_id = m_queryModel->data(index, Qt::EditRole).toInt();

    if (index.isValid() && (index.column() == status_column)) {
      QString title = m_queryModel->data(index, Qt::EditRole).toString();
      StatusDialog dialog(title, this);
      int set = dialog.exec();
      if (set == QDialog::Accepted) {
        int status = dialog.index();
        if (sqlExecQuery(progresUpdate(article_id, status)))
          sqlExecQuery(defaultQuery());

        return; // end loop
      }
    }
  }
}

void OrdersTable::updatePaymentStatus() {
  QModelIndexList list = selectedIndexes();
  int article_id = 0;
  for (int i = 0; i < list.size(); i++) {
    QModelIndex index = list.at(i);
    if (index.isValid() && (index.column() == 0))
      article_id = m_queryModel->data(index, Qt::EditRole).toInt();

    if (index.isValid() && (index.column() == payment_column)) {
      QString title = m_queryModel->data(index, Qt::EditRole).toString();
      PaymentStatusDialog dialog(title, this);
      int set = dialog.exec();
      if (set == QDialog::Accepted) {
        bool status = dialog.status();
        if (sqlExecQuery(paymentUpdate(article_id, status)))
          sqlExecQuery(defaultQuery());

        return; // end loop
      }
    }
  }
}

void OrdersTable::closeInventoryOrder() {
  QModelIndexList list = selectedIndexes();
  int order_id = 0;
  for (int i = 0; i < list.size(); i++) {
    QModelIndex index = list.at(i);
    if (index.isValid() && (index.column() == 0))
      order_id = m_queryModel->data(index, Qt::EditRole).toInt();

    if (index.isValid() && (index.column() == close_column)) {
      if (order_id < 1)
        return;

      QString body("<p>");
      body.append(tr("Do you really want to close this order and pass it on "
                     "to accounting?"));
      body.append("</p><p>");
      body.append(tr("If so, the entry will no longer be visible here!"));
      body.append("</p>");
      int ret = QMessageBox::question(this, tr("Finish order"), body);
      if (ret == QMessageBox::Yes) {
        if (sqlExecQuery(closeOrder(order_id)))
          sqlExecQuery(defaultQuery());
      }
      return; // end loop
    }
  }
}
