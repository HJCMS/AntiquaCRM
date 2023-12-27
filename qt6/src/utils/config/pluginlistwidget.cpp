// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "pluginlistwidget.h"
#include "pluginlistwidgetitem.h"

#include <QAbstractItemView>
#include <QListWidgetItem>

PluginListWidget::PluginListWidget(QWidget *parent) : QListWidget{parent} {
  setSpacing(10);
  setFlow(QListView::TopToBottom);
  setResizeMode(QListView::Adjust);
  setSelectionMode(QAbstractItemView::SingleSelection);
  setWrapping(false);
  setDragEnabled(true);
  setAcceptDrops(true);
  setDragDropMode(QAbstractItemView::InternalMove);
  setMovement(QListView::Snap);
  setSortingEnabled(false);
  connect(this, SIGNAL(doubleClicked(const QModelIndex &)),
          SLOT(changeState(const QModelIndex &)));
}

const QString PluginListWidget::itemTip() const {
  return tr("set item checked, "
            "if you want to view this tab on application start.");
}

PluginListWidgetItem *PluginListWidget::rowItem(int r) {
  return static_cast<PluginListWidgetItem *>(item(r));
}

void PluginListWidget::changeState(const QModelIndex &index) {
  rowItem(index.row())->toggleState();
}

void PluginListWidget::addListItem(const QJsonObject &meta) {
  // Prevent duplicates
  const QString _id = meta.value("SerialId").toString();
  for (int r = 0; r < count(); r++) {
    if (rowItem(r)->option("SerialId").toString() == _id)
      return;
  }

  PluginListWidgetItem *item = new PluginListWidgetItem(meta, this);
  item->setData(Qt::ToolTipRole, itemTip());
  addItem(item);
}

bool PluginListWidget::setStatus(const QMap<QString, bool> &map) {
  if (map.size() < 0 || count() < 1)
    return false; // nothing todo

  for (int r = 0; r < count(); r++) {
    PluginListWidgetItem *_item = rowItem(r);
    _item->setChecked(map.value(_item->option("SerialId").toString()));
  }
  return true;
}

const QMap<QString, bool> PluginListWidget::getStatus() {
  QMap<QString, bool> _m;
  for (int r = 0; r < count(); r++) {
    PluginListWidgetItem *_item = rowItem(r);
    _m.insert(_item->option("SerialId").toString(), _item->getChecked());
  }
  return _m;
}

const QMap<int, QString> PluginListWidget::getSort() {
  QMap<int, QString> _m;
  for (int r = 0; r < count(); r++) {
    _m.insert(r, rowItem(r)->option("SerialId").toString());
  }
  return _m;
}
