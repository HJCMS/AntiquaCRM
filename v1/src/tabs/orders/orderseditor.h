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

  EditorActionBar *m_actionBar;

  void setInputFields() override;

  /**
   * @brief Lieferschein Nummer generieren
   * @note Ist Abhängig von OrderId
   */
  const QString generateDeliveryNumber();

  bool setDataField(const QSqlField &field, const QVariant &value) override;
  void importSqlResult() override;
  bool sendSqlQuery(const QString &query) override;
  const QHash<QString, QVariant> createSqlDataset() override;
  void createSqlUpdate() override;
  void createSqlInsert() override;
  qint64 searchCustomer(const QJsonObject &obj);
  AntiquaCRM::ArticleOrderItem addArticleItem(const QString &key,
                                              const QVariant &value) const;
  bool saveArticleOrders();

private Q_SLOTS:
  void setSaveData() override;
  void setCheckLeaveEditor() override;
  void setFinalLeaveEditor() override;
  void searchInsertArticleId(qint64 aid);
  void createMailMessage(const QString &type);
  void createPrintDeliveryNote();
  void createPrintInvoiceNote();
  void createPrintPaymentReminder();

public Q_SLOTS:
  void setRestore() override;

public:
  explicit OrdersEditor(QWidget *parent = nullptr);
  bool openEditEntry(qint64 orderId) override;
  bool createNewEntry() override;
  bool createNewProviderOrder(const QString &providerId);
};

#endif // ANTIQUACRM_ORDERSEDITOR_H
