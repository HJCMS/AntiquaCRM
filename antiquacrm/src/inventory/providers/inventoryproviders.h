// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_PROVIDERS_H
#define INVENTORY_PROVIDERS_H

#include "inventory.h"
#include <AntiquaCRM>

#include <QObject>
#include <QSplitter>
#include <QTabWidget>
#include <QWidget>

class ProvidersToolBar;
class ProvidersTreeView;

class BF_JSonQuery;

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

  QStringList p_providerList;

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
   * @brief Eine Bestellung einfügen
   * Such nach vorhandenen Tab mit dem Objektnamen Wenn eines gefunden wird dann
   * nach vorne holen und "false" zurück geben. Nur bei neuen Tabs wird "true"
   * zurück gegeben damit die Signale nicht doppelt Registriert werden.
   * @note Die Identifizierung der Objektnamen erfolgt
   *  durch die Bestellnummer des Dienstleisters!
   */
  bool addTab(QWidget *);

  /**
   * @brief Erstelle Daten für AuftragsEditor
   * Wenn alle Datensätze Aufgearbeitet und Vorhanden sende Daten an den
   * Workspace.
   */
  void openEditor(const QString &customerId);

private Q_SLOTS:
  void searchConvert(const QString &){/* dummy */};
  void searchConvert(){/* dummy */};
  void openTableView(){/* dummy */};

  /**
   * @brief Beim Aktuellen Tab den Kunden ermitteln!
   */
  void checkCustomer();

  /**
   * @brief Dienstleister Initialisieren und Bestelliste setzen.
   * Wird nur einmal beim Start aufgerufen und die Variable @ref firstStart
   * gesetzt. Danach muss der Refresh Knopf des ToolBars verwendet werden.
   * @return bool
   */
  bool initProviders();

  void readBFOrders(const QJsonDocument &doc);

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
   * @brief Knopf Signal verarbeiten und an @ref openEditor weiterleiten.
   */
  void createEditOrders();

Q_SIGNALS:
  /**
   * @brief Sende SIGNAL in Editoransicht öffnen.
   */
  void openEditCustomer(int cid);

  /**
   * @brief Sende SIGNAL an Auftragseditor.
   */
  void createEditOrder(/* TODO */);

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
