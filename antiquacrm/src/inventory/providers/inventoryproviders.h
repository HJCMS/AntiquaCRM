// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERS_H
#define INVENTORY_PROVIDERS_H

#include "inventory.h"
#include <AntiquaInterface>
#include <SqlCore>

#include <QObject>
#include <QTabWidget>
#include <QWidget>

class ProvidersToolBar;
class ProvidersTreeView;
class ProvidersPageView;

/**
 * @class InventoryProviders
 * Primäre Klasse für das Dienstleister Auftrags System
 */
class InventoryProviders final : public Inventory {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Wird beim Start gesetzt.
   * Soll verhindern das die Plugins mehrfach geladen werden.
   */
  bool firstStart = false;

  /**
   * @brief Kundennummer des Aktuellen Fensters
   */
  int customerId = -1;

  /**
   * @brief SQL Abfragen
   */
  HJCMS::SqlCore *m_sql;

  /**
   * @brief Dienstleister Liste
   */
  QStringList p_providerList;

  /**
   * @brief Alle geladenen Plugins
   */
  QList<Antiqua::Interface *> p_iFaces;

  /**
   * @brief Alle Dienstleisteraufträge hier anzeigen.
   */
  ProvidersPageView *m_pageView;

  /**
   * @brief Liste der Dienstleisteraufträge
   */
  ProvidersTreeView *m_listView;

  /**
   * @brief Einfacher Toolbar mit den wichtigsten Aktionen
   */
  ProvidersToolBar *m_toolBar;

  /**
   * @brief Wird hier nicht eingesetzt ...
   */
  void openEditor(const QString &){/* Unused */};

  /**
   * @brief Einfache Fensterabfrage, bei vorhanden in den Vordergrund schieben.
   * Suche nach vorhandenen Tab mit dem Objektnamen Wenn eines gefunden wird
   * dann nach vorne holen und "true" zurück geben.
   * @note Die Identifizierung der Objektnamen erfolgt
   *  durch die Bestellnummer des Dienstleisters!
   */
  bool tabExists(const QString &id);

  /**
   * @brief Nachricht über Verfügbarkeit
   * @param articleId - Artikel Nummer
   * @param count     - Verfügbarer Bestand
   */
  void statusMessageArticle(int articleId, int count = 0);

private Q_SLOTS:
  /**
   * @brief Wird hier nicht eingesetzt ...
   */
  void searchConvert(const QString &){/* Unused */};

  /**
   * @brief Suche alle offenen Tabs und sendet ein Update an TreeWidget
   * Wird von Signal @ref ProvidersToolBar::s_refresh aufgerufen.
   * Durchläuft alle Einträge von @ref p_iFaces und stellt eine Netzwerk
   * abfrage @ref Antiqua::Interface::queryMenueEntries
   * @note Die Signale wurden bereits in @ref loadInterfaces Registriert!
   */
  void searchConvert();

  /**
   * @brief Beim Aktuellen Tab den Kunden ermitteln!
   * @note Zweckentfremdung damit nicht ungenutzt!
   * Wird von Signal @ref ProvidersToolBar::s_customerAction aufgerufen.
   * @li Es wird vom aktuell aktivierten Tab das InterfaceWidget ermittelt.
   * @li Im nächsten Schritt wird mit @ref InterfaceWidget::getCustomerId nach
   *     der Kundennummer gefragt.
   * @li Ist diese vorhanden wird mit @ref createEditCustomer ein Anfrage an den
   *     Kunden Editor gesendet.
   * @li Wenn nicht stelle Anfrage @ref InterfaceWidget::createCustomerDocument
   */
  void openTableView();

  /**
   * @brief Dienstleister Initialisieren und Bestellerliste setzen.
   * Wird nur einmal beim Start aufgerufen und die Variable @ref firstStart
   * gesetzt damit das Plugin nicht zweimal geladen wird.
   * @note Hier werden alle verfügbaren Dienstleister Plugins geladen.
   * @return bool
   */
  bool loadInterfaces();

  /**
   * @brief Dienstleister Seite aufrufen
   */
  void queryProviderPage(const QString &provider);

  /**
   * @brief Verarbeitet das Signal @ref ProvidersTreeView::s_queryOrder
   * Primärer Slot in dem Dienstleister Name und Bestellnummer zum erstellen
   * eines Neunen Interface Fensters verwendet wird.
   * Hier wird geprüft ob ein Fenster mit der Id"orderId" existiert.
   * @li Ist es vorhanden, wird es mit @ref tabExists nach vorne geholt.
   * @li Wenn nicht, wird ein neues @ref Antiqua::InterfaceWidget erstellt
   *     und die Signale für dieses Fenster registriert.
   * @li Zum abschluss wird mit @ref createOrderRequest eine Anfrage an das
   *     Fenster gestellt.
   *
   * @param provider  - Dienstleistername
   * @param orderId   - Auftrags Id des Dienstleisters
   */
  void queryOrder(const QString &provider, const QString &orderId);

  /**
   * @brief Kundennummer prüfen ...
   * Wenn die Kundennummer enthalten ist, diese in @ref customerId schreiben und
   * den Erstellen Knopf aktivieren! Danach das Signal::openEditCustomer an der
   * Workspace weitergeben, damit der Kunden-Editor geöffnet wird. Dieser
   * Schritt ist Notwendig damit die Adressdaten überprüft werden.
   * @note Es ist Aufgabe des Benutzers die Daten zu prüfen!
   */
  void createEditCustomer(int cid);

  /**
   * @brief Kundendatensetz erstellen und absenden
   * @ingroup Providers SQL Statements
   * Erstelle SQL Insert für "PgSQL:Table:customers"
   */
  void createNewCustomer(const QJsonDocument &doc);

  /**
   * @brief Kundenabfrage erstellen und absenden
   * @ingroup Providers SQL Statements
   * Erstellt SQL @ref queryCustomerExists Abfrage aus den Übermittelten Daten.
   * @see Antiqua::InterfaceWidget::checkCustomer
   */
  void createQueryCustomer(const QJsonDocument &doc);

  /**
   * @brief Suche Artikelbestand von Tabelle Bestellungen.
   * Wird vom Plugin das Signal "checkArticleIds" gesendet. Werden hier
   * alle Artikel Nummern mit einer SQL Stückzahlanfrage versehen und
   * verarbeitet. Ist einer der Artikel @b nicht Verfügbar, wird der Auftrag
   * erstellen Knopf mit @ref ProvidersToolBar::enableOrderButton Deaktiviert.
   * Es wird Zusätzlich eine Info mit @ref statusMessageArticle erzeugt.
   */
  void checkArticleExists(QList<int> &);

  /**
   * @brief Knopf Auftrag erstellen verarbeiten.
   * Wird von Signal @ref ProvidersToolBar::s_createOrder aufgerufen und sucht
   * zuerst nach @ref customerId, prüft ob diese Gesetzt ist. Danach wird die Id
   * mit @ref ProvidersPageView::currentPage() verglichen. Wenn ok, erstelle mit
   * @ref Antiqua::InterfaceWidget::getProviderOrder den benötigten Datensatz
   * und sende das Signal @ref createOrder an das Eltern Fenster.
   */
  void createEditOrders();

  /**
   * @brief Rückgabe Daten von Interface verarbeiten.
   * Wenn @ref Antiqua::Interface::listResponse antwortet. Die Daten hier
   * einlesen und an @ref ProvidersTreeView::addOrder senden. Es wird
   * folgendes Format erwartet und alle Parameter müssen gesetzt sein.
   *
   * @code
   * QJsonObject({
   *  "provider":"Providername",
   *  "items":[{
   *    "datum":"Datum/Zeit in ISO 8601 Format",
   *    "id":"Provider Bestellungs Id"
   *  }]
   * })
   * @endcode
   *
   * @li provider - Identifizierung der Hauptgruppe
   * @li items - Einträge mit Datum  und Id
   *
   * @param doc - Siehe Formatvorgabe
   */
  void readOrderList(const QJsonDocument &doc);

  /**
   * @brief Anfrage Antwort Status
   * @param errors  true = Antwortete mit fehlern!
   */
  void hasResponsed(bool errors);

Q_SIGNALS:
  /**
   * @brief Sende SIGNAL in Kunde im Editor öffnen.
   * @see InventoryCustomers::editCustomer
   */
  void openEditCustomer(int cid);

  /**
   * @brief Sende SIGNAL an Auftrag erstellen.
   * @see InventoryOrders::createOrder
   */
  QT_DEPRECATED void openEditOrder(int cid);

  /**
   * @brief Signal - Auftrag erstellen und an Elternfenster senden.
   * @see InventoryOrders::createOrder
   */
  void createOrder(const ProviderOrder &);

public Q_SLOTS:
  /**
   * Wenn das Tab Initialisiert wird, eine Abfrage für die aktuelle Listansicht
   * ausführen.
   * @note Hier wird @ref firstStart gesetzt und @ref loadInterfaces aufgerufen!
   */
  void onEnterChanged();

public:
  /**
    @param index  Ist der Tab-Index
    @param parent TabWidget
  */
  explicit InventoryProviders(QWidget *parent = nullptr);

  /**
   * @brief Sende Bestandsänderung
   * @param articleId - Artikel Nummer
   * @param count     - Aktuelle Anzahl im Bestand
   */
  bool updateArticleCount(int articleId, int count);

};

#endif // INVENTORY_PROVIDERS_H
