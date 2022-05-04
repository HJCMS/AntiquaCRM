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

class OrderPaymentsTable;

/**
 * @ingroup InventoryOrders
 * @class OrdersItemList
 */
class OrdersItemList : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

public:
  struct Article {
    int articleId;   /**< Artikel Nummer */
    int count;       /**< Anzahl */
    QString title;   /**< Titel */
    double price;    /**< Aktuelle Preis */
    QString summary; /**< Info Anzeige */
  };

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
  Article p_article;

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
   * Wenn ein Artikel gefunden wird diesen
   * hier einfügen.
   */
  void foundArticle(const Article &);
};
Q_DECLARE_METATYPE(OrdersItemList::Article);

#endif // INVENTORY_ORDERSITEMLIST_H
