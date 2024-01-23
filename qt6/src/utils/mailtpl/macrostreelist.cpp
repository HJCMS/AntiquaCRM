// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "macrostreelist.h"

#include <QDrag>
#include <QHeaderView>
#include <QMimeData>

MacrosTreeList::MacrosTreeList(QWidget *parent) : QTreeWidget{parent} {
  setColumnCount(2);
  setHeaderLabels(QStringList({tr("Category"), tr("Template")}));
  setSelectionMode(QAbstractItemView::SingleSelection);
  setAlternatingRowColors(true);
  setExpandsOnDoubleClick(true);
  setAcceptDrops(false);
  setDragEnabled(true);
  setDropIndicatorShown(true);
  setDragDropMode(QAbstractItemView::DragOnly);
  header()->setSectionResizeMode(QHeaderView::ResizeToContents);
  setToolTip(tr("Select a macro, Drag & Drop into editor."));
}

QTreeWidgetItem *MacrosTreeList::getSection(const QString &section,
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

bool MacrosTreeList::insertMacro(QTreeWidgetItem *parent,
                                 const QJsonObject &jsObj) {
  if (parent == nullptr)
    return false;

  QTreeWidgetItem *item = new QTreeWidgetItem(parent);
  item->setText(0, jsObj.value("title").toString());
  item->setData(0, Qt::UserRole, jsObj);
  item->setText(1, jsObj.value("key").toString());
  return true;
}

void MacrosTreeList::mousePressEvent(QMouseEvent *event) {
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

void MacrosTreeList::addMacro(const QString &section,
                              const QJsonObject &jsObj) {
  QString key = jsObj.value("key").toString();
  QTreeWidgetItem *parent = getSection(section, key);
  if (parent != nullptr)
    insertMacro(parent, jsObj);
}
