// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "costumertableview.h"
#include "applsettings.h"
#include "costumerstablemodel.h"
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

  //  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
  //          SLOT(queryArticleID(const QModelIndex &)));
}

bool CostumerTableView::sqlExecQuery(const QString &statement) {
  if (!statement.contains("SELECT"))
    return false;

  QSqlDatabase db(m_sql->db());
  if (db.open()) {
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
    qWarning("No SQL Connection in Booktable");
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

void CostumerTableView::orderByContext() {
  qDebug() << Q_FUNC_INFO << "Noch nicht Implementiert";
}

void CostumerTableView::contextMenuEvent(QContextMenuEvent *ev) {
  p_modelIndex = indexAt(ev->pos());
  // Aktiviere/Deaktivieren der Einträge
  bool b = p_modelIndex.isValid();

  QMenu *m = new QMenu("Actions", this);
  // Eintrag öffnen  Bestellung anlegen
  QAction *ac_open = m->addAction(myIcon("spreadsheet"), tr("Open entry"));
  ac_open->setObjectName("ac_context_open_costumer");
  ac_open->setEnabled(b);
  // connect(ac_open, SIGNAL(triggered()), this, SLOT(openByContext()));

  QAction *ac_create = m->addAction(myIcon("db_add"), tr("Create entry"));
  ac_create->setObjectName("ac_context_create_costumer");
  ac_create->setEnabled(b);
  // connect(ac_create, SIGNAL(triggered()), this, SLOT(createByContext()));

  QAction *ac_order = m->addAction(myIcon("autostart"), tr("Create order"));
  ac_order->setObjectName("ac_context_order_costumer");
  // connect(ac_order, SIGNAL(triggered()), this, SLOT(orderByContext()));
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

void CostumerTableView::queryStatement(const QString &search) {
/*
c_id               | integer
c_trusted          | smallint
c_transactions     | smallint
c_purchases        | smallint
c_company          | boolean
c_company_name     | character varying(80)
c_company_employer | character varying(80)
c_gender           | smallint
c_title            | character varying(25)
c_firstname        | character varying(80)
c_lastname         | character varying(80)
c_postalcode       | character varying(10)
c_country          | character varying(100)
c_location         | character varying(80)
c_street           | character varying(80)
c_email_0          | character varying(80)
c_email_1          | character varying(80)
c_shipping_email   | character varying(80)
c_website          | character varying(80)
c_phone_0          | character varying(21)
c_phone_1          | character varying(21)
c_mobil_0          | character varying(21)
c_mobil_1          | character varying(21)
c_fax_0            | character varying(21)
c_postal_address   | text
c_shipping_address | text
c_comments         | text
c_iban             | character varying(34)
c_swift_bic        | character varying(11)
c_tax_id           | character varying(34)
c_since            | timestamp without time zone
c_changed          | timestamp without time zone
*/

  QString sql("SELECT c_id,c_transactions,c_firstname");
  sql.append(",c_lastname,c_country,c_location,c_street,c_phone_0");
  sql.append(" FROM costumers");
  sql.append(" WHERE c_id!=0 ");
  sql.append(" ORDER BY c_id;");
  if (sqlExecQuery(sql)) {
    resizeRowsToContents();
    resizeColumnsToContents();
    p_historyQuery = sql;
  }
}
