// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORYPRINTS_H
#define INVENTORYPRINTS_H

#include "inventory.h"

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

class SearchBar;
class PrintsTableView;
class PrintsEditor;
class StatsActionBar;

class InventoryPrints : public Inventory {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  /**
    @brief Ist der TabIndex siehe Konstruktor
  */
  int tabIndex;

  /**
    @brief Ab Zeichenanzahl startet die Abfrage!
    @see searchConvert
  */
  int minLength;

  /**
    @brief Fenster gruppierung
  */
  QStackedWidget *m_stackedWidget;

  /**
    @defgroup StackeWidget Page 1
    @{
  */

  /**
    @brief Sucheingabe für Bücher
  */
  SearchBar *m_searchBar;

  /**
     @brief Zeige alles was im bestand ist!
   */
  StatsActionBar *m_statsPrintBar;

  /**
    @brief Tabellenansicht
  */
  PrintsTableView *m_tableView;

  /**
    @}

    @defgroup StackeWidget Page 2
    @{
  */

  /**
    @brief Editor Fenster
  */
  PrintsEditor *m_printsEditor;

  /** @} */

  /**
    @brief Öffne mit der SQL-Abfrage den Printsditor
    Wenn die Abfrage nicht leer ist wechsle zum
    Printsditor und aktiviere das Editor Widget.
    Die Abfrage condition besteht aus @b ib_id
    und @b ib_title Feldabfrage.
    @note Wegen Tastenbindungen ist das Editor
      Fenster deaktiviert und muss hier wieder
      aktiviert werden.
    Ablauf des Fensterwechsel ist:
      @li Prüfe Abfrage auf nicht Leer
      @li Aktiviere Editor Widget
      @li Rufe queryPrintsEntry(SQL) auf
      @li Setze das StackedWidget auf den Editor.

    @param SQL Anfrageausdruck
  */
  void openEditor(const QString &condition);

private Q_SLOTS:
  /**
    @brief Reagiert auf LineEdit
      Überwacht QLineEdit::textChanged
      und startet erst wenn @ref minLength
      überstiegen wurde. Leitet dann weiter
      an @ref searchConvert()

    @param Sucheingabe
   */
  void searchConvert(const QString &);

  /**
    @brief Reagiert auf Suchknopf & EnterTaste
    Liest @ref m_searchBar::currentSearchText()
    aus und Prüft auf min. 2 Zeichen (Performance)!
    @note In @class SearchBar wird bereits die
        Zeichenersetzung durchgeführt!
   */
  void searchConvert();

  /**
    @brief Öffne Tabellenansicht
    Wird mit @ref PrintsEditor::s_leaveEditor aufgerufen.
    @note Standard ist Tabellenansicht "Page 1"
  */
  void openTableView();

  /**
     @brief Ein Artikel wurd ausgewählt
     Wird bei der Tabellen Suchansicht mit einem Doppelklick
     das Signal @ref PrintsTableView::s_articleSelected ausgelöst
     und an Hand des @b "ib_id" Feldes eine SQL Abfrage erstellt,
     jetzt der Editor mit @ref openEditor("ib_id={$id}") geöffnet!
     @param id
  */
  void articleSelected(int id);

  /**
     @brief Setze den Validator für die Ausgewählte suche.
     Wird der Suchfilter geändert dann wird hier an
     @ref SearchBar::setValidation ein update gesendet.
   */
  void updateValidator(int);

public Q_SLOTS:
  /**
    @brief Öffnet das Nachrichtenfenster
    @param MessageBody
  */
  void displayMessageBox(const QString &);

  /**
    @brief Aktiviert das PrintsEditor Fenster
    @li Aktiviere PrintsEditor Widget
    @li Wechsle zum PrintsEditor
  */
  void createPrintsEntry();

public:
  explicit InventoryPrints(QWidget *parent = nullptr);
};

#endif // INVENTORYPRINTS_H
