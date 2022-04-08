#ifndef BOOKEDITOR_H
#define BOOKEDITOR_H

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>

class SetLanguage;
class ArticleID;
class YearEdit;
class PriceEdit;
class StorageEdit;
class EditionEdit;
class StrLineEdit;
class IsbnEdit;
class IsbnRequest;

/**
   @brief BookDataField
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
struct BookDataField {
  QString field; /**< @brief Fieldname */
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
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  QCheckBox *ib_signed;           /**< @brief Signiert? */
  QCheckBox *ib_restricted;       /**< @brief Zensiert? */
  EditionEdit *ib_edition;        /**< @brief Ausgabe */
  SetLanguage *ib_language;       /**< @brief Sprache */
  StorageEdit *ib_storage;        /**< @brief Lager bestimmung */
  YearEdit *ib_year;              /**< @brief Jahr */
  PriceEdit *ib_price;            /**< @brief Preis */
  QSpinBox *ib_count;             /**< @brief Bestandsangabe */
  QSpinBox *ib_pagecount;         /**< @brief Seitenanzahl */
  QSpinBox *ib_weight;            /**< @brief Gewicht */
  QSpinBox *ib_volume;            /**< @brief Band ? */
  ArticleID *ib_id;               /**< @brief ReadOnly:ArticleID */
  IsbnEdit *ib_isbn;              /**< @brief ISBN */
  StrLineEdit *ib_author;         /**< @brief Buchautor */
  StrLineEdit *ib_condition;      /**< @brief Zustands beschreibung */
  StrLineEdit *ib_designation;    /**< @brief Umschreibung */
  StrLineEdit *ib_keyword;        /**< @brief Schlüsselwort */
  StrLineEdit *ib_publisher;      /**< @brief Herausgeber/Verlag */
  StrLineEdit *ib_title;          /**< @brief Buch Titel */
  StrLineEdit *ib_title_extended; /**< @brief Ereiterte Titel  */
  QTextEdit *ib_description;      /**< @brief Textfeld Beschreibung */
  QPushButton *btn_imaging;       /**< @brief IMage Import/Edit Dialog */
  IsbnRequest *m_isbnRequest;     /**< @brief ISBN Abfrage Klasse */
  QTabWidget *m_tabWidget;   /**< @brief BeschreibungsText und ISBN Info  */
  QListWidget *m_listWidget; /**< @brief ISBN Abfrage-Vorschau */

  /**
     @brief image_preview
     @todo Siehe @b imaging Unterprojekt!
   */
  QWidget *image_preview;

  /**
   @brief Hier werden die Daten aus der Abfrage eingefügt.

   Weil beim einfügen blockSignals() wegen isModified()
   eingesetz wird. Darf das nicht innerhalb der SQLQuery
   schleife erfolgen!
  */
  QList<BookDataField> dbDataSet;

  /**
     @brief sendToDatabase
     @param INSERT/UPDATE SQL Statement senden!
   */
  void sendQueryDatabase(const QString &sqlStatement);

  /**
     @brief Alles was Gift ist muss raus ...
     @return String
   */
  const QString stripValue(const QVariant &);

  /**
     @brief SQL UPDATE Statement erstellen!
     @note In @ref saveData() erfolgt eine @i ib_id Abfrage!
   */
  void updateDataSet();

  /**
     @brief SQL INSERT Statement erstellen!
     @note In @ref saveData() erfolgt eine @i ib_id Abfrage!
   */
  void insertDataSet();

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
  void addDataFromQuery(const QString &key, const QVariant &value);

private Q_SLOTS:
  /**
     @brief Button open Imaging clicked()
   */
  void triggerImageEdit();

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

public Q_SLOTS:
  /**
     @brief Durchläuft @ref dbDataSet und ruft @ref addDataFromQuery auf.
     @note Die Größe von @ref dbDataSet muss mindestenz 16 betragen!
   */
  void createDataSet();

  /**
     @brief Methode für Zurücksetzen Button
   */
  void restoreDataset();

Q_SIGNALS:
  /**
     @brief Bildbearbeitung öffnen
     @todo
  */
  void s_openImageEditor(double);

public:
  BookEditor(QDialog *parent = nullptr);

  /**
    @brief Wenn Bearbeiten darf der Eintrag nicht 0 sein!
    @param sql Abfragekorpus @i ohne WHERE
  */
  void readDataBaseEntry(const QString &sql);
};

Q_DECLARE_METATYPE(BookDataField);

#endif // BOOKEDITOR_H
