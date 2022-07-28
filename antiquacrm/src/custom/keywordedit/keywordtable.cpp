// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordtable.h"
#include "keywordtablemodel.h"

#include <QAbstractItemModel>
#include <QDebug>
#include <QModelIndex>

KeywordTable::KeywordTable(const QSqlDatabase &db, QWidget *parent)
    : QTableView{parent}, p_db{db} {
  setObjectName("keyword_table");
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setCornerButtonEnabled(false);
  setSortingEnabled(true);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);
  setAlternatingRowColors(true);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);
  m_headerView = horizontalHeader();
  m_headerView->setDefaultAlignment(Qt::AlignCenter);

  m_model = new KeywordTableModel(db, this);
  setModel(m_model);

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(keywordEditClicked(const QModelIndex &)));
}

void KeywordTable::keywordEditClicked(const QModelIndex &index) {
  if (index.isValid()) {
    QModelIndex idIndex = index.sibling(index.row(), 0);
    int id = m_model->data(idIndex, Qt::DisplayRole).toInt();
    if (id > 0)
      emit sendEditKeyword(id);
  }
}

void KeywordTable::refresh() {
  if (m_model->select()) {
    m_headerView->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_headerView->setSectionResizeMode(1, QHeaderView::Stretch);
    m_headerView->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    m_headerView->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  }
}

void KeywordTable::searchKeyword(const QString &str) {
  QModelIndex mi = indexAt(QPoint(1, 1));
  for (int r = 0; r < m_model->rowCount(); r++) {
    QString key  = m_model->data(mi.sibling(r, 1), Qt::DisplayRole).toString();
    if(key.contains(str,Qt::CaseInsensitive)) {
      selectRow(r);
      break;
    }
  }
}
