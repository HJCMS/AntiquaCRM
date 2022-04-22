// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef INVENTORYBOOKS_H
#define INVENTORYBOOKS_H

#include "inventory.h"

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtWidgets/QStackedWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

class SearchBar;
class BooksTableView;
class StatsActionBar;
class BookEditor;

/**
 @class InventoryBooks
 Primäre Klasse für das Bücherarchiv
 Die Fenster werden mit einem StackedWidget gruppiert.
 Es sind enthalten:
  @li Suchbeingabe
  @li Tabellenansicht
  @li Statusbalken und Historienauswahl
  @li Bucheditor
 @note Im ladezustand ist das BuchEditor Fenster deaktiviert.
*/
class InventoryBooks : public Inventory {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
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
    @brief Tabellenansicht
  */
  BooksTableView *m_tableView;

  /**
    @brief Statusbalken mit Historienauswahl
  */
  StatsActionBar *m_statsBookBar;

  /**
    @}

    @defgroup StackeWidget Page 2
    @{
  */

  /**
    @brief BuchEditor Fenster
  */
  BookEditor *m_bookEditor;

  /** @} */

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
    Wird mit @ref BookEditor::s_leaveEditor aufgerufen.
    @note Standard ist Tabellenansicht "Page 1"
  */
  void openTableView();

  /**
     @brief Ein Artikel wurd ausgewählt
     Wird bei der Tabellen Suchansicht mit einem Doppelklick
     das Signal @ref BooksTableView::s_articleSelected ausgelöst
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
    @brief Aktiviert das BuchEditor Fenster
    @li Aktiviere BuchEditor Widget
    @li Wechsle zum BuchEditor
  */
  void createBookEntry();

public:
  /**
    @param index  Ist der Tab-Index
    @param parent TabWidget
  */
  explicit InventoryBooks(QWidget *parent = nullptr);
};

#endif // INVENTORYBOOKS_H
