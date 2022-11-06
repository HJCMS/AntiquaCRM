// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERSITEMLIST_H
#define ANTIQUACRM_ORDERSITEMLIST_H

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QDoubleSpinBox>
#include <QLineEdit>
#include <QObject>
#include <QSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QWidget>

class OrdersPaymentTable;

class OrdersItemList : public QWidget {
  Q_OBJECT
  Q_PROPERTY(bool modified READ isModified WRITE setModified NOTIFY hasModified)

private:
  bool modified;
  qreal minPrice;
  qreal maxPrice;
  QByteArray currency;

  AntiquaCRM::ASettings *m_cfg;

  /**
   * @brief Tabelle für Artikel einfügen
   */
  OrdersPaymentTable *m_table;

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
  AntiquaCRM::AProviderOrders p_payments;

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
  QSpinBox *addCount(const QVariant &val, int row, int max = 1);
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
   * @brief Signal Änderungen am Objekt
   */
  void hasModified(bool);

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

  /**
   * @brief Benachrichtigung Doppelter Eintrag
   */
  void duplicateEntry();

public Q_SLOTS:
  /**
   * @brief Such/Einfügen Felder nach dem einfügen leeren.
   * @note OrderArticle wird auch geleert!
   */
  void clearSearchInput();

  /**
   * @brief Tabellen Zeilen zurücksetzen!
   */
  void clearTable();

  /**
   * @brief Änderungen
   */
  void setModified(bool);

public:
  OrdersItemList(QWidget *parent = nullptr);

  /**
   * @brief Füge Artikel ID in die Suche ein!
   */
  void insertSearchId(int articleId);

  /**
   * @brief Gibt es Änderungen am Objekt
   */
  inline bool isModified() { return modified; }

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
  void importPayments(const AntiquaCRM::AProviderOrders &list);

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
   *  Datenfelder wie order_id, artikel_id und customer_id sind
   *  hierin @b nicht enthalten!
   */
  void addArticleRow(const AntiquaCRM::AProviderOrder &set);
};

#endif // ANTIQUACRM_ORDERSITEMLIST_H
