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

class OrderStatusActionFrame;
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
  OrderStatusActionFrame *m_orderStatus;

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
    bool isValid = false;      /**< Prüfergebnis */
    qint64 or_id = -1;         /**< Order Id */
    qint64 cu_id = -1;         /**< Customer Id */
    qint64 in_id = -1;         /**< Invoice Id */
    QString de_id = QString(); /**< Delivery Id */
  };
  const Idset identities();

  /**
   * @brief Erstelle Datenkopf für Druckaufträge
   * @param oid - Order id
   *
   * Erstelle mit Idset und einer SQL Anfrage an inventory_orders Tabelle ein
   * Json object und füge die SQL Daten den Idest Feldern hinzu.
   *
   * Wird von Rechung, Erinnerung, Mahnung und E-Mail Dialogen benötigt!
   *
   * @sa identities()
   * @code
   * QJsonObject({
   *    "order_id" : qint64,
   *    "customer_id" : qint64,
   *    "invoice_id" : qint64,
   *    "delivery_id" : QString
   * })
   * @endcode
   */
  const QJsonObject createDialogData(qint64 oid = -1) const;

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
   *
   * SQL Abfrage auf die Artikel-Nr. durchführen und einfügen wenn der Bestand
   * größer als 0 ist.
   *
   * @see PgSQL::query_article_order_with_id.sql
   * @param aid = Article Id
   */
  bool addOrderTableArticle(qint64 aid);

  /**
   * @brief SQL-Abfragen von Artikeltabelle entnehmen.
   *
   * Wird von \b createSqlInsert und \b createSqlUpdate verwendet und erweitert
   * das SQL-Statement auf die Bestellten Artikel.
   *
   * @return SQL-Statements
   */
  const QString getOrderSqlArticleQuery();

  /**
   * @brief SQL Datenbank Status des Auftrags
   *
   * Wird verwendet um zu Prüfen ob der SQL-Eintrag geschlossen ist.
   *
   * Prüft wiefolgt ob die SQL Tabellen Werte gesetzt sind:
   * @li ASqlDataQuery:o_payment_status == AntiquaCRM::OrderPayment::PAYED
   * @li ASqlDataQuery:o_order_status == AntiquaCRM::OrderStatus::DELIVERED
   *
   * Wenn beide Werte ok sind wird true zurück gegeben!
   * @sa OrderStatusActionFrame::setStatusProtection
   */
  bool databaseOrderStatus();

  /**
   * @brief Kundennummer Suchen/Prüfen
   * @param obj = Daten
   * @param cid = Kundennummer
   * @code
   *  QJsonObject{
   *   c_firstname : QString
   *   c_lastname : QString
   *   c_postalcode : QString
   *   c_email_0 : QString
   *  }
   * @endcode
   * @return c_id
   */
  qint64 findCustomer(const QJsonObject &obj, qint64 cid = -1);

  /**
   * @brief Vorarbeiten für neuen Eintrag.
   *
   * Eingabe masken leeren und AntiquaCRM::ASqlDataQuery neu einlesen.
   */
  bool prepareCreateEntry();

  /**
   * @brief Setze Standard Werte
   *
   * Es werden Standard Werte für Felder gesetzt die bei einem Neu erstellen
   * noch nicht gesetzt sind aber immer Erforderlich sind!
   */
  void setDefaultValues();

private Q_SLOTS:
  /**
   * @brief Speichern einleiten
   *
   * Prüfung auf "o_id" - wenn vorhanden wird \b createSqlUpdate ausgeführt
   * andernfalls \b createSqlInsert.
   */
  void setSaveData() override;

  /**
   * @brief Beenden Prüfung durchführen
   *
   * Prüft ob Fenster den Modified Status enthalten und gibt bei Bedarf ein
   * Warnungs Fenster aus.
   *
   * Wenn Erflogreich wird @sa setFinalLeaveEditor aufgerufen.
   */
  void setCheckLeaveEditor() override;

  /**
   * @brief Beenden und aufräumen
   * @param force - Beenden erwingen bei Abbrechen/Ignorieren
   *
   * Führt eingige Aufräum arbeiten durch.
   * @li Artikel Tabelle leeren!
   * @li Eingabe masken leeren.
   * @li Abschliessend - Gehe zurück zur Hauptsansicht
   */
  void setFinalLeaveEditor(bool force = true) override;

  /**
   * @defgroup UNUSED
   * @{
   */
  void setStorageCompartments() override{/* unused */};
  void setLoadThumbnail(qint64) override{/* unused */};
  void setRemoveThumbnail(qint64) override{/* unused */};
  void setImportEditImage() override{/* unused */};
  /**
   * @}
   */

  /**
   * @brief Dialog zum erstellen von E-Mail Nachrichten
   * @param caller - PgSQL::ui_template_body.tb_caller
   */
  void createMailMessage(const QString &caller);

  /**
   * @brief Dialog zum erstellen und Drucken eines Lieferscheins
   */
  void createPrintDeliveryNote();

  /**
   * @brief Dialog zum erstellen und Drucken einer Rechnung
   */
  void createPrintInvoice();

  /**
   * @brief Dialog zum erstellen und Drucken einer Zahlungserinnerung
   */
  void createPrintPaymentReminder();

  /**
   * @brief Dialog zum Suchen und Einfügen eines Artikels.
   */
  void openSearchInsertArticle();

public Q_SLOTS:
  /**
   * @brief Datenfelder zurücksetzen
   * @sa importSqlResult
   */
  void setRestore() override;

public:
  explicit OrdersEditor(QWidget *parent = nullptr);
  virtual ~OrdersEditor() override;

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
   * @param cid - Kunden Nummer
   * @sa prepareCreateEntry
   * @sa setDefaultValues
   */
  bool createNewOrder(qint64 cid);

  /**
   * @brief Auftrag aus Json Object erstellen
   * @param object - Datenstruktur
   * @sa prepareCreateEntry
   * @sa setDefaultValues
   * @code
   *  QJsonObject{
   *    ACTION : QString  -- Aktions typ
   *    PRORDER : QString -- Provider ID sql::pr_order
   *    ARTICLES : QStringList -- Artikel Liste
   *  }
   * @endcode
   *
   * Liest bei einem Provider-Import aus Tabelle "provider_orders" mit "PRORDER"
   * die aktuelle Bestell-/Kunden- und Artikeldaten aus.
   * Fügt die ermittelten Daten in die Eingabefelder ein.
   */
  bool createCustomEntry(const QJsonObject &object) override;
};

#endif // ANTIQUACRM_PLUGIN_ORDERSEDIT_H
