// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_TABCUSTOMERS_H
#define ANTIQUACRM_TABCUSTOMERS_H

#include <AntiquaWidgets>
#include <QObject>
#include <QScrollArea>

class CustomersSearchBar;
class CustomersTableView;
class CustomersStatusBar;
class CustomersEditor;

/**
 * @brief Inventory Customers
 * @ingroup _customers
 */
class TabCustomers final : public Inventory {
  Q_OBJECT

private:
  QWidget *m_mainPage;
  CustomersSearchBar *m_searchBar;
  CustomersTableView *m_table;
  CustomersStatusBar *m_statusBar;
  QScrollArea *m_editorPage;
  CustomersEditor *m_editorWidget;

private Q_SLOTS:
  void popupWarningTabInEditMode() override;
  void setDefaultTableView() override;
  void setDeleteCustomer(qint64 customerId);
  void createNewOrder(qint64 customerId);

public Q_SLOTS:
  void openStartPage() override;
  void createSearchQuery(const QString &query = QString()) override;
  void createNewEntry() override;
  void openEntry(qint64 customerId) override;
  void onEnterChanged() override;

public:
  explicit TabCustomers(QWidget *parent = nullptr);
  Inventory::ViewIndex currentView() override;
  bool customAction(const QJsonObject &obj) override;
};

#endif // ANTIQUACRM_TABCUSTOMERS_H
