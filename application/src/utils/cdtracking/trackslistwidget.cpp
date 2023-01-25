// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "trackslistwidget.h"

#include <QJsonValue>

TracksListWidget::TracksListWidget(QWidget *parent) : QTreeWidget{parent} {
  setColumnCount(2);
  setItemsExpandable(false);
  setSortingEnabled(false);
  setWordWrap(false);
  setAlternatingRowColors(true);

  QTreeWidgetItem *m_header = headerItem();
  m_header->setText(0, tr("Nr."));
  m_header->setToolTip(0, tr("Number"));
  m_header->setTextAlignment(0, Qt::AlignRight);
  m_header->setText(1, tr("Title"));
  m_header->setToolTip(1, tr("Track title"));
  m_header->setTextAlignment(1, Qt::AlignLeft);
}

void TracksListWidget::createEntry(const DiscInfo::Track &track) {
  // An Audio CD (CD-DA) can hold up to 99 audio tracks.
  QString index = QString::number(track.index).rightJustified(2, '0');
  QTreeWidgetItem *item = new QTreeWidgetItem(this, QTreeWidgetItem::UserType);
  item->setChildIndicatorPolicy(QTreeWidgetItem::DontShowIndicator);
  item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
  item->setText(0, index);
  item->setText(1, track.title);
  addTopLevelItem(item);
}

void TracksListWidget::setTracks(const QList<DiscInfo::Track> &list) {
  QListIterator<DiscInfo::Track> it(list);
  while (it.hasNext()) {
    createEntry(it.next());
  }
  resizeColumnToContents(0);
  resizeColumnToContents(1);
}

const QJsonObject TracksListWidget::getTracks() {
  QJsonObject arr;
  for (int r = 0; r < topLevelItemCount(); r++) {
    QTreeWidgetItem *item = topLevelItem(r);
    arr.insert(item->text(0), QJsonValue(item->text(1)));
  }
  return arr;
}
