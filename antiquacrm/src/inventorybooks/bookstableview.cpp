// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookstableview.h"
#include "bookstablemodel.h"
#include "pmetatypes.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtCore/QRegExp>
#include <QtSql/QSqlQuery>
#include <QtWidgets/QHeaderView>

/**
    @brief queryFields
    Die Feldabfragen sind immer gleich!
    Wenn sich etwas ändern sollte, dann muss in Klasse
    \ref BooksTableModel die Feld Definition geändert werden!
    b.ib_id,b.ib_count,b.ib_title,b.ib_author,b.ib_publisher,b.ib_year,b.ib_price,s.sl_storage,b.ib_isbn
*/
static const QString queryFields() {
  QString s("b.ib_id,b.ib_count,b.ib_title,b.ib_author,");
  s.append("b.ib_publisher,b.ib_year,b.ib_price,s.sl_storage,b.ib_isbn");
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

  m_queryModel = new BooksTableModel(this);
  setModel(m_queryModel);

  /* Kopfzeilen anpassen */
  QHeaderView *tHeader = horizontalHeader();
  tHeader->setDefaultAlignment(Qt::AlignCenter);
  tHeader->setStretchLastSection(true);

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(clickedGetArticleID(const QModelIndex &)));
}

/**
 * @brief SQLTitleSearch::clickedGetArticleID
 * @short Suche nach Artikel ID und Titeltext
 * @param index
 */
void BooksTableView::clickedGetArticleID(const QModelIndex &index) {
  // ib_id
  if (m_queryModel->data(index.sibling(index.row(), 0), Qt::EditRole).toInt() >= 1) {
    // ib_title
    QString id = m_queryModel->data(index.sibling(index.row(), 0), Qt::EditRole).toString();
    QString str = m_queryModel->data(index.sibling(index.row(), 2), Qt::EditRole).toString();
    QHash<QString, QString> data;
    data.insert(id, str);
    emit articleIdSelected(data);
  }
}

void BooksTableView::queryHistory(const QString &str) {
  QString q("SELECT ");
  q.append(queryFields());
  q.append(" FROM inventory_books AS b ");
  q.append(" LEFT JOIN ref_storage_location AS s ON s.sl_id=b.ib_storage");
  q.append(" WHERE ");
  if (str.contains("#today")) {
    q.append("DATE(b.ib_changed)=(DATE(now()))");
  } else if (str.contains("#yesterday")) {
    q.append("DATE(b.ib_changed)=(DATE(now() - INTERVAL '1 day'))");
  } else if (str.contains("#last7days")) {
    q.append("DATE(b.ib_changed)=(DATE(now() - INTERVAL '7 days'))");
  } else if (str.contains("#thismonth")) {
    q.append("EXTRACT(MONTH FROM b.ib_changed)=(EXTRACT(MONTH FROM now()))");
  } else if (str.contains("#thisyear")) {
    q.append("EXTRACT(ISOYEAR FROM b.ib_changed)=(EXTRACT(YEAR FROM now()))");
  } else {
    return;
  }
  q.append(" AND b.ib_count>0");
  q.append(" ORDER BY b.ib_count DESC LIMIT 3500;");

  p_db = QSqlDatabase::database(sqlConnectionName);
  if (p_db.open()) {
    qDebug() << "BooksTableView::queryHistory" << q;
    m_queryModel->setQuery(q, p_db);
    resizeRowsToContents();
    resizeColumnsToContents();
  }
}

void BooksTableView::queryStatement(const SearchStatement &cl) {
  QRegExp reg("^(\\s+)");
  // NOTE ist in combofieldbox.cpp Definiert!
  QString field = cl.SearchField;
  QString str = cl.SearchString;
  if (reg.exactMatch(str)) {
    qDebug() << "Rejected:" << str;
    return;
  }

  QString q("SELECT ");
  q.append(queryFields());
  q.append(" FROM inventory_books AS b ");
  q.append(" LEFT JOIN ref_storage_location AS s ON s.sl_id=b.ib_storage");
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
    q.append(str);
    q.append("%'");
  } else {
    // String Search
    q.append(" WHERE (b.ib_title ILIKE '%");
    q.append(str);
    q.append("%') OR (b.ib_title_extended ILIKE '");
    q.append(str);
    q.append("%'");
  }
  q.append(") ORDER BY b.ib_count DESC LIMIT 1000;");
  p_db = QSqlDatabase::database(sqlConnectionName);
  if (p_db.open()) {
    // qDebug() << "{SQL Title Search} " << q;
    m_queryModel->setQuery(q, p_db);
    resizeRowsToContents();
    resizeColumnsToContents();
  }
}
