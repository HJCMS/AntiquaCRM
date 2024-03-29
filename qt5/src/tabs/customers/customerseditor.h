// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_CUSTOMERSEDITOR_H
#define ANTIQUACRM_CUSTOMERSEDITOR_H

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QLabel>
#include <QObject>
#include <QWidget>

class CustomersHeaderFrame;
class CustomersTabWidget;
class CustomersData;
class CustomersBillings;
class CustomersOrders;

/**
 * @brief Customers Editor
 * @ingroup _customers
 */
class CustomersEditor final : public InventoryEditor {
  Q_OBJECT

private:
  CustomersHeaderFrame *m_headerFrame;
  CustomersTabWidget *m_tabWidget;
  CustomersData *m_dataWidget;
  CustomersBillings *m_billingWidget;
  CustomersOrders *m_ordersWidget;
  EditorActionBar *m_actionBar;

  const QString generateProviderId();

  void setInputFields() override;
  bool setDataField(const QSqlField &field, const QVariant &value) override;
  void importSqlResult() override;
  bool sendSqlQuery(const QString &query) override;
  const QHash<QString, QVariant> createSqlDataset() override;
  void createSqlUpdate() override;
  void createSqlInsert() override;

  /**
   * @brief Einkäufe vom Kunden suchen und einfügen!
   */
  void findPurchases();

private Q_SLOTS:
  void setSaveData() override;
  void setCreateOrderSignal();
  void setCheckLeaveEditor() override;
  void setFinalLeaveEditor(bool force = true) override;
  void setCreateMailMessage(const QString &action);

public Q_SLOTS:
  void setRestore() override;

public:
  explicit CustomersEditor(QWidget *parent = nullptr);
  bool openEditEntry(qint64 customerId) override;
  bool createNewEntry() override;
  qint64 queryCustomerByOrderId(qint64 orderId);
};

#endif // ANTIQUACRM_CUSTOMERSEDITOR_H
