// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_ORDERS_WIDGET_H
#define ANTIQUACRM_PLUGIN_ORDERS_WIDGET_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QObject>

class OrdersSearchBar;
class OrdersTableView;
class OrdersStatusBar;
class OrdersEditor;

class ANTIQUACRM_LIBRARY OrdersWidget final : public AntiquaCRM::TabsIndex {
  Q_OBJECT

private:
  QWidget *m_mainPage;
  OrdersSearchBar *m_searchBar;
  OrdersTableView *m_table;
  OrdersStatusBar *m_statusBar;
  QScrollArea *m_editorPage;
  OrdersEditor *m_editorWidget;

  void popupWarningTabInEditMode() override;

  void setDefaultTableView() override;

public:
  explicit OrdersWidget(QWidget *parent = nullptr);

  void openStartPage() override;

  void createSearchQuery(const QString &history = QString()) override;

  void createNewEntry() override;

  void openEntry(qint64 oid) override;

  void onEnterChanged() override;

  const QString getTitle() const override;

  bool customAction(const QJsonObject &obj) override;
};

#endif // ANTIQUACRM_PLUGIN_ORDERS_WIDGET_H
