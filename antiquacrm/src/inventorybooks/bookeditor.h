// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8
// @COPYRIGHT_HOLDER@

#ifndef BOOKEDITOR_H
#define BOOKEDITOR_H

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
class BooksImageView;
class BoolBox;
class EditorActionBar;
class IntSpinBox;
class IsbnEdit;
class IsbnRequest;
class PriceEdit;
class SetLanguage;
class StorageEdit;
class StrLineEdit;
class TextField;
class YearEdit;

/**
   @brief BookData
   Lese die Datenfelder mit @ref BookEditor::readDataBaseEntry
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
struct BookData {
  QString field; /**< @brief SQL-Fieldname is equil to {INPUT}.objectName() */
  int vtype;     /**< @brief QVariant::Type */
  QVariant data; /**< @brief Values */
};

/**
   @brief BookEditor
   Primäre Klasse zum erstellen/bearbeiten von Bucheinträgen.
 */
class BookEditor : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "https://www.hjcms.de")

private:
  HJCMS::SqlCore *db;             /**< @brief SQL Database Connection */
  BoolBox *ib_signed;             /**< @brief Signiert? */
  BoolBox *ib_restricted;         /**< @brief Zensiert? */
  SetLanguage *ib_language;       /**< @brief Sprache */
  StorageEdit *ib_storage;        /**< @brief Lager bestimmung */
  YearEdit *ib_year;              /**< @brief Jahr */
  PriceEdit *ib_price;            /**< @brief Preis */
  IntSpinBox *ib_count;           /**< @brief Bestandsangabe */
  IntSpinBox *ib_pagecount;       /**< @brief Seitenanzahl */
  IntSpinBox *ib_weight;          /**< @brief Gewicht */
  IntSpinBox *ib_volume;          /**< @brief Band ? */
  IntSpinBox *ib_edition;         /**< @brief Ausgabe */
  ArticleID *ib_id;               /**< @brief ReadOnly:ArticleID */
  IsbnEdit *ib_isbn;              /**< @brief ISBN */
  StrLineEdit *ib_author;         /**< @brief Buchautor */
  StrLineEdit *ib_condition;      /**< @brief Zustands beschreibung */
  StrLineEdit *ib_designation;    /**< @brief Umschreibung */
  StrLineEdit *ib_keyword;        /**< @brief Schlüsselwort */
  StrLineEdit *ib_publisher;      /**< @brief Herausgeber/Verlag */
  StrLineEdit *ib_title;          /**< @brief Buch Titel */
  StrLineEdit *ib_title_extended; /**< @brief Ereiterte Titel  */
  TextField *ib_description;      /**< @brief Öffentliche Beschreibung */
  QPushButton *btn_createJob;     /**< @brief Auftragserstellung */
  QPushButton *btn_imaging;       /**< @brief IMage Import/Edit Dialog */
  IsbnRequest *m_isbnRequest;     /**< @brief ISBN Abfrage Klasse */
  QTabWidget *m_tabWidget; /**< @brief BeschreibungsText und ISBN Info  */
  TextField *ib_internal_description; /**< @brief Interne Beschreibung */
  QListWidget *m_listWidget;          /**< @brief ISBN Abfrage-Vorschau */

  /**
     @brief Wird für QObject::findchild benötigt!
  */
  const QRegularExpression p_objPattern = QRegularExpression("^ib_[a-z_]+\\b$");

  /**
     @brief Beinhaltet Zurücksetzen/Speichern/Verlassen
   */
  EditorActionBar *m_actionBar;

  /**
    @brief Wird im Konstruktor bei TabWidget gesetzt
    und in @ref setIsbnInfo verwendet!
  */
  int isbnTabIndex;

  /**
    @brief m_imageView
   */
  BooksImageView *m_imageView;

  /**
    @brief Liste der Eingabefelder Objektnamen
  */
  QStringList inputList;

  /**
   @brief Hier werden die Daten aus der Abfrage eingefügt.

   Weil beim einfügen blockSignals() wegen isModified()
   eingesetz wird. Darf das nicht innerhalb der SQLQuery
   schleife erfolgen!
  */
  QVector<BookData> sqlQueryResult;

  /**
    @brief Erstellt aus allen Feldern den Datensatz.
  */
  const QHash<QString, QVariant> createSqlDataset();

  /**
    @brief Alle Eingabefelder in @ref inputList einfügen.
  */
  void setInputList();

  /**
    @brief Setzt alle Markierungen wieder zurück.
    Wird nach einem Speichern ausgeführt!
    @see sendQueryDatabase
  */
  void resetModified();

  /**
     @brief Sende SQL Statement an die Datenbank
     Bei erfolg wird @ref resetModified ausgeführt!

     @param INSERT/UPDATE SQL Statement senden!
   */
  bool sendSqlQuery(const QString &sqlStatement);

  /**
     @brief SQL UPDATE Statement erstellen!
     @note In @ref saveData() erfolgt eine @i ib_id Abfrage!
   */
  void createSqlUpdate();

  /**
     @brief SQL INSERT Statement erstellen!
     @note In @ref saveData() erfolgt eine @i ib_id Abfrage!
   */
  void createSqlInsert();

  /**
     @brief Durchläuft @ref sqlQueryResult und ruft @ref addDataFromQuery auf.
     @note Die Größe von @ref sqlQueryResult muss mindestenz 16 betragen!
   */
  void importSqlResult();

  /**
     @brief Suche nach Objektnamen mit @i findChild(key);
     @param key   - Der SQL Datenfeldbezeichner.
                    Dieser ist/muss Identisch mit
                    dem Eingabe Objektnamen sein.
     @param value - Tabellenspalten Wert
     @note Es werden die Datenfeldtypen vom SQL Query gelesen!
           Wenn sich also hier etwas ändert!
           Muss die Methode überarbeitet werden.

     Je nach Objekttyp werden die Eingabefelder manuel zugewiesen
     und dann mit @i findChild(key) Identiufiziert. Das wird
     überwiegend bei den (QCheckBox,QSpinBox) Klassen eingesetzt.
     @code
      // "ib_isbn" QVariant(qulonglong)
      if (key.contains("ib_isbn")) {
        ib_isbn->setIsbn(value.toLongLong());
        return;
      }
      // "ib_count" QVariant(int)
      if (value.type() == QVariant::Int) {
        QSpinBox *v = findChild<QSpinBox *>(key, Qt::FindDirectChildrenOnly);
        if (v != nullptr)
          v->setValue(value.toInt());
        return;
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
     @brief Signal accept() abfangen.
   */
  void saveData();

  /**
     @brief Öffne den ISBN Dialog
     @see triggerIsbnQuery()
   */
  void setIsbnInfo(bool);

  /**
     @brief triggerISBNQuery
     @todo ISBN Anfragen Versenden
   */
  void triggerIsbnQuery();

  /**
     @brief Öffne Url
  */
  void infoISBNDoubleClicked(QListWidgetItem *);

  /**
     @brief Alle Datenfelder zurücksetzen!
   */
  void clearDataFields();

  /**
    @brief Prüfe auf Datensatzänderungen!
    @return Bei @b true, wurden Datenfelder nicht gespeichert!
   */
  bool checkIsModified();

  /**
   @brief Suche nach nicht gespeicherten Daten
   Wenn es keine Daten zu Speichern gibt sende
   das Signal @ref s_leaveEditor(), wenn doch
   an dieser Stelle das verlassen verweigern
   und den Nutzer einen Hinweis geben!
 */
  void checkLeaveEditor();

protected:
  /**
    @brief Fange @ref QEvent::EnabledChange ab!
    Lade Datenfelder nur wenn das Fenster Aktiviert wurde!
    Um fehlerhafte Tastenbindungen oder eingaben zu verhindern
    ist das Fenster im Standard erst mal deaktiviert.
    Und wird es serst, wenn von StackedWidget aufgerufen.
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
  BookEditor(QWidget *parent = nullptr);

  /**
    @brief Wenn Bearbeiten darf der Eintrag nicht 0 sein!
    @param condition Abfragekorpus @i ohne WHERE
  */
  void openBookEntry(const QString &condition);
  void createBookEntry();
};

Q_DECLARE_METATYPE(BookData);

#endif // BOOKEDITOR_H
