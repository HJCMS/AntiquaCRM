// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "publishersview.h"
#include "publishersviewmodel.h"

PublishersView::PublishersView(QWidget *parent) : QTableView{parent} {
  setCornerButtonEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);
  setAlternatingRowColors(true);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);
  setEditTriggers(QAbstractItemView::NoEditTriggers);

  m_model = new PublishersViewModel(this);
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

void PublishersView::selected(const QModelIndex &current,
                              const QModelIndex &previous) {
  Q_UNUSED(current);
  resizeColumnToContents(previous.column());
  m_headerView->setStretchLastSection(true);
  setWindowModified(true);
}

void PublishersView::showPublisher(const QString &str) {
  QModelIndex _index = find(str, false);
  if (_index.isValid()) {
    selectRow(_index.row());
    return;
  }

  clearSelection();
  scrollToTop();
}

QPair<QString, QString> PublishersView::getData(const QModelIndex &index) {
  QPair<QString, QString> _p;
  _p.first = m_model->data(index.sibling(index.row(), 0)).toString();
  _p.second = m_model->data(index.sibling(index.row(), 1)).toString();
  return _p;
}

const QModelIndex PublishersView::find(const QString &publisher, bool strict) {
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
      if (_current == _search)
        return _child;
    } else {
      if (_current.contains(_search, Qt::CaseInsensitive))
        return _child;
    }
  }

  return QModelIndex();
}

bool PublishersView::init() {
  bool b = m_model->querySelect("SELECT * FROM " + m_model->tableName() + ";");
  if (b)
    m_headerView->setStretchLastSection(true);

  setWindowModified(false);
  return b;
}
