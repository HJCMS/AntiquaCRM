// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "messagekeywordlist.h"

#include <QHeaderView>
#include <QMimeData>
#include <QDrag>
#include <QLabel>

MessageKeywordList::MessageKeywordList(QWidget *parent) : QTreeWidget{parent} {
  setObjectName("message_keyword_list");
  setColumnCount(2);
  QStringList titles({tr("Category"),tr("Template")});
  setHeaderLabels(titles);
  setSelectionMode(QAbstractItemView::SingleSelection);
  setAlternatingRowColors(true);
  setExpandsOnDoubleClick(true);
  setAcceptDrops(false);
  setDragEnabled(true);
  setDropIndicatorShown(true);
  setDragDropMode(QAbstractItemView::DragOnly);
  header()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

QTreeWidgetItem *MessageKeywordList::getSection(const QString &section) {
  for (int i = 0; i < topLevelItemCount(); i++) {
    QTreeWidgetItem *p = topLevelItem(i);
    if (p->text(0) == section)
      return p;
  }

  QTreeWidgetItem *item = new QTreeWidgetItem(this);
  item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
  item->setFlags(Qt::ItemIsEnabled);
  item->setText(0, section);
  return item;
}

bool MessageKeywordList::insertTplKey(QTreeWidgetItem *parent,
                                      const QJsonObject &jsObj) {
  if (parent == nullptr)
    return false;

  QTreeWidgetItem *item = new QTreeWidgetItem(parent);
  item->setText(0, jsObj.value("title").toString());
  item->setData(0, Qt::UserRole, jsObj);
  item->setText(1, jsObj.value("key").toString());
  return true;
}

void MessageKeywordList::mousePressEvent(QMouseEvent *event) {
  QModelIndex index = indexAt(event->pos());
  if (!index.isValid()) {
    event->setAccepted(false);
    return;
  }

  if (index.flags() & Qt::ItemIsDragEnabled) {
    event->setAccepted(true);
    QTreeWidgetItem *item = itemAt(event->pos());
    if (item == nullptr || item->text(1).isEmpty())
      return;

    QMimeData *m_mimeData = new QMimeData;
    m_mimeData->setText(item->text(1));

    QPoint itemPoint = event->pos() - visualItemRect(item).topLeft();
    QLabel *m_label = new QLabel(item->text(1), this);

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
  QTreeWidget::mousePressEvent(event);
}

void MessageKeywordList::addKey(const QString &section, const QJsonObject &jsObj) {
  QTreeWidgetItem *parent = getSection(section);
  if (parent != nullptr)
    insertTplKey(parent, jsObj);
}
