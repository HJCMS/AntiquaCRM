// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "inventorytable.h"

#include <QDateTime>
#include <QDebug>
#include <QFont>
#include <QIcon>
#include <QPainter>
#include <QPalette>

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
  m_header = horizontalHeader();
  m_header->setSectionsMovable(false);
  m_header->setDefaultAlignment(Qt::AlignCenter);
  m_header->setSectionResizeMode(QHeaderView::ResizeToContents);
  setHorizontalHeader(m_header);

  connect(m_header, SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this,
          SLOT(setSortByColumn(int, Qt::SortOrder)));

  // Warning not before HeaderView initialed
  setEnableTableViewSorting(false);
}

void InventoryTable::paintEvent(QPaintEvent *ev) {
  if (rowCount() == 0) {
    QString time = QTime::currentTime().toString("hh:mm");
    QStringList l(tr("The query at %1 returned no result.").arg(time));
    l.append(tr("Change the search query or choose a different history query."));
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

void InventoryTable::getSqlModelError(const QString &table,
                                      const QString &message) {
#ifndef ANTIQUA_DEVELOPEMENT
  Q_UNUSED(table);
  Q_UNUSED(message);
#else
  qDebug() << "SqlModelError:" << table << message;
#endif
}

void InventoryTable::setQueryLimit(int limit) {
  if (limit < 1)
    return;

  QueryLimit = limit;
}

const QIcon InventoryTable::cellIcon(const QString &name) {
  return QIcon(":icons/" + name + ".png");
}

int InventoryTable::getQueryLimit() { return QueryLimit; }

bool InventoryTable::isAutoRefreshEnabled() {
  return (rowCount() > 0 && rowCount() < QueryAutoUpdate);
}
