// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "storagetable.h"
#include "storagetablemodel.h"

#include <QMessageBox>

StorageTable::StorageTable(QWidget *parent) : QTableView{parent} {
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setCornerButtonEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setAlternatingRowColors(true);
  setSortingEnabled(false);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);

  m_header = new AntiquaCRM::TableHeader(this);
  m_header->setSectionResizeMode(QHeaderView::ResizeToContents);
  setHorizontalHeader(m_header);

  m_model = new StorageTableModel("ref_storage_location", this);
  setModel(m_model);

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(itemClicked(const QModelIndex &)));
}

void StorageTable::itemClicked(const QModelIndex &index) {
  QModelIndex p = index.sibling(index.row(), 0);
  int sl_id = m_model->data(p).toInt();
  if (sl_id > 0) {
    StorageItemData i;
    i.sl_id = sl_id;
    i.sl_storage = m_model->data(p.sibling(p.row(), 1)).toString();
    i.sl_identifier = m_model->data(p.sibling(p.row(), 2)).toString();
    i.sl_location = m_model->data(p.sibling(p.row(), 3)).toString();
    i.sl_zvab_id = m_model->data(p.sibling(p.row(), 4)).toInt();
    i.sl_zvab_name = m_model->data(p.sibling(p.row(), 5)).toString();
    i.sl_booklooker_id = m_model->data(p.sibling(p.row(), 6)).toInt();
    i.sl_booklooker_name = m_model->data(p.sibling(p.row(), 7)).toString();
    emit itemChanged(i);
  }
}

void StorageTable::findColumn(const QString &str) {
  const QStringList _cols({"sl_storage", "sl_identifier"});
  const QString _search = str.trimmed();
  int _len = _search.length();
  if (_len > 1) {
    QStringList _query;
    foreach (QString _c, _cols) {
      QString _sql(_c + " ILIKE '");
      if (_len > 2)
        _sql.append("%");

      _sql.append(_search);
      _sql.append("%'");
      _query.append(_sql);
    }

    m_model->querySelect(m_model->statement(_query.join(" OR ")));
    return;
  }
  m_model->select();
}

bool StorageTable::startQuery(const QString &query) {
  QString _sql = query.trimmed();
  if (_sql.isEmpty())
    return false;

  if (!m_model->update(_sql)) {
    emit queryMessages(tr("an error occurred"));
    return false;
  }
  emit queryMessages(tr("successful saved"));
  return m_model->select();
}

bool StorageTable::initTable() {
  if (m_model->select()) {
    m_header->setStretchLastSection(true);
    emit tableRecordChanged(m_model->record());
    return true;
  }
  return false;
}
