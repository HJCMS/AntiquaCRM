// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERS_TREEVIEW_H
#define INVENTORY_PROVIDERS_TREEVIEW_H

#include <QObject>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>

class ProvidersTreeView : public QTreeWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private Q_SLOTS:
  void itemSelected(QTreeWidgetItem *, int);

public Q_SLOTS:
  void addOrder(const QString &provider, const QString &id);

Q_SIGNALS:
  void s_queryOrder(const QString &provider, const QString &id);

public:
  explicit ProvidersTreeView(QWidget *parent = nullptr);
  void setTreeViewHeaders();
  void addProviders(const QStringList &);
};

#endif // INVENTORY_PROVIDERS_TREEVIEW_H
