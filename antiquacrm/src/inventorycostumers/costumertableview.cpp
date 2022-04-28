// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "costumertableview.h"
#include "applsettings.h"
#include "costumerstablemodel.h"
#include "searchbar.h"
#include "sqlcore.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QItemSelectionModel>
#include <QtCore/QMutex>
#include <QtCore/QPoint>
#include <QtCore/QRegExp>
#include <QtCore/QSignalMapper>
#include <QtCore/QTime>
#include <QtSql/QSqlTableModel>
#include <QtWidgets/QAction>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMenu>

// Schalte SQL ausgaben ein
#ifndef SHOW_SQL_QUERIES
#define SHOW_SQL_QUERIES false
#endif

CostumerTableView::CostumerTableView(QWidget *parent) : QTableView{parent} {
  setObjectName("CostumerTableView");
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

  m_tableModel = new CostumersTableModel(this);
  setModel(m_tableModel);

  /* Kopfzeilen anpassen */
  QHeaderView *tHeader = horizontalHeader();
  tHeader->setDefaultAlignment(Qt::AlignCenter);
  tHeader->setStretchLastSection(true);

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(queryCostumerID(const QModelIndex &)));
}

bool CostumerTableView::sqlExecQuery(const QString &statement) {
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
    qWarning("No SQL Connection for Costumers query.");
  }
  return false;
}

void CostumerTableView::queryCostumerID(const QModelIndex &index) {
  QModelIndex id(index);
  if (m_tableModel->data(id.sibling(id.row(), 0), Qt::EditRole).toInt() >= 1) {
    int i = m_tableModel->data(id.sibling(id.row(), 0), Qt::EditRole).toInt();

    if (i >= 1)
      emit s_updateCostumer(i);
  }
}

void CostumerTableView::openByContext() { queryCostumerID(p_modelIndex); }

void CostumerTableView::createByContext() { emit s_insertCostumer(); }

void CostumerTableView::orderByContext() { qDebug() << Q_FUNC_INFO << "TODO"; }

void CostumerTableView::contextMenuEvent(QContextMenuEvent *ev) {
  p_modelIndex = indexAt(ev->pos());
  // Aktiviere/Deaktivieren der Einträge
  bool b = p_modelIndex.isValid();

  QMenu *m = new QMenu("Actions", this);
  // Eintrag öffnen  Bestellung anlegen
  QAction *ac_open = m->addAction(myIcon("spreadsheet"), tr("Open entry"));
  ac_open->setObjectName("ac_context_open_costumer");
  ac_open->setEnabled(b);
  connect(ac_open, SIGNAL(triggered()), this, SLOT(openByContext()));

  QAction *ac_create = m->addAction(myIcon("db_add"), tr("Create entry"));
  ac_create->setObjectName("ac_context_create_costumer");
  ac_create->setEnabled(b);
  connect(ac_create, SIGNAL(triggered()), this, SLOT(createByContext()));

  QAction *ac_order = m->addAction(myIcon("autostart"), tr("Create order"));
  ac_order->setObjectName("ac_context_order_costumer");
  connect(ac_order, SIGNAL(triggered()), this, SLOT(orderByContext()));
  ac_order->setEnabled(b);

  m->exec(ev->globalPos());
  delete m;
}

void CostumerTableView::refreshView() {
  if (sqlExecQuery(p_historyQuery)) {
    resizeRowsToContents();
    resizeColumnsToContents();
  }
}

void CostumerTableView::queryStatement(const SearchStatement &search) {
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

  QString q("SELECT c_id AS id, c_purchases,");
  q.append("(CASE WHEN c_company IS TRUE THEN 'C' ELSE 'P' END) AS company,");
  q.append("concat_ws(' ',c_title,c_firstname,c_lastname) AS shurename,");
  q.append("(CASE WHEN c_phone_0 IS NULL THEN c_phone_1 ELSE c_phone_0 END) AS "
           "phone,");
  q.append("(CASE WHEN c_mobil_0 IS NULL THEN c_mobil_1 ELSE c_mobil_0 END) AS "
           "mobil,");
  q.append("concat_ws(' ',c_postalcode,c_location,c_street) AS address,");
  q.append("c_since AS since ");
  q.append("FROM public.costumers WHERE c_id!=0 AND ");
  q.append(clause.join(" OR "));
  q.append(" ORDER BY " + search.SearchField);
  q.append(" ASC;");

  if (sqlExecQuery(q)) {
    resizeRowsToContents();
    resizeColumnsToContents();
    p_historyQuery = q;
  }
}
