// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "pluginlistwidget.h"
#include "ainputwidget.h"

#include <QAbstractItemView>

PluginListWidgetItem::PluginListWidgetItem(const QString id,
                                           QListWidget *parent)
    : QListWidgetItem{parent, QListWidgetItem::UserType}, p_id{id} {}

void PluginListWidgetItem::setChecked(bool b) {
  setData(Qt::CheckStateRole, ((b) ? Qt::Checked : Qt::Unchecked));
}

bool PluginListWidgetItem::getChecked() {
  // enum Qt::CheckState
  return (data(Qt::CheckStateRole).toInt() == Qt::Checked);
}

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

  connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
          SLOT(switchItemState(QListWidgetItem *)));
}

PluginListWidgetItem *PluginListWidget::rowItem(int r) const {
  return static_cast<PluginListWidgetItem *>(item(r));
}

void PluginListWidget::switchItemState(QListWidgetItem *item) {
  PluginListWidgetItem *_item = static_cast<PluginListWidgetItem *>(item);
  if (_item->checkState() == Qt::Checked) {
    item->setData(Qt::CheckStateRole, Qt::Unchecked);
  } else {
    item->setData(Qt::CheckStateRole, Qt::Checked);
  }
}

void PluginListWidget::addListWidgetItem(const QJsonObject &jso) {
  QString _serial = jso.value("SerialId").toString().trimmed();
  QString _toolTip = jso.value("Description").toString();
  QString _display = jso.value("Title").toString();
  _display.append(" - " + _toolTip);

  // Prevent duplicates
  for (int r = 0; r < count(); r++) {
    if (item(r)->text() == _display)
      return;
  }

  PluginListWidgetItem *item = new PluginListWidgetItem(_serial, this);
  item->setData(Qt::DisplayRole, _display);
  item->setData(Qt::ToolTipRole, _toolTip);
  item->setData(Qt::CheckStateRole, Qt::Checked);
  item->setData(Qt::DecorationRole, AntiquaCRM::AntiquaApplIcon("bookmark"));
  item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsDragEnabled |
                 Qt::ItemIsEnabled | Qt::ItemNeverHasChildren |
                 Qt::ItemIsUserCheckable);

  insertItem(count(), item);
}

void PluginListWidget::setStatus(const QMap<QString, bool> &map) {
  for (int r = 0; r < count(); r++) {
    PluginListWidgetItem *_item = rowItem(r);
    _item->setChecked(map.value(_item->id()));
  }
}

const QMap<QString, bool> PluginListWidget::getStatus() {
  QMap<QString, bool> _m;
  for (int r = 0; r < count(); r++) {
    PluginListWidgetItem *_item = rowItem(r);
    _m.insert(_item->id(), _item->getChecked());
  }
  return _m;
}

const QMap<QString, int> PluginListWidget::getSort() {
  QMap<QString, int> _m;
  for (int r = 0; r < count(); r++) {
    _m.insert(rowItem(r)->id(), r);
  }
  return _m;
}
