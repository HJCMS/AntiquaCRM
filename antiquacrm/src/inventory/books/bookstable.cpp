// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookstable.h"
#include "bookstablemodel.h"
#include "myicontheme.h"
#include "searchbar.h"

#include <QAction>
#include <QDebug>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QKeySequence>
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
  QString s("ib_id,ib_count,ib_title,ib_author,");
  s.append("ib_publisher,ib_year,ib_price,sl_storage,ib_isbn,ib_changed");
  s.append(",(CASE WHEN im_id IS NOT NULL THEN true ELSE false END)");
  s.append(" AS image_exists ");
  return s;
}

/**
   @brief queryTables
   Tabellen Definition
*/
static const QString queryTables() {
  QString s(" FROM inventory_books");
  s.append(" LEFT JOIN ref_storage_location ON sl_id=ib_storage");
  s.append(" LEFT JOIN inventory_images ON im_id=ib_id ");
  // s.append(" ");
  return s;
}

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

  // qDebug() << Q_FUNC_INFO << statement;

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

const QString BooksTable::prepareSearch(const QString &fieldname,
                                        const QString &search) const {
  QStringList words = search.split(" ");
  QString sql(fieldname);
  sql.append(" ILIKE '");
  if (words.count() == 2) {
    sql.append(words.first());
    sql.append("%");
    sql.append(words.last());
    sql.append("%' OR ");
    sql.append(fieldname);
    sql.append(" ILIKE '");
    sql.append(words.last());
    sql.append("%");
    sql.append(words.first());
  } else {
    sql.append(search);
  }
  sql.append("%'");
  return sql;
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

  QMenu *m = new QMenu("Actions", this);
  // Eintrag öffnen  Bestellung anlegen
  QAction *ac_open = m->addAction(myIcon("spreadsheet"), tr("Open entry"));
  ac_open->setObjectName("ac_context_open_book");
  ac_open->setEnabled(b);
  connect(ac_open, SIGNAL(triggered()), this, SLOT(openByContext()));

  QAction *ac_create = m->addAction(myIcon("db_add"), tr("Create entry"));
  ac_create->setObjectName("ac_context_create_book");
  ac_create->setShortcut(QKeySequence(Qt::ControlModifier + Qt::Key_N));
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
    q.append("DATE(ib_changed)=(DATE(now()))");
  } else if (str.contains("#yesterday")) {
    q.append("DATE(ib_changed)=(DATE(now() - INTERVAL '1 day'))");
  } else if (str.contains("#last7days")) {
    q.append("DATE(ib_changed)>=(DATE(now() - INTERVAL '7 days'))");
  } else if (str.contains("#thismonth")) {
    q.append("EXTRACT(MONTH FROM ib_changed)=(EXTRACT(MONTH FROM now()))");
    q.append(" AND ib_count>0");
  } else if (str.contains("#thisyear")) {
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
  QString q("SELECT DISTINCT ");
  q.append(querySelect());
  q.append(queryTables());
  if (field.contains("id")) {
    // Artikel ID Suche
    q.append(" WHERE (ib_id=");
    q.append(str);
  } else if (field.contains("isbn")) {
    // ISBN Suche
    q.append(" WHERE (ib_isbn=");
    q.append(str);
  } else if (field.contains("author")) {
    // Autoren suche
    q.append(" WHERE (");
    q.append(prepareSearch("ib_author", str));
  } else if (field.contains("publisher")) {
    // Herausgeber
    q.append(" WHERE (ib_publisher ILIKE '");
    q.append(str.replace(" ", "%"));
    q.append("%'");
  } else if (field.contains("storage")) {
    // mit Lager Kategorie
    q.append(" WHERE (ib_count>0 AND sl_identifier ILIKE '");
    q.append(str.replace("%", "")); /**< @note Wegen Performance */
    q.append("%'");
  } else {
    // Titelsuche
    q.append(" WHERE (ib_title ILIKE '");
    if (!exact_match) {
      q.append("%");
    }
    q.append(str);
    q.append("%') OR (ib_title_extended ILIKE '");
    if (!exact_match) {
      q.append("%");
    }
    q.append(str);
    q.append("%'");
  }
  q.append(") ORDER BY ib_count DESC LIMIT ");
  q.append(QString::number(maxRowCount));
  q.append(";");

  if (sqlExecQuery(q)) {
    resizeRowsToContents();
    resizeColumnsToContents();
    p_historyQuery = q;
  }
}
