// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKEDITOR_H
#define BOOKEDITOR_H

#include <QEvent>
#include <QHash>
#include <QListWidget>
#include <QObject>
#include <QPushButton>
#include <QRegularExpression>
#include <QTabWidget>
#include <QVariant>
#include <QWidget>

#include <EditorMain>
#include <Imaging>
#include <Utils>

class ISBNResults;

/**
   @brief BookEditor
   Primäre Klasse zum erstellen/bearbeiten von Bucheinträgen.
 */
class BookEditor final : public EditorMain {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  BoolBox *ib_signed;                   /**< @brief Signiert? */
  BoolBox *ib_restricted;               /**< @brief Zensiert? */
  BoolBox *ib_including_vat;            /**< @brief inkl. Mehwertsteuer */
  DateTimeDisplay *ib_since;            /**< @brief Ersteller Datum */
  SetLanguage *ib_language;             /**< @brief Sprache */
  StorageEdit *ib_storage;              /**< @brief Lager bestimmung */
  YearEdit *ib_year;                    /**< @brief Jahr */
  PriceEdit *ib_price;                  /**< @brief Preis */
  IntSpinBox *ib_count;                 /**< @brief Bestandsangabe */
  IntSpinBox *ib_pagecount;             /**< @brief Seitenanzahl */
  IntSpinBox *ib_weight;                /**< @brief Gewicht */
  IntSpinBox *ib_volume;                /**< @brief Band ? */
  IntSpinBox *ib_edition;               /**< @brief Ausgabe */
  SerialID *ib_id;                      /**< @brief ReadOnly:ArticleID */
  IsbnEdit *ib_isbn;                    /**< @brief ISBN */
  StrLineEdit *ib_author;               /**< @brief Buchautor */
  StrLineEdit *ib_condition;            /**< @brief Zustands beschreibung */
  StrLineEdit *ib_designation;          /**< @brief Umschreibung */
  StrLineEdit *ib_keyword;              /**< @brief Schlüsselwort */
  CategorySubject *ib_category_subject; /**< @brief Dienstleister Kategorie */
  StrLineEdit *ib_publisher;            /**< @brief Herausgeber/Verlag */
  StrLineEdit *ib_title;                /**< @brief Buch Titel */
  StrLineEdit *ib_title_extended;       /**< @brief Ereiterte Titel  */
  QTabWidget *m_tabWidget;   /**< @brief BeschreibungsText und ISBN Info  */
  TextField *ib_description; /**< @brief Öffentliche Beschreibung */
  TextField *ib_internal_description; /**< @brief Interne Beschreibung */
  ISBNResults *m_isbnWidget;          /**< @brief ISBN Abfrage-Vorschau */

  /**
   * @brief Speichert die Anzahl Temporär
   * Soll verhindern das wenn sich die Menge geändert hat nicht nach jedem
   * zwischenspeichern die Daten noch mal an den Dienstleister gesendet werden!
   * @see EditorMain::s_articleCount
   */
  qint8 count_temp = -1;

  /**
   * @brief Wird für QObject::findchild benötigt!
   * Dieser Reguläre Ausdruck wird verwendet um die Eingabe-Objektklassen zu
   * finden. Sie sind Identisch mit den SQL Feldern und beginnen bei der
   * Buchtabelle "inventory_books" immer mit "ib_".
   */
  const QRegularExpression p_objPattern = QRegularExpression("^ib_[a-z_]+\\b$");

  /**
   * @brief Beinhaltet Cancel, Restore, Save und go-back.
   */
  EditorActionBar *m_actionBar;

  /**
   * @brief Bilder operations Knopfleiste
   */
  ImageToolBar *m_imageToolBar;

  /**
   * @brief Eingebettete Bildansicht
   */
  ImageView *m_imageView;

  /**
   * @brief Prüft und erstellt die Datensatzfelder.
   *
   * In dieser Methode werden alle Datenfelder von UtilsMain abgefragt und bei
   * Erfolg in den Hash geschrieben. Die Erstellung ist von mehreren Faktoren
   * abhängig und beinhaltet folgende Vorgangsweise, welche sich für jedes
   * Datenfeld wiederholt.
   *
   * @li Prüfe Datenfeld Klasse auf @b isRequired() @b isValid()
   * @li Wenn die vorherige Abfrage fehlschlägt dann:
   *  a) MessageBox aufrufen,
   *  b) setFocus auf das Datenfeld,
   *  c) den Hash wieder leeren und abbrechen.
   *
   * Die Leerung muss zur Fehlervermeidung und für die Abfrage Methoden
   * durchgeführt werden.
   *
   * @warning Alle @i caller verwenden eine Abfrage auf data.size()
   */
  const QHash<QString, QVariant> createSqlDataset();

  /**
   * @brief Alle Eingabefelder in @ref inputList einfügen.
   * Erstellt mit HJCMS::SqlCore::fields("tabelle") ein Liste aller Eingabe
   * Objekte und schreibt diese in @ref inputList. Sie wird am Ende des
   * Konstruktors aufgerufen und dient als Helfer für die findChild Methode
   * welche z.B. beim Erstellen der SQL Statements verwendet wird.
   */
  void setInputList();

  /**
     @brief Sende das erstellte SQL Statements an die Datenbank
     Bei Erfolg wird @ref resetModified() ausgeführt!
     Es wird zusätzlich bei Erfolg/Fehler, Messagebox ausgeführt.

     @note Wird nur von @ref createSqlUpdate() und
           @ref createSqlInsert() aufgerufen.

     @param Übergabewert ist das INSERT/UPDATE SQL Statement senden!
   */
  bool sendSqlQuery(const QString &sqlStatement);

  /**
   * @brief SQL UPDATE Statement erstellen!
   * @note Wird nur von Slot @ref saveData() aufgerufen!
   */
  void createSqlUpdate();

  /**
   * @brief SQL INSERT Statement erstellen!
   * @note Wird nur von Slot @ref saveData() aufgerufen!
   */
  void createSqlInsert();

  /**
   * @brief Durchläuft @ref sqlQueryResult und ruft @ref setData auf.
   * Die Abfolge ist:
   *  @li Lese @ref sqlQueryResult
   *  @li Setze Datenfelder
   *  @li Suche Bilddaten
   */
  void importSqlResult();

  /**
   * @brief Suche nach Objektnamen mit @i findChild(objectName);
   * @param key - Ist der SQL Datenfeldbezeichner.
   *  Dieser muss Identisch mit dem Eingabe Objektnamen sein.
   * @param value - Tabellenspalten Wert
   * @note Es werden die Datenfeldtypen vom SQL Query gelesen!
   *  Wenn sich also bei SQL Datenfeldern etwas ändert!
   *  Muss die Methode überarbeitet werden.
   */
  void setData(const QString &key, const QVariant &value,
               bool required = false);

  /**
   * Wenn der Benutzer den Artikel Bestand auf 0 setzt!
   * Einen Hinweis ausgeben das der Artikel gleichzeitig
   * auch aus dem Auftrags-System geworfen wird und die
   * Online Shops auf einen Löschenauftrag erhalten!
   * @note Die Meldung wird in @ref saveData ausgelöst!
   */
  bool realyDeactivateBookEntry();

private Q_SLOTS:
  /**
   * @brief Button open Imaging clicked()
   */
  void openImageDialog();

  /**
   * @brief Enferne Bild mit Artikel ID - Dialog.
   */
  void removeImageDialog(int);

  /**
   * @brief Signal Verarbeitung für @ref m_actionBar::s_saveClicked()
   * Kontrolliert ob @ref ib_id ein gültige Artikel ID enthält.
   *  @li Ist das Ergebnis Negativ - wird @ref createSqlInsert aufgerufen.
   *  @li Ist das Ergebnis Positiv - wird @ref createSqlUpdate aufgerufen.
   */
  void saveData();

  /**
   * @brief Sendet eine ISBN Anfragen
   */
  void createIsbnQuery();

  /**
   * @brief Wenn ein Ergebnis kommt auf das Tabe wechseln!
   */
  void viewIsbnTab();

  /**
   * @brief Öffne die Url aus der Ergebnisanzeige.
   */
  void infoISBNDoubleClicked(QListWidgetItem *);

  /**
   * @brief Eine Buchkarte drucken
   */
  void printingBookCard();

  /**
   * @brief Vor dem verlassen nach Änderungen suchen.
   * @note Die Methode verwendet @ref checkIsModified()
   *
   * Wenn es keine Daten zu Speichern gibt, gehe weiter zu @ref
   * finalLeaveEditor, wenn doch, an dieser Stelle das verlassen Verweigern und
   * dem Nutzer einen MessageBox Hinweis geben!
   */
  void checkLeaveEditor();

  /**
   * @brief Kommt nach @ref checkLeaveEditor() und beendet den Editor.
   * Wird auch von Signal @ref EditorActionBar::s_cancelClicked() aufgerufen!
   * Die Methode arbeitet folgende Operationen durch und beendet den Editor!
   * @li OpenLibrary.org Ausgabe leeren,
   * @li SQL Ergebnis Historie leeren,
   * @li alle Datenfelder leeren,
   * @li den ResetButton auschalten,
   * @li signal @ref s_leaveEditor an Parent senden!
   */
  void finalLeaveEditor();

protected:
  /**
   * @brief Fange QEvent::EnabledChange ab!
   * Lade Datenfelder nur wenn das Fenster Aktiviert wurde! Um fehlerhafte
   * Tastenbindungen oder eingaben zu verhindern ist das Fenster im Standard
   * erst mal deaktiviert.
   * Also erst Aktiviert, wenn es von StackedWidget aufgerufen wird.
   * @note Für Unterklassen die eine SQL Abfrage erfordern, kann hier die
   * "loadDataset" Methode aufgerufen werden.
   */
  virtual void changeEvent(QEvent *event);

public Q_SLOTS:
  /**
   * @brief Methode für Zurücksetzen Button
   */
  void restoreDataset();

public:
  BookEditor(QWidget *parent = nullptr);

  /**
   * @brief Wenn Bearbeiten darf der Eintrag nicht 0 sein!
   * @param condition Abfragekorpus @i ohne WHERE
   */
  void editBookEntry(const QString &condition);

  /**
   * @brief Neuer Bucheintrag erstellen!
   */
  void createBookEntry();
};

#endif // BOOKEDITOR_H
