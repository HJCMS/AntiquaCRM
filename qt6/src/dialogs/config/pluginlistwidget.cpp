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
  setSortingEnabled(false);

  connect(this, SIGNAL(itemDoubleClicked(QListWidgetItem *)),
          SLOT(switchItemState(QListWidgetItem *)));
}

PluginListWidgetItem *PluginListWidget::rowItem(int r) {
  return static_cast<PluginListWidgetItem *>(item(r));
}

PluginListWidgetItem *PluginListWidget::removeItem(int r) {
  QListWidgetItem *_item = takeItem(r);
  if (_item == nullptr) {
    qWarning("Unknown Plugin ListItem");
    return nullptr;
  }
  return static_cast<PluginListWidgetItem *>(_item);
}

void PluginListWidget::switchItemState(QListWidgetItem *item) {
  PluginListWidgetItem *_item = static_cast<PluginListWidgetItem *>(item);
  if (_item->checkState() == Qt::Checked) {
    item->setData(Qt::CheckStateRole, Qt::Unchecked);
  } else {
    item->setData(Qt::CheckStateRole, Qt::Checked);
  }
}

void PluginListWidget::addListItem(const QJsonObject &jso) {
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

  addItem(item);
}

bool PluginListWidget::setStatus(const QMap<QString, bool> &map) {
  if (map.size() < 0 || count() < 1)
    return false; // nothing todo

  for (int r = 0; r < count(); r++) {
    PluginListWidgetItem *_item = rowItem(r);
    _item->setChecked(map.value(_item->id()));
  }
  return true;
}

const QMap<QString, bool> PluginListWidget::getStatus() {
  QMap<QString, bool> _m;
  for (int r = 0; r < count(); r++) {
    PluginListWidgetItem *_item = rowItem(r);
    _m.insert(_item->id(), _item->getChecked());
  }
  return _m;
}

const QMap<int, QString> PluginListWidget::getSort() {
  QMap<int, QString> _m;
  for (int r = 0; r < count(); r++) {
    _m.insert(r, rowItem(r)->id());
  }
  return _m;
}
