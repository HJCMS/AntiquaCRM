// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "designationslist.h"
#include "antiquaicon.h"

#include <QDebug>
#include <QIcon>

DesignationsListItem::DesignationsListItem(const DesignationData &data,
                                           DesignationsList *parent)
    : QListWidgetItem{parent, QListWidgetItem::Type} {
  setText(data.keyword);
  setIcon(AntiquaCRM::antiquaIcon("view-list-text"));
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

void DesignationsList::insertQuery(QSqlQuery &query) {
  if (query.size() < 1)
    return;

  while (query.next()) {
    DesignationData data_t;
    data_t.id = query.value("bd_id").toInt();
    data_t.keyword = query.value("bd_keyword").toString();
    data_t.description = query.value("bd_description").toString();
    addItem(new DesignationsListItem(data_t, this));
  }
}
