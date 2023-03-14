// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providerstreeview.h"

#include <QDate>
#include <QDebug>
#include <QIcon>

ProvidersTreeView::ProvidersTreeView(QWidget *parent) : QTreeWidget{parent} {
  setColumnCount(3);
  setItemsExpandable(true);
  setSortingEnabled(false);
  setWordWrap(false);
  setAlternatingRowColors(true);

  QTreeWidgetItem *m_header = headerItem();
  m_header->setText(0, tr("Orders"));
  m_header->setIcon(0, QIcon("://icons/user_group.png"));
  m_header->setText(1, tr("Date"));
  m_header->setIcon(1, QIcon("://icons/user_group.png"));
  m_header->setText(2, tr("Buyer"));
  m_header->setIcon(2, QIcon("://icons/user_group.png"));

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

  if (item->flags() == Qt::NoItemFlags)
    return;

  QString provider = item->parent()->text(0);
  if (!item->text(0).isEmpty() && !provider.isEmpty()) {
    emit sendQueryOrder(provider, item->text(0));
  }
}

void ProvidersTreeView::updateOrderStatus(QTreeWidgetItem *item,
                                          AntiquaCRM::OrderStatus status) {
  QString tip = item->toolTip(0);
  bool modified = item->data(1, Qt::UserRole).toBool();

  switch (status) {
  case (AntiquaCRM::OrderStatus::OPEN):
    return; /**< Nothing todo */

  case (AntiquaCRM::OrderStatus::STARTED): {
    QString mTip = tr("Created");
    if (modified != true || !tip.contains(mTip))
      item->setToolTip(0, tip + " " + mTip);

    item->setIcon(1, QIcon("://icons/edit.png"));
    item->setData(1, Qt::UserRole, true); // setModified
    return;
  }

  case (AntiquaCRM::OrderStatus::FETCHET): {
    QString mTip = tr("Fetchet");
    if (modified != true || !tip.contains(mTip))
      item->setToolTip(0, tip + " " + mTip);

    item->setIcon(1, QIcon("://icons/action_ready_pickup.png"));
    item->setData(1, Qt::UserRole, true); // setModified
    return;
  }

  case (AntiquaCRM::OrderStatus::DELIVERY): {
    QString mTip = tr("Delivery");
    if (modified != true || !tip.contains(mTip))
      item->setToolTip(0, tip + " " + mTip);

    item->setIcon(1, QIcon("://icons/delivery48.png"));
    item->setData(1, Qt::UserRole, true); // setModified
    return;
  }

  case (AntiquaCRM::OrderStatus::DELIVERED): {
    QString mTip = tr("Delivered");
    if (modified != true || !tip.contains(mTip))
      item->setToolTip(0, tip + " " + mTip);

    item->setIcon(1, QIcon("://icons/action_ok.png"));
    item->setData(1, Qt::UserRole, true); // setModified
    return;
  }

  case (AntiquaCRM::OrderStatus::CANCELED): {
    QString mTip = tr("Canceled");
    if (modified != true || !tip.contains(mTip))
      item->setToolTip(0, tip + " " + mTip);

    item->setIcon(1, QIcon("://icons/action_cancel.png"));
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

void ProvidersTreeView::updateItemStatus(const QString &provider,
                                         const QString &orderId,
                                         AntiquaCRM::OrderStatus status) {
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

void ProvidersTreeView::loadUpdate() {
  // Untermenü-Einträge entfernen!
  if (topLevelItemCount() > 0) {
    for (int t = 0; t < topLevelItemCount(); t++) {
      clearProvider(topLevelItem(t)->text(0));
    }
  }

  QString table("query_provider_orders");
  AntiquaCRM::ASqlCore *m_sql = new AntiquaCRM::ASqlCore(this);
  QString sql = AntiquaCRM::ASqlFiles::queryStatement(table);
  QSqlQuery q = m_sql->query(sql);
  int count = 0;
  if (q.size() > 0) {
    while (q.next()) {
      QString provider = q.value("order_provider").toString();
      addProvider(provider);
      QString id = q.value("order_number").toString();
      AntiquaCRM::OrderStatus status =
          static_cast<AntiquaCRM::OrderStatus>(q.value("order_status").toInt());
      count++;

      if (exists(provider, id)) {
        updateItemStatus(provider, id, status);
        continue;
      }
      //  TODO order_comment
      QString d_time = q.value("order_datetime").toString();
      TreeOrderItem data;
      data.id = id;
      data.datetime = QDateTime::fromString(d_time, Qt::ISODate);
      data.buyer = q.value("order_buyername").toString();
      data.status = status;
      addOrder(provider, data);
    }
  } else if (!m_sql->lastError().isEmpty()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
#endif
    return;
  }

  if (count > 0)
    sortAndResize();
}

bool ProvidersTreeView::exists(const QString &provider, const QString &id) {
  if (provider.isEmpty() || id.isEmpty())
    return true; // skip empty insert requests

  return (getChild(provider, id) != nullptr);
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
  // Erst mal alles Ausgewählte aufheben!
  if (selectedItems().size() > 0) {
    QListIterator<QTreeWidgetItem *> selections(selectedItems());
    while (selections.hasNext()) {
      selections.next()->setSelected(false);
    }
  }

  // Spaltenbreiten anpassen
  for (int i = 0; i < columnCount(); i++) {
    resizeColumnToContents(i);
  }

  // Jetzt nach Bestellnummer sortieren!
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
