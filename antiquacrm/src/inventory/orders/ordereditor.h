// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_ORDEREDITOR_H
#define INVENTORY_ORDEREDITOR_H

#include <QListWidget>
#include <QObject>
#include <QVariant>
#include <QWidget>

#include <EditorMain>
#include <Utils>

class OrdersItemList;

/**
 * @ingroup InventoryOrders
 * @class OrderPaymentsTable
 */
class OrderEditor : public EditorMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  SerialID *o_id;                      /**< Order ID */
  OrderStatusBox *o_order_status;      /**< o_order_status */
  OrdersPaymentBox *o_payment_status;  /**< o_payment_status */
  SerialID *o_costumer_id;             /**< o_costumer_id */
  TextField *m_costumer_address;       /**< Kunden Adresse */
  DeliveryService *o_delivery_service; /**< o_delivery_service */
  LineEdit *o_provider;                /**< o_provider */
  TextField *m_provider_info;          /**< Informationen zum Dienstleister */
  BoolBox *o_locked;                   /**< o_locked */
  BoolBox *o_closed;                   /**< o_closed */
  EditorActionBar *m_actionBar;        /**< Actions Bar */
  OrdersItemList *m_paymentList;       /**< Liste der Bestellten Artikel */

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
  void setCostumerAddress(int);

private Q_SLOTS:
  void findArticle(int);
  void saveData();
  void checkLeaveEditor();
  void finalLeaveEditor();

  /**
   * @brief SQL Spalte "o_closed" true setzen.
   * Wenn diese Aktion ausgeführt wird ist es nur noch
   * für die Buchhaltung sichtbar und der Editor wird verlassen!
   * Der Auftrag ist somit abgeschlossen!
   */
  void createCloseOrder(bool b = false);

public Q_SLOTS:
  void restoreDataset();
  void showMessagePoUp(const QString &);

public:
  explicit OrderEditor(QWidget *parent = nullptr);

  /**
   * @brief updateOrder
   */
  void updateOrder(int);

  /**
   * @brief Einen Auftrag mit ID erstellen
   * @param Costumer Id
   */
  void createOrder(int costumerId = 0);
};

#endif // INVENTORY_ORDEREDITOR_H
