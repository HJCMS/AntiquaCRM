// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERS_H
#define INVENTORY_PROVIDERS_H

#include "inventory.h"
#include <AntiquaCRM>
#include <AntiquaInterface>
#include <SqlCore>

#include <QObject>
#include <QSplitter>
#include <QTabWidget>
#include <QWidget>

class ProvidersToolBar;
class ProvidersTreeView;

/**
 * @group Providers
 * @class InventoryProviders
 * Primäre Klasse für das Auftrags System
 */
class InventoryProviders : public Inventory {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  bool firstStart = false;
  int customerId = -1;

  HJCMS::SqlCore *m_sql;

  QStringList p_providerList;

  QList<Antiqua::Interface *> p_iFaces;

  /**
   * @brief Hauptansicht
   */
  QSplitter *m_splitter;

  /**
   * @brief Fenster gruppierung
   */
  QTabWidget *m_pageView;

  /**
   * @brief Liste der Dienstleister Bestellungen
   */
  ProvidersTreeView *m_listView;

  /**
   * @brief ToolBar
   */
  ProvidersToolBar *m_toolBar;

  /**
   * @brief Aktuelles Tab Widget ermitteln.
   */
  Antiqua::InterfaceWidget *currentTabWidget();

  /**
   * @brief Erstelle Daten für AuftragsEditor
   * Wenn alle Datensätze Aufgearbeitet und Vorhanden sende Daten an den
   * Workspace.
   */
  void openEditor(const QString &) {
    /* Disabled in this class */
  };

  /**
   * @brief Eine Bestellung einfügen
   * Such nach vorhandenen Tab mit dem Objektnamen Wenn eines gefunden wird dann
   * nach vorne holen und "true" zurück geben.
   * @note Die Identifizierung der Objektnamen erfolgt
   *  durch die Bestellnummer des Dienstleisters!
   */
  bool tabExists(const QString &id);

private Q_SLOTS:
  /**
   * @brief Suche ...
   * @note Wird hier nicht eingesetzt
   */
  void searchConvert(const QString &){};

  /**
   * @brief Suche alle offenen Tabs und sende ein Update and TreeWidget
   */
  void searchConvert();

  /**
   * @brief Beim Aktuellen Tab den Kunden ermitteln!
   * @note Zweckentfremdung damit nicht ungenutzt!
   */
  void openTableView();

  /**
   * @brief Dienstleister Initialisieren und Bestellerliste setzen.
   * Wird nur einmal beim Start aufgerufen und die Variable @ref firstStart
   * gesetzt.
   * @return bool
   */
  bool loadInterfaces();

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
   */
  void createNewCustomer(const QJsonDocument &doc);

  /**
   * @brief Kundenabfrage erstellen und absenden
   */
  void createQueryCustomer(const QJsonDocument &doc);

  /**
   * @brief Knopf Signal verarbeiten und an @ref openEditor weiterleiten.
   */
  void createEditOrders();

  /**
   * @brief readOrderList
   * @code
   * Erwartet dieses Format:
   * QJsonObject({
   *  "items":[{
   *    "datum":"2022-05-14T18:10:23",
   *    "id":"BF-2251499"
   *  }],
   *  "provider":"Provider ObjekName"
   *  })
   */
  void readOrderList(const QJsonDocument &doc);

Q_SIGNALS:
  /**
   * @brief Sende SIGNAL in Editoransicht öffnen.
   */
  void openEditCustomer(int cid);

  /**
   * @brief Sende SIGNAL an Auftragseditor.
   */
  void openEditOrder(int cid);

  /**
   * @brief Auftrag erstellen and Elternfenster senden ...
   */
  void createOrder(const ProviderOrder &);

public Q_SLOTS:
  /**
   * Wenn das Tab in den Vordergrund geht, eine Abfrage für die aktuelle
   * Listansicht auf dass StackedWidget, welches zu sehen ist, machen.
   */
  void onEnterChanged();

public:
  /**
    @param index  Ist der Tab-Index
    @param parent TabWidget
  */
  explicit InventoryProviders(QWidget *parent = nullptr);
};

#endif // INVENTORY_PROVIDERS_H
