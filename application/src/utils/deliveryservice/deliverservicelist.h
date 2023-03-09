// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DELIVERSERVICELIST_H
#define DELIVERSERVICELIST_H

#include <QContextMenuEvent>
#include <QJsonObject>
#include <QObject>
#include <QIcon>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>

class DeliverItem;

/**
 * @ingroup utils
 * @brief Deliver Services TreeWidget
 */
class DeliverServiceList : public QTreeWidget {
  Q_OBJECT

private:
  Qt::ItemFlags p_flags;
  QString currency;
  const QIcon deliverIcon() const;
  const QIcon sectionIcon() const;
  const QIcon itemIcon() const;
  const QString priceDisplay(qreal price);
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

#endif // DELIVERSERVICELIST_H
