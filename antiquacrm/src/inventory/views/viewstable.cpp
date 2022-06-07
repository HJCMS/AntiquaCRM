// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "viewstable.h"

#include <QDebug>
#include <QHeaderView>
#include <QSqlQuery>

ViewsTableModel::ViewsTableModel(const QSqlDatabase &db, QWidget *parent)
    : QSqlTableModel{parent, db} {
  setObjectName("views_table_model");
}

QVariant ViewsTableModel::headerData(int section, Qt::Orientation orientation,
                                     int role) const {
  if (role != Qt::DisplayRole)
    return QVariant();

  if (orientation == Qt::Horizontal) {
    switch (section) {
    case 0: // articleid
      return QString(" %1 ").arg(tr("Article"));

    case 1: // title
      return QString(" %1 ").arg(tr("Title"));

    case 2: // author
      return QString(" %1 ").arg(tr("Author"));

    case 3: // lager
      return QString(" %1 ").arg(tr("Storage"));

    default:
      return QString(" %1 ").arg(section);
    }
  }
  return QString("%1").arg(section);
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
  }
}

void ViewsTable::sqlQuery(const QString &viewtable) {
  if (viewtable.isEmpty() || !viewtable.contains("view_"))
    return;

  m_model->setTable(viewtable);
  if (m_model->select()) {
    m_headerView->hideSection(0); /**< ID */
    m_headerView->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    m_headerView->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_headerView->setStretchLastSection(true);
  }
}

ViewsTable::~ViewsTable() {
  m_sql->deleteLater();
  m_model->deleteLater();
}
