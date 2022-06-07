// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printstable.h"
#include "messagebox.h"
#include "myicontheme.h"
#include "printstablemodel.h"
#include "searchbar.h"

#include <QAction>
#include <QDebug>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QMenu>
#include <QPoint>
#include <QRegExp>
#include <QSignalMapper>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>

static const QString querySelect() {
  QString s("b.ip_id,b.ip_count,b.ip_title,b.ip_author,");
  s.append("t.rpt_type,b.ip_year,b.ip_price,s.sl_storage,b.ip_landscape,b."
           "ip_changed");
  s.append(",(CASE WHEN i.im_id IS NOT NULL THEN true ELSE false END) AS "
           "image_exists ");
  // s.append("");
  return s;
}

static const QString queryTables() {
  QString s(" FROM inventory_prints AS b");
  s.append(" LEFT JOIN ref_storage_location AS s ON s.sl_id=b.ip_storage");
  s.append(" LEFT JOIN ref_print_technique AS t ON t.rpt_id=b.ip_technique");
  s.append(" LEFT JOIN inventory_images AS i ON i.im_id=b.ip_id ");
  // s.append(" ");
  return s;
}

PrintsTable::PrintsTable(QWidget *parent) : QTableView{parent} {
  setObjectName("PrintsTable");
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

  m_queryModel = new PrintsTableModel(this);
  setModel(m_queryModel);

  /* Kopfzeilen anpassen */
  QHeaderView *tHeader = horizontalHeader();
  tHeader->setDefaultAlignment(Qt::AlignCenter);
  tHeader->setStretchLastSection(true);

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(articleClicked(const QModelIndex &)));
}

int PrintsTable::queryArticleID(const QModelIndex &index) {
  QModelIndex id(index);
  if (m_queryModel->data(id.sibling(id.row(), 0), Qt::EditRole).toInt() >= 1) {
    int i = m_queryModel->data(id.sibling(id.row(), 0), Qt::EditRole).toInt();
    return i;
  }
  return -1;
}

void PrintsTable::articleClicked(const QModelIndex &index) {
  int i = queryArticleID(index);
  if (i >= 1)
    emit s_articleSelected(i);
}

void PrintsTable::openByContext() {
  int i = queryArticleID(p_modelIndex);
  if (i >= 1)
    emit s_articleSelected(i);
}

void PrintsTable::createByContext() {
  qDebug() << Q_FUNC_INFO << "TODO"
           << "Check Rowcount before add";
  emit s_newEntryPlease();
}

bool PrintsTable::sqlExecQuery(const QString &statement) {
  if (statement.isEmpty() || statement.length() < 10)
    return false;

  QSqlDatabase db(m_sql->db());
  if (db.open()) {
    // qDebug() << Q_FUNC_INFO << statement;
    m_queryModel->setQuery(statement, db);
    if (m_queryModel->lastError().isValid()) {
      MessageBox message(this);
      message.failed(statement, m_queryModel->lastError().text());
      return false;
    }
    emit s_rowsChanged(m_queryModel->rowCount());
    return true;
  } else {
    qWarning("No SQL Connection in Booktable");
  }
  return false;
}

void PrintsTable::contextMenuEvent(QContextMenuEvent *ev) {
  p_modelIndex = indexAt(ev->pos());
  // Aktiviere/Deaktivieren der Einträge
  bool b = p_modelIndex.isValid();

  QMenu *m = new QMenu("Actions", this);
  // Eintrag öffnen  Bestellung anlegen
  QAction *ac_open = m->addAction(myIcon("spreadsheet"), tr("Open entry"));
  ac_open->setObjectName("ac_context_open_print");
  ac_open->setEnabled(b);
  connect(ac_open, SIGNAL(triggered()), this, SLOT(openByContext()));

  QAction *ac_create = m->addAction(myIcon("db_add"), tr("Create entry"));
  ac_create->setObjectName("ac_context_create_print");
  ac_create->setEnabled(b);
  connect(ac_create, SIGNAL(triggered()), this, SLOT(createByContext()));

  // BEGIN Einträge für Auftrag
  QAction *ac_copy = m->addAction(myIcon("edit"), tr("Copy Article Id"));
  ac_copy->setObjectName("ac_context_copy_print");
  ac_copy->setEnabled(b);
  connect(ac_copy, SIGNAL(triggered()), this, SLOT(copyToClipboard()));

  QAction *ac_order = m->addAction(myIcon("autostart"), tr("add Article to opened Order"));
  ac_order->setObjectName("ac_context_book_to_order");
  ac_order->setEnabled(b);
  connect(ac_order, SIGNAL(triggered()), this, SLOT(createOrderSignal()));
  // END

  QAction *ac_refresh = m->addAction(myIcon("reload"), tr("Refresh"));
  ac_refresh->setObjectName("ac_context_refresh_view");
  connect(ac_refresh, SIGNAL(triggered()), this, SLOT(refreshView()));
  ac_refresh->setEnabled(b);

  m->exec(ev->globalPos());
  delete m;
}

void PrintsTable::copyToClipboard() {
  QVariant id = queryArticleID(p_modelIndex);
  emit s_toClibboard(id);
}

void PrintsTable::createOrderSignal() {
  QVariant id = queryArticleID(p_modelIndex);
  emit s_articleToOrders(id.toInt());
}

void PrintsTable::refreshView() {
  if (sqlExecQuery(p_historyQuery)) {
    resizeRowsToContents();
    resizeColumnsToContents();
  }
}

void PrintsTable::queryHistory(const QString &str) {
  if (!isVisible())
    return;

  QString q("SELECT ");
  q.append(querySelect());
  q.append(queryTables());
  q.append(" WHERE ");
  if (str.contains("#today")) {
    q.append("DATE(b.ip_changed)=(DATE(now()))");
  } else if (str.contains("#yesterday")) {
    q.append("DATE(b.ip_changed)=(DATE(now() - INTERVAL '1 day'))");
  } else if (str.contains("#last7days")) {
    q.append("DATE(b.ip_changed)>=(DATE(now() - INTERVAL '7 days'))");
  } else if (str.contains("#thismonth")) {
    q.append("EXTRACT(MONTH FROM b.ip_changed)=(EXTRACT(MONTH FROM now()))");
    q.append(" AND b.ip_count>0");
  } else if (str.contains("#thisyear")) {
    q.append("EXTRACT(ISOYEAR FROM b.ip_changed)=(EXTRACT(YEAR FROM now()))");
    q.append(" AND b.ip_count>0");
  } else {
    return;
  }
  q.append(" ORDER BY b.ip_count DESC LIMIT ");
  q.append(QString::number(maxRowCount));
  q.append(";");

  if (sqlExecQuery(q)) {
    resizeRowsToContents();
    resizeColumnsToContents();
    p_historyQuery = q;
  }
}

void PrintsTable::queryStatement(const SearchStatement &cl) {
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
  if (field == "id") {
    // Numeric Search
    q.append(" WHERE (b.ip_id=");
    q.append(str);
  } else if (field == "author") {
    // String Search
    q.append(" WHERE (b.ip_author ILIKE '%");
    q.append(str.replace(" ", "%"));
    q.append("%'");
  } else {
    // String Search
    if (exact_match) {
      q.append(" WHERE (b.ip_title ILIKE '");
    } else {
      q.append(" WHERE (b.ip_title ILIKE '%");
    }
    q.append(str);
    q.append("%')");
    if (exact_match) {
      q.append(" OR (b.ip_title_extended ILIKE '");
      q.append(str);
    } else {
      q.append(" OR (b.ip_title_extended ILIKE '%");
      q.append(str.replace(" ", "%"));
    }
    q.append("%'");
  }
  q.append(") ORDER BY b.ip_count DESC LIMIT ");
  q.append(QString::number(maxRowCount));
  q.append(";");

  if (sqlExecQuery(q)) {
    resizeRowsToContents();
    resizeColumnsToContents();
    p_historyQuery = q;
  }
}
