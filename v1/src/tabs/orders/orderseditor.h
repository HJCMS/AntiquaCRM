// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERSEDITOR_H
#define ANTIQUACRM_ORDERSEDITOR_H

#include <AntiquaWidgets>
#include <QObject>
#include <QWidget>

class OrdersCustomerInfo;
class OrdersCostSettings;
class OrdersItemList;

class OrdersEditor final : public InventoryEditor {
  Q_OBJECT

private:
  SerialID *o_id;
  SerialID *o_invoice_id;
  BoolBox *o_payment_status;
  OrderStatusSelecter *o_order_status;

  QStringList customInput;
  OrdersCustomerInfo *m_customerInfo;
  OrdersCostSettings *m_costSettings;

  OrdersItemList *m_ordersList;

  void setInputFields() override;
  bool setDataField(const QSqlField &field, const QVariant &value) override;
  void importSqlResult() override;
  bool sendSqlQuery(const QString &query) override;
  const QHash<QString, QVariant> createSqlDataset() override;
  void createSqlUpdate() override;
  void createSqlInsert() override;

private Q_SLOTS:
  void setSaveData() override;
  void setCheckLeaveEditor() override;
  void setFinalLeaveEditor() override;

public Q_SLOTS:
  void setRestore() override;

public:
  explicit OrdersEditor(QWidget *parent = nullptr);
  bool openEditEntry(qint64 orderId) override;
  bool createNewEntry() override;
};

#endif // ANTIQUACRM_ORDERSEDITOR_H
