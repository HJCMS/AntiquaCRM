// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_ORDERSITEMLIST_H
#define INVENTORY_ORDERSITEMLIST_H

#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QObject>
#include <QSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QWidget>

#include <AntiquaCRM>

class ApplSettings;
class OrderPaymentsTable;

/**
 * @ingroup InventoryOrders
 * @class OrdersItemList
 */
class OrdersItemList : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  ApplSettings *m_cfg;

  /**
   * @brief Tabelle für Artikel einfügen
   */
  OrderPaymentsTable *m_table;

  /**
   * @brief Kontrollanzeige für das Ergebnis
   */
  QLineEdit *m_searchInfo;

  /**
   * @brief Sucheingabe für Artikel ID
   */
  QSpinBox *m_insertID;

  /**
   * @brief Temporärer Artikeldatensatz
   * Diese Klasse enthält den aktuellen Datensatz der gerade
   * eingefügt wird. Sie dient zur Vermeidung von doppelten
   * Einträgen und zur Verteilung der Daten auf mehrere Methoden.
   */
  OrderArticle p_payments;

  /**
   * @brief Standard Preisfeld erstellen
   */
  QDoubleSpinBox *addPrice(double val, int row);
  double getPrice(int row);

  /**
   * @brief Verkaufspreis (VK) erstellen
   */
  QDoubleSpinBox *addSellPrice(double val, int row);
  double getSellPrice(int row);

  /**
   * @brief Artikel Bestandsanzeige
   */
  QSpinBox *addCount(const QVariant &val, int row);
  int getCount(int row);

  /**
   * @brief Standard Feldzeilen: Artikel ID & Titel
   */
  QTableWidgetItem *createItem(const QVariant &val);

  /**
   * @brief Erstellt eine neuen Tabellen Eintrag
   * Zum erstellen werden die Daten von @ref p_article
   * verwendet.
   */
  void addTableRow();

private Q_SLOTS:
  /**
   * @brief Artikel mit den Eingabefeldern zusammenstellen.
   */
  void insertArticle();

  /**
   * @brief Löst nach einigen Abfragen das Signal @ref searchArticle aus.
   */
  void createSearchSignal();

Q_SIGNALS:
  /**
   * @brief Suche Artikel mit ID
   */
  void searchArticle(int);

  /**
   * @brief Nachricht an Elternfenster
   */
  void statusMessage(const QString &);

  /**
   * @brief Signal Tabellenzeile entfernen
   * @param row
   */
  void askToRemoveRow(int row);

public Q_SLOTS:
  /**
   * @brief Such/Einfügen Felder nach dem einfügen leeren.
   */
  void clearSearchInput();

  void clearTable();

public:
  OrdersItemList(QWidget *parent = nullptr);

  /**
   * @brief Tabellenreihe enfernen!
   * @param row
   */
  void removeTableRow(int row);

  /**
   * @brief Anzahl der Bestellartikel von Tabelle.
   */
  int payments();

  /**
   * @brief Bestellartikel aus SQL Abfrage Importieren
   */
  void importPayments(const QList<OrderArticle> &list);

  /**
   * @brief Erstellt einen Spaltendatensatz
   * Es wird ein Spaltendatensatz für @em "article_orders" erstellt.
   * Der Hashkey enthält die Tabellenfeldnamen der PgSQL Tabelle.
   * @note Felder, welche hier noch nicht gesetzt werden können,
   * sind mit dem Wert 0 versehen und müssen Später gesetzt werden!
   * @warning Wert 0 ist bei den FOREIGN KEYs Feldern "*_id" nicht zulässig!
   * @return Spaltenliste<QString:Feldname,QVariant:Wert>
   */
  const QHash<QString, QVariant> getTableRow(int row);

  /**
   * @brief Wenn ein Artikel gefunden wird dieser hier einfügen.
   * @note Hier wird nur ein Teildatensatz erstellt!
   *  Datenfelder wie order_id, artikel_id und costumer_id sind
   *  hierin @b nicht enthalten!
   */
  void addArticleRow(const OrderArticle &set);
};

#endif // INVENTORY_ORDERSITEMLIST_H
