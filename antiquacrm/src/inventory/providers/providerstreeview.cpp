// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerstreeview.h"
#include "myicontheme.h"

ProvidersTreeView::ProvidersTreeView(QWidget *parent) : QTreeWidget{parent} {
  setColumnCount(1);
  setItemsExpandable(true);
  setSortingEnabled(false);
  setWordWrap(false);
  setMaximumWidth(250);

  connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
          SLOT(itemSelected(QTreeWidgetItem *, int)));
}

void ProvidersTreeView::itemSelected(QTreeWidgetItem *item, int) {
  if (item->type() != QTreeWidgetItem::UserType)
    return;

  QString provider = item->parent()->text(0);
  if (!item->text(0).isEmpty() && !provider.isEmpty()) {
    emit s_queryOrder(provider, item->text(0));
  }
}

void ProvidersTreeView::setTreeViewHeaders() {
  QTreeWidgetItem *hItem = headerItem();
  hItem->setText(0, tr("Orders"));
  hItem->setIcon(0, myIcon("autostart"));
}

void ProvidersTreeView::addOrder(const QString &provider, const QString &id) {
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
      p->addChild(item);
    }
  }
  resizeColumnToContents(0);
}

void ProvidersTreeView::addProviders(const QStringList &list) {
  foreach (QString t, list) {
    QTreeWidgetItem *item = new QTreeWidgetItem(this, QTreeWidgetItem::Type);
    item->setText(0, t);
    item->setIcon(0, myIcon("autostart"));
    item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
    item->setExpanded(true);
    addTopLevelItem(item);
  }
  resizeColumnToContents(0);
}
