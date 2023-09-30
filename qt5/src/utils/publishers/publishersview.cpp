// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "publishersview.h"
#include "publishersviewmodel.h"

#include <AntiquaCRM>
#include <QAction>
#include <QMenu>

PublishersView::PublishersView(QWidget *parent) : QTableView{parent} {
  setCornerButtonEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);
  setAlternatingRowColors(true);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);
  setEditTriggers(QAbstractItemView::NoEditTriggers);

  // TODO : dynamic table location by QLocale::bcp47Name()
  m_model = new PublishersViewModel(QString("ref_book_publisher_de"), this);
  setModel(m_model);

  m_headerView = horizontalHeader();
  m_headerView->setSectionResizeMode(QHeaderView::ResizeToContents);
  m_headerView->setStretchLastSection(true);
  m_headerView->setHighlightSections(true);
  setHorizontalHeader(m_headerView);

  connect(m_model,
          SIGNAL(dataChanged(const QModelIndex &, const QModelIndex &)),
          SLOT(selected(const QModelIndex &, const QModelIndex &)));
}

const QString PublishersView::queryContent() const {
  QString _sql("SELECT p_name, p_location FROM " + getTableName());
  _sql.append(" ORDER BY p_name;");
  return _sql;
}

void PublishersView::contextMenuEvent(QContextMenuEvent *event) {
  QModelIndex _index = indexAt(event->pos());
  if (!_index.isValid())
    return;

  selectRow(_index.row());
  QIcon icon_remove("://icons/action_remove.png");
  QMenu *m = new QMenu("Actions", this);
  QAction *ac_del = m->addAction(icon_remove, tr("Delete"));
  ac_del->setEnabled((m_model->rowCount() > 1));
  connect(ac_del, SIGNAL(triggered()), SLOT(deleteItem()));
  m->exec(event->globalPos());
  delete m;
}

void PublishersView::selected(const QModelIndex &current,
                              const QModelIndex &previous) {
  Q_UNUSED(current);
  resizeColumnToContents(previous.column());
  m_headerView->setStretchLastSection(true);
  setWindowModified(true);
}

void PublishersView::deleteItem() {
  QModelIndexList _list = selectedIndexes();
  if (_list.size() < 1)
    return;

  QModelIndex _index(_list.first());
  if (!_index.isValid())
    return;

  QPair<QString, QString> _p = getData(_index);
  QString _sql("DELETE FROM " + getTableName());
  _sql.append(" WHERE (p_name='" + _p.first + "' AND");
  _sql.append(" p_location='" + _p.second + "');");
  if (commitQuery(_sql))
    reload();
}

void PublishersView::showPublisher(const QString &name) {
  QModelIndex _index = index(name, false);
  if (_index.isValid()) {
    selectRow(_index.row());
    return;
  }

  clearSelection();
  scrollToTop();
}

void PublishersView::reload() {
  clearSelection();
  m_model->querySelect(queryContent());
  scrollToTop();
}

const QString PublishersView::getTableName() const {
  return m_model->tableName();
}

bool PublishersView::commitQuery(const QString &query) {
  AntiquaCRM::ASqlCore pgsql(this);
  pgsql.query(query);
  return pgsql.lastError().isEmpty();
}

QPair<QString, QString> PublishersView::getData(const QModelIndex &index) {
  QPair<QString, QString> _p;
  _p.first = m_model->data(index.sibling(index.row(), 0)).toString();
  _p.second = m_model->data(index.sibling(index.row(), 1)).toString();
  return _p;
}

const QModelIndex PublishersView::index(const QString &publisher, bool strict) {
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

bool PublishersView::init() {
  bool b = m_model->querySelect(queryContent());
  if (b)
    m_headerView->setStretchLastSection(true);

  setWindowModified(false);
  return b;
}
