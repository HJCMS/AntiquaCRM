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

    QDrag *m_drag = new QDrag(this);
    m_drag->setMimeData(m_mimeData);
    m_drag->exec(Qt::CopyAction);
  } else {
    event->setAccepted(false);
  }
}

void DragListWidget::setItemHidden(const QString &name) {
  for(int r = 0; r < count(); r++) {
    if(item(r)->text() == name) {
      item(r)->setHidden(true);
      break;
    }
  }
}

void DragListWidget::setItemVisible(const QString &name) {
  for(int r = 0; r < count(); r++) {
    if(item(r)->text() == name) {
      item(r)->setHidden(false);
      break;
    }
  }
}
