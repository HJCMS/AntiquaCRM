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
   * Such nach vorhandenen Tab mit dem Objektnamen
   * Wenn eines gefunden wird dann nach vorne holen
   * und "false" zurück geben. Nur bei neuen Tabs
   * wird "true" zurück gegeben damit die Signale
   * nicht doppelt Resgistriert werden.
   * @note Die Identifizierung der Objektnamen erfolgt
   *  durch die Bestellnummer des Dienstleisters!
   */
  bool addTab(QWidget *);

  void openEditor(const QString &){/* TODO */};

Q_SIGNALS:
  /**
   * @brief Sende SIGNAL in Editoransicht öffnen.
   */
  void openEditCustomer(int cid);

  /**
   * @brief Sende SIGNAL an Auftragseditor.
   */
  void createEditOrder(/* TODO */);

private Q_SLOTS:
  void searchConvert(const QString &search);
  void searchConvert();
  void openTableView();

  void readProviderOrders(const QJsonDocument &doc);

  void queryOrder(const QString &provider, const QString &orderId);

  /**
   * @brief Anhand der Kundennummer bestimmen
   *  ob der Erstellen Knopf aktiviert wird!
   */
  void createEditCustomer(int cid);

  /**
   * @brief Knopf Signal verarbeiten
   */
  void createEditOrders();

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
