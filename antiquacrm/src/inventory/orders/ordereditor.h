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
  bool showSuccessFully = true;
  SerialID *o_id;                      /**< Order ID */
  OrderStatusBox *o_order_status;      /**< o_order_status */
  OrdersPaymentBox *o_payment_status;  /**< o_payment_status */
  SerialID *o_costumer_id;             /**< o_costumer_id */
  TextField *m_costumer_address;       /**< Kunden Adresse */
  DeliveryService *o_delivery_service; /**< o_delivery_service */
  LineEdit *o_provider_name;           /**< o_provider */
  TextField *o_provider_order;         /**< Informationen zum Dienstleister */
  BoolBox *o_notify;                   /**< o_notify */
  BoolBox *o_locked;                   /**< o_locked */
  BoolBox *o_closed;                   /**< o_closed */
  LineEdit *o_modified;                /**< o_modified */

  /**
   * @brief Standard ActionsBar
   */
  EditorActionBar *m_actionBar;

  /**
   * @brief Liste der Bestellten Artikel
   */
  OrdersItemList *m_paymentList;

  /**
   * @brief Suchpattern für Eingabefelder
   */
  const QRegularExpression p_objPattern = QRegularExpression("^o_[a-z_]+\\b$");

  /**
   * @brief Lese die aktuellen Datenfelder mit QSqlRecord aus Tabelle!
   */
  void setInputList();

  /**
   * @brief Datensatz in Felder importieren
   */
  void importSqlResult();

  /**
   * @brief Aktuelle eingabe einlesen!
   * Wenn @b isRequired() und nicht @b isValid() zuschlagen.
   * Wird der Hash geleert und eine Fehlermeldung ausgegeben!
   * @return Datensatz
   */
  const QHash<QString, QVariant> createSqlDataset();

  bool sendSqlQuery(const QString &);
  bool createSqlArticleOrder();
  void createSqlUpdate();
  void createSqlInsert();
  void setData(const QString &key, const QVariant &value,
               bool required = false);

  void initDefaults();
  const QList<OrderArticle> getOrderArticles(int orderId, int costumerId);
  bool getCostumerAddress(int costumerId);

private Q_SLOTS:
  void findCostumer(int);
  void findArticle(int);
  void findRemoveTableRow(int);
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

Q_SIGNALS:
  void postMessage(const QString &);
  void isModified(bool);

public:
  explicit OrderEditor(QWidget *parent = nullptr);

  /**
   * @brief Eintrage Bearbeiten
   * @parem Order ID
   */
  void openUpdateOrder(int oid = -1);

  /**
   * @brief Einen Auftrag mit Kunden ID erstellen!
   * @param Costumer Id
   */
  void openCreateOrder(int cid = 0);
};

#endif // INVENTORY_ORDEREDITOR_H
