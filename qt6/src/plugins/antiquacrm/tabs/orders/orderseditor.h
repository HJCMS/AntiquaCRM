// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
//
// SPDX-FileCopyrightText: 2023 Juergen Heinemann <nospam@hjcms.de>
//
// SPDX-License-Identifier: LGPL-3.0-or-later
//

#ifndef ANTIQUACRM_PLUGIN_ORDERSEDIT_H
#define ANTIQUACRM_PLUGIN_ORDERSEDIT_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QObject>

class OrdersCustomerInfo;
class OrdersCostSettings;
class OrdersPayedArticles;

class ANTIQUACRM_LIBRARY OrdersEditor final : public AntiquaCRM::TabsEditor {
  Q_OBJECT

private:
  /**
   * @brief Additional Input Fields
   */
  QStringList customInput;

  // Layout Row0
  AntiquaCRM::SerialId *o_id;
  AntiquaCRM::SerialId *o_invoice_id;
  AntiquaCRM::SelectOrderStatus *o_order_status;
  AntiquaCRM::SelectOrderPayment *o_payment_status;

  // Layout Row1
  OrdersCustomerInfo *m_customerInfo;
  OrdersCostSettings *m_costSettings;

  // Layout Row2
  OrdersPayedArticles *m_ordersList;

  // BEGIN:Row3
  AntiquaCRM::TabsEditActionBar *m_actionBar;

  void setInputFields() override;

  /**
   * @brief Editor Eingabefeld befüllen
   */
  bool setDataField(const QSqlField &field, const QVariant &value) override;

  void importSqlResult() override;

  bool sendSqlQuery(const QString &query) override;

  const QHash<QString, QVariant> createSqlDataset() override;

  void createSqlUpdate() override;

  void createSqlInsert() override;

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
   * @brief Datenbank Status des Auftrags
   * Wenn SQL o_payment_status="Geliefert" und o_order_status="Bezahlt".
   */
  bool databaseOrderStatus();

  /**
   * @brief Abfrage Status editierten Auftrags
   * Wenn Input o_payment_status="Geliefert" und o_order_status="Bezahlt".
   */
  bool currentOrderStatus();

  /**
   * @brief Kundennummer Suchen/Prüfen
   */
  qint64 findCustomer(const QJsonObject &obj, qint64 customerId = -1);

  /**
   * @brief Setze Standard Werte für bestimmte Felder.
   */
  void setDefaultValues();

  /**
   * @brief Vorarbeiten für neuen Eintrag. Datenfelder leeren etc.
   */
  bool prepareCreateEntry();

private Q_SLOTS:
  /**
   * @brief OrderStatus und PaymentStatus versiegeln!
   */
  void setStatusProtection(bool b = false);

  void setSaveData() override;

  void setCheckLeaveEditor() override;

  void setFinalLeaveEditor(bool force = true) override;

  void setStorageCompartments() override{/* unused */};

  void setLoadThumbnail(qint64) override{/* unused */};

  void setRemoveThumbnail(qint64) override{/* unused */};

  void setImportEditImage() override{/* unused */};

  void createMailMessage(const QString &type);

  void createPrintDeliveryNote();

  void createPrintInvoiceNote();

  void createPrintPaymentReminder();

  void openSearchInsertArticle();

public Q_SLOTS:
  void setRestore() override;

public:
  explicit OrdersEditor(QWidget *parent = nullptr);
  virtual ~OrdersEditor() override;

  /**
   * @brief Zahlungsstatus abfragen
   */
  AntiquaCRM::OrderPayment paymentStatus();

  /**
   * @brief Auftragsstatus abfragen
   */
  AntiquaCRM::OrderStatus orderStatus();

  /**
   * @brief Insert Article to Current Order
   * @param articleId - Article Number
   */
  bool addArticle(qint64 articleId);

  /**
   * @brief Auftrag öffnen
   */
  bool openEditEntry(qint64 orderId) override;

  /**
   * @brief Manuelles Auftrag erstellen...
   */
  bool createNewEntry() override;

  /**
   * @brief Auftrag mit Kunden numemr erstellen.
   * @param customerId - Kunden Nummer
   */
  bool createNewOrder(qint64 customerId);

  /**
   * @brief Auftrag aus Json Object erstellen
   * @param object - Datenstruktur
   */
  bool createCustomEntry(const QJsonObject &object) override;
};

#endif // ANTIQUACRM_PLUGIN_ORDERSEDIT_H
