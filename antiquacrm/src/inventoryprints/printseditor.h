// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef PRINTSEDITOR_H
#define PRINTSEDITOR_H

#include <QtCore/QEvent>
#include <QtCore/QHash>
#include <QtCore/QObject>
#include <QtCore/QRegularExpression>
#include <QtCore/QVariant>
#include <QtCore/QVector>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

namespace HJCMS {
class SqlCore;
};

class ArticleID;
class ImageWidget;
class BoolBox;
class EditorActionBar;
class IntSpinBox;
class IsbnEdit;
class IsbnRequest;
class PriceEdit;
class StorageEdit;
class StrLineEdit;
class TextField;
class YearEdit;
class ImageToolBar;
class TechniqueEdit;

class PrintsEditor : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  HJCMS::SqlCore *db;                 /**< @brief SQL Database Connection */
  BoolBox *ip_kolorit;                /**< @brief koloriert */
  BoolBox *ip_landscape;              /**< @brief Ansicht */
  BoolBox *ip_views;                  /**< @brief koloriert */
  BoolBox *ip_restricted;             /**< @brief Zensiert */
  StorageEdit *ip_storage;            /**< @brief Lager bestimmung */
  YearEdit *ip_year;                  /**< @brief Jahr */
  PriceEdit *ip_price;                /**< @brief Preis */
  IntSpinBox *ip_count;               /**< @brief Bestandsangabe */
  ArticleID *ip_id;                   /**< @brief ReadOnly:ArticleID */
  StrLineEdit *ip_author;             /**< @brief Printautor */
  StrLineEdit *ip_condition;          /**< @brief Zustands beschreibung */
  StrLineEdit *ip_designation;        /**< @brief Umschreibung */
  StrLineEdit *ip_format;             /**< @brief Ausgabeformat */
  StrLineEdit *ip_title;              /**< @brief Print Titel */
  StrLineEdit *ip_title_extended;     /**< @brief Ereiterte Titel  */
  TextField *ip_internal_description; /**< @brief Interne Beschreibung */
  TextField *ip_description;          /**< @brief Öffentliche Beschreibung */
  TechniqueEdit *ip_technique;        /**< @brief Herausgeber/Verlag */
  QTabWidget *m_tabWidget; /**< @brief BeschreibungsText und ISBN Info  */

  /**
     @brief PrintData
     Lese die Datenfelder mit @ref editPrintEntry
     und schreibe sie für die Zurücksetzen funktion hier rein.

     @note Die Objektnamen die Typangabe in @b field (Feldnamen)
      stimmen mit den SQL Tabellenspalten Bezeichnungen überein!

     @li Die Typangabe @b field ist für das Identifizieren
      der Eingabefelder in der Klasse zuständig.

     @li Mit @b vtype wird die Entscheidung getroffen welches
      Datensatzformat verwendet werden soll. Die Entscheidung
      liegt zu 100% beim Rückgabe ergebnis von QSqlQuery.

     @li Der Wert @b data ist vom Type Variant, was die Erstellung
      der SQL INSERT/UPDATE Statements vereinfachen soll.

     @warning Wenn sich bei der Datenbank Tabelle ein Spalten Typ
      ändert. Muss das hier Kontrolliert und Überarbeitet werden!
   */
  struct DataEntries {
    QString field; /**< @brief Feld ist gleichwertig mit {INPUT}.objectName() */
    int vtype;     /**< @brief QVariant::Type */
    QVariant data; /**< @brief Datenwert */
  };

  /**
     @brief Wird für QObject::findchild benötigt!
     Dieser Reguläre Ausdruck wird verwendet um die Eingabe-Objektklassen
     zu finden. Sie sind Identisch mit den SQL Feldern und beginnen bei
     der Printtabelle "inventory_prints" immer mit "ip_".
  */
  const QRegularExpression p_objPattern = QRegularExpression("^ip_[a-z_]+\\b$");

  /**
     @brief Beinhaltet Cancel, Restore, Save und GoBack.
   */
  EditorActionBar *m_actionBar;

  /**
    @brief Bilder operations Knopfleiste
  */
  ImageToolBar *m_imageToolBar;

  /**
    @brief Eingebettete Bildansicht
   */
  ImageWidget *m_imageView;

  /**
    @brief Objektnamen-Liste der Eingabefelder
  */
  QStringList inputList;

  /**
   @brief Hier werden die Daten aus der Abfrage eingefügt.
   Er wird nur in @ref editPrintsEntry befüllt und in
   @ref finalLeaveEditor wieder geleert!
  */
  QVector<DataEntries> sqlQueryResult;

  /**
    @brief Prüft und erstellt die Datensatzfelder.

    In dieser Methode werden alle Datenfelder abgefragt und bei
    Erfolg in den Hash geschrieben. Je nach Klassenhäufigkeit
    werden Schleifen verwendet. Jedoch sind auch viele
    Datenfeldabfragen manuell eingefügt. Die Erstellung ist von
    mehreren Faktoren abhängig und beinhaltet folgende Vorgangsweise,
    welche sich für jedes Datenfeld wiederholt.

    @note Es werden keine leeren Datenfelder in den Hash geschrieben!

    1) Suche je nach Klassen-Type mit findChild die Datenfelder.
    2) Prüfe Datenfeld Klasse auf @b isRequired() @b isValid()
    3) Wenn die vorherige Abfrage fehlschlägt dann:
      a) MessageBox aufrufen,
      b) setFocus auf das Datenfeld,
      c) den Hash wieder leeren und abbrechen.

    Die Leerung muss zur Fehlervermeidung und für die Abfrage Methoden
    durchgeführt werden.

    @note Alle @i caller verwenden eine Abfrage auf data.size()
  */
  const QHash<QString, QVariant> createSqlDataset();

  /**
    @brief Alle Eingabefelder in @ref inputList einfügen.
    Erstellt mit findChildren(p_objPattern, Qt::FindChildrenRecursively)
    ein Liste aller Eingabe Objekte und schreibt diese in @ref inputList
    Sie wird am Ende des Konstruktors aufgerufen und dient als Helfer für
    die QObject::findChild(objectName,*) Methode welche z.B. beim Erstellen
    der SQL Statements verwendet wird.
  */
  void setInputList();

  /**
    @brief Setzt alle Markierungen wieder zurück.
    Wird z.B. nach erfolgreichen Speichern ausgeführt!
    @see sendSqlQuery
  */
  void resetModified();

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
     @brief SQL UPDATE Statement erstellen!
     @note Wird nur von Slot @ref saveData() aufgerufen!
   */
  void createSqlUpdate();

  /**
     @brief SQL INSERT Statement erstellen!
     @note Wird nur von Slot @ref saveData() aufgerufen!
   */
  void createSqlInsert();

  /**
     @brief Durchläuft @ref sqlQueryResult und ruft @ref setSqlQueryData auf.
      Die Abfolge ist:
        @li blockSignals(true);
        @li Schleife: setSqlQueryData(Feld,Daten);
        @li blockSignals(false);
        @li resetModified();
        @li m_imageView->searchImageById(ArticleID);
   */
  void importSqlResult();

  /**
     @brief Suche nach Objektnamen mit @i findChild(objectName);
     @param key - Ist der SQL Datenfeldbezeichner.
       Dieser muss Identisch mit dem Eingabe Objektnamen sein.
     @param value - Tabellenspalten Wert
     @note Es werden die Datenfeldtypen vom SQL Query gelesen!
           Wenn sich also bei SQL Datenfeldern etwas ändert!
           Muss die Methode überarbeitet werden.

     Je nach Objekttyp werden die Eingabefelder manuel zugewiesen
     und dann mit @i findChild(objectName) Identifiziert. Das wird
     überwiegend bei den (IntSpinBox,StrLineEdit) Klassen eingesetzt.
     @code
      // "ip_isbn" QVariant(qulonglong)
      if (key.contains("ip_isbn")) {
        ip_isbn->setIsbn(value.toLongLong());
        return;
      }
      // "ip_count" QVariant(int)
      if (value.type() == QVariant::Int) {
        IntSpinBox *v = findChild<IntSpinBox *>(key,
     Qt::FindDirectChildrenOnly); if (v != nullptr) v->setValue(value.toInt());
      }
     @endcode
   */
  void setSqlQueryData(const QString &key, const QVariant &value);

private Q_SLOTS:
  /**
     @brief Button open Imaging clicked()
   */
  void openImageDialog();

  /**
     @brief Signal Verarbeitung für @ref m_actionBar::s_saveClicked()
     Kontrolliert ob @ref ip_id ein gültige Artikel ID enthält.
        @li Ist das Ergebnis Negativ - wird @ref createSqlInsert aufgerufen.
        @li Ist das Ergebnis Positiv - wird @ref createSqlUpdate aufgerufen.
   */
  void saveData();

  /**
     @brief Alle Datenfelder zurücksetzen!
     Verwended die Methoden:
        @li @ref clearDataFields();
        @li @ref importSqlResult();
     um die Datenfelder wieder zurück zu setzen.
  */
  void clearDataFields();

  /**
    @brief Prüfe auf Datensatzänderungen!
    @return Bei @b true, wurden Datenfelder nicht gespeichert!
   */
  bool checkIsModified();

  /**
   @brief Vor dem verlassen nach Änderungen suchen.
   @note Die Methode verwendet @ref checkIsModified()

   Wenn es keine Daten zu Speichern gibt, gehe weiter zu
   @ref finalLeaveEditor, wenn doch, an dieser Stelle das
   verlassen Verweigern und dem Nutzer einen MessageBox
   Hinweis geben!
 */
  void checkLeaveEditor();

  /**
    @brief Kommt nach @ref checkLeaveEditor() und beendet den Editor.
    Wird auch von Signal @ref EditorActionBar::s_cancelClicked() aufgerufen!
    Die Methode arbeitet folgende Operationen durch und beendet den Editor!
     @li OpenLibrary.org Ausgabe leeren,
     @li SQL Ergebnis Historie leeren,
     @li alle Datenfelder leeren,
     @li den ResetButton auschalten,
     @li signal @ref s_leaveEditor an Parent senden!
  */
  void finalLeaveEditor();

protected:
  /**
    @brief Fange @ref QEvent::EnabledChange ab!
    Lade Datenfelder nur wenn das Fenster Aktiviert wurde!
    Um fehlerhafte Tastenbindungen oder eingaben zu verhindern
    ist das Fenster im Standard erst mal deaktiviert.
    Und wird erst Aktiviert, wenn von StackedWidget aufgerufen.
    @note Für Unterklassen die eine SQL Abfrage erfordern,
       kann hier die "loadDataset" Methode aufgerufen werden.
  */
  virtual void changeEvent(QEvent *event);

public Q_SLOTS:
  /**
     @brief Methode für Zurücksetzen Button
   */
  void restoreDataset();

Q_SIGNALS:
  /**
     @brief Sende Änderungen an parent::
  */
  void s_isModified(bool);

  /**
    @brief Sende Signal das die Ansicht verlassen werden kann!
  */
  void s_leaveEditor();

  /**
    @brief Bildbearbeitung öffnen
  */
  void s_openImageEditor(double);

  /**
    @brief Nachricht Ausgeben
  */
  void s_sendMessage(const QString &);

public:
  explicit PrintsEditor(QWidget *parent = nullptr);

  /**
    @brief Wenn Bearbeiten darf der Eintrag nicht 0 sein!
    @param condition Abfragekorpus @i ohne WHERE
  */
  void editPrintsEntry(const QString &condition);
  void createPrintsEntry();
};

#endif // PRINTSEDITOR_H
