// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2024 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_ORDERS_WIDGET_H
#define ANTIQUACRM_PLUGIN_ORDERS_WIDGET_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QObject>

class OrdersSearchBar;
class OrdersTableOverView;
class OrdersStatusBar;
class OrdersEditor;

class ANTIQUACRM_LIBRARY OrdersWidget final : public AntiquaCRM::TabsIndex {
  Q_OBJECT

private:
  QWidget *m_mainPage;
  OrdersSearchBar *m_searchBar;
  OrdersTableOverView *m_table;
  OrdersStatusBar *m_statusBar;
  QScrollArea *m_editorPage;
  OrdersEditor *m_editorWidget;
  bool firstStartOnWorkday();
  void setDefaultTableView() override;

public Q_SLOTS:
  void openStartPage() override;
  void createSearchQuery(const QString &history = QString()) override;
  void createNewEntry() override;
  void openEntry(qint64 oid) override;
  void refundEntry(qint64 oid);
  void onEnterChanged() override;

public:
  explicit OrdersWidget(QWidget *parent = nullptr);

  const QString getTitle() const override;

  /**
   * @brief Open/Edit Inport actions
   * @param obj - JsonObject
   *
   * Creates restricted custom operations for open/create and import Orders.
   *
   * @code
   *  QJsonObject {
   *    "TARGET": "orders_tab"
   *    "ACTION": QString
   *    "CUSTOMER": qint64
   *    "PRORDER": QString
   *    "ARTICLES": QStringList
   *  }
   * @endcode
   *
   * Acceptable "ACTION" operations:
   * @li add_article   - Add to current order
   * @li open_order    - Open order with OrderId
   * @li create_order  - Create new order with Customer Id
   * @li import_order  - Import Order from Provider
   *
   * @return acception
   */
  bool customAction(const QJsonObject &obj) override;

  const QStringList acceptsCustomActions() const override;
};

#endif // ANTIQUACRM_PLUGIN_ORDERS_WIDGET_H
