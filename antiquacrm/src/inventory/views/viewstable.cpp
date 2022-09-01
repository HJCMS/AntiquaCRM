// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "viewstable.h"

#include <QDebug>
#include <QHeaderView>
#include <QModelIndex>
#include <QSqlQuery>

ViewsTableModel::ViewsTableModel(const QSqlDatabase &db, QWidget *parent)
    : QSqlTableModel{parent, db} {
  setObjectName("views_table_model");
}

const QString ViewsTableModel::translations(const QString &key) const {
  QMap<QString, QString> l;
  l.insert("id", "Id");
  l.insert("articleid", tr("Article Id"));
  l.insert("identifier", tr("Identity"));
  l.insert("title", tr("Title"));
  l.insert("categorie", tr("Categorie"));
  l.insert("year", tr("Year"));
  l.insert("price", tr("Price"));
  l.insert("fullprice", tr("Full Price"));
  l.insert("count", tr("Count"));
  l.insert("counts", tr("Count"));
  l.insert("author", tr("Author"));
  l.insert("keyword", tr("Keyword"));
  l.insert("keywords", tr("Keywords"));
  l.insert("publisher", tr("Pusblisher"));
  l.insert("storage", tr("Storage"));
  l.insert("storages", tr("Storage"));
  l.insert("changed", tr("Changed"));
  l.insert("exists", tr("Exists"));
  // l.insert("",tr(""));

  if (l.contains(key))
    return " " + l.value(key) + " ";

  return " " + key.toUpper() + " ";
}

const QString ViewsTableModel::getFieldName(const QModelIndex &index) const {
  int c = index.column();
  if (c == 0)
    return QString();

  return record().fieldName(c);
}

QVariant ViewsTableModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const {
  if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
    return QVariant();

  QString fn = record().fieldName(section).split("_").last();
  return translations(fn);
}

ViewsTable::ViewsTable(QWidget *parent) : QTableView{parent} {
  setObjectName("views_table");
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

  m_model = new ViewsTableModel(m_sql->db(), this);
  m_model->setObjectName("views_table_model");
  setModel(m_model);

  /* Kopfzeilen anpassen */
  m_headerView = horizontalHeader();
  m_headerView->setDefaultAlignment(Qt::AlignCenter);
  m_headerView->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_headerView->setStretchLastSection(true);
  setHorizontalHeader(m_headerView);

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(articleClicked(const QModelIndex &)));
}

void ViewsTable::articleClicked(const QModelIndex &index) {
  QModelIndex mid = index.sibling(index.row(), 0);
  if (mid.isValid()) {
    int id = m_model->data(mid, Qt::DisplayRole).toInt();
    if (id > 0 && m_model->tableName().contains("_book_"))
      emit s_articleSelected(id, "bookeditor");
    else if (id > 0 && m_model->tableName().contains("_print_"))
      emit s_articleSelected(id, "printseditor");
    else if (id > 0 && m_model->tableName().contains("_customer_"))
      emit s_articleSelected(id, "customereditor");
    else
      qInfo("No action defined for this '%s' View.",
            qPrintable(m_model->tableName()));
  }
}

void ViewsTable::sqlQuery(const QString &viewtable) {
  if (viewtable.isEmpty() || !viewtable.contains("view_"))
    return;

  m_model->setTable(viewtable);
  if (m_model->select()) {
    emit s_queryFinished();
  }
}

ViewsTable::~ViewsTable() {
  m_sql->deleteLater();
  m_model->deleteLater();
}
