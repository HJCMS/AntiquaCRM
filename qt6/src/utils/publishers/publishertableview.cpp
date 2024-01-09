// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "publishertableview.h"
#include "publishertablemodel.h"

PublisherTableView::PublisherTableView(QWidget *parent)
    : AntiquaCRM::TableView{parent} {
  setObjectName("publisher_table_view");
  m_model = new PublisherTableModel(this);
  m_header = new AntiquaCRM::TableHeader(this);
  setHorizontalHeader(m_header);
  // Warning not before HeaderView initialed
  setEnableTableViewSorting(false);
  connect(this, SIGNAL(doubleClicked(const QModelIndex &)),
          SLOT(getSelectedItem(const QModelIndex &)));
}

qint64 PublisherTableView::getTableID(const QModelIndex &index, int column) {
  Q_UNUSED(column);
  return index.row();
}

const QModelIndex PublisherTableView::index(const QString &publisher,
                                            bool strict) {
  QString _search = publisher.trimmed();
  if (_search.isEmpty())
    return QModelIndex();

  const QModelIndex _index(indexAt(QPoint(0, 0)));
  for (int r = 0; r < m_model->rowCount(); r++) {
    QModelIndex _child = _index.sibling(r, 0);
    if (!_child.isValid())
      continue;

    QString _current = m_model->data(_child).toString();
    if (_current.isEmpty())
      continue;

    // qDebug() << _search << _child.row() << _current;
    if (strict) {
      if (_current.toLower() == _search.toLower())
        return _child;
    } else {
      if (_current.contains(_search, Qt::CaseInsensitive))
        return _child;
    }
  }

  return QModelIndex();
}

bool PublisherTableView::sqlModelQuery(const QString &query) {
  if (m_model->querySelect(query)) {
    bool stretch = (m_model->rowCount() > 0);
    queryFinished(stretch);
    m_header->setStretchLastSection(stretch);
    return true;
  }
  return false;
}

const QPair<QString, QString>
PublisherTableView::getData(const QModelIndex &index) {
  QPair<QString, QString> _p;
  _p.first = m_model->data(index.sibling(index.row(), 0)).toString();
  _p.second = m_model->data(index.sibling(index.row(), 1)).toString();
  return _p;
}

void PublisherTableView::contextMenuEvent(QContextMenuEvent *event) {
  const QModelIndex index = indexAt(event->pos());
  qint64 rows = m_model->rowCount();
  AntiquaCRM::TableContextMenu *m_menu =
      new AntiquaCRM::TableContextMenu(index, rows, this);
  m_menu->addDeleteAction(tr("Remove"));
  connect(m_menu,
          SIGNAL(sendAction(AntiquaCRM::TableContextMenu::Actions,
                            const QModelIndex &)),
          SLOT(contextMenuAction(AntiquaCRM::TableContextMenu::Actions,
                                 const QModelIndex &)));

  m_menu->exec(event->globalPos());
  m_menu->deleteLater();
}

bool PublisherTableView::setQuery(const QString &query) {
  if (query.contains(m_model->tableName())) {
    AntiquaCRM::ASqlCore pgsql(this);
    pgsql.query(query);
    return pgsql.lastError().isEmpty();
  } else
    return sqlModelQuery(defaultWhereClause());
}

const QString PublisherTableView::defaultWhereClause() {
  return QString("SELECT p_name, p_location FROM %1 ORDER BY p_name ASC;")
      .arg(m_model->tableName());
}

void PublisherTableView::deleteItem(const QModelIndex &index) {
  if (!index.isValid())
    return;

  const QPair<QString, QString> _p = getData(index);
  QString _sql("DELETE FROM " + m_model->tableName());
  _sql.append(" WHERE (p_name='" + _p.first + "' AND");
  _sql.append(" p_location='" + _p.second + "');");
  if (setQuery(_sql))
    setReloadView();
}

void PublisherTableView::contextMenuAction(
    AntiquaCRM::TableContextMenu::Actions action, const QModelIndex &index) {
  if (action == AntiquaCRM::TableContextMenu::Actions::Delete)
    deleteItem(index);
}

void PublisherTableView::setSortByColumn(int column, Qt::SortOrder order) {
  Q_UNUSED(column);
  Q_UNUSED(order);
}

void PublisherTableView::getSelectedItem(const QModelIndex &index) {
  if (getPublisher(index).length() > 0)
    emit sendItemSelected(index);
}

void PublisherTableView::createSocketOperation(const QModelIndex &index) {
  Q_UNUSED(index);
}

void PublisherTableView::setReloadView() {
  clearSelection();
  sqlModelQuery(defaultWhereClause());
  scrollToTop();
}

bool PublisherTableView::saveData(const QString &publisher,
                                  const QString &location) {
  QString _sql;
  const QModelIndex _index = index(publisher);
  if (_index.isValid() && _index.row() > 0) {
    QPair<QString, QString> _tmp = getData(_index);
    _sql = QString("UPDATE " + m_model->tableName() + " SET ");
    _sql.append("p_name='" + publisher + "', ");
    _sql.append("p_location='" + location + "' ");
    _sql.append("WHERE (p_name='" + _tmp.first + "' AND ");
    _sql.append("p_location='" + _tmp.second + "');");
  } else {
    _sql = QString("INSERT INTO " + m_model->tableName());
    _sql.append(" (p_name,p_location) VALUES (");
    _sql.append("'" + publisher + "',");
    _sql.append("'" + location + "');");
  }

  if (_sql.contains(m_model->tableName()))
    return setQuery(_sql);

  return false;
}

int PublisherTableView::rowCount() { return m_model->rowCount(); }

const QString PublisherTableView::getPublisher(const QModelIndex &index) {
  QModelIndex _index(index);
  QString _buf =
      m_model->data(_index.sibling(_index.row(), 0), Qt::EditRole).toString();
  return _buf.trimmed();
}

const QString PublisherTableView::getLocation(const QModelIndex &index) {
  QModelIndex _index(index);
  QString _buf =
      m_model->data(_index.sibling(_index.row(), 1), Qt::EditRole).toString();
  return _buf.trimmed();
}

bool PublisherTableView::init() {
  setModel(m_model);
  bool b = sqlModelQuery(defaultWhereClause());
  setWindowModified(false);
  return b;
}
