// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tableview.h"
#include "antiqua_global.h"
#include "applsettings.h"
#include "myicontheme.h"
#include "sqlcore.h"
#include "tablemodel.h"

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

// Schalte SQL ausgaben ein
#ifndef SHOW_SQL_QUERIES
#define SHOW_SQL_QUERIES false
#endif

static const QString defaultQuery() {
  QString fields("a.o_id,a.o_since,a.o_order_status,a.o_article_id");
  fields.append(",concat_ws(' ',c.c_firstname,c.c_lastname) AS costumer");
  fields.append(",d.d_name,a.o_locked,a.o_closed,a.o_order_type");
  fields.append(",age(CURRENT_TIMESTAMP,o_since) AS age");
  QString sql("SELECT " + fields + " ");
  sql.append("FROM inventory_orders AS a ");
  sql.append("LEFT JOIN costumers AS c ON c.c_id=a.o_costumer_id ");
  sql.append(
      "LEFT JOIN ref_delivery_service AS d ON d.d_id=a.o_delivery_service ");
  sql.append("WHERE o_closed=false ORDER BY o_since DESC;");
  return sql;
}

TableView::TableView(QWidget *parent) : QTableView{parent} {
  setObjectName("AssigmentTableView");
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

  m_queryModel = new TableModel(this);
  setModel(m_queryModel);

  /* Kopfzeilen anpassen */
  QHeaderView *tHeader = horizontalHeader();
  tHeader->setDefaultAlignment(Qt::AlignCenter);
  tHeader->setStretchLastSection(true);

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(queryOrder(const QModelIndex &)));
}

bool TableView::sqlExecQuery(const QString &statement) {
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
    return true;
  } else {
    qWarning("No SQL Connection in Booktable");
  }
  return false;
}

void TableView::queryOrder(const QModelIndex &index) {
  QModelIndex id(index);
  if (m_queryModel->data(id.sibling(id.row(), 0), Qt::EditRole).toInt() >= 1) {
    int i = m_queryModel->data(id.sibling(id.row(), 0), Qt::EditRole).toInt();

    if (i >= 1)
      emit s_editOrder(i);
  }
}

void TableView::openByContext() { queryOrder(p_modelIndex); }

void TableView::createByContext() { emit s_createOrder(); }

void TableView::orderByContext() { qDebug() << Q_FUNC_INFO << "_TODO_"; }

void TableView::contextMenuEvent(QContextMenuEvent *ev) {
  p_modelIndex = indexAt(ev->pos());
  // Aktiviere/Deaktivieren der Einträge
  bool b = p_modelIndex.isValid();

  QMenu *m = new QMenu("Actions", this);
  // Eintrag öffnen  Bestellung anlegen
  QAction *ac_open = m->addAction(myIcon("spreadsheet"), tr("Open entry"));
  ac_open->setObjectName("ac_context_open_book");
  ac_open->setEnabled(b);
  connect(ac_open, SIGNAL(triggered()), this, SLOT(openByContext()));

  QAction *ac_create = m->addAction(myIcon("db_add"), tr("Create entry"));
  ac_create->setObjectName("ac_context_create_book");
  ac_create->setEnabled(b);
  connect(ac_create, SIGNAL(triggered()), this, SLOT(createByContext()));

  QAction *ac_order = m->addAction(myIcon("autostart"), tr("Create order"));
  ac_order->setObjectName("ac_context_order_book");
  connect(ac_order, SIGNAL(triggered()), this, SLOT(orderByContext()));
  ac_order->setEnabled(b);

  m->exec(ev->globalPos());
  delete m;
}

void TableView::refreshView() { initOrders(); }

void TableView::initOrders() {
  if (sqlExecQuery(defaultQuery())) {
    resizeRowsToContents();
    resizeColumnsToContents();
  }
}
