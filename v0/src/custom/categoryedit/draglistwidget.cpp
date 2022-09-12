// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "draglistwidget.h"
#include "draglabel.h"
#include "myicontheme.h"

#include <QApplication>
#include <QDebug>
#include <QDrag>
#include <QMetaType>
#include <QMimeData>
#include <QPixmap>
#include <QScreen>

DragListWidget::DragListWidget(QWidget *parent) : QListWidget{parent} {
  setAlternatingRowColors(true);
  setAcceptDrops(false);
  setDragEnabled(true);
  setSelectionMode(QAbstractItemView::SingleSelection);
}

void DragListWidget::mousePressEvent(QMouseEvent *event) {
  QModelIndex index = indexAt(event->pos());
  if (!index.isValid()) {
    event->setAccepted(false);
    return;
  }

  if (index.flags() & Qt::ItemIsDragEnabled) {
    event->setAccepted(true);
    QListWidgetItem *item = itemAt(event->pos());
    if (item == nullptr || item->text().isEmpty())
      return;

    QMimeData *m_mimeData = new QMimeData;
    m_mimeData->setText(item->text());

    QPoint itemPoint = event->pos() - visualItemRect(item).topLeft();
    QLabel *m_label = new DragLabel(item->text(), this);

    QDrag *m_drag = new QDrag(this);
    m_drag->setMimeData(m_mimeData);
    m_drag->setPixmap(m_label->pixmap(Qt::ReturnByValue));
    m_drag->setHotSpot(itemPoint);
    m_label->hide();

    m_drag->exec(Qt::CopyAction, Qt::CopyAction);
    m_label->hide();
    m_label->deleteLater();
  } else {
    event->setAccepted(false);
  }
}

void DragListWidget::setItemHidden(const QString &name) {
  for (int r = 0; r < count(); r++) {
    if (item(r)->text() == name) {
      item(r)->setHidden(true);
      break;
    }
  }
}

void DragListWidget::setItemVisible(const QString &name) {
  for (int r = 0; r < count(); r++) {
    if (item(r)->text() == name) {
      item(r)->setHidden(false);
      break;
    }
  }
}

void DragListWidget::setItemsHidden(const QStringList &list)
{
  foreach(QString name, list) {
    for (int r = 0; r < count(); r++) {
      if (item(r)->text() == name) {
        item(r)->setHidden(true);
      }
    }
  }
}
