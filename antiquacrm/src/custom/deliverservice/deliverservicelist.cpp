// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "deliverservicelist.h"
#include "applsettings.h"
#include "deliveritem.h"
#include "myicontheme.h"

#include <QDebug>
#include <QHeaderView>

DeliverServiceList::DeliverServiceList(QWidget *parent) : QTreeWidget{parent} {
  setObjectName("deliver_service_list");
  setRootIsDecorated(true);
  setSortingEnabled(true);
  setWordWrap(false);

  ApplSettings cfg;
  currency = cfg.value("payment/currency", "€").toString();

  QStringList h({tr("Service"), tr("Price"), tr("Definition"), tr("Package")});
  setHeaderLabels(h);
  header()->setSectionResizeMode(QHeaderView::ResizeToContents);

  connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
          SLOT(getDeliverServiceId(QTreeWidgetItem *, int)));
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
      return (item->data(0, Qt::UserRole).toInt() == srv) ? item : nullptr;
    }
  }
  return nullptr;
}

void DeliverServiceList::getDeliverServiceId(QTreeWidgetItem *item,
                                             int column) {
  if (item->childCount() == 0) {
    int id = item->data(0, Qt::UserRole).toInt();
    QString n = item->data(0, Qt::DisplayRole).toString();
    emit deliverPackageClicked(id, n);
  }
}

void DeliverServiceList::addDeliverService(int id, const QString &name) {
  QTreeWidgetItem *parent = new QTreeWidgetItem(this);
  parent->setData(0, Qt::UserRole, id);
  parent->setData(0, Qt::DisplayRole, name);
  parent->setData(0, Qt::DecorationRole, myIcon("autostart"));
  addTopLevelItem(parent);

  QTreeWidgetItem *s1 = new QTreeWidgetItem(parent);
  s1->setData(0, Qt::UserRole, true);
  s1->setData(0, Qt::DisplayRole, tr("International"));
  s1->setData(0, Qt::DecorationRole, myIcon("autostart"));
  parent->addChild(s1);

  QTreeWidgetItem *s2 = new QTreeWidgetItem(parent);
  s2->setData(0, Qt::UserRole, false);
  s2->setData(0, Qt::DisplayRole, tr("National"));
  s2->setData(0, Qt::DecorationRole, myIcon("autostart"));
  parent->addChild(s2);

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
      QTreeWidgetItem *item = new QTreeWidgetItem(cItem);
      item->setData(0, Qt::UserRole, srv);
      item->setData(0, Qt::DisplayRole, di.name());
      item->setData(0, Qt::DecorationRole, myIcon("autostart"));
      item->setData(1, Qt::UserRole, di.price());
      item->setData(1, Qt::DisplayRole, priceDisplay(di.price()));
      item->setData(2, Qt::DisplayRole, di.definition());
      item->setData(3, Qt::DisplayRole, di.description());
      cItem->addChild(item);
    }
  }
}
