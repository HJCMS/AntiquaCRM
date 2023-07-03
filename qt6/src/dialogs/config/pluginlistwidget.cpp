// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "pluginlistwidget.h"

#include <QAbstractItemView>
#include <QMimeData>
#include <QRect>
#include <QStandardItem>

PluginListWidget::PluginListWidget(QWidget *parent) : QListWidget{parent} {
  setSpacing(5);
  setFlow(QListView::TopToBottom);
  setResizeMode(QListView::Adjust);
  setSelectionMode(QAbstractItemView::SingleSelection);
  setWrapping(false);
  setDragEnabled(true);
  setAcceptDrops(true);
  setMovement(QListView::Snap);

  connect(this, SIGNAL(indexesMoved(const QModelIndexList &)),
          SLOT(setPluginSort(const QModelIndexList &)));
}

void PluginListWidget::mousePressEvent(QMouseEvent *event) {
  if (event->button() != Qt::LeftButton)
    return;

//  QModelIndex index;
//  p_moveStartPosi = event->pos();
//  QDrag drag(index);
}

void PluginListWidget::dragEnterEvent(QDragEnterEvent *event) {
  if (event->source() == this) {
    event->setDropAction(Qt::MoveAction);
    event->accept();
  } else {
    event->acceptProposedAction();
  }
}

void PluginListWidget::dropEvent(QDropEvent *event) {
  if (event->source() != this) {
    event->ignore();
    return;
  } else if (event->possibleActions() & Qt::MoveAction) {
    qDebug() << Q_FUNC_INFO << p_moveStartPosi << event->position();
    event->accept();
  }
}

void PluginListWidget::setPluginSort(const QModelIndexList &indexes) {
  qDebug() << Q_FUNC_INFO << indexes.size();
}

void PluginListWidget::insertPlugin(const QJsonObject &jso) {
  QListWidgetItem *item = new QListWidgetItem(this, QListWidgetItem::UserType);
  item->setData(Qt::UserRole, jso.value("Name").toString());
  item->setData(Qt::DisplayRole, jso.value("Title").toString());
  item->setData(Qt::ToolTipRole, jso.value("Description").toString());
  item->setData(Qt::DecorationRole, QIcon(":/icons/bookmark.png"));
  item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsDragEnabled |
                 Qt::ItemIsEnabled | Qt::ItemNeverHasChildren);
  insertItem(count(), item);
}
