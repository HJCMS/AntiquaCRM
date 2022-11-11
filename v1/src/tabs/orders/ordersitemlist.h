// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef ANTIQUACRM_ORDERSITEMLIST_H
#define ANTIQUACRM_ORDERSITEMLIST_H

#include <AntiquaCRM>
#include <AntiquaWidgets>
#include <QDoubleSpinBox>
#include <QGroupBox>
#include <QLabel>
#include <QObject>
#include <QSpinBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QWidget>

class OrdersPaymentTable;

class OrdersItemList final : public QWidget {
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
   * @brief Notifications
   */
  QGroupBox *m_addArticleGroup;
  QLabel *m_notifier;

  /**
   * @brief Sucheingabe für Artikel ID
   */
  IntSpinBox *m_insertID;

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

private Q_SLOTS:
  void setSearchArticle();

Q_SIGNALS:
  /**
   * @brief Signal Änderungen am Objekt
   */
  void hasModified(bool);

  /**
   * @brief Suche Artikel mit ID
   */
  void searchArticleById(qint64);

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
   * @brief Erstellt einen Roten Text neben dem Article Einfügen!
   * Wenn der kein Text dann wird die Ausgabe geleert!
   */
  void setAlertMessage(const QString &message = QString());

  /**
   * @brief Erstellt eine neuen Tabellen Eintrag
   */
  void insertArticle(const AntiquaCRM::OrderArticleItems &article);

  /**
   * @brief Änderungen
   */
  void setModified(bool);

public:
  explicit OrdersItemList(QWidget *parent = nullptr);

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
  bool importPayments(const QList<AntiquaCRM::OrderArticleItems> &list);

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
};

#endif // ANTIQUACRM_ORDERSITEMLIST_H
