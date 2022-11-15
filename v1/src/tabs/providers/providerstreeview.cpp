// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerstreeview.h"

#include <AntiquaCRM>
#include <QDate>
#include <QDebug>
#include <QIcon>

ProvidersTreeView::ProvidersTreeView(QWidget *parent) : QTreeWidget{parent} {
  setColumnCount(3);
  setItemsExpandable(true);
  setSortingEnabled(false);
  setWordWrap(false);
  setAlternatingRowColors(true);

  QTreeWidgetItem *aItem = headerItem();
  aItem->setText(0, tr("Orders"));
  aItem->setIcon(0, QIcon("://icons/user_group.png"));
  QTreeWidgetItem *bItem = headerItem();
  bItem->setText(1, tr("Date"));
  bItem->setIcon(1, QIcon("://icons/user_group.png"));
  QTreeWidgetItem *cItem = headerItem();
  cItem->setText(2, tr("Buyer"));
  cItem->setIcon(2, QIcon("://icons/user_group.png"));

  connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
          SLOT(itemSelected(QTreeWidgetItem *, int)));
}

const QString ProvidersTreeView::setDateString(const QDateTime &dt) const {
  QString str = tr("Today");
  if (dt.date() != QDate::currentDate()) {
    str = dt.toString("dd.MM.yy");
  }
  return str;
}

QTreeWidgetItem *ProvidersTreeView::getParent(const QString &name) {
  for (int i = 0; i < topLevelItemCount(); i++) {
    QTreeWidgetItem *parent = topLevelItem(i);
    if (parent != nullptr && parent->type() == QTreeWidgetItem::Type) {
      if (parent->text(0) == name)
        return parent;
    }
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

void ProvidersTreeView::addProvider(const QString &provider) {
  QTreeWidgetItem *parent = getParent(provider);
  if (parent != nullptr) {
    parent->setSelected(true);
    return;
  }

  QFont fi = font();
  fi.setItalic(true);

  QTreeWidgetItem *item = new QTreeWidgetItem(this, QTreeWidgetItem::Type);
  item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator);
  item->setFlags(Qt::ItemIsEnabled);
  item->setText(0, provider);
  item->setText(1, tr("Status"));
  item->setFont(1, fi);
  item->setExpanded(true);
  addTopLevelItem(item);
  resizeColumnToContents(0);
}

void ProvidersTreeView::itemSelected(QTreeWidgetItem *item, int) {
  if (item->type() != QTreeWidgetItem::UserType)
    return;

  qDebug() << Q_FUNC_INFO << item->text(0) << item->type();

  if (item->flags() == Qt::NoItemFlags)
    return;

  QString provider = item->parent()->text(0);
  if (!item->text(0).isEmpty() && !provider.isEmpty()) {
    emit sendQueryOrder(provider, item->text(0));
  }
}

void ProvidersTreeView::updateOrderStatus(QTreeWidgetItem *item, int status) {
  QString tip = item->toolTip(0);
  bool modified = item->data(1, Qt::UserRole).toBool();

  switch (static_cast<AntiquaCRM::OrderStatus>(status)) {
  case (AntiquaCRM::OrderStatus::OPEN):
    return; /**< Nothing todo */

  case (AntiquaCRM::OrderStatus::STARTED): {
    item->setSelected(false);
    QString mTip = tr("Created");
    if (modified != true || !tip.contains(mTip))
      item->setToolTip(0, tip + " " + mTip);

    item->setIcon(1, QIcon("://icons/edit.png"));
    item->setData(1, Qt::UserRole, true); // setModified
    return;
  }

  case (AntiquaCRM::OrderStatus::FETCHET): {
    item->setSelected(false);
    QString mTip = tr("Fetchet");
    if (modified != true || !tip.contains(mTip))
      item->setToolTip(0, tip + " " + mTip);

    item->setIcon(1, QIcon("://icons/action_delivered.png"));
    item->setData(1, Qt::UserRole, true); // setModified
    return;
  }

  case (AntiquaCRM::OrderStatus::DELIVERED): {
    item->setSelected(false);
    QString mTip = tr("Delivered");
    if (modified != true || !tip.contains(mTip))
      item->setToolTip(0, tip + " " + mTip);

    item->setIcon(1, QIcon("://icons/action_ok.png"));
    item->setData(1, Qt::UserRole, true); // setModified
    return;
  }

  case (AntiquaCRM::OrderStatus::REMINDET): {
    item->setSelected(false);
    QString mTip = tr("Remindet");
    if (modified != true || !tip.contains(mTip))
      item->setToolTip(0, tip + " " + mTip);

    item->setIcon(1, QIcon("://icons/action_redo.png"));
    item->setData(1, Qt::UserRole, true); // setModified
    return;
  }

  case (AntiquaCRM::OrderStatus::COMPLETED): {
    item->setSelected(false);
    item->setIcon(1, QIcon("://icons/action_ok.png"));

    QString mTip = tr("Finished");
    if (modified != true || !tip.contains(mTip))
      item->setToolTip(0, tip + " " + mTip);

    item->setData(1, Qt::UserRole, true); // setModified
    return;
  }

  case (AntiquaCRM::OrderStatus::CANCELED): {
    item->setSelected(false);
    QString mTip = tr("Canceled");
    if (modified != true || !tip.contains(mTip))
      item->setToolTip(0, tip + " " + mTip);

    item->setIcon(1, QIcon("://icons/action_cancel.png"));
    item->setData(1, Qt::UserRole, true); // setModified
    return;
  }

  case (AntiquaCRM::OrderStatus::RETURNING): {
    item->setSelected(false);
    QString mTip = tr("Returning");
    if (modified != true || !tip.contains(mTip))
      item->setToolTip(0, tip + " " + mTip);

    item->setIcon(1, QIcon("://icons/action_undo.png"));
    item->setData(1, Qt::UserRole, true); // setModified
    return;
  }

  default:
    return;
  };
}

void ProvidersTreeView::clearProvider(const QString &provider) {
  QTreeWidgetItem *p = getParent(provider);
  if (p != nullptr) {
    for (int i = 0; i < p->childCount(); i++) {
      QTreeWidgetItem *item = p->child(i);
      p->removeChild(item);
    }
  }
}

void ProvidersTreeView::addOrder(const QString &pro,
                                 const TreeOrderItem &data) {
  if (exists(pro, data.id))
    return;

  QFont font(ProvidersTreeView::font());
  font.setItalic(true);

  QTreeWidgetItem *p = getParent(pro);
  if (p != nullptr) {
    QTreeWidgetItem *item = new QTreeWidgetItem(p, QTreeWidgetItem::UserType);
    item->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled |
                   Qt::ItemNeverHasChildren);

    QDateTime dt(data.datetime);
    QString tip(data.buyer + "\n" + dt.toString("ddd dd. MMMM yyyy hh:mm"));
    item->setText(0, data.id);
    item->setFont(0, font);
    item->setIcon(0, QIcon("://icons/groups.png"));
    item->setToolTip(0, tip.trimmed());
    item->setText(1, setDateString(dt));
    item->setIcon(1, QIcon("://icons/warning.png"));
    item->setToolTip(1, tip.trimmed());
    item->setText(2, data.buyer);
    updateOrderStatus(item, data.status);
    p->addChild(item);
  }
}

void ProvidersTreeView::loadUpdate() {
  if (topLevelItemCount() > 0) {
    for (int t = 0; t < topLevelItemCount(); t++) {
      clearProvider(topLevelItem(t)->text(0));
    }
  }

  AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
  QString sql =
      AntiquaCRM::ASqlFiles::queryStatement("query_provider_order_history");
  QSqlQuery q = m_sql->query(sql);
  int count = 0;
  if (q.size() > 0) {
    while (q.next()) {
      QString provider = q.value("pr_name").toString();
      addProvider(provider);
      QString id = q.value("pr_order").toString();
      int status = q.value("pr_status").toInt();
      count++;

      if (exists(provider, id)) {
        updateItemStatus(provider, id, status);
        continue;
      }

      QString d_time = q.value("pr_datetime").toString();
      TreeOrderItem data;
      data.id = id;
      data.datetime = QDateTime::fromString(d_time, Qt::ISODate);
      data.buyer = q.value("pr_buyer").toString();
      data.status = status;
      addOrder(provider, data);
    }
  } else if (!m_sql->lastError().isEmpty()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
#endif
    return;
  }
#ifdef ANTIQUA_DEVELOPEMENT
  qInfo("Providers Tree Refresh with %d entries.", count);
#endif

  if (count > 0)
    sortAndResize();
}

bool ProvidersTreeView::exists(const QString &provider, const QString &id) {
  if (provider.isEmpty() || id.isEmpty())
    return true; // skip empty insert requests

  return (getChild(provider, id) != nullptr);
}

void ProvidersTreeView::updateItemStatus(const QString &provider,
                                         const QString &orderId, int status) {
  QTreeWidgetItem *parent = getParent(provider);
  if (parent == nullptr)
    return;

  for (int i = 0; i < parent->childCount(); i++) {
    QTreeWidgetItem *item = parent->child(i);
    if (item != nullptr && item->text(0) == orderId) {
      updateOrderStatus(item, status);
    }
  }
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

void ProvidersTreeView::sortAndResize() {
  for (int i = 0; i < columnCount(); i++) {
    resizeColumnToContents(i);
  }

  if (topLevelItemCount() > 0) {
    for (int t = 0; t < topLevelItemCount(); t++) {
      topLevelItem(t)->sortChildren(0, Qt::AscendingOrder);
    }
  }
}

int ProvidersTreeView::ordersCount() {
  int c = 0;
  for (int t = 0; t < topLevelItemCount(); t++) {
    c += topLevelItem(t)->childCount();
  }
  return c;
}
