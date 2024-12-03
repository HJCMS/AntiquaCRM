// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "articledescwordstable.h"
#include "descwordstablemodel.h"

ArticleDescWordsTable::ArticleDescWordsTable(QWidget* parent) : AntiquaCRM::TableView{parent} {
  setObjectName("article_descriptions_table");
  setEnableTableViewSorting(true);
  setSelectionMode(QAbstractItemView::SingleSelection);
  setSelectionBehavior(QAbstractItemView::SelectItems);
  setEditTriggers(QAbstractItemView::DoubleClicked);

  m_model = new DescWordsTableModel(this);
  p_wehreClause = defaultWhereClause();

  // ??? horizontalHeader()->setSectionHidden(0, true);

  connect(m_model, SIGNAL(sqlErrorMessage(QString, QString)),
          SLOT(sqlModelError(QString, QString)));
  connect(this, SIGNAL(doubleClicked(QModelIndex)), SLOT(getSelectedItem(QModelIndex)));
}

qint64 ArticleDescWordsTable::getTableID(const QModelIndex& index, int column) {
  QModelIndex id(index);
  if (m_model->data(id.sibling(id.row(), column), Qt::EditRole).toInt() >= 1) {
    return m_model->data(id.sibling(id.row(), column), Qt::EditRole).toInt();
  }
  return -1;
}

void ArticleDescWordsTable::contextMenuAction(AntiquaCRM::TableContextMenu::Actions,
                                              const QModelIndex&) {
}

void ArticleDescWordsTable::setSortByColumn(int column, Qt::SortOrder order) {
  if (column < 0)
    return;

  QString order_by = m_model->fieldName(column);
  /**
   * @warning Bei Alias basierenden SELECT abfragen!
   * ORDER BY "Multisort" Abfragen können nicht mit Aliases gemischt werden!
   */
  if (!p_tableRecord.isEmpty()) {
    QStringList fieldList;
    for (int i = 0; i < p_tableRecord.count(); i++) {
      fieldList << p_tableRecord.field(i).name();
    }
    if (fieldList.contains(order_by)) {
      order_by.prepend("(");
      order_by.append(",aes_id)");
    }
  }

  QString _sql("SELECT " + m_model->fieldList() + " FROM " + m_model->tableName() + " ");
  _sql.append("WHERE " + p_wehreClause + " ");
  _sql.append("ORDER BY " + m_model->orderColumn() + " ");
  if (order == Qt::AscendingOrder) {
    _sql.append("ASC;");
  } else {
    _sql.append("DESC;");
  }
  sqlModelQuery(_sql);
}

void ArticleDescWordsTable::getSelectedItem(const QModelIndex& index) {
  qint64 _id = getTableID(index);
  if (_id >= 1)
    emit sendOpenEntry(_id);
}

void ArticleDescWordsTable::createSocketOperation(const QModelIndex& index) {
  qint64 _id = getTableID(index);
  qDebug() << Q_FUNC_INFO << _id;
}

bool ArticleDescWordsTable::sqlModelQuery(const QString& query) {
  // qDebug() << Q_FUNC_INFO << query;
  if (m_model->querySelect(query)) {
    QueryHistory = query;
    setModel(m_model);
    // Table Record und NICHT QueryRecord abfragen!
    // Siehe: setSortByColumn
    p_tableRecord = m_model->tableRecord();
    queryFinished(m_model->rowCount() > 0);
    return true;
  }
  return false;
}

void ArticleDescWordsTable::setReloadView() {
  sqlModelQuery(m_model->query().lastQuery());
}

int ArticleDescWordsTable::rowCount() {
  return m_model->rowCount();
}

bool ArticleDescWordsTable::setQuery(const QString& clause) {
  QString _sql("SELECT " + m_model->fieldList() + " FROM " + m_model->tableName() + " ");
  if (clause.isEmpty()) {
    _sql.append("WHERE " + defaultWhereClause() + " ");
  } else {
    _sql.append("WHERE " + clause + " ");
  }
  _sql.append("ORDER BY " + m_model->orderColumn() + " ASC;");
  return sqlModelQuery(_sql);
}

const QString ArticleDescWordsTable::defaultWhereClause() {
  return QString("aes_title IS NOT NULL");
}
