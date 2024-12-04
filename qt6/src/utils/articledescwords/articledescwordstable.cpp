// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "articledescwordstable.h"
#include "descwordstablemodel.h"

#include <QHeaderView>

ArticleDescWordsTable::ArticleDescWordsTable(QWidget* parent) : AntiquaCRM::TableView{parent} {
  setObjectName("article_descriptions_table");
  setSelectionMode(QAbstractItemView::SingleSelection);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setEditTriggers(QAbstractItemView::DoubleClicked);
  setEnableTableViewSorting(true);

  m_model = new DescWordsTableModel(this);
  p_wehreClause = defaultWhereClause();

  m_header = horizontalHeader();

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

void ArticleDescWordsTable::contextMenuAction(AntiquaCRM::TableContextMenu::Actions ac,
                                              const QModelIndex& index) {
  qint64 _id = getTableID(index);
  if (_id < 1)
    return;

  switch (ac) {
    case (AntiquaCRM::TableContextMenu::Actions::Open):
      emit sendOpenEntry(_id);
      break;

    case (AntiquaCRM::TableContextMenu::Actions::Delete):
      emit sendDeleteEntry(_id);
      break;

    default:
      return;
  };
}

void ArticleDescWordsTable::contextMenuEvent(QContextMenuEvent* event) {
  QModelIndex index = indexAt(event->pos());
  qint64 rows = m_model->rowCount();
  AntiquaCRM::TableContextMenu* m_menu = new AntiquaCRM::TableContextMenu(index, rows, this);
  m_menu->addOpenAction(tr("Open entry"));
  m_menu->addDeleteAction(tr("Delete entry"));

  connect(m_menu, SIGNAL(sendAction(AntiquaCRM::TableContextMenu::Actions, QModelIndex)),
          SLOT(contextMenuAction(AntiquaCRM::TableContextMenu::Actions, QModelIndex)));

  m_menu->exec(event->globalPos());
  m_menu->deleteLater();
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

void ArticleDescWordsTable::createSocketOperation(const QModelIndex&) {
}

bool ArticleDescWordsTable::sqlModelQuery(const QString& query) {
  if (m_model->querySelect(query)) {
    QueryHistory = query;
    setModel(m_model);
    p_tableRecord = m_model->tableRecord();
    queryFinished(m_model->rowCount() > 0);
    m_header->setSectionHidden(0, true);
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

const QString ArticleDescWordsTable::tableName() {
  return m_model->tableName();
}

qint64 ArticleDescWordsTable::getItemId(const QModelIndex& index) {
  QModelIndex _child = index.sibling(index.row(), 0);
  return m_model->data(_child).toInt();
}
