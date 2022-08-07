// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookstable.h"
#include "bookstablemodel.h"
#include "bookstatements.h"
#include "myicontheme.h"

#include <QDebug>
#include <QAction>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QKeySequence>
#include <QMenu>
#include <QPainter>
#include <QPoint>
#include <QMutex>
#include <QRegExp>
#include <QSignalMapper>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QStatusTipEvent>
#include <QTime>

BooksTable::BooksTable(QWidget *parent) : QTableView{parent} {
  setObjectName("BooksTable");
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setCornerButtonEnabled(false);
  setSortingEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);
  setAlternatingRowColors(true);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);
  setStatusTip(
      tr("A table row must be selected in order to perform an action."));

  m_sql = new HJCMS::SqlCore(this);

  m_queryModel = new BooksTableModel(this);
  m_queryModel->setObjectName("book_table_model");
  setModel(m_queryModel);

  /* Kopfzeilen anpassen */
  QHeaderView *tHeader = horizontalHeader();
  tHeader->setDefaultAlignment(Qt::AlignCenter);
  tHeader->setStretchLastSection(true);

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(articleClicked(const QModelIndex &)));
}

bool BooksTable::sqlExecQuery(const QString &statement) {
  if (!statement.contains("SELECT"))
    return false;

#ifdef BOOK_INVENTORY_DEBUG
  qDebug() << Q_FUNC_INFO << statement;
#endif

  QSqlDatabase db(m_sql->db());
  if (db.open()) {
    QMutex mutex;
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

int BooksTable::queryArticleID(const QModelIndex &index) {
  QModelIndex id(index);
  if (m_queryModel->data(id.sibling(id.row(), 0), Qt::EditRole).toInt() >= 1) {
    return m_queryModel->data(id.sibling(id.row(), 0), Qt::EditRole).toInt();
  }
  return -1;
}

void BooksTable::articleClicked(const QModelIndex &index) {
  int i = queryArticleID(index);
  if (i >= 1)
    emit s_articleSelected(i);
}

void BooksTable::openByContext() {
  int i = queryArticleID(p_modelIndex);
  if (i >= 1)
    emit s_articleSelected(i);
}

void BooksTable::createByContext() { emit s_newEntryPlease(); }

void BooksTable::contextMenuEvent(QContextMenuEvent *ev) {
  p_modelIndex = indexAt(ev->pos());
  /**
   * Aktiviere/Deaktivieren der Einträge, wenn das Model gültig ist!
   */
  bool b = p_modelIndex.isValid();

  /**
   * Eintrag erstellen erst freischalten wenn Tabelle nicht leer ist!
   */
  bool bn = (m_queryModel->rowCount() > 0);

  QMenu *m = new QMenu(this);

  // Eintrag öffnen  Bestellung anlegen
  QAction *ac_open = m->addAction(myIcon("spreadsheet"), tr("Open entry"));
  ac_open->setObjectName("ac_context_open_book");
  ac_open->setEnabled(b);
  connect(ac_open, SIGNAL(triggered()), this, SLOT(openByContext()));

  QAction *ac_create = m->addAction(myIcon("db_add"), tr("Create entry"));
  ac_create->setObjectName("ac_context_create_book");
  ac_create->setEnabled(bn);
  connect(ac_create, SIGNAL(triggered()), this, SLOT(createByContext()));

  // BEGIN Einträge für Auftrag
  QAction *ac_copy = m->addAction(myIcon("edit"), tr("Copy Article Id"));
  ac_copy->setObjectName("ac_context_copy_book");
  ac_copy->setEnabled(b);
  connect(ac_copy, SIGNAL(triggered()), this, SLOT(copyToClipboard()));

  QAction *ac_order =
      m->addAction(myIcon("autostart"), tr("add Article to opened Order"));
  ac_order->setObjectName("ac_context_book_to_order");
  ac_order->setEnabled(b);
  connect(ac_order, SIGNAL(triggered()), this, SLOT(createOrderSignal()));
  // END

  // Zeige alle Offenen Auftrags Artikel
  QAction *ac_orders = m->addAction(myIcon("edit"), tr("Open Orders"));
  ac_orders->setObjectName("ac_context_open_orders");
  connect(ac_orders, SIGNAL(triggered()), this, SLOT(queryViewOpenOrders()));

  QAction *ac_refresh = m->addAction(myIcon("reload"), tr("Refresh"));
  ac_refresh->setObjectName("ac_context_refresh_books");
  connect(ac_refresh, SIGNAL(triggered()), this, SLOT(refreshView()));

  m->exec(ev->globalPos());
  delete m;
}

void BooksTable::copyToClipboard() {
  QVariant id = queryArticleID(p_modelIndex);
  emit s_toClibboard(id);
}

void BooksTable::createOrderSignal() {
  QVariant id = queryArticleID(p_modelIndex);
  emit s_articleToOrders(id.toInt());
}

void BooksTable::refreshView() {
  if (sqlExecQuery(p_historyQuery)) {
    resizeRowsToContents();
    resizeColumnsToContents();
  }
}

void BooksTable::queryHistory(const QString &query) {
  QString q("SELECT ");
  q.append(InventoryBooksSelect());
  q.append(InventoryBooksTables());
  q.append(" WHERE ");
  if (query.contains("#today")) {
    q.append("DATE(ib_changed)=(DATE(now()))");
  } else if (query.contains("#yesterday")) {
    q.append("DATE(ib_changed)=(DATE(now() - INTERVAL '1 day'))");
  } else if (query.contains("#last7days")) {
    q.append("DATE(ib_changed)>=(DATE(now() - INTERVAL '7 days'))");
  } else if (query.contains("#thismonth")) {
    q.append("EXTRACT(MONTH FROM ib_changed)=(EXTRACT(MONTH FROM now()))");
    q.append(" AND ib_count>0");
  } else if (query.contains("#thisyear")) {
    q.append("EXTRACT(ISOYEAR FROM ib_changed)=(EXTRACT(YEAR FROM now()))");
    q.append(" AND ib_count>0");
  } else {
    return;
  }
  q.append(" ORDER BY ib_count DESC LIMIT ");
  q.append(QString::number(maxRowCount));
  q.append(";");

  if (sqlExecQuery(q)) {
    resizeRowsToContents();
    resizeColumnsToContents();
    p_historyQuery = q;
  }
}

void BooksTable::queryStatement(const QString &statement) {
  QString q("SELECT DISTINCT ");
  q.append(InventoryBooksSelect());
  q.append(InventoryBooksTables());
  q.append(" WHERE ");
  q.append(statement);
  q.append(" ORDER BY ib_count DESC LIMIT ");
  q.append(QString::number(maxRowCount));
  q.append(";");

  if (sqlExecQuery(q)) {
    resizeRowsToContents();
    resizeColumnsToContents();
    p_historyQuery = q;
  }
}

void BooksTable::queryViewOpenOrders() {
  QString q("SELECT * FROM view_books_from_open_orders");
  q.append(" LIMIT ");
  q.append(QString::number(maxRowCount));
  q.append(";");

  if (sqlExecQuery(q)) {
    resizeRowsToContents();
    resizeColumnsToContents();
    p_historyQuery = q;
  }
}
