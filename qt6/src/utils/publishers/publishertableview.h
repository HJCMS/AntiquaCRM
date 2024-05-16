// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_UTILS_PUBLISHERTABLEVIEW_H
#define ANTIQUACRM_UTILS_PUBLISHERTABLEVIEW_H

#include <AntiquaWidgets>
#include <QContextMenuEvent>
#include <QModelIndex>
#include <QPair>
#include <QWidget>

class PublisherTableModel;

class PublisherTableView final : public AntiquaCRM::TableView {
  Q_OBJECT

private:
  PublisherTableModel *m_model;
  qint64 getTableID(const QModelIndex &index, int column = 0) override;
  const QModelIndex index(const QString &publisher, bool strict = true);
  bool sqlModelQuery(const QString &query) override;
  const QPair<QString, QString> getData(const QModelIndex &index);
  void contextMenuEvent(QContextMenuEvent *) override;
  const QString defaultWhereClause() override;
  bool setQuery(const QString &query = QString()) override;

private Q_SLOTS:
  void deleteItem(const QModelIndex &index);
  void contextMenuAction(AntiquaCRM::TableContextMenu::Actions,
                         const QModelIndex &) override;
  void setSortByColumn(int column, Qt::SortOrder order) override;
  void getSelectedItem(const QModelIndex &) override;
  void createSocketOperation(const QModelIndex &) override;

Q_SIGNALS:
  void sendItemSelected(const QModelIndex &);

public Q_SLOTS:
  void setReloadView() override;

public:
  explicit PublisherTableView(QWidget *parent = nullptr);
  bool saveData(const QString &publisher, const QString &location);
  int rowCount() override;
  const QString getPublisher(const QModelIndex &index);
  const QString getLocation(const QModelIndex &index);
  bool init();
};

#endif // ANTIQUACRM_UTILS_PUBLISHERTABLEVIEW_H
