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

public Q_SLOTS:
  /**
   * @brief Tabellen Zeilen zurücksetzen!
   */
  void clearTable();

  /**
   * @brief Importiert einen neuen Tabellen Eintrag
   */
  void insertArticle(const AntiquaCRM::OrderArticleItems &article);

public:
  explicit OrdersItemList(QWidget *parent = nullptr);

  /**
   * @brief Nach einem insert die Tabelle überarbeiten.
   */
  bool setArticleOrderId(qint64 oid);

  /**
   * @brief Bestellartikel aus SQL Abfrage Importieren
   */
  bool importArticles(const QList<AntiquaCRM::OrderArticleItems> &list);

  const QStringList getQueryData();
};

#endif // ANTIQUACRM_ORDERSITEMLIST_H
