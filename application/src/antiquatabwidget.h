// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_TABWIDGET_H
#define ANTIQUACRM_TABWIDGET_H

#include <AntiquaTabs>
#include <AntiquaWidgets>
#include <QJsonObject>
#include <QMainWindow>
#include <QMap>
#include <QObject>
#include <QTabWidget>
#include <QWidget>

namespace AntiquaCRM {
class AReceiver;
};

class AntiquaTabBar;

/**
 * @ingroup ui
 * @brief Verwaltungs Klasse Registerkarten
 * @class AntiquaTabWidget
 * @ingroup window
 */
class AntiquaTabWidget final : public QTabWidget {
  Q_OBJECT

private:
  AntiquaCRM::AReceiver *m_server;
  AntiquaTabBar *m_tabBar;
  TabViews *m_views;           /**< Views */
  TabStatistics *m_statistics; /**< Statistics */
  TabReports *m_reports;       /**< Reports */
  QWidget *m_testing;

  /**
   * @brief Der lokalen Socketserver starten.
   */
  bool createSocketListener();

  /**
   * @brief Primäre Methode zum einfügen von Registerkarten.
   * @param name - Ist Identisch mit @ref availableTabs().key()
   * @list Es können nur Karten geöffnet werden die hier Registriert sind!
   * @li Ist die Karte bereits geöffnet, hole Sie nach vorne.
   * @li Ist die Karte hier Registriert, Initialisiere Sie und einfügen.
   */
  bool addInventoryTab(const QString &name);

  /**
   * @brief qobject_cast auf TabWidget
   */
  inline Inventory *tabWidget(int index) const;

  /**
   * @brief Suche TabIndex mit Klassenbezeichner
   * @note Jede Registerkarte hat einen eindeutigen Index.
   * @see Konstruktor @ref Inventory
   */
  int indexByName(const QString &name) const;

private Q_SLOTS:
  void setAction(const QJsonObject &obj);
  void addViewsTab(const QString &query);
  void addStatisticsTab(const QString &query);
  void addReportsTab(const QString &query);
  void setTabChanged(int index);
  void setTabToClose(int index);
  void setTabToVisit(int index);

Q_SIGNALS:
  /**
   * @brief Statusnachrichten an Hauptfenster weiterleiten
   * @note Siehe AntiquaCRM::AReceiver
   */
  void sendInfoMessage(const QString &);
  void sendWarnMessage(const QString &);

  /**
   * @brief Änderungen an Hauptfenster senden.
   */
  void sendModified(bool);

  /**
   * @brief Sende eine Plugin Aktion
   */
  void sendPluginOperation(const QJsonObject &);

public Q_SLOTS:
  /**
   * @brief Tab suchen und nach vorne holen!
   */
  void setShowTab(const QString &tabId);

public:
  explicit AntiquaTabWidget(QMainWindow *parent = nullptr);

  /**
   * @brief Liste der Tabs
   * @note Wird von Menüklassen verwendet!
   */
  static const QMap<QString, QString> availableTabs();

  /**
   * @brief Alle Registerkarten schließen!
   * Wenn eine Karte im Editormodus ist, wird das schließen verweigert! Und die
   * Methode bricht ab! Wahr wird nur bei Erfolgreichem schließen aller
   * Registerkarten zurück gegeben!
   */
  bool beforeCloseAllTabs();

  /**
   * @brief TimerEvents und Sockets sauber runterfahren!
   */
  virtual ~AntiquaTabWidget();
};

#endif // ANTIQUACRM_TABWIDGET_H
