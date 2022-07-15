// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "draglistwidget.h"
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
}

void DragListWidget::mousePressEvent(QMouseEvent *event) {
  QModelIndex index = indexAt(event->pos());
  if (!index.isValid()) {
    event->setAccepted(false);
    return;
  }

  if (index.flags() & Qt::ItemIsDragEnabled) {
    event->setAccepted(true);
    QString text = itemAt(event->pos())->text();
    if (text.isEmpty())
      return;

    QMimeData *m_mimeData = new QMimeData;
    m_mimeData->setText(text);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(m_mimeData);
    drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction);
  } else {
    event->setAccepted(false);
  }
}
