// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "tableview.h"
#include "antiquaicon.h"
#include "apopupmessage.h"
#include "tableheader.h"

#include <QDateTime>
#include <QDebug>
#include <QFont>
#include <QFontMetrics>
#include <QIcon>
#include <QPainter>
#include <QPalette>

namespace AntiquaCRM {

TableView::TableView(QWidget *parent) : QTableView{parent} {
  setEditTriggers(QAbstractItemView::NoEditTriggers);
  setCornerButtonEnabled(false);
  setDragEnabled(false);
  setDragDropOverwriteMode(false);
  setWordWrap(false);
  setAlternatingRowColors(true);
  setSelectionBehavior(QAbstractItemView::SelectRows);
  setSelectionMode(QAbstractItemView::SingleSelection);

  m_cfg = new AntiquaCRM::ASettings(this);
  m_cfg->beginGroup("database");
  QueryLimit = m_cfg->value("SqlQueryLimit", 1000).toInt();
  QueryAutoUpdate = m_cfg->value("SqlAutoUpdateCount", 50).toInt();
  m_cfg->endGroup();

  /* Kopfzeilen anpassen */
  m_header = new TableHeader(this);
  setHorizontalHeader(m_header);
  // Warning not before HeaderView initialed
  setEnableTableViewSorting(false);

  connect(m_header, SIGNAL(sortIndicatorChanged(int, Qt::SortOrder)), this,
          SLOT(setSortByColumn(int, Qt::SortOrder)));
}

void TableView::paintEvent(QPaintEvent *ev) {
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

void TableView::setEnableTableViewSorting(bool b) {
  setSortingEnabled(b);
  m_header->setSectionsClickable(b);
}

void TableView::queryFinished(bool b) {
  m_header->resizeToContents(b);
  emit sendQueryFinished();
}

void TableView::sqlModelError(const QString &table, const QString &message) {
  qWarning("SQL-Model-Error in Table:%s\n%s\n", // verbose
           qPrintable(table),                   // table
           qPrintable(message));
}

void TableView::sqlErrorPopUp(const QSqlError &error) {
  AntiquaCRM::APopUpMessage *d = new AntiquaCRM::APopUpMessage(this);
  d->setWindowTitle(tr("SQL Table query error"));
  d->setText(error.driverText());
  const QString _code = tr("Error code: %1\n").arg(error.nativeErrorCode());
  d->setErrorMessage(_code, error.text());
  d->exec();
  d->deleteLater();
}

void TableView::setQueryLimit(int limit) {
  if (limit < 1)
    return;

  QueryLimit = limit;
  emit sendQueryLimitChanged(QueryLimit);
}

const QIcon TableView::cellIcon(const QString &name) {
  return AntiquaCRM::antiquaIcon(name);
}

int TableView::getQueryLimit() { return QueryLimit; }

bool TableView::isAutoRefreshEnabled() {
  return (rowCount() > 0 && rowCount() < QueryAutoUpdate);
}

} // namespace AntiquaCRM
