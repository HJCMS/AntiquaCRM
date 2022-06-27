// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORY_ORDERS_H
#define INVENTORY_ORDERS_H

#include "inventory.h"
#include "providerorders.h"

#include <QLabel>
#include <QList>
#include <QObject>
#include <QStackedWidget>
#include <QTabWidget>
#include <QWidget>

class StatsActionBar;
class OrdersTable;
class OrderEditor;

/**
 * @class InventoryOrders
 * Primäre Klasse für das Auftrags System
 */
class InventoryOrders final : public Inventory {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
   * @brief Fenster gruppierung
   */
  QStackedWidget *m_stackedWidget;

  /**
   * @brief Tabellenansicht
   */
  OrdersTable *m_tableView;

  /**
   * @brief Status Informationen
   */
  QLabel *m_statusInfo;

  /**
   *  @brief Editor
   */
  OrderEditor *m_editor;

private Q_SLOTS:
  /**
   * @brief Reagiert auf LineEdit
   * Überwacht QLineEdit::textChanged und startet erst wenn @ref minLength
   * überstiegen wurde. Leitet dann weiter an @ref searchConvert()
   * @param Sucheingabe
   */
  void searchConvert(const QString &);

  /**
   * @brief Reagiert auf Suchknopf & EnterTaste
   * Liest @ref m_searchBar::currentSearchText() aus und Prüft auf min. 2
   * Zeichen (Performance)!
   * @note In @class SearchBar wird bereits die Zeichenersetzung durchgeführt!
   */
  void searchConvert();

  /**
   * @brief Öffne Tabellenansicht
   * Wird mit @ref OrderEditor::s_leaveEditor aufgerufen.
   * @note Standard ist Tabellenansicht "Page 1"
   */
  void openTableView();

  /**
   * @brief Wenn das Tab im Vordergund geht, dann eine Aktionen starten
   */
  void onEnterChanged(){/* TODO */};

protected:
  /**
    @brief Öffne mit der SQL-Abfrage den Bucheditor
    Wenn die Abfrage nicht leer ist wechsle zum
    Bucheditor und aktiviere das Editor Widget.
    Die Abfrage condition besteht aus @b ib_id
    und @b ib_title Feldabfrage.
    @note Wegen Tastenbindungen ist das Editor
      Fenster deaktiviert und muss hier wieder
      aktiviert werden.
    Ablauf des Fensterwechsel ist:
      @li Prüfe Abfrage auf nicht Leer
      @li Aktiviere Editor Widget
      @li Rufe queryBookEntry(SQL) auf
      @li Setze das StackedWidget auf den Editor.

    @param SQL Anfrageausdruck
  */
  void openEditor(const QString &condition);

public Q_SLOTS:
  /**
   * @brief Ein Artikel wurd ausgewählt
   * Wird bei der Tabellen Suchansicht mit einem Doppelklick das Signal @ref
   * OrdersTable::s_editOrder ausgelöst und an Hand des @b "ib_id" Feldes
   * eine SQL Abfrage erstellt, jetzt der Editor mit @ref
   * openEditor("ib_id={$id}") geöffnet!
   * @param customerId
   */
  void updateOrder(int customerId);

  /**
   * @brief Erstelle einen neuen Auftrag für Kunden Nummer.
   */
  void createOrder(int customerId);

  /**
   * @brief Erstelle neuen Eintrag mit Datensatz
   */
  void createOrder(const ProviderOrder &order);

public:
  /**
    @param index  Ist der Tab-Index
    @param parent TabWidget
  */
  explicit InventoryOrders(QWidget *parent = nullptr);

  /**
   * @brief Füge einen Artikel zu einem Auftrag hinzu.
   */
  bool addArticleToOrder(int articleId);

  /**
   * @brief Ist der Editor aktiv?
   * @return
   */
  bool isEditorActive();
};

#endif // INVENTORY_ORDERS_H
