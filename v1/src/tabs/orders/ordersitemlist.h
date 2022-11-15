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
class PurchaseTable;

class OrdersItemList final : public QWidget {
  Q_OBJECT

private:
  /**
   * @brief Tabelle für Artikel einfügen
   */
  PurchaseTable *m_table;

  /**
   * @brief Notifications
   */
  QGroupBox *m_addArticleGroup;
  QLabel *m_notifier;

  /**
   * @brief Sucheingabe für Artikel ID
   */
  IntSpinBox *m_insertID;

private Q_SLOTS:
  void setSearchArticle();

Q_SIGNALS:
  /**
   * @brief Suche Artikel mit ID
   */
  void searchArticleById(qint64);

  /**
   * @brief Benachrichtigung Doppelter Eintrag
   */
  void duplicateEntry();

  /**
   * @brief A new Article was add ...
   */
  void articleChanged();

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
   * @brief Importiert einen neuen Tabellen Eintrag
   */
  void importOrder(const AntiquaCRM::OrderArticleItems &article);

  /**
   * @brief Bestehende Artikel Einkäufe mit orderId einlesen!
   */
  void queryOrderArticles(qint64 orderId);

public:
  explicit OrdersItemList(QWidget *parent = nullptr);

  /**
   * @brief Füge Artikel ID in die Suche ein!
   */
  void insertSearchId(int articleId);

  /**
   * @brief Aktueller Tabelleninhalt speichern!
   * Bei bedarf werden die fehlenden Nummer eingefügt!
   */
  bool saveTableData(qint64 orderId, qint64 customerId);

  /**
   * @brief Anzahl der Bestellartikel von Tabelle.
   */
  int payments();

  /**
   * @brief Bestellartikel aus SQL Abfrage Importieren
   * Weil noch keine Order ID vorhanden ist wird die Zeile nur eingefügt aber
   * noch nicht gespeichert!
   * @see saveTableData
   */
  bool addProviderArticles(const QList<AntiquaCRM::OrderArticleItems> &list);
};

#endif // ANTIQUACRM_ORDERSITEMLIST_H
