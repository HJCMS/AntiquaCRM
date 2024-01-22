// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sellerssaleslist.h"
#include "sellerimportrepair.h"

#include <QAction>
#include <QApplication>
#include <QClipboard>
#include <QDate>
#include <QDebug>
#include <QIcon>
#include <QMenu>

SellersSalesList::SellersSalesList(QWidget *parent) : QTreeWidget{parent} {
  setColumnCount(3);
  setItemsExpandable(true);
  setSortingEnabled(false);
  setWordWrap(false);
  setAlternatingRowColors(true);

  QTreeWidgetItem *m_header = headerItem();
  m_header->setText(0, tr("Orders"));
  m_header->setIcon(0, AntiquaCRM::antiquaIcon("view-task-child"));
  m_header->setText(1, tr("Date"));
  m_header->setIcon(1, AntiquaCRM::antiquaIcon("chronometer"));
  m_header->setText(2, tr("Buyer"));
  m_header->setIcon(2, AntiquaCRM::antiquaIcon("system-users"));

  connect(this, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this,
          SLOT(itemSelected(QTreeWidgetItem *, int)));
}

void SellersSalesList::contextMenuEvent(QContextMenuEvent *event) {
  QModelIndex _index = indexAt(event->pos());
  if (!_index.isValid())
    return;

  QTreeWidgetItem *_item = itemFromIndex(_index);
  if (_item == nullptr || _item->parent() == nullptr)
    return;

  QString _id = _item->text(0);
  if (_id.length() < 3)
    return;

  p_pair.first = _item->parent()->text(0);
  p_pair.second = _id;

  QMenu *m = new QMenu("Actions", this);
  QAction *ac_update = m->addAction(AntiquaCRM::antiquaIcon("view-refresh"),
                                    tr("Update orders tree"));
  connect(ac_update, SIGNAL(triggered()), SLOT(loadUpdate()));

  QAction *ac_copy = m->addAction(AntiquaCRM::antiquaIcon("edit-paste"),
                                  tr("Copy Ordering Id"));
  connect(ac_copy, SIGNAL(triggered()), SLOT(copyProviderId()));

  QAction *ac_edit = m->addAction(AntiquaCRM::antiquaIcon("document-edit"),
                                  tr("Repair Customer data"));
  connect(ac_edit, SIGNAL(triggered()), SLOT(editProviderData()));

  QAction *ac_ignore = m->addAction(AntiquaCRM::antiquaIcon("dialog-cancel"),
                                  tr("Order canceled by Client"));
  connect(ac_ignore, SIGNAL(triggered()), SLOT(ignoreOrder()));

  m->exec(event->globalPos());
  m->deleteLater();
}

const QString SellersSalesList::setDateString(const QDateTime &dt) const {
  QString str = tr("Today");
  if (dt.date() != QDate::currentDate()) {
    str = dt.toString("dd.MM.yy");
  }
  return str;
}

QTreeWidgetItem *SellersSalesList::getParent(const QString &name) {
  for (int i = 0; i < topLevelItemCount(); i++) {
    QTreeWidgetItem *parent = topLevelItem(i);
    if (parent != nullptr && parent->type() == QTreeWidgetItem::Type) {
      if (parent->text(0) == name)
        return parent;
    }
  }
  return nullptr;
}

QTreeWidgetItem *SellersSalesList::getChild(const QString &provider,
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

void SellersSalesList::addProvider(const QString &provider) {
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

void SellersSalesList::itemSelected(QTreeWidgetItem *item, int) {
  if (item->type() != QTreeWidgetItem::UserType)
    return;

  if (item->flags() == Qt::NoItemFlags)
    return;

  QString provider = item->parent()->text(0);
  if (!item->text(0).isEmpty() && !provider.isEmpty()) {
    emit sendQueryOrder(provider, item->text(0));
  }
}

void SellersSalesList::updateOrderStatus(QTreeWidgetItem *item,
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

    item->setIcon(1, AntiquaCRM::antiquaIcon("action-edit-document"));
    item->setData(1, Qt::UserRole, true); // setModified
    return;
  }

  case (AntiquaCRM::OrderStatus::FETCHET): {
    QString mTip = tr("Fetchet");
    if (modified != true || !tip.contains(mTip))
      item->setToolTip(0, tip + " " + mTip);

    item->setIcon(1, AntiquaCRM::qrcIcon("package-created"));
    item->setData(1, Qt::UserRole, true); // setModified
    return;
  }

  case (AntiquaCRM::OrderStatus::DELIVERY): {
    QString mTip = tr("Delivery");
    if (modified != true || !tip.contains(mTip))
      item->setToolTip(0, tip + " " + mTip);

    item->setIcon(1, AntiquaCRM::qrcIcon("package-deliver"));
    item->setData(1, Qt::UserRole, true); // setModified
    return;
  }

  case (AntiquaCRM::OrderStatus::DELIVERED): {
    QString mTip = tr("Delivered");
    if (modified != true || !tip.contains(mTip))
      item->setToolTip(0, tip + " " + mTip);

    item->setIcon(1, AntiquaCRM::antiquaIcon("package-delivered"));
    item->setData(1, Qt::UserRole, true); // setModified
    return;
  }

  case (AntiquaCRM::OrderStatus::CANCELED): {
    QString mTip = tr("Canceled");
    if (modified != true || !tip.contains(mTip))
      item->setToolTip(0, tip + " " + mTip);

    item->setIcon(1, AntiquaCRM::antiquaIcon("dialog-cancel"));
    item->setData(1, Qt::UserRole, true); // setModified
    return;
  }

  default:
    return;
  };
}

void SellersSalesList::clearProvider(const QString &provider) {
  QTreeWidgetItem *p = getParent(provider);
  if (p != nullptr) {
    for (int i = 0; i < p->childCount(); i++) {
      QTreeWidgetItem *item = p->child(i);
      p->removeChild(item);
    }
  }
}

void SellersSalesList::addOrder(const QString &pro, const TreeOrderItem &data) {
  if (exists(pro, data.id))
    return;

  QFont font(SellersSalesList::font());
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
    item->setToolTip(0, tip.trimmed());
    item->setText(1, setDateString(dt));
    item->setIcon(1, AntiquaCRM::antiquaIcon("dialog-warning"));
    item->setToolTip(1, tip.trimmed());
    item->setText(2, data.buyer);
    updateOrderStatus(item, data.status);
    p->addChild(item);
  }
}

void SellersSalesList::updateItemStatus(const QString &provider,
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

void SellersSalesList::editProviderData() {
  if (p_pair.second.length() < 3)
    return;

  SellerImportRepair *d =
      new SellerImportRepair(p_pair.first, p_pair.second, this);
  d->exec();
  d->deleteLater();
}

void SellersSalesList::ignoreOrder() {
  if (p_pair.second.length() < 3 || p_pair.first.length() < 3)
    return;

  const QString _ask = tr("<p>This action is not reversible.</p>"
                          "<b>Do you really want that?</b>");
  if (QMessageBox::question(this, tr("Hide order"), _ask) == QMessageBox::No)
    return;

  QString _sql("UPDATE provider_orders SET pr_ignore=true WHERE ");
  _sql.append("pr_name ILIKE '" + p_pair.first);
  _sql.append("' AND pr_order='" + p_pair.second);
  _sql.append("';");

  AntiquaCRM::ASqlCore pgsql(this);
  pgsql.query(_sql);
  if (pgsql.lastError().isEmpty()) {
    updateItemStatus(p_pair.first, p_pair.second,
                     AntiquaCRM::OrderStatus::CANCELED);
  }
}

void SellersSalesList::copyProviderId() {
  if (p_pair.second.length() < 3)
    return;

  qApp->clipboard()->setText(p_pair.second);
}

void SellersSalesList::loadUpdate() {
  // Untermenü-Einträge entfernen!
  if (topLevelItemCount() > 0) {
    for (int t = 0; t < topLevelItemCount(); t++) {
      clearProvider(topLevelItem(t)->text(0));
    }
  }

  QString table("query_provider_orders");
  AntiquaCRM::ASqlCore pgsql(this);
  QString sql = AntiquaCRM::ASqlFiles::queryStatement(table);
  QSqlQuery q = pgsql.query(sql);
  int count = 0;
  if (q.size() > 0) {
    while (q.next()) {
      QString id = q.value("order_number").toString();
      QString provider = q.value("order_provider").toString();
      addProvider(provider);

      AntiquaCRM::OrderStatus status = AntiquaCRM::OrderStatus::CANCELED;
      if (!q.value("pr_ignore").toBool())
        status = static_cast<AntiquaCRM::OrderStatus>(
            q.value("order_status").toInt());

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
  } else if (!pgsql.lastError().isEmpty()) {
    return;
  }

  if (count > 0)
    sortAndResize();
}

bool SellersSalesList::exists(const QString &provider, const QString &id) {
  if (provider.isEmpty() || id.isEmpty())
    return true; // skip empty insert requests

  return (getChild(provider, id) != nullptr);
}

void SellersSalesList::removeOrder(const QString &provider, const QString &id) {
  if (exists(provider, id))
    return;

  QTreeWidgetItem *c = getChild(provider, id);
  if (c == nullptr)
    return;

  QTreeWidgetItem *p = c->parent();
  p->removeChild(c);
  resizeColumnToContents(0);
}

void SellersSalesList::sortAndResize() {
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

int SellersSalesList::ordersCount() {
  int c = 0;
  for (int t = 0; t < topLevelItemCount(); t++) {
    c += topLevelItem(t)->childCount();
  }
  return c;
}
