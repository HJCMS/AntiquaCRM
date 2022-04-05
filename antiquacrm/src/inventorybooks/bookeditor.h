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

class YearEdit;
class PriceEdit;
class StorageEdit;
class EditionEdit;
class StrLineEdit;
class IsbnEdit;

class BookEditor : public QWidget {
  Q_OBJECT
  Q_CLASSINFO("Author", "Jürgen Heinemann")
  Q_CLASSINFO("URL", "http://www.hjcms.de")

private:
  // INPUT
  QCheckBox *ib_signed;
  QCheckBox *ib_restricted;
  EditionEdit *ib_edition;
  QComboBox *ib_language;
  StorageEdit *ib_storage;
  YearEdit *ib_year;
  PriceEdit *ib_price;
  QSpinBox *ib_count;
  QSpinBox *ib_pagecount;
  QSpinBox *ib_weight;
  QSpinBox *ib_volume;
  // LineINInputs
  QLineEdit *ib_id;
  IsbnEdit *ib_isbn;
  StrLineEdit *ib_author;
  StrLineEdit *ib_condition;
  StrLineEdit *ib_designation;
  StrLineEdit *ib_keyword;
  StrLineEdit *ib_publisher;
  StrLineEdit *ib_title;
  StrLineEdit *ib_title_extended;
  // QWidget
  QWidget *image_preview;
  QTextEdit *ib_description;
  //
  QHash<QString, QVariant> dbDataSet;
  void addDataFromQuery(const QString &key, const QVariant &value);

private Q_SLOTS:
  void openISBNQuery();

public Q_SLOTS:
  void restoreDataset();

public:
  BookEditor(QDialog *parent = nullptr);

  /**
    @brief Wenn Bearbeiten darf der Eintrag nicht 0 sein!
    @param sql WHERE CLAUSE
  */
  void editDataBaseEntry(const QString &sql);

  void newDataBaseEntry();
};

#endif // BOOKEDITOR_H
