// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// SPDX-License-Identifier: LGPL-3.0-or-later

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

/**
 * @brief Inventory Orders Items list
 * @ingroup _orders
 */
class OrdersItemList final : public QWidget {
  Q_OBJECT
  Q_PROPERTY(bool ArticleChanged READ getArticleChanged WRITE setArticleChanged NOTIFY articleChanged)

private:
  /**
   * @brief ArticleChanged
   */
  bool ArticleChanged = false;

  /**
   * @brief Tabelle für Artikel einfügen
   */
  PurchaseTable *m_table;

Q_SIGNALS:
  /**
   * @brief Signal wenn es in Artikeltabelle änderungen gibt.
   */
  void articleChanged(bool);

public Q_SLOTS:
  /**
   * @brief Tabellen Zeilen zurücksetzen!
   */
  void clearTable();

  /**
   * @brief Importiert einen neuen Tabellen Eintrag
   */
  void insertArticle(const AntiquaCRM::OrderArticleItems &article);

  /**
   * @brief Artikel Tabellen Status::Änderungen setzen!
   */
  void setArticleChanged(bool);

public:
  explicit OrdersItemList(QWidget *parent = nullptr);

  /**
   * @brief Abfrage ob artikel vorhanden sind!
   * Soll verhindern das der Klient Aufträge ohne Inhalt speichert!
   */
  bool isEmpty();

  /**
   * @brief Abfrage auf Artikeltabellen änderungen!
   */
  bool getArticleChanged();

  /**
   * @brief Nach einem INSERT die Tabelle überarbeiten.
   */
  bool setArticleOrderId(qint64 oid);

  /**
   * @brief Bestellartikel aus SQL Abfrage Importieren
   */
  bool importArticles(const QList<AntiquaCRM::OrderArticleItems> &list);

  /**
   * @brief PgSQL-Befehlskette von Tabelle weiterleiten!
   */
  const QStringList getQueryData();
};

#endif // ANTIQUACRM_ORDERSITEMLIST_H
