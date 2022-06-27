// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef DELIVERSERVICELIST_H
#define DELIVERSERVICELIST_H

#include <QObject>
#include <QTreeWidget>
#include <QJsonObject>
#include <QTreeWidgetItem>
#include <QWidget>

class DeliverItem;

class DeliverServiceList : public QTreeWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  QString currency;
  const QString priceDisplay(qreal price);
  QTreeWidgetItem *getParentDeliverService(int srv);

private Q_SLOTS:
  void getDeliverServiceId(QTreeWidgetItem *, int column);

Q_SIGNALS:
  void deliverServiceChanged(int id);
  void deliverPackageClicked(int id, const QJsonObject &name);

public Q_SLOTS:
  void addDeliverService(int id, const QString &name);
  void addDeliverPackage(int srv, const DeliverItem &info);

public:
  explicit DeliverServiceList(QWidget *parent = nullptr);
};

#endif // DELIVERSERVICELIST_H
