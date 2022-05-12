// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookstable.h"
#include "antiqua_global.h"
#include "applsettings.h"
#include "bookstablemodel.h"
#include "myicontheme.h"
#include "searchbar.h"

#include <QAction>
#include <QDebug>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QMenu>
#include <QMutex>
#include <QPoint>
#include <QRegExp>
#include <QSignalMapper>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QTime>

/**
    @brief querySelect
    Die Feldabfragen für die Suche sind immer gleich!
    Wenn sich etwas ändern sollte, dann muss in Klasse
    \ref BooksTableModel die Feld Definition geändert werden!
*/
static const QString querySelect() {
  QString s("b.ib_id,b.ib_count,b.ib_title,b.ib_author,");
  s.append("b.ib_publisher,b.ib_year,b.ib_price,s.sl_storage,b.ib_isbn,b.ib_"
           "changed");
  s.append(",(CASE WHEN i.im_id IS NOT NULL THEN true ELSE false END) AS "
           "image_exists ");
  // s.append("");
  return s;
}

/**
   @brief queryTables
   Tabellen Definition
*/
static const QString queryTables() {
  QString s(" FROM inventory_books AS b");
  s.append(" LEFT JOIN ref_storage_location AS s ON s.sl_id=b.ib_storage");
  s.append(" LEFT JOIN inventory_images AS i ON i.im_id=b.ib_id ");
  // s.append(" ");
  return s;
}

BooksTable::BooksTable(QWidget *parent) : QTableView{parent} {
  setObjectName("BooksTable");
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

  // BEGIN Einträge für Auftrag
  QAction *ac_copy = m->addAction(myIcon("edit"), tr("Copy Article Id"));
  ac_copy->setObjectName("ac_context_copy_book");
  ac_copy->setEnabled(b);
  connect(ac_copy, SIGNAL(triggered()), this, SLOT(copyToClipboard()));

  QAction *ac_order = m->addAction(myIcon("autostart"), tr("add Article to opened Order"));
  ac_order->setObjectName("ac_context_book_to_order");
  ac_order->setEnabled(b);
  connect(ac_order, SIGNAL(triggered()), this, SLOT(createOrderSignal()));
  // END

  QAction *ac_refresh = m->addAction(myIcon("reload"), tr("Refresh"));
  ac_refresh->setObjectName("ac_context_refresh_books");
  connect(ac_refresh, SIGNAL(triggered()), this, SLOT(refreshView()));
  ac_refresh->setEnabled(b);

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

void BooksTable::queryHistory(const QString &str) {
  QString q("SELECT ");
  q.append(querySelect());
  q.append(queryTables());
  q.append(" WHERE ");
  if (str.contains("#today")) {
    q.append("DATE(b.ib_changed)=(DATE(now()))");
  } else if (str.contains("#yesterday")) {
    q.append("DATE(b.ib_changed)=(DATE(now() - INTERVAL '1 day'))");
  } else if (str.contains("#last7days")) {
    q.append("DATE(b.ib_changed)>=(DATE(now() - INTERVAL '7 days'))");
  } else if (str.contains("#thismonth")) {
    q.append("EXTRACT(MONTH FROM b.ib_changed)=(EXTRACT(MONTH FROM now()))");
    q.append(" AND b.ib_count>0");
  } else if (str.contains("#thisyear")) {
    q.append("EXTRACT(ISOYEAR FROM b.ib_changed)=(EXTRACT(YEAR FROM now()))");
    q.append(" AND b.ib_count>0");
  } else {
    return;
  }
  q.append(" ORDER BY b.ib_count DESC LIMIT ");
  q.append(QString::number(maxRowCount));
  q.append(";");

  if (sqlExecQuery(q)) {
    resizeRowsToContents();
    resizeColumnsToContents();
    p_historyQuery = q;
  }
}

void BooksTable::queryStatement(const SearchStatement &cl) {
  /**
     @brief exact_match
      false = irgendwo im feld
      true = genau ab Anfang
  */
  bool exact_match = false;

  QRegExp reg("^(\\s+)");
  QString field = cl.SearchField;
  if (field.contains("title_first")) {
    field = QString("title");
    exact_match = true;
  }

  QString str = cl.SearchString;
  if (reg.exactMatch(str)) {
    qWarning("Rejected:%s\n", qPrintable(str));
    return;
  }

  str.replace("*", "%");
  QString q("SELECT ");
  q.append(querySelect());
  q.append(queryTables());
  if (field.contains("id")) {
    // Artikel ID Suche
    q.append(" WHERE (b.ib_id=");
    q.append(str);
  } else if (field.contains("isbn")) {
    // ISBN Suche
    q.append(" WHERE (b.ib_isbn=");
    q.append(str);
  } else if (field.contains("author")) {
    // Autoren suche
    q.append(" WHERE (b.ib_author ILIKE '%");
    q.append(str.replace(" ", "%"));
    q.append("%'");
  } else if (field.contains("publisher")) {
    // Herausgeber
    q.append(" WHERE (b.ib_publisher ILIKE '%");
    q.append(str.replace(" ", "%"));
    q.append("%'");
  } else if (field.contains("storage")) {
    // mit Lager Kategorie
    q.append(" WHERE (b.ib_count!=0 AND s.sl_identifier ILIKE '");
    q.append(str.replace("%", "")); /**< @note Wegen Performance */
    q.append("%'");
  } else {
    // Titelsuche
    q.append(" WHERE (b.ib_title ILIKE '");
    if (!exact_match) {
      q.append("%");
    }
    q.append(str);
    q.append("%') OR (b.ib_title_extended ILIKE '");
    if (!exact_match) {
      q.append("%");
    }
    q.append(str);
    q.append("%'");
  }
  q.append(") ORDER BY b.ib_count DESC LIMIT ");
  q.append(QString::number(maxRowCount));
  q.append(";");

  if (sqlExecQuery(q)) {
    resizeRowsToContents();
    resizeColumnsToContents();
    p_historyQuery = q;
  }
}
