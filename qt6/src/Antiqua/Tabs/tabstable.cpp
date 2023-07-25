// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tabstable.h"

#include <QPainter>
#include <QTime>

namespace AntiquaCRM {

TabsTableHeader::TabsTableHeader(QWidget *parent)
    : QHeaderView{Qt::Horizontal, parent} {
  setSectionsMovable(false);
  setDefaultAlignment(Qt::AlignCenter);
  setSectionResizeMode(QHeaderView::Interactive);
}

void TabsTableHeader::resizeToContents(bool b) {
  if (b)
    resizeSections(QHeaderView::ResizeToContents);
}

TabsTable::TabsTable(QWidget *parent) : QTableView{parent} {
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setCornerButtonEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);
  setAlternatingRowColors(true);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);

  m_cfg = new AntiquaCRM::ASettings(this);
  QueryLimit = m_cfg->value("SqlQueryLimit", 1000).toInt();
  QueryAutoUpdate = m_cfg->value("SqlAutoUpdateCount", 50).toInt();

  /* Kopfzeilen anpassen */
  m_header = new TabsTableHeader(this);
  setHorizontalHeader(m_header);
  // Warning not before HeaderView initialed
  setEnableTableViewSorting(false);

  connect(m_header, SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)),
          SLOT(setSortByColumn(int, Qt::SortOrder)));

  connect(this, SIGNAL(sendResultExists(bool)), m_header,
          SLOT(resizeToContents(bool)));
}

void TabsTable::paintEvent(QPaintEvent *event) {
  if (getRowCount() == 0) {
    QString _time = QTime::currentTime().toString("hh:mm");
    QStringList _list(tr("The query at %1 returned no result.").arg(_time));
    _list.append(
        tr("Change the search query or choose a different history query."));

    QPainter _painter(viewport());
    _painter.setBrush(palette().text());
    _painter.setFont(font());
    _painter.setOpacity(0.8);
    _painter.drawText(rect(), Qt::AlignCenter, _list.join("\n"));
  }
  QTableView::paintEvent(event);
}

void TabsTable::setEnableTableViewSorting(bool b) {
  setSortingEnabled(b);
  m_header->setSectionsClickable(b);
}

void TabsTable::sqlModelError(const QString &table, const QString &message) {
  qWarning("SQL-Model-Error in Table:%s\n%s\n", // verbose
           qPrintable(table), qPrintable(message));
}

void TabsTable::setQueryLimit(int limit) {
  if (limit < 1)
    return;

  QueryLimit = limit;
  emit sendQueryLimitChanged(QueryLimit);
}

const QIcon TabsTable::getIcon(const QString &name) {
  return AntiquaCRM::antiquaIcon(name);
}

int TabsTable::getQueryLimit() { return QueryLimit; }

bool TabsTable::isAutoRefreshEnabled() {
  return (getRowCount() > 0 && getRowCount() < QueryAutoUpdate);
}

} // namespace AntiquaCRM
