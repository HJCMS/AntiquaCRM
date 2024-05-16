// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_DELIVERSERVICELIST_H
#define ANTIQUACRM_UTILS_DELIVERSERVICELIST_H

#include <QContextMenuEvent>
#include <QIcon>
#include <QJsonObject>
#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>

class DeliverItem;

class DeliverServiceList : public QTreeWidget {
  Q_OBJECT

private:
  Qt::ItemFlags p_flags;
  const QIcon deliverIcon() const;
  const QIcon sectionIcon() const;
  const QIcon itemIcon() const;
  const QString priceDisplay(double price);
  QTreeWidgetItem *getParentDeliverService(int srv);

private Q_SLOTS:
  void createByContext();
  void removeByContext();
  void getDeliverServiceId(QTreeWidgetItem *, int column);

protected:
  void contextMenuEvent(QContextMenuEvent *);

Q_SIGNALS:
  void message(const QString &);
  void createDeliveryService(int id);
  void removeDeliveryPackage(const QJsonObject &relation);
  void deliverServiceChanged(int id);
  void deliverPackageClicked(int id, const QJsonObject &name);

public Q_SLOTS:
  void addDeliverService(int id, const QString &name);
  void addDeliverPackage(int srv, const DeliverItem &info);

public:
  explicit DeliverServiceList(QWidget *parent = nullptr);
};

#endif // ANTIQUACRM_UTILS_DELIVERSERVICELIST_H
