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
  /**
   * @brief Speichern erfolgreich Nachricht anzeigen.
   */
  bool showSuccessFully = true;

  /**
   * @brief Auftragsnummer
   */
  SerialID *o_id;

  /**
   * @brief Auftragsstatus
   */
  OrderStatusBox *o_order_status;

  /**
   * @brief Zahlungsstatus
   */
  OrdersPaymentBox *o_payment_status;

  /**
   * @brief Kunden ID
   */
  SerialID *o_costumer_id;

  /**
   * @brief Kundenandresse
   */
  TextField *m_costumer_address;

  /**
   * @brief Lieferservice
   */
  DeliveryService *o_delivery_service;

  /**
   * @brief Dienstleister
   */
  LineEdit *o_provider_name;

  /**
   * @brief Bestellinformationen
   */
  TextField *o_provider_order;

  /**
   * @brief Benachrichtigungen
   * cron job system
   */
  BoolBox *o_notify;

  /**
   * @brief Sperren
   */
  BoolBox *o_locked;

  /**
   * @brief Abschliessen
   */
  BoolBox *o_closed;

  /**
   * @brief Zeitstempel
   */
  LineEdit *o_modified;

  /**
   * @brief Standard ActionsBar
   */
  EditorActionBar *m_actionBar;

  /**
   * @brief Liste der Bestellten Artikel
   */
  OrdersItemList *m_paymentList;

  /**
   * @brief Wenn ein Auftrag hinzugeügt wurde!
   * Hier die "a_payment_id" einfügen!
   */
  int last_payment_id = -1;

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
   * Wenn (isRequired != isValid) ist, dann abbrechen.
   * Bei Fehlerne wird der Hash geleert und eine PoUp-Meldung ausgegeben!
   * @return Nur wenn alle Bedingungen erfüllt sind!
   */
  const QHash<QString, QVariant> createSqlDataset();

  /**
   * @brief SQL and die Datenbank senden ...
   */
  bool sendSqlQuery(const QString &);

  /**
   * @brief Erstelle SQL für Tabelle "article_orders"
   */
  bool createSqlArticleOrder();

  /**
   * @brief Erstelle SQL Update für "inventory_orders"
   */
  void createSqlUpdate();

  /**
   * @brief Erstelle SQL Insert für "inventory_orders"
   */
  void createSqlInsert();

  /**
   * @brief Füge Daten in Datenfeld ein
   * Sucht mit Objektname nach dem Eingabeobjekt und
   * fügt die Daten ein.
   * @param key      - (SQL Feldname = Objektname)
   * @param value    - Eingabewert
   * @param required - (SQL not null = isRequired)
   */
  void setData(const QString &key, const QVariant &value,
               bool required = false);

  /**
   * @brief Editor vorbereiten
   *  @li Aufruf von @ref setInputList
   *  @li Aktiviere das Editor Widget
   *  @li Rufe @ref setModified auf
   *  @li Initialisiere Datensätze für die Eingabefelder
   *      welche ebenfalls aus der Datenbank lesen.
   */
  void initDefaults();

  /**
   * @brief Lese Daten aus Tabelle "article_orders"
   * Die Daten werden hier für @class OrdersItemList vorbereitet
   * und an @ref OrdersItemList::importPayments übergeben.
   * @param oid - Auftrags ID
   * @param cid - Kunden ID
   */
  const QList<OrderArticle> getOrderArticles(int oid, int cid);

  /**
   * Erstellt, sendet SQL Abfrage für Kunden und Lieferadresse.
   * @warning Darf nicht direkt aufgerufen werden, @see findCostumer
   * @param cid - Kundennummer
   */
  bool getCostumerAddress(int cid);

private Q_SLOTS:
  /**
   * Wird aufgerufen wenn @ref SerialID::s_serialChanged ausgelöst wird.
   * Weil die Datenfeld abfragen aus mehreren SQL Abfragen bestehen muss
   * hier erst Sichergestellt werden das die Kunden Nummer vorhanden ist.
   * @param cid - Kundennummer
   */
  void findCostumer(int cid);

  /**
   * @brief Wird von Signal @ref OrdersItemList::searchArticle ausgelöst.
   * Suche mit Artikelnummern nach Datensatz in den Inventar Tabellen.
   * Bei erfolg erstelle ein @ref OrderArticle und füge den Eintrag
   * mit @ref OrdersItemList::addArticleRow in die Bestellliste ein.
   * Der Ablauf ist:
   *  @li Abfrage mit @ref inventoryArticle
   *  @li Erstelle @ref OrderArticle
   *  @li Sende an @ref OrdersItemList::addArticleRow
   *
   * @param aid - Artikelnummern
   */
  void findArticle(int aid);

  /**
   * @brief Datensatz aus Tabelle "article_orders" entfernen.
   * Der Ablauf ist:
   *  @li Nehme mit @ref OrdersItemList::getTableRow den gewählten Datensatz.
   *  @li Ermittle "a_payment_id" und "a_article_id" aus dem rückgabe Hash.
   *  @li Erstelle eine Benutzeranfrage ob gelöscht werden soll.
   * Wenn ja:
   *  @li Entferne Zeile aus Bestellliste @ref OrdersItemList::removeTableRow
   *  @li Erstelle mit @ref paymentRemove das SQL Statement.
   *  @li Sende SQL and @ref sendSqlQuery
   *  @li Bei erfolg Signale @ref postMessage und @ref s_isModified auslösen.
   *  @li Final: Speicher freigeben
   *
   * @param row
   */
  void findRemoveTableRow(int row);

  /**
   * @brief Daten Speichern
   * Überprüfung ob Auftrags ID vorhanden ist.
   *  @li Ja - Dann @ref createSqlUpdate aufrufen
   *  @li Nein - @ref createSqlInsert
   */
  void saveData();

  /**
   * @brief Prüfungen vor dem verlassen durchführen
   */
  void checkLeaveEditor();

  /**
   * @brief Aufräumen und beenden
   */
  void finalLeaveEditor();

  /**
   * @brief Drucker Dialog öffnen
   */
  void openPrinterDialog();

  /**
   * @brief SQL Spalte "o_closed" true setzen.
   * Wenn diese Aktion ausgeführt wird ist es nur noch
   * für die Buchhaltung sichtbar und der Editor wird verlassen!
   * Der Auftrag ist somit abgeschlossen!
   */
  void createCloseOrder(bool b = false);

public Q_SLOTS:
  /**
   * @brief Alle Datenfelder zurück setzen
   */
  void restoreDataset();

  /**
   * @brief Hinweise an Benutzer
   */
  void showMessagePoUp(const QString &);

Q_SIGNALS:
  /**
   * @brief Statusbar Nachrichten
   */
  void postMessage(const QString &);

  /**
   * @brief Änderungen an Elternfenster
   */
  void isModified(bool);

public:
  explicit OrderEditor(QWidget *parent = nullptr);

  /**
   * @brief Füge Artikel ID zur Prüfung ein!
   */
  void addArticleId(int articleId);

  /**
   * @brief Eintrag Bearbeiten
   * Erstellt SQL mit Auftragsnummer und sendet Anfrage
   * an Datenbank, bei erfold werden die Daten eingefügt.
   * @param oid Auftrags ID
   */
  void openUpdateOrder(int oid = -1);

  /**
   * @brief Einen Auftrag mit Kundennummer erstellen!
   * @param cid - Kunden ID
   */
  void openCreateOrder(int cid = -1);
};

#endif // INVENTORY_ORDEREDITOR_H
