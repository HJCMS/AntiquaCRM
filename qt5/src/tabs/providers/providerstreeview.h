// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef ANTIQUACRM_PROVIDERSTREEVIEW_H
#define ANTIQUACRM_PROVIDERSTREEVIEW_H

#include <AntiquaCRM>
#include <QContextMenuEvent>
#include <QDateTime>
#include <QLabel>
#include <QObject>
#include <QPair>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QWidget>

/**
 * @brief Ansicht: Dienstleister Bestellungen
 * @ingroup _providers
 */
class ProvidersTreeView final : public QTreeWidget {
  Q_OBJECT

private:
  /**
   * @brief OrderId für Kontextmenüs
   */
  QPair<QString, QString> p_pair;

  /**
   * @brief Unterklasse für die Einträge
   */
  struct TreeOrderItem {
    QString id;                     /**< Bestellnummer */
    QDateTime datetime;             /**< Auftrag entgegen genommen */
    QString buyer;                  /**< Käufername des Dienstleisters */
    AntiquaCRM::OrderStatus status; /**< Aktueller Auftrags-Status */
  };

  /**
   * @brief Datumskonvertierung
   */
  const QString setDateString(const QDateTime &dt) const;

  /**
   * @brief Suche Dienstleister
   */
  QTreeWidgetItem *getParent(const QString &name);

  /**
   * @brief Nehme eine Bestellung
   */
  QTreeWidgetItem *getChild(const QString &provider, const QString &id);

  /**
   * @brief Dienstleister hinzufügen!
   */
  void addProvider(const QString &provider);

  /**
   * @brief Dienstleister Kontexteinträge
   */
  void contextMenuEvent(QContextMenuEvent *) override;

private Q_SLOTS:
  /**
   * @brief Bestellung ausgewählt und gültig dann @ref sendQueryOrder auslösen!
   */
  void itemSelected(QTreeWidgetItem *, int);

  /**
   * @brief Statusänderung mit Icon visualisieren.
   */
  void updateOrderStatus(QTreeWidgetItem *item, AntiquaCRM::OrderStatus status);

  /**
   * @brief Dienstleister liste leeren!
   */
  void clearProvider(const QString &provider);

  /**
   * @brief Eine Bestellung einfügen
   */
  void addOrder(const QString &pro,
                const ProvidersTreeView::TreeOrderItem &data);

  /**
   * @brief Darstellung der Einträge auf Bestand anpassen!
   * @param provider
   * @param orderId
   * @param status
   */
  void updateItemStatus(const QString &provider, const QString &orderId,
                        AntiquaCRM::OrderStatus status = AntiquaCRM::OPEN);

  /**
   * @brief Öffnet den @class ImportsEdit Dialog
   */
  void editProviderData();

  /**
   * @brief Bestellnumber in das Clipboard kopieren
   */
  void copyProviderId();

Q_SIGNALS:
  /**
   * @brief Eine Bestellung wurde ausgewählt ...
   * @param provider  - Dienstleister Name
   * @param id        - Bestellnummer der Dienstleisters
   */
  void sendQueryOrder(const QString &provider, const QString &id);

public Q_SLOTS:
  /**
   * @brief Eintrag entfernen mit ...
   * @param provider - Dienstleister Name
   * @param id       - Bestellnummer der Dienstleisters
   */
  void removeOrder(const QString &provider, const QString &id);

  /**
   * @brief Alle Bestellungen sortieren!
   */
  void sortAndResize();

  /**
   * @brief Alle Bestellungen entfernen, erneut aus Datenbank lesen.
   * @note Nur in dieser Methode wird aus der Datenbank gelesen!
   */
  void loadUpdate();

public:
  explicit ProvidersTreeView(QWidget *parent = nullptr);

  /**
   * @brief Doppelte Einträge vermeiden
   * @param provider - Dienstleister
   * @param id       - Bestellnummer
   */
  bool exists(const QString &provider, const QString &id);

  /**
   * @brief Anzahl der Bestellungen
   * @note Toplevel Einträge werden \b nicht mitgezählt!
   */
  int ordersCount();
};

#endif // ANTIQUACRM_PROVIDERSTREEVIEW_H
