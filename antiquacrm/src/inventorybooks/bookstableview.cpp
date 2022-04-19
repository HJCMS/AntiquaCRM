// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookstableview.h"
#include "applsettings.h"
#include "bookstablemodel.h"
#include "searchbar.h"
#include "sqlcore.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QPoint>
#include <QtCore/QRegExp>
#include <QtCore/QSignalMapper>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtWidgets/QAction>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMenu>

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

BooksTableView::BooksTableView(QWidget *parent) : QTableView{parent} {
  setObjectName("BooksTableView");
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
  setModel(m_queryModel);

  /* Kopfzeilen anpassen */
  QHeaderView *tHeader = horizontalHeader();
  tHeader->setDefaultAlignment(Qt::AlignCenter);
  tHeader->setStretchLastSection(true);

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(queryArticleID(const QModelIndex &)));
}

void BooksTableView::queryArticleID(const QModelIndex &index) {
  QModelIndex id(index);
  if (m_queryModel->data(id.sibling(id.row(), 0), Qt::EditRole).toInt() >= 1) {
    int i = m_queryModel->data(id.sibling(id.row(), 0), Qt::EditRole).toInt();

    if (i >= 1)
      emit s_articleSelected(i);
  }
}

void BooksTableView::openBookByContext() { queryArticleID(p_modelIndex); }

void BooksTableView::newBookByContext() {
  qDebug() << Q_FUNC_INFO << "TODO"
           << "Check Rowcount before add";
  emit s_newEntryPlease();
}

void BooksTableView::createOrderByContext() {
  qDebug() << Q_FUNC_INFO << "Noch nicht Implementiert";
}

bool BooksTableView::sqlExecQuery(const QString &statement) {
  if (!statement.contains("SELECT"))
    return false;

  QSqlDatabase db(m_sql->db());
  if (db.open()) {
    m_queryModel->setQuery(statement, db);
    if (m_queryModel->lastError().isValid()) {
      qDebug() << Q_FUNC_INFO << "{SQL Query} " << statement << "{SQL Error} "
               << m_queryModel->lastError() << Qt::endl
               << m_sql->fetchErrors() << Qt::endl;
      return false;
    }
    return true;
    emit s_rowsChanged(m_queryModel->rowCount());
  } else {
    qWarning("No SQL Connection in Booktable");
  }
  return false;
}

void BooksTableView::contextMenuEvent(QContextMenuEvent *ev) {
  p_modelIndex = indexAt(ev->pos());
  QMenu *m = new QMenu("Actions", this);
  // Eintrag öffnen  Bestellung anlegen
  QAction *ac_open = m->addAction(myIcon("spreadsheet"), tr("Open entry"));
  ac_open->setObjectName("ac_context_open_book");
  connect(ac_open, SIGNAL(triggered()), this, SLOT(openBookByContext()));

  QAction *ac_create = m->addAction(myIcon("db_add"), tr("Create entry"));
  ac_create->setObjectName("ac_context_create_book");
  connect(ac_create, SIGNAL(triggered()), this, SLOT(newBookByContext()));

  QAction *ac_order = m->addAction(myIcon("autostart"), tr("Create order"));
  ac_order->setObjectName("ac_context_create_order");
  connect(ac_order, SIGNAL(triggered()), this, SLOT(createOrderByContext()));
  m->exec(ev->globalPos());
  delete m;
}

void BooksTableView::refreshView() {
  if (sqlExecQuery(p_historyQuery)) {
    resizeRowsToContents();
    resizeColumnsToContents();
  }
}

void BooksTableView::queryHistory(const QString &str) {
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

void BooksTableView::queryStatement(const SearchStatement &cl) {
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
    qDebug() << "Rejected:" << str;
    return;
  }

  str.replace("*", "%");
  QString q("SELECT ");
  q.append(querySelect());
  q.append(queryTables());
  if (field.contains("id")) {
    // Numeric Search
    q.append(" WHERE (b.ib_id=");
    q.append(str);
  } else if (field.contains("isbn")) {
    // Numeric Search
    q.append(" WHERE (b.ib_isbn=");
    q.append(str);
  } else if (field.contains("author")) {
    // String Search
    q.append(" WHERE (b.ib_author ILIKE '%");
    q.append(str.replace(" ", "%"));
    q.append("%'");
  } else {
    // String Search
    if (exact_match) {
      q.append(" WHERE (b.ib_title ILIKE '");
    } else {
      q.append(" WHERE (b.ib_title ILIKE '%");
    }
    q.append(str);
    q.append("%')");
    if (exact_match) {
      q.append(" OR (b.ib_title_extended ILIKE '");
      q.append(str);
    } else {
      q.append(" OR (b.ib_title_extended ILIKE '%");
      q.append(str.replace(" ", "%"));
    }
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
