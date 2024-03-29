// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

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

/**
 * @brief Inventory Orders Editor
 * @ingroup _orders
 */
class OrdersEditor final : public InventoryEditor {
  Q_OBJECT

private:
  SerialID *o_id;
  SerialID *o_invoice_id;
  OrderPaymentStatusSelecter *o_payment_status;
  OrderStatusSelecter *o_order_status;

  QStringList customInput;
  OrdersCustomerInfo *m_customerInfo;
  OrdersCostSettings *m_costSettings;

  OrdersItemList *m_ordersList;

  EditorActionBar *m_actionBar;

  void setInputFields() override;

  /**
   * @brief Editor Eingabefeld befüllen
   */
  bool setDataField(const QSqlField &field, const QVariant &value) override;

  /**
   * @brief Wenn die Lieferscheinnummer fehlt, diese generieren!
   */
  void generateDeliveryNumber(qint64 orderId);

  /**
   * @brief Gültigkeitsprüfung der Lieferschein Nummer.
   */
  inline bool checkDeliveryNumber();

  /**
   * @brief Generiere Rechnungs- Lieferscheinnummer.
   * Es wird bei einem INSERT der Trigger „BEFORE INSERT new_invoice_id()“
   * aufgerufen und eine neue Rechnungsnummer generiert.
   * Diese „invoice_id“ wird hier abgefragt und in die Temporären Container
   * eingefügt!
   */
  void setOrderPaymentNumbers(qint64 orderId);

  /**
   * @brief Temporäre Klasse zum Prüfen der Identifikations Nummern.
   * Gleichzeitig ein Helfer die wichtigsten Id’s sofort abrufen zu können!
   */
  struct IdsCheck {
    bool isNotValid = true; /**< Prüfergebnis */
    qint64 or_id;           /**< Order Id */
    qint64 cu_id;           /**< Customer Id */
    qint64 in_id;           /**< Invoide Id */
  };
  const IdsCheck getCheckEssentialsIds();

  /**
   * @brief Umsatzsteuersatz einbinden oder nicht
   * Sehe nach ob „o_vat_country“ gesetzt und nicht „XX“="Kein EU Land" ist.
   * Abhängig von diesem Wert den Paramter AntiquaCRM::SalesTax setzen.
   */
  AntiquaCRM::SalesTax initSalesTax();

  /**
   * @brief Umsatzsteuersatz durch ArtikelTyp bestimmen.
   * @param int - AntiquaCRM::ArticleType
   * Die Rückgabewerte „Standard“ oder „Reduziert“ werden bei den Einstellungen
   * in der Gruppe „payment“ gesetzt!
   */
  int getSalesTaxValue(int);

  /**
   * @brief Umsatzsteuerart durch ArtikelTyp bestimmen.
   * @param int - AntiquaCRM::ArticleType
   * @return 0|1 = (0:Standard oder 1:Reduziert)
   * Ermittelt an Hand des ArtikelTyps welcher Umsatzsteuersatz verwendet wird!
   */
  int getSalesTaxType(int);

  /**
   * @brief Abfrage Paketlieferservice.
   * @param current - Abfrageart
   *  @li true  - „currentDeliveryService()“ verwenden.
   *  @li false - „defaultDeliveryService()“ verwenden.
   */
  const QPair<int, int> getDeliveryService(bool current = false);

  /**
   * @brief Lieferservice setzen
   * Holse aus Daten von AntiquaCRM::ASqlDataQuery den Paketdienstleister
   * @note Kann erst nach einer Abfrage verwendet werden.
   */
  void setDeliveryService();

  /**
   * @brief Sql-Abfrage-Ergebnis Importieren
   * Lese die Übermittelten Tabellen Datenfelder aus AntiquaCRM::ASqlDataQuery
   * und sende sie mit „setDataField“ an die Editoreingabefelder.
   */
  void importSqlResult() override;

  /**
   * @brief SQL verarbeiten senden...
   */
  bool sendSqlQuery(const QString &query) override;

  /**
   * @brief Aktuellen Eingabedatensatz ermitteln.
   * Suche alle Eingabefelder und erstelle mit dem Schlüssel Objektnamen
   * die Datensatzfelder.
   * @note Objektnamen der Eingabefelder sind Identisch mit SQL-Felder
   */
  const QHash<QString, QVariant> createSqlDataset() override;

  /**
   * @brief Sql Update Statement erstellen
   */
  void createSqlUpdate() override;

  /**
   * @brief Sql Insert Statement erstellen
   */
  void createSqlInsert() override;

  /**
   * @brief Kundennummer Suchen/Prüfen
   */
  qint64 searchCustomer(const QJsonObject &obj, qint64 customerId = -1);

  /**
   * @brief Artikel von Auftrag in PgSQL suchen und in Container schreiben!
   */
  const QList<AntiquaCRM::OrderArticleItems> queryOrderArticles(qint64 orderId);

  /**
   * @brief Artikel in die Auftrags Tabelle einfügen!
   * SQL Abfrage auf die Artikel-Nr. durchführen und einfügen wenn der Bestand
   * größer als 0 ist.
   * @see PgSQL::query_article_order_with_id.sql
   */
  bool addArticleToOrderTable(qint64 articleId);

  /**
   * @brief Abfrage des in „OrdersItemList“ erstellen SQL-Statements.
   */
  const QString getSqlArticleOrders();

  /**
   * @brief Aktuelle Verkaufsinfos ermitteln!
   * @see PgSQL::query_printing_billing_info.sql
   */
  const QList<BillingInfo> queryBillingInfo(qint64 oid, qint64 cid);

  /**
   * @brief Artikel einfügen
   * @see AntiquaCRM::AProviderOrder::createItem
   */
  inline AntiquaCRM::ArticleOrderItem addArticleItem( // Schlüssel
      const QString &key, const QVariant &value) const;

  /**
   * @brief Setze Standard Werte für bestimmte Felder.
   */
  void setDefaultValues();

  /**
   * @brief Vorarbeiten für neuen Eintrag. Datenfelder leeren etc.
   */
  bool beforeCreate();

  /**
   * @brief Neue Einträge ohne Parameter wird hier nicht unterstützt!
   */
  bool createNewEntry() override { return false; /* unused */ };

  /**
   * @brief Datenbank Status des Auftrags
   * Wenn SQL o_payment_status="Geliefert" und o_order_status="Bezahlt".
   */
  bool databaseOrderStatus();

  /**
   * @brief Abfrage Status editierten Auftrags
   * Wenn Input o_payment_status="Geliefert" und o_order_status="Bezahlt".
   */
  bool currentOrderStatus();

private Q_SLOTS:
  /**
   * @brief OrderStatus und PaymentStatus versiegeln!
   */
  void setProtected(bool);

  /**
   * @brief Prüfung auf (INSERT|UPDATE) und Datenkollisionen!
   */
  void setSaveData() override;

  void setCheckLeaveEditor() override;
  void setFinalLeaveEditor(bool force = true) override;
  void createMailMessage(const QString &type);
  void createPrintDeliveryNote();
  void createPrintInvoiceNote();
  void createPrintPaymentReminder();
  void openSearchAddArticle();

public Q_SLOTS:
  void setRestore() override;

public:
  explicit OrdersEditor(QWidget *parent = nullptr);

  /**
   * @brief Auftrag öffnen
   */
  bool openEditEntry(qint64 orderId) override;

  /**
   * @brief Einen Artikel hinzufügen
   */
  bool addArticle(qint64 articleId);

  /**
   * @brief Manuelles Auftrag erstellen...
   */
  bool createNewOrder(qint64 customerId);

  /**
   * @brief Auftrag mit Dienstleister Daten erstellen.
   */
  bool createNewProviderOrder(const QJsonObject &prObject);

  /**
   * @brief Zahlungsstatus abfragen
   */
  AntiquaCRM::OrderPayment paymentStatus();

  /**
   * @brief Auftragsstatus abfragen
   */
  AntiquaCRM::OrderStatus orderStatus();
};

#endif // ANTIQUACRM_ORDERSEDITOR_H
