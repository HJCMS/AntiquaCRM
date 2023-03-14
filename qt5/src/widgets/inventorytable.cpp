// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventorytable.h"

#include <QDateTime>
#include <QDebug>
#include <QFont>
#include <QFontMetrics>
#include <QIcon>
#include <QPainter>
#include <QPalette>

InventoryTableHeader::InventoryTableHeader(QWidget *parent)
    : QHeaderView{Qt::Horizontal, parent} {
  setSectionsMovable(false);
  setDefaultAlignment(Qt::AlignCenter);
  setSectionResizeMode(QHeaderView::Interactive);
}

void InventoryTableHeader::resizeToContents(bool b) {
  if (b)
    resizeSections(QHeaderView::ResizeToContents);
}

InventoryTable::InventoryTable(QWidget *parent) : QTableView{parent} {
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
  m_header = new InventoryTableHeader(this);
  setHorizontalHeader(m_header);
  // Warning not before HeaderView initialed
  setEnableTableViewSorting(false);

  connect(m_header, SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this,
          SLOT(setSortByColumn(int, Qt::SortOrder)));

  connect(this, SIGNAL(sendResultExists(bool)), m_header,
          SLOT(resizeToContents(bool)));
}

void InventoryTable::paintEvent(QPaintEvent *ev) {
  if (rowCount() == 0) {
    QString time = QTime::currentTime().toString("hh:mm");
    QStringList l(tr("The query at %1 returned no result.").arg(time));
    l.append(
        tr("Change the search query or choose a different history query."));

    QPainter painter(viewport());
    painter.setBrush(palette().text());
    painter.setFont(font());
    painter.setOpacity(0.8);
    painter.drawText(rect(), Qt::AlignCenter, l.join("\n"));
  }
  QTableView::paintEvent(ev);
}

void InventoryTable::setEnableTableViewSorting(bool b) {
  setSortingEnabled(b);
  m_header->setSectionsClickable(b);
}

void InventoryTable::sqlModelError(const QString &table,
                                   const QString &message) {
  qWarning("SQL-Model-Error in Table:%s\n%s\n", // verbose
           qPrintable(table),                   // table
           qPrintable(message));
}

void InventoryTable::setQueryLimit(int limit) {
  if (limit < 1)
    return;

  QueryLimit = limit;
  emit sendQueryLimitChanged(QueryLimit);
}

const QIcon InventoryTable::cellIcon(const QString &name) {
  return QIcon(":icons/" + name + ".png");
}

int InventoryTable::getQueryLimit() { return QueryLimit; }

bool InventoryTable::isAutoRefreshEnabled() {
  return (rowCount() > 0 && rowCount() < QueryAutoUpdate);
}
