// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "storagetable.h"
#include "storagetablemodel.h"

#include <QMessageBox>

StorageTable::StorageTable(QWidget *parent) : AntiquaCRM::TableView{parent} {
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

  // handle errors
  connect(m_model, SIGNAL(sendSqlError(const QSqlError &)),
          SLOT(sqlErrorPopUp(const QSqlError &)));

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)), this,
          SLOT(getSelectedItem(const QModelIndex &)));
}

qint64 StorageTable::getTableID(const QModelIndex &index, int column) {
  QModelIndex id(index);
  if (m_model->data(id.sibling(id.row(), column), Qt::EditRole).toInt() >= 1) {
    return m_model->data(id.sibling(id.row(), column), Qt::EditRole).toInt();
  }
  return -1;
}

void StorageTable::contextMenuEvent(QContextMenuEvent *event) {
  QModelIndex index = indexAt(event->pos());
  qint64 rows = m_model->rowCount();
  AntiquaCRM::TableContextMenu *m_m =
      new AntiquaCRM::TableContextMenu(index, rows, this);
  m_m->addOpenAction(tr("Open entry"));
  m_m->addCreateAction(tr("Create entry"));
  m_m->addDeleteAction(tr("Remove entry"));
  connect(m_m,
          SIGNAL(sendAction(AntiquaCRM::TableContextMenu::Actions,
                            const QModelIndex &)),
          SLOT(contextMenuAction(AntiquaCRM::TableContextMenu::Actions,
                                 const QModelIndex &)));

  connect(m_m, SIGNAL(sendCreate()), SIGNAL(sendCreateEntry()));

  m_m->exec(event->globalPos());
  m_m->deleteLater();
}

void StorageTable::contextMenuAction(AntiquaCRM::TableContextMenu::Actions ac,
                                     const QModelIndex &index) {
  qint64 _sl_id = getTableID(index);
  if (_sl_id < 0)
    return;

  switch (ac) {
  case (AntiquaCRM::TableContextMenu::Actions::Open): {
    getSelectedItem(index);
  } break;

  case (AntiquaCRM::TableContextMenu::Actions::Delete): {
    QString _sql("DELETE FROM " + m_model->tableName() + " WHERE ");
    _sql.append("sl_id=" + QString::number(_sl_id) + ";");
    setQuery(_sql);
  } break;

  default:
    return;
  };
}

void StorageTable::createSocketOperation(const QModelIndex &) {}

bool StorageTable::sqlModelQuery(const QString &query) {
  return m_model->querySelect(query);
}

bool StorageTable::askBeforeDelete() {
  QString _title = tr("Storage location deletion");
  QString _body = tr("<b>You really want to delete this Entry?</b>"
                     "<p>This operation can not be reverted.</p>"
                     "<b>Please notice:</b> <i>A delete will fail when this "
                     "entry is used in one of the inventory tables.</i>"
                     "<p>If not sure, click no for cancel this action.</p>");
  QMessageBox::StandardButton _ret = QMessageBox::question(this, _title, _body);
  return (_ret == QMessageBox::Yes);
}

void StorageTable::setSortByColumn(int column, Qt::SortOrder order) {
  Q_UNUSED(column);
  Q_UNUSED(order);
}

void StorageTable::getSelectedItem(const QModelIndex &index) {
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

void StorageTable::setReloadView() { m_model->select(); }

int StorageTable::rowCount() { return m_model->rowCount(); }

bool StorageTable::setQuery(const QString &query) {
  QString _sql = query.trimmed();
  if (_sql.isEmpty())
    return false;

  if (_sql.startsWith("DELETE") && !askBeforeDelete())
    return false;

  if (!m_model->update(_sql)) {
    emit queryMessages(tr("an error occurred"));

    return false;
  }
  emit queryMessages(tr("successful saved"));
  return m_model->select();
}

const QString StorageTable::defaultWhereClause() {
  return m_model->statement();
}

bool StorageTable::initTable() {
  if (m_model->select()) {
    m_header->setStretchLastSection(true);
    emit tableRecordChanged(m_model->record());
    return true;
  }
  return false;
}
