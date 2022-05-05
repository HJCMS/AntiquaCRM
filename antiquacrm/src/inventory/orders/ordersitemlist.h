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
   * @brief Artikeldaten
   */
  OrderArticle p_article;

  /**
   * @brief Preiseingabe Box
   */
  QDoubleSpinBox *addPriceBox(double val, bool edit = false);

  /**
   * @brief Bestandsanzeige
   */
  QSpinBox *addCountBox(const QVariant &val);

  /**
   * @brief Standard Lesezelle
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
   * @brief Artikel einfügen
   */
  void insertArticle();

  /**
   * @brief createSignal
   */
  void createSignal();

Q_SIGNALS:
  /**
   * @brief Suche Artikel mit ID
   */
  void searchArticle(int);

  /**
   * @brief nachricht an Elternfenster
   */
  void statusMessage(const QString &);

public Q_SLOTS:
  /**
   * @brief Eingabe leeren
   */
  void clearInput();

public:
  OrdersItemList(QWidget *parent = nullptr);

  /**
   * @brief Aktuelle Bestelliste ermitteln.
   */
  const OrderArticleList getArticleOrder();

  /**
   * Wenn ein Artikel gefunden wird diesen
   * hier einfügen.
   */
  void foundArticle(const OrderArticle &);
};

#endif // INVENTORY_ORDERSITEMLIST_H
