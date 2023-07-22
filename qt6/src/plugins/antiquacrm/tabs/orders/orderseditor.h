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
class OrdersTableView;

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
  OrdersTableView *m_ordersTable;

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
   * @brief Lieferscheinnummer genrieren
   *
   * Wenn die Lieferscheinnummer fehlt, diese generieren und Parameter Werte
   * o_delivery setzen.
   */
  void generateDeliveryNumber(qint64 orderId);

  /**
   * @brief Gültigkeitsprüfung der Lieferschein Nummer.
   */
  inline bool checkDeliveryNumber();

  /**
   * @brief Generiere Rechnungs- Lieferscheinnummer.
   * @param oid - Order Id
   *
   * Es wird bei einem INSERT der Trigger „BEFORE INSERT new_invoice_id()“
   * aufgerufen und eine neue Rechnungsnummer generiert.
   * Diese „invoice_id“ wird hier abgefragt und in die Temporären Container
   * eingefügt!
   */
  void setOrderPaymentNumbers(qint64 oid);

  /**
   * @brief Temporäre Klasse zum Prüfen der Identifikations Nummern.
   * Gleichzeitig ein Helfer die wichtigsten Id’s sofort abrufen zu können!
   */
  struct Idset {
    bool isValid = false; /**< Prüfergebnis */
    qint64 or_id = -1;    /**< Order Id */
    qint64 cu_id = -1;    /**< Customer Id */
    qint64 in_id = -1;    /**< Invoice Id */
  };
  const Idset identities();

  /**
   * @brief Umsatzsteuersatz einbinden oder nicht
   * Sehe nach ob „o_vat_country“ gesetzt und nicht „XX“="Kein EU Land" ist.
   * Abhängig von diesem Wert den Paramter AntiquaCRM::SalesTax setzen.
   */
  AntiquaCRM::SalesTax initSalesTax();

  /**
   * @brief Umsatzsteuerart durch ArtikelTyp bestimmen.
   * @param type - AntiquaCRM::ArticleType
   * @return 0|1 = (0:Standard oder 1:Reduziert)
   *
   * Ermittelt an Hand des ArtikelTyps welcher Umsatzsteuersatz verwendet wird!
   */
  int getSalesTaxType(int type);

  /**
   * @brief Artikel von Auftrag in PgSQL suchen und in Container schreiben!
   */
  const QList<AntiquaCRM::OrderArticleItems> queryArticles(qint64 oid);

  /**
   * @brief Artikel einfügen
   * @see AntiquaCRM::AProviderOrder::createItem
   */
  inline AntiquaCRM::ArticleOrderItem
  addArticleItem(const QString &key, const QVariant &value) const;

  /**
   * @brief Artikel in die Auftrags Tabelle einfügen!
   * SQL Abfrage auf die Artikel-Nr. durchführen und einfügen wenn der Bestand
   * größer als 0 ist.
   * @see PgSQL::query_article_order_with_id.sql
   * @param aid = Article Id
   */
  bool addOrderTableArticle(qint64 aid);

  /**
   * @brief Tabellen SQL-Abfragen Speichern ziehen.
   */
  const QString getOrderTableSqlQuery();

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
   * @param obj = Daten
   * @param cid = Kundennummer
   */
  qint64 findCustomer(const QJsonObject &obj, qint64 cid = -1);

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
   * @param aid - Article Number
   */
  bool addArticle(qint64 aid);

  /**
   * @brief Auftrag öffnen
   * @param oid - Order Id
   */
  bool openEditEntry(qint64 oid) override;

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
