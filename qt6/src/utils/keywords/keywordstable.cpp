// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "keywordstable.h"

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
  m_horizontalHeader = new AntiquaCRM::TableHeader(this);
  setHorizontalHeader(m_horizontalHeader);

  connect(this, SIGNAL(doubleClicked(const QModelIndex &)),
          SLOT(setItemRow(const QModelIndex &)));
}

void KeywordsTable::setItemRow(const QModelIndex &index) {
  if (index.isValid()) {
    const QSqlRecord _r = m_tableModel->record(index.row());
    if (_r.count() == m_horizontalHeader->count()) {
      emit signalRowSelected(_r);
    }
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

void KeywordsTable::commitQuery(const QSqlRecord &r) {
  QString _sql;
  if (r.field("ci_id").value().toInt() == 0) {
    _sql.append("INSERT");
  } else {
    _sql.append("UPDATE");
  }
  qDebug() << Q_FUNC_INFO << r << _sql;
}
