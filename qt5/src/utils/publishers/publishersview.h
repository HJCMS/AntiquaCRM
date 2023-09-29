// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef PUBLISHERS_VIEW_H
#define PUBLISHERS_VIEW_H

#include <QHeaderView>
#include <QModelIndex>
#include <QObject>
#include <QTableView>
#include <QWidget>

class PublishersViewModel;

class PublishersView final : public QTableView {
  Q_OBJECT

private:
  PublishersViewModel *m_model;
  QHeaderView *m_headerView;

private Q_SLOTS:
  void selected(const QModelIndex &, const QModelIndex &);

public Q_SLOTS:
  void showPublisher(const QString &);

public:
  explicit PublishersView(QWidget *parent = nullptr);
  QPair<QString, QString> getData(const QModelIndex &index);
  const QModelIndex find(const QString &publisher, bool strict = true);
  bool init();
};

#endif // PUBLISHERS_VIEW_H
