// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerstreeview.h"
#include "myicontheme.h"

#include <QDebug>

ProvidersTreeView::ProvidersTreeView(QWidget *parent) : QTreeWidget{parent} {
  setColumnCount(1);
  setItemsExpandable(true);
  setSortingEnabled(false);
  setWordWrap(false);
  setMaximumWidth(250);

  connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
          SLOT(itemSelected(QTreeWidgetItem *, int)));
}

QTreeWidgetItem *ProvidersTreeView::getParent(const QString &name) {
  for (int i = 0; i < topLevelItemCount(); i++) {
    QTreeWidgetItem *parent = topLevelItem(i);
    if (parent != nullptr && parent->type() == QTreeWidgetItem::Type)
      return parent;
  }
  return nullptr;
}

QTreeWidgetItem *ProvidersTreeView::getChild(const QString &provider,
                                             const QString &id) {
  QTreeWidgetItem *parent = getParent(provider);
  if (parent == nullptr)
    return nullptr;

  for (int i = 0; i < parent->childCount(); i++) {
    QTreeWidgetItem *item = parent->child(i);
    if (item != nullptr && item->text(0) == id)
      return item;
  }
  return nullptr;
}

void ProvidersTreeView::itemSelected(QTreeWidgetItem *item, int) {
  if (item->type() != QTreeWidgetItem::UserType) {
    emit s_queryProvider(item->text(0));
    return;
  }

  QString provider = item->parent()->text(0);
  if (!item->text(0).isEmpty() && !provider.isEmpty()) {
    emit s_queryOrder(provider, item->text(0));
  }
}

bool ProvidersTreeView::exists(const QString &provider, const QString &id) {
  return (getChild(provider, id) != nullptr);
}

void ProvidersTreeView::setTreeViewHeaders() {
  QTreeWidgetItem *hItem = headerItem();
  hItem->setText(0, tr("Orders"));
  hItem->setIcon(0, myIcon("autostart"));
}

void ProvidersTreeView::addOrder(const QString &provider, const QString &id,
                                 const QDateTime &dt) {
  if (exists(provider, id))
    return;

  QFont font(ProvidersTreeView::font());
  font.setItalic(true);
  QList<QTreeWidgetItem *> l = findItems(provider, Qt::MatchFixedString, 0);
  for (int i = 0; i < l.count(); i++) {
    QTreeWidgetItem *p = l.at(i);
    if (p != nullptr) {
      QTreeWidgetItem *item = new QTreeWidgetItem(p, QTreeWidgetItem::UserType);
      item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
      item->setText(0, id);
      item->setFont(0, font);
      item->setIcon(0, myIcon("group"));
      item->setToolTip(0, dt.toString("ddd dd. MMMM yyyy"));
      p->addChild(item);
    }
  }
  resizeColumnToContents(0);
}

void ProvidersTreeView::removeOrder(const QString &provider,
                                    const QString &id) {
  if (exists(provider, id))
    return;

  QTreeWidgetItem *c = getChild(provider, id);
  if (c == nullptr)
    return;

  QTreeWidgetItem *p = c->parent();
  p->removeChild(c);
  resizeColumnToContents(0);
}

void ProvidersTreeView::addProvider(const QString &provider) {
  QTreeWidgetItem *parent = getParent(provider);
  if (parent != nullptr) {
    parent->setSelected(true);
    return;
  }

  QTreeWidgetItem *item = new QTreeWidgetItem(this, QTreeWidgetItem::Type);
  item->setText(0, provider);
  item->setIcon(0, myIcon("autostart"));
  item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
  item->setExpanded(true);
  addTopLevelItem(item);
  resizeColumnToContents(0);
}
