// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printstableview.h"
#include "printstablemodel.h"
#include "searchbar.h"
#include "applsettings.h"
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
  QString s("b.ip_id,b.ip_count,b.ip_title,b.ip_author,");
  s.append("b.ip_technique,b.ip_year,b.ip_price,s.sl_storage,b.ip_landscape,b.ip_changed");
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
  QString s(" FROM inventory_prints AS b");
  s.append(" LEFT JOIN ref_storage_location AS s ON s.sl_id=b.ip_storage");
  s.append(" LEFT JOIN inventory_images AS i ON i.im_id=b.ip_id ");
  // s.append(" ");
  return s;
}

PrintsTableView::PrintsTableView(QWidget *parent) : QTableView{parent} {
  setObjectName("PrintsTableView");
  setEditTriggers(QAbstractItemView::DoubleClicked);
  setCornerButtonEnabled(false);
  setSortingEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);
  setAlternatingRowColors(true);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);

  m_queryModel = new PrintsTableModel(this);
  setModel(m_queryModel);

  /* Kopfzeilen anpassen */
  QHeaderView *tHeader = horizontalHeader();
  tHeader->setDefaultAlignment(Qt::AlignCenter);
  tHeader->setStretchLastSection(true);

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(clickedGetArticleID(const QModelIndex &)));
}

void PrintsTableView::clickedGetArticleID(const QModelIndex &index) {
  QModelIndex id(index);
  if (m_queryModel->data(id.sibling(id.row(), 0), Qt::EditRole).toInt() >= 1) {
    int i = m_queryModel->data(id.sibling(id.row(), 0), Qt::EditRole).toInt();

    if (i >= 1)
      emit s_articleSelected(i);
  }
}

void PrintsTableView::openBookByContext() { clickedGetArticleID(p_modelIndex); }

void PrintsTableView::createOrderByContext() {
  // TODO create order --> p_modelIndex
  qInfo("currently not implemented");
}

void PrintsTableView::contextMenuEvent(QContextMenuEvent *ev) {
  p_modelIndex = indexAt(ev->pos());
  QMenu *m = new QMenu("Actions", this);
  // Eintrag öffnen  Bestellung anlegen
  QAction *ac_open = m->addAction(myIcon("spreadsheet"), tr("Open entry"));
  ac_open->setObjectName("ac_context_open_book");
  connect(ac_open, SIGNAL(triggered()), this, SLOT(openBookByContext()));
  QAction *ac_order = m->addAction(myIcon("autostart"), tr("Create order"));
  ac_order->setObjectName("ac_context_create_order");
  connect(ac_order, SIGNAL(triggered()), this, SLOT(createOrderByContext()));
  m->exec(ev->globalPos());
  delete m;
}

void PrintsTableView::queryHistory(const QString &str) {
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

  qDebug() << Q_FUNC_INFO << q;
  return;
  p_db = QSqlDatabase::database(ApplSettings::sqlConnectioName());
  if (p_db.open()) {
    // qDebug() << "PrintsTableView::queryHistory" << q << Qt::endl;
    m_queryModel->setQuery(q, p_db);
    if (m_queryModel->lastError().isValid()) {
      qDebug() << Q_FUNC_INFO << Qt::endl
               << "{SQL Query} " << q << Qt::endl
               << "{SQL Error} " << m_queryModel->lastError() << Qt::endl;
      return;
    }
    resizeRowsToContents();
    resizeColumnsToContents();
  }
}

void PrintsTableView::queryStatement(const SearchStatement &cl) {
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

  str.replace("*","%");
  QString q("SELECT ");
  q.append(querySelect());
  q.append(queryTables());
  if (field.contains("id")) {
    // Numeric Search
    q.append(" WHERE (b.ip_id=");
    q.append(str);
  } else if (field.contains("author")) {
    // String Search
    q.append(" WHERE (b.ip_author ILIKE '%");
    q.append(str.replace(" ","%"));
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
      q.append(str.replace(" ","%"));
    }
    q.append("%'");
  }
  q.append(") ORDER BY b.ip_count DESC LIMIT ");
  q.append(QString::number(maxRowCount));
  q.append(";");

  qDebug() << Q_FUNC_INFO << q;
  return;
  p_db = QSqlDatabase::database(ApplSettings::sqlConnectioName());
  if (p_db.open()) {
    m_queryModel->setQuery(q, p_db);
    if (m_queryModel->lastError().isValid()) {
      qDebug() << Q_FUNC_INFO << Qt::endl
              << "{SQL Query} " << q << Qt::endl
              << "{SQL Error} " << m_queryModel->lastError() << Qt::endl;
      return;
    }
    resizeRowsToContents();
    resizeColumnsToContents();
    emit s_rowsChanged(m_queryModel->rowCount());
  }
}
