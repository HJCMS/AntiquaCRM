// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_ORDEREDITOR_H
#define INVENTORY_ORDEREDITOR_H

#include <QObject>
#include <QVariant>
#include <QWidget>

#include <EditorMain>
#include <Utils>

class OrderStatusBox;
class OrdersPaymentBox;

class OrderEditor : public EditorMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  SerialID *o_id;                      /**< Order ID */
  OrderStatusBox *o_order_status;      /**< o_order_status */
  OrdersPaymentBox *o_payment_status;  /**< o_payment_status */
  LineEdit *o_costumer_id;             /**< o_costumer_id */
  DeliveryService *o_delivery_service; /**< o_delivery_service */
  LineEdit *o_provider;                /**< o_provider */
  BoolBox *o_locked;                   /**< o_locked */
  BoolBox *o_closed;                   /**< o_closed */

  const QString tableName = QString("inventory_orders");

  const QRegularExpression p_objPattern = QRegularExpression("^o_[a-z_]+\\b$");

  void setInputList();
  void importSqlResult();
  bool sendSqlQuery(const QString &);
  void createSqlUpdate();
  void createSqlInsert();
  void setData(const QString &key, const QVariant &value,
               bool required = false);

  void initDefaults();

private Q_SLOTS:
  void saveData();
  void checkLeaveEditor();
  void finalLeaveEditor();

public Q_SLOTS:
  void restoreDataset();

public:
  explicit OrderEditor(QWidget *parent = nullptr);
  void updateOrder(int);
  void createOrder(int costumerId = 0);
};

#endif // INVENTORY_ORDEREDITOR_H
