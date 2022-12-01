// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERSEDITOR_H
#define ANTIQUACRM_ORDERSEDITOR_H

#include <AntiquaMailing>
#include <AntiquaPrinting>
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

  bool setDataField(const QSqlField &field, const QVariant &value) override;

  void generateDeliveryNumber(qint64 orderId);

  void setOrderPaymentNumbers(qint64 orderId);

  struct IdsCheck {
    bool isNotValid = true;
    qint64 or_id; /**< Order Id */
    qint64 cu_id; /**< Customer Id */
    qint64 in_id; /**< Invoide Id */
  };
  const IdsCheck checkEssentialsIds();

  void importSqlResult() override;

  bool sendSqlQuery(const QString &query) override;

  const QHash<QString, QVariant> createSqlDataset() override;

  void createSqlUpdate() override;

  void createSqlInsert() override;

  qint64 searchCustomer(const QJsonObject &obj);

  bool addArticleToOrderTable(qint64 articleId);

  const QString getSqlArticleOrders();

  const QList<BillingInfo> queryBillingInfo(qint64 oid, qint64 cid);

  inline AntiquaCRM::ArticleOrderItem
  addArticleItem(const QString &key, const QVariant &value) const;

  void setDefaultValues();

  bool createNewEntry() override;

private Q_SLOTS:
  void setSaveData() override;
  void setCheckLeaveEditor() override;
  void setFinalLeaveEditor() override;
  void createMailMessage(const QString &type);
  void createPrintDeliveryNote();
  void createPrintInvoiceNote();
  void createPrintPaymentReminder();
  void openSearchAddArticle();

public Q_SLOTS:
  void setRestore() override;

public:
  explicit OrdersEditor(QWidget *parent = nullptr);
  bool openEditEntry(qint64 orderId) override;
  bool addArticle(qint64 articleId);
  bool createNewOrder(qint64 customerId);
  bool createNewProviderOrder(const QJsonObject &prObject);
};

#endif // ANTIQUACRM_ORDERSEDITOR_H
