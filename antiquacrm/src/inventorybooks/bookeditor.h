#ifndef BOOKEDITOR_H
#define BOOKEDITOR_H

#include <QtCore/QObject>
#include <QtCore/QVariant>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QSpinBox>
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

/**
   @brief Buch Editor Klasse
 */
class BookEditor : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  QCheckBox *ib_signed;     /**< @brief Signiert? */
  QCheckBox *ib_restricted; /**< @brief Zensiert? */
  EditionEdit *ib_edition;
  SetLanguage *ib_language;
  StorageEdit *ib_storage;
  YearEdit *ib_year;
  PriceEdit *ib_price;
  QSpinBox *ib_count;
  QSpinBox *ib_pagecount;
  QSpinBox *ib_weight;
  QSpinBox *ib_volume;
  ArticleID *ib_id; /**< @brief ReadOnly:ArticleID */
  IsbnEdit *ib_isbn;
  StrLineEdit *ib_author;
  StrLineEdit *ib_condition;
  StrLineEdit *ib_designation;
  StrLineEdit *ib_keyword;
  StrLineEdit *ib_publisher;
  StrLineEdit *ib_title;
  StrLineEdit *ib_title_extended;
  QTextEdit *ib_description;

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
  QHash<QString, QVariant> dbDataSet;

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
     @brief Signal accept() abfangen.
   */
  void saveData();

  /**
     @brief openISBNQuery
     @todo Im moment noch NICHT Implimetiert!
   */
  void openISBNQuery();

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

public:
  BookEditor(QDialog *parent = nullptr);

  /**
    @brief Wenn Bearbeiten darf der Eintrag nicht 0 sein!
    @param sql Abfragekorpus @i ohne WHERE
  */
  void editDataBaseEntry(const QString &sql);

  /**
     @brief Wird aufgerufen wenn keine Daten vorhanden sind!
   */
  void createDataBaseEntry();
};

#endif // BOOKEDITOR_H
