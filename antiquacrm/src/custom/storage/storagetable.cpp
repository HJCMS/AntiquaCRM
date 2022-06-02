// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "storagetable.h"
#include "sqlcore.h"
#include "storagemodel.h"

#include <QDebug>

StorageHeader::StorageHeader(QWidget *parent)
    : QHeaderView{Qt::Horizontal, parent} {
  setSectionResizeMode(QHeaderView::Stretch);
  setMinimumSectionSize(30);
}

StorageTable::StorageTable(QWidget *parent) : QTableView{parent} {
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setCornerButtonEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setAlternatingRowColors(true);
  setSortingEnabled(true);

  m_sql = new HJCMS::SqlCore(this);

  m_header = new StorageHeader(this);
  setHorizontalHeader(m_header);

  m_model = new StorageModel(m_sql->db(), this);
  setModel(m_model);
  m_model->select();

  setColumnHidden(m_model->record().indexOf("sl_id"), true);
  m_header->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  m_header->setSectionResizeMode(2, QHeaderView::ResizeToContents);

  connect(this, SIGNAL(clicked(const QModelIndex &)),
          this, SLOT(itemClicked(const QModelIndex &)));
}

void StorageTable::itemClicked(const QModelIndex &index)
{
  QModelIndex p = index.sibling(index.row(), 0);
  int sl_id = m_model->data(p).toInt();
  if(sl_id>0)
  {
    Item i;
    i.sl_id = sl_id;
    i.sl_storage = m_model->data(p.sibling(p.row(), 1)).toString();
    i.sl_identifier = m_model->data(p.sibling(p.row(), 2)).toString();
    i.sl_location = m_model->data(p.sibling(p.row(), 3)).toString();
    emit itemChanged(i);
  }
}

void StorageTable::findColumn(const QString &str) {
  if (str.length() > 3) {
    QString sql("sl_id>0 AND sl_identifier ILIKE '%");
    sql.append(str.trimmed());
    sql.append("%'");
    m_model->setFilter(sql);
  } else {
    m_model->setFilter("sl_id>0");
  }
}
