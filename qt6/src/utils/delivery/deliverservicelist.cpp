// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "deliverservicelist.h"
#include "deliveritem.h"

#include <ASettings>
#include <AntiquaInput>
#include <QAction>
#include <QHeaderView>
#include <QJsonValue>
#include <QMenu>

DeliverServiceList::DeliverServiceList(QWidget *parent) : QTreeWidget{parent} {
  setObjectName("deliver_service_list");
  setRootIsDecorated(true);
  setSortingEnabled(true);
  setWordWrap(false);

  p_flags = (Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);

  AntiquaCRM::ASettings cfg(this);
  currency = cfg.value("payment/currency", "€").toString();

  QStringList _list;
  _list.append(tr("Service"));
  _list.append(tr("Price"));
  _list.append(tr("Packageclass"));
  _list.append(tr("Description"));
  setHeaderLabels(_list);

  header()->setSectionResizeMode(QHeaderView::ResizeToContents);

  connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
          SLOT(getDeliverServiceId(QTreeWidgetItem *, int)));
}

const QIcon DeliverServiceList::deliverIcon() const {
  return AntiquaCRM::antiquaIcon("folder-bookmark");
}

const QIcon DeliverServiceList::sectionIcon() const {
  return AntiquaCRM::antiquaIcon("bookmarks");
}

const QIcon DeliverServiceList::itemIcon() const {
  return AntiquaCRM::antiquaIcon("bookmarks");
}

const QString DeliverServiceList::priceDisplay(qreal price) {
  QString out(currency);
  out.prepend(" ");
  QString str = QString::number(price, 'f', 2);
  out.prepend(str);
  return out;
}

QTreeWidgetItem *DeliverServiceList::getParentDeliverService(int srv) {
  for (int t = 0; t < topLevelItemCount(); t++) {
    QTreeWidgetItem *item = topLevelItem(t);
    if (item != nullptr) {
      if ((item->data(0, Qt::UserRole).toInt() == srv)) {
        return item;
      }
    }
  }
  return nullptr;
}

void DeliverServiceList::createByContext() {
  int id = currentItem()->data(0, Qt::UserRole).toInt();
  if (id > 0)
    emit createDeliveryService(id);
}

void DeliverServiceList::removeByContext() {
  QJsonObject obj;
  QTreeWidgetItem *it = currentItem();
  obj.insert("d_srv", QJsonValue(it->data(0, Qt::UserRole).toInt()));
  obj.insert("d_class", QJsonValue(it->data(0, Qt::DisplayRole).toString()));
  obj.insert("d_price", QJsonValue(it->data(1, Qt::UserRole).toDouble()));
  obj.insert("d_definition", QJsonValue(it->data(2, Qt::UserRole).toString()));
  obj.insert("d_description", QJsonValue(it->data(3, Qt::UserRole).toString()));
  emit removeDeliveryPackage(obj);
}

void DeliverServiceList::getDeliverServiceId(QTreeWidgetItem *item,
                                             int column) {
  qDebug() << Q_FUNC_INFO << "TODO CHECK" << column;
  if (!item->flags().testFlag(Qt::ItemNeverHasChildren))
    return;

  if (item->childCount() != 0)
    return;

  QJsonObject obj;
  int id = item->data(0, Qt::UserRole).toInt();

  QString d_class = item->data(0, Qt::DisplayRole).toString();
  if (d_class.isEmpty()) {
    return;
  }
  obj.insert("d_class", QJsonValue(d_class));

  QString d_definition = item->data(2, Qt::DisplayRole).toString();
  if (d_definition.isEmpty()) {
    return;
  }
  obj.insert("d_definition", QJsonValue(d_definition));

  QString d_description = item->data(3, Qt::DisplayRole).toString();
  if (d_description.isEmpty()) {
    return;
  }
  obj.insert("d_description", QJsonValue(d_description));

  emit deliverPackageClicked(id, obj);
}

void DeliverServiceList::contextMenuEvent(QContextMenuEvent *ev) {
  QModelIndex p_modelIndex = indexAt(ev->pos());
  if (p_modelIndex.isValid()) {
    QTreeWidgetItem *ti = currentItem();
    QMenu *m = new QMenu(this);
    QAction *ac_create =
        m->addAction(QIcon(":icons/db_add.png"), tr("Create entry"));
    ac_create->setEnabled(ti->type() == QTreeWidgetItem::UserType);
    connect(ac_create, SIGNAL(triggered()), this, SLOT(createByContext()));
    QAction *ac_remove =
        m->addAction(QIcon(":icons/db_remove.png"), tr("Remove entry"));
    ac_remove->setEnabled(ti->flags().testFlag(Qt::ItemNeverHasChildren));
    connect(ac_remove, SIGNAL(triggered()), this, SLOT(removeByContext()));
    m->exec(ev->globalPos());
    delete m;
  }
}

void DeliverServiceList::addDeliverService(int id, const QString &name) {
  QTreeWidgetItem *p = new QTreeWidgetItem(this, QTreeWidgetItem::UserType);
  p->setData(0, Qt::UserRole, id);
  p->setData(0, Qt::DisplayRole, name);
  p->setData(0, Qt::DecorationRole, deliverIcon());
  p->setFlags(p_flags ^ Qt::ItemIsSelectable);
  addTopLevelItem(p);

  QTreeWidgetItem *s1 = new QTreeWidgetItem(p, QTreeWidgetItem::Type);
  s1->setData(0, Qt::UserRole, true);
  s1->setData(0, Qt::DisplayRole, tr("International"));
  s1->setData(0, Qt::DecorationRole, sectionIcon());
  s1->setFlags(p_flags);
  p->addChild(s1);

  QTreeWidgetItem *s2 = new QTreeWidgetItem(p, QTreeWidgetItem::Type);
  s2->setData(0, Qt::UserRole, false);
  s2->setData(0, Qt::DisplayRole, tr("National"));
  s2->setData(0, Qt::DecorationRole, sectionIcon());
  s2->setFlags(p_flags);
  p->addChild(s2);

  emit deliverServiceChanged(id);
}

void DeliverServiceList::addDeliverPackage(int srv, const DeliverItem &info) {

  QTreeWidgetItem *parent = getParentDeliverService(srv);
  if (parent == nullptr)
    return;

  DeliverItem di(info);
  bool type = di.international();
  for (int c = 0; c < parent->childCount(); c++) {
    QTreeWidgetItem *cItem = parent->child(c);
    if (cItem != nullptr && cItem->data(0, Qt::UserRole).toBool() == type) {
      QTreeWidgetItem *item = new QTreeWidgetItem(cItem, QTreeWidgetItem::Type);
      item->setData(0, Qt::UserRole, srv);
      item->setData(0, Qt::DisplayRole, di.name());
      item->setData(0, Qt::DecorationRole, itemIcon());
      item->setData(1, Qt::UserRole, di.price());
      item->setData(1, Qt::DisplayRole, priceDisplay(di.price()));
      item->setData(2, Qt::UserRole, di.definition());
      item->setData(2, Qt::DisplayRole, di.definition());
      item->setData(3, Qt::UserRole, di.description());
      item->setData(3, Qt::DisplayRole, di.description());
      item->setFlags(p_flags | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren);
      cItem->addChild(item);
    }
  }
}
