// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordstable.h"

#include <QAction>
#include <QMenu>

KeywordsTable::KeywordsTable(QWidget *parent) : QTableView{parent} {
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setCornerButtonEnabled(false);
  setSortingEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);
  setAlternatingRowColors(true);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);
  setContextMenuPolicy(Qt::CustomContextMenu);

  m_horizontalHeader = new AntiquaCRM::TableHeader(this);
  setHorizontalHeader(m_horizontalHeader);

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)),
          SLOT(itemSelected(const QModelIndex &)));
  connect(this, SIGNAL(customContextMenuRequested(const QPoint &)),
          SLOT(contextMenuRequested(const QPoint &)));
}

const QModelIndex KeywordsTable::searchKeyword(const QString &search) {
  QModelIndex _found = QModelIndex();
  const QModelIndex _parent = indexAt(QPoint(1, 1));
  for (int r = 0; r < m_tableModel->rowCount(); r++) {
    const QModelIndex _index = _parent.sibling(r, 1);
    QVariant _v = m_tableModel->data(_index, Qt::EditRole);
    if (_v.isNull())
      continue;

    const QString _cell = _v.toString().trimmed();
    if (_cell.startsWith(search, Qt::CaseInsensitive)) {
      _found = _index;
      break;
    }
  }
  return _found;
}

void KeywordsTable::contextMenuRequested(const QPoint &p) {
  const QModelIndex _idx = indexAt(p);
  if (_idx.isValid()) {
    qint64 _id =
        m_tableModel->data(_idx.sibling(_idx.row(), 0), Qt::EditRole).toInt();
    if (_id < 1)
      return;

    QPoint _p = mapToGlobal(p);
    QMenu *m_m = new QMenu(tr("Action"), this);
    QAction *m_ac = m_m->addAction(tr("Delete"));
    m_ac->setObjectName("ci_id=" + QString::number(_id));
    m_ac->setIcon(AntiquaCRM::antiquaIcon("action-remove"));
    connect(m_ac, SIGNAL(triggered()), SLOT(deleteItem()));
    m_m->exec(_p);
    m_m->deleteLater();
  }
}

void KeywordsTable::itemSelected(const QModelIndex &index) {
  if (index.isValid()) {
    const QSqlRecord _r = m_tableModel->record(index.row());
    if (_r.count() == m_horizontalHeader->count()) {
      emit signalRowSelected(_r);
    }
  }
}

void KeywordsTable::deleteItem() {
  QAction *_obj = qobject_cast<QAction *>(sender());
  if (_obj == nullptr)
    return;

  if (_obj->objectName().startsWith("ci_id=")) {
    QString _sql("DELETE FROM " + m_tableModel->tableName());
    _sql.append(" WHERE " + _obj->objectName() + ";");
    m_sql->query(_sql);
    if (m_sql->lastError().isNull())
      select();
  }
}

void KeywordsTable::select() {
  if (m_tableModel == nullptr) {
    qWarning("Call initTable() before select!");
    return;
  }

  if (m_tableModel->tableName().isEmpty())
    initTable();

  m_tableModel->select();

  const QSqlRecord _record = m_tableModel->record();
  if (_record.count() > 0) {
    hideColumn(_record.indexOf("ci_id"));
    hideColumn(_record.indexOf("ci_company_usage"));
  }
}

void KeywordsTable::find(const QString &search) {
  if (m_tableModel == nullptr || search.length() < 3)
    return;

  const QModelIndex _index = searchKeyword(search);
  if (_index.isValid() && _index.row() > 0) {
    int _row = _index.row();
    selectRow(_row);
    QVariant _id = m_tableModel->data(_index.sibling(_row, 0));
    emit signalFound(_id.toInt());
  }
}

bool KeywordsTable::initTable(const QString &name) {
  if (m_sql == nullptr)
    m_sql = new AntiquaCRM::ASqlCore(this);

  m_tableModel = new QSqlTableModel(this, m_sql->db());
  m_tableModel->setTable(name);
  m_tableModel->setSort(0, Qt::AscendingOrder);
  setModel(m_tableModel);

  m_tableModel->setHeaderData(0, Qt::Horizontal, tr("ID"));
  m_tableModel->setHeaderData(1, Qt::Horizontal, tr("Keyword"));
  m_tableModel->setHeaderData(2, Qt::Horizontal, tr("Usage"));

  m_horizontalHeader->setSectionResizeMode(1, QHeaderView::Stretch);

  return true;
}

bool KeywordsTable::commitQuery(const QSqlRecord &r) {
  qint64 _id = -1;
  QString _keyword = r.field("ci_name").value().toString();
  bool _usage = r.field("ci_company_usage").value().toBool();
  if (r.field("ci_id").value().toInt() == 0) {
    const QModelIndex _index = searchKeyword(_keyword);
    if (_index.isValid()) {
      foreach (QModelIndex _index, selectedIndexes()) {
        const QModelIndex _idm = _index.sibling(_index.row(), 0);
        _id = m_tableModel->data(_idm, Qt::EditRole).toInt();
        if (_id > 0)
          break;
      }
    }
  } else {
    _id = r.field("ci_id").value().toInt();
  }

  QString _key = _keyword.trimmed();
  QString _use = (_usage ? "true" : "false");
  QString _sql;
  if (_id > 0) {
    _sql.append("UPDATE " + m_tableModel->tableName() + " SET ");
    _sql.append(" ci_name='" + _key + "',");
    _sql.append(" ci_company_usage='" + _use + "'");
    _sql.append(" WHERE ci_id=" + QString::number(_id) + ";");
  } else {
    _sql.append("INSERT INTO " + m_tableModel->tableName());
    _sql.append(" (ci_name,ci_company_usage) VALUES (");
    _sql.append("'" + _key + "','" + _use + "');");
  }

  if (m_sql == nullptr)
    m_sql = new AntiquaCRM::ASqlCore(this);

  m_sql->query(_sql);
  if (m_sql->lastError().isNull()) {
    select();
    return true;
  }

  const QSqlError _err = m_sql->lastError();
  qDebug() << Q_FUNC_INFO << _err.type() << _err.text();
  return false;
}
