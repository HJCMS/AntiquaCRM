// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "designationslist.h"

#include <QDebug>
#include <QIcon>

DesignationsListItem::DesignationsListItem(const DesignationData &data,
                                           DesignationsList *parent)
    : QListWidgetItem{parent, QListWidgetItem::Type} {
  setText(data.keyword);
  setIcon(QIcon("://icons/folder_txt.png"));
  data_t = data;
}

const DesignationData DesignationsListItem::getData() { return data_t; }

DesignationsList::DesignationsList(QWidget *parent) : QListWidget{parent} {
  setAlternatingRowColors(true);
  setSortingEnabled(true);
  setDragEnabled(false);
  setAcceptDrops(false);
  setContextMenuPolicy(Qt::NoContextMenu);

  connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
          SLOT(setPrepareClicked(QListWidgetItem *)));
}

void DesignationsList::setPrepareClicked(QListWidgetItem *item) {
  DesignationsListItem *i = reinterpret_cast<DesignationsListItem *>(item);
  if (i != nullptr)
    emit sendEditItem(i->getData());
}

void DesignationsList::insertQuery(const QSqlQuery &query) {
  if (query.size() < 1)
    return;

  QSqlQuery q(query);
  while (q.next()) {
    DesignationData data_t;
    data_t.id = q.value("bd_id").toInt();
    data_t.keyword = q.value("bd_keyword").toString();
    data_t.description = q.value("bd_description").toString();

    QString search("bd_id=" + QString::number(data_t.id));
    data_t.whereclause = search;
    addItem(new DesignationsListItem(data_t, this));
  }
}
