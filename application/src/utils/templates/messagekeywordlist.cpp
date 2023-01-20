// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "messagekeywordlist.h"

#include <QBrush>
#include <QDrag>
#include <QHeaderView>
#include <QLabel>
#include <QMimeData>

MessageKeywordList::MessageKeywordList(QWidget *parent) : QTreeWidget{parent} {
  setObjectName("message_keyword_list");
  QStringList titles({tr("Category"), tr("Template")});
  setColumnCount(titles.count());
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

QTreeWidgetItem *MessageKeywordList::getSection(const QString &section,
                                                const QString &type) {
  for (int i = 0; i < topLevelItemCount(); i++) {
    QTreeWidgetItem *p = topLevelItem(i);
    if (p->text(0) == section)
      return p;
  }

  QTreeWidgetItem *item = new QTreeWidgetItem(this);
  item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
  item->setFlags(Qt::ItemIsEnabled);
  item->setText(0, section);

  if (!type.isEmpty()) {
    if (type.contains("CRM")) {
      item->setText(1, tr("Database queries"));
    } else if (type.contains("COMPANY")) {
      item->setText(1, tr("Company settings"));
    }

    QFont f = font();
    f.setStyleHint(QFont::Serif, QFont::PreferDefault);
    f.setStyle(QFont::StyleItalic);
    item->setFont(1, f);
    item->setForeground(1, QBrush(Qt::gray));
  }
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

    QString pasteText(delimiter);
    pasteText.append(item->text(1).trimmed());
    pasteText.append(delimiter);

    QMimeData *m_mimeData = new QMimeData;
    m_mimeData->setText(pasteText);

    QPoint itemPoint = event->pos() - visualItemRect(item).topLeft();
    QDrag *m_drag = new QDrag(this);
    m_drag->setMimeData(m_mimeData);
    m_drag->setHotSpot(itemPoint);
    m_drag->exec(Qt::CopyAction, Qt::CopyAction);
  } else {
    event->setAccepted(false);
  }
  QTreeWidget::mousePressEvent(event);
}

void MessageKeywordList::addKey(const QString &section,
                                const QJsonObject &jsObj) {
  QString key = jsObj.value("key").toString();
  QTreeWidgetItem *parent = getSection(section, key);
  if (parent != nullptr)
    insertTplKey(parent, jsObj);
}
