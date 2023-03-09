// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABORDERS_H
#define ANTIQUACRM_TABORDERS_H

#include <AntiquaWidgets>
#include <QObject>
#include <QTimerEvent>
#include <QScrollArea>

class OrdersSearchBar;
class OrdersTableView;
class OrdersStatusBar;
class OrdersEditor;
class OrdersReturning;

/**
 * @brief Inventory Orders System
 * @ingroup _orders
 */
class TabOrders final : public Inventory {
  Q_OBJECT

private:
  QWidget *m_mainPage;
  OrdersSearchBar *m_searchBar;
  OrdersTableView *m_table;
  OrdersStatusBar *m_statusBar;

  QScrollArea *m_editorPage;
  OrdersEditor *m_editorWidget;

  OrdersReturning *m_ordersReturning;

  void popupWarningTabInEditMode() override;

private Q_SLOTS:
  void openCustomSearchDialog();

public Q_SLOTS:
  void setDefaultTableView() override;
  void openStartPage() override;
  void createSearchQuery(const QString &query = QString()) override;
  void createNewEntry() override  { /* UNUSED */ };
  void openEntry(qint64 o_id) override;
  void onEnterChanged() override;

public:
  explicit TabOrders(QWidget *parent = nullptr);
  Inventory::ViewIndex currentView() override;
  bool customAction(const QJsonObject &obj) override;
};

#endif // ANTIQUACRM_TABORDERS_H
