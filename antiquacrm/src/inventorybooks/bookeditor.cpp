#include "bookeditor.h"
#include "applsettings.h"
#include "articleid.h"
#include "booksimageview.h"
#include "boolbox.h"
#include "editoractionbar.h"
#include "imagedialog.h"
#include "intspinbox.h"
#include "isbnedit.h"
#include "isbnrequest.h"
#include "messagebox.h"
#include "priceedit.h"
#include "setlanguage.h"
#include "storageedit.h"
#include "strlineedit.h"
#include "textfield.h"
#include "version.h"
#include "yearedit.h"

#include <QtCore/QDebug>
#include <QtCore/QMetaObject>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtCore/QList>
#include <QtCore/QUrl>
#include <QtGui/QDesktopServices>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidgetItem>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

// @see triggerImageEdit
static const QString imgPath() {
  QString p = QDir::homePath();
  p.append("/Developement/antiqua/database/tmp/BildDaten");
  return p;
};

BookEditor::BookEditor(QWidget *parent) : QWidget{parent} {
  setObjectName("BookEditor");
  setWindowTitle(tr("Edit Book Title"));
  setMinimumSize(800, 600);

  Qt::Alignment defaultAlignment =
      (Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("mainLayout");

  // Begin ErsteZeile
  QHBoxLayout *lay1 = new QHBoxLayout();
  lay1->setObjectName("lay1");

  QLabel *ib_idLabel = new QLabel(this);
  ib_idLabel->setObjectName("ib_idLabel");
  ib_idLabel->setAlignment(defaultAlignment);
  ib_idLabel->setText(tr("Article ID:"));

  lay1->addWidget(ib_idLabel);

  ib_id = new ArticleID(this);
  ib_id->setObjectName("ib_id");

  lay1->addWidget(ib_id);

  QLabel *ibCountLabel = new QLabel(this);
  ibCountLabel->setObjectName("ib_countLabel");
  ibCountLabel->setAlignment(defaultAlignment);
  ibCountLabel->setText(tr("Count:"));

  lay1->addWidget(ibCountLabel);

  ib_count = new IntSpinBox(this);
  ib_count->setObjectName("ib_count");

  lay1->addWidget(ib_count);

  QLabel *ibPriceLabel = new QLabel(this);
  ibPriceLabel->setObjectName("ib_priceLabel");
  ibPriceLabel->setText(tr("Price:"));

  lay1->addWidget(ibPriceLabel);

  ib_price = new PriceEdit(this);
  ib_price->setObjectName("ib_price");

  lay1->addWidget(ib_price);

  ib_signed = new BoolBox(this);
  ib_signed->setObjectName("ib_signed");
  ib_signed->setText(tr("Signed Version"));

  lay1->addWidget(ib_signed);

  ib_restricted = new BoolBox(this);
  ib_restricted->setObjectName("ib_restricted");
  ib_restricted->setText(tr("Restricted Sale"));
  ib_restricted->setToolTip(
      tr("Is the title not for sale nationally or is it on a censorship list. "
         "This is relevant for the Shopsystem."));

  lay1->addWidget(ib_restricted);

  lay1->addStretch(1);

  mainLayout->addLayout(lay1);
  // End ErsteZeile

  QGridLayout *gridLayout = new QGridLayout();
  gridLayout->setObjectName("gridLayout");

  // Begin gridLayout:Row(0)
  QHBoxLayout *lay2 = new QHBoxLayout();
  lay2->setObjectName("lay2");
  lay2->addStretch(1);

  QLabel *ibPagecountLabel = new QLabel(this);
  ibPagecountLabel->setObjectName("ib_pagecountLabel");
  ibPagecountLabel->setAlignment(defaultAlignment);
  ibPagecountLabel->setText(tr("Page Count:"));

  lay2->addWidget(ibPagecountLabel);

  ib_pagecount = new IntSpinBox(this);
  ib_pagecount->setObjectName("ib_pagecount");
  ib_pagecount->setContextMenuPolicy(Qt::DefaultContextMenu);
  ib_pagecount->setMinimum(10);
  ib_pagecount->setMaximum(100000);
  ib_pagecount->setSingleStep(10);

  lay2->addWidget(ib_pagecount);

  QLabel *ibWeightLabel = new QLabel(this);
  ibWeightLabel->setObjectName("ib_weightLabel");
  ibWeightLabel->setAlignment(defaultAlignment);
  ibWeightLabel->setText(tr("Weight:"));

  lay2->addWidget(ibWeightLabel);

  ib_weight = new IntSpinBox(this);
  ib_weight->setObjectName("ib_weight");
  ib_weight->setMinimum(100);
  ib_weight->setMaximum(100000000);
  ib_weight->setSuffix(tr("g"));

  lay2->addWidget(ib_weight);

  QLabel *ibYearLabel = new QLabel(this);
  ibYearLabel->setObjectName("ib_yearLabel");
  ibYearLabel->setAlignment(defaultAlignment);
  ibYearLabel->setText(tr("Year:"));

  lay2->addWidget(ibYearLabel);

  ib_year = new YearEdit(this);
  ib_year->setObjectName("ib_year");

  lay2->addWidget(ib_year);

  gridLayout->addLayout(lay2, 0, 0, 1, 1);

  m_imageView = new BooksImageView(this);
  gridLayout->addWidget(m_imageView, 0, 1, 3, 1);
  // END gridLayout:Row(0)

  // Begin gridLayout:Row(1)
  QHBoxLayout *lay3 = new QHBoxLayout();
  lay3->setObjectName("lay3");
  lay3->addStretch(1);

  ib_storage = new StorageEdit(this);
  ib_storage->setObjectName("ib_storage");

  lay3->addWidget(ib_storage);

  QLabel *ibEditionLabel = new QLabel(this);
  ibEditionLabel->setObjectName("ibeditionlabel");
  ibEditionLabel->setAlignment(defaultAlignment);
  ibEditionLabel->setText(tr("Edition:"));
  lay3->addWidget(ibEditionLabel);

  ib_edition = new IntSpinBox(this);
  ib_edition->setObjectName("ib_edition");
  lay3->addWidget(ib_edition);

  QLabel *ibVolumeLabel = new QLabel(this);
  ibVolumeLabel->setObjectName("ib_volumeLabel");
  ibVolumeLabel->setAlignment(defaultAlignment);
  ibVolumeLabel->setText(tr("Volume:"));

  lay3->addWidget(ibVolumeLabel);

  ib_volume = new IntSpinBox(this);
  ib_volume->setObjectName("ib_volume");
  ib_volume->setPrefix(tr("Bd."));

  lay3->addWidget(ib_volume);

  gridLayout->addLayout(lay3, 1, 0, 1, 1);
  // END gridLayout:Row(1)

  // Begin gridLayout:Row(2)
  //  Zeile 0
  QGridLayout *lay4 = new QGridLayout();
  lay4->setObjectName("lay4");
  int glc = 0; /**< Gridlayout Row Counter */

  QLabel *ibTitleLabel = new QLabel(this);
  ibTitleLabel->setObjectName("ib_titleLabel");
  ibTitleLabel->setAlignment(defaultAlignment);
  ibTitleLabel->setText(tr("Book &Title:"));

  lay4->addWidget(ibTitleLabel, glc, 0, 1, 1);

  ib_title = new StrLineEdit(this);
  ib_title->setObjectName("ib_title");
  ib_title->setMaxAllowedLength(80);
  ib_title->setToolTip(tr("Required input field. Limited to 80 characters, "
                          "Webshop Systems require this."));
  ibTitleLabel->setBuddy(ib_title);

  lay4->addWidget(ib_title, glc++, 1, 1, 1);

  // Zeile 1
  QLabel *ibExtendedLabel = new QLabel(this);
  ibExtendedLabel->setObjectName("ib_extendedLabel");
  ibExtendedLabel->setAlignment(defaultAlignment);
  ibExtendedLabel->setText(tr("Book Title Extended:"));

  lay4->addWidget(ibExtendedLabel, glc, 0, 1, 1);

  ib_title_extended = new StrLineEdit(this);
  ib_title_extended->setObjectName("ib_title_extended");
  ib_title_extended->setMaxAllowedLength(148);
  ib_title_extended->setToolTip(tr("Extended Title or Book Subtitle."));

  lay4->addWidget(ib_title_extended, glc++, 1, 1, 1);

  // Zeile 2
  QLabel *ibAuthorLabel = new QLabel(this);
  ibAuthorLabel->setObjectName("ib_authorLabel");
  ibAuthorLabel->setAlignment(defaultAlignment);
  ibAuthorLabel->setText(tr("&Author:"));

  lay4->addWidget(ibAuthorLabel, glc, 0, 1, 1);

  ib_author = new StrLineEdit(this);
  ib_author->setObjectName("ib_author");
  ib_author->setMaxAllowedLength(128);
  ib_author->setToolTip(
      tr("Format: Firstname lastname (Different Authors separated by comma)."));
  ibAuthorLabel->setBuddy(ib_author);

  lay4->addWidget(ib_author, glc++, 1, 1, 1);

  // Zeile 3
  QLabel *ibPublisherLabel = new QLabel(this);
  ibPublisherLabel->setObjectName("ib_publisherLabel");
  ibPublisherLabel->setAlignment(defaultAlignment);
  ibPublisherLabel->setText(tr("Publisher:"));
  ibPublisherLabel->setToolTip(tr("Enter hier the Book Publisher."));

  lay4->addWidget(ibPublisherLabel, glc, 0, 1, 1);

  ib_publisher = new StrLineEdit(this);
  ib_publisher->setObjectName("ib_publisher");
  ib_publisher->setMaxAllowedLength(128);

  lay4->addWidget(ib_publisher, glc++, 1, 1, 1);

  // Zeile 4
  QLabel *ibKeywordLabel = new QLabel(this);
  ibKeywordLabel->setObjectName("ib_keywordLabel");
  ibKeywordLabel->setAlignment(defaultAlignment);
  ibKeywordLabel->setText(tr("Keyword:"));

  lay4->addWidget(ibKeywordLabel, glc, 0, 1, 1);

  ib_keyword = new StrLineEdit(this);
  ib_keyword->setObjectName("ib_keyword");
  ib_keyword->setMaxAllowedLength(60);
  ib_keyword->setToolTip(tr("Category Keywords for Shopsystems."));

  lay4->addWidget(ib_keyword, glc++, 1, 1, 1);

  // Zeile 5
  QLabel *ibConditionLabel = new QLabel(this);
  ibConditionLabel->setObjectName("ib_conditionLabel");
  ibConditionLabel->setAlignment(defaultAlignment);
  ibConditionLabel->setText(tr("Condition:"));

  lay4->addWidget(ibConditionLabel, glc, 0, 1, 1);

  ib_condition = new StrLineEdit(this);
  ib_condition->setObjectName("ib_condition");
  ib_condition->setMaxAllowedLength(128);
  ib_condition->setToolTip(
      tr("Condition of this Book. See also Configuration conditions Table."));

  lay4->addWidget(ib_condition, glc++, 1, 1, 1);

  // Zeile 6
  QLabel *ibDesignationLabel = new QLabel(this);
  ibDesignationLabel->setObjectName("ib_designationLabel");
  ibDesignationLabel->setAlignment(defaultAlignment);
  ibDesignationLabel->setText(tr("Designation:"));

  lay4->addWidget(ibDesignationLabel, glc, 0, 1, 1);

  ib_designation = new StrLineEdit(this);
  ib_designation->setObjectName("ib_designation");
  ib_designation->setMaxAllowedLength(128);

  lay4->addWidget(ib_designation, glc++, 1, 1, 1);

  // Zeile 7
  QLabel *ibLanguageLabel = new QLabel(this);
  ibLanguageLabel->setObjectName("ib_languageLabel");
  ibLanguageLabel->setAlignment(defaultAlignment);
  ibLanguageLabel->setText(tr("Language:"));

  lay4->addWidget(ibLanguageLabel, glc, 0, 1, 1);

  ib_language = new SetLanguage(this);
  ib_language->setObjectName("ib_language");

  lay4->addWidget(ib_language, glc++, 1, 1, 1);

  // Zeile 8
  QPushButton *m_btnQueryISBN = new QPushButton(this);
  m_btnQueryISBN->setText("OpenLibrary ISBN");
  m_btnQueryISBN->setToolTip(tr("Send ISBN request to openlibrary.org"));
  m_btnQueryISBN->setEnabled(false);
  m_btnQueryISBN->setIcon(myIcon("folder_txt"));
  lay4->addWidget(m_btnQueryISBN, glc, 0, 1, 1);

  ib_isbn = new IsbnEdit(this);
  ib_isbn->setObjectName("ib_isbn");
  lay4->addWidget(ib_isbn, glc++, 1, 1, 1);

  // Zeile 9
  QHBoxLayout *lay5 = new QHBoxLayout();
  lay5->setObjectName("last_horizontal_layout");

  btn_createJob =
      new QPushButton(myIcon("autostart"), tr("Create order"), this);
  btn_createJob->setObjectName("CreateJobFromThis");
  btn_createJob->setToolTip(tr("Create a purchase order from this listing."));
  btn_createJob->setEnabled(false);
  lay5->addWidget(btn_createJob);

  lay5->addStretch(1);

  btn_imaging = new QPushButton(myIcon("image"), tr("Picture"), this);
  btn_imaging->setObjectName("OpenImagingButton");
  btn_imaging->setToolTip(
      tr("Open the Imaging Dialog for Import and Edit Pictures."));
  lay5->addWidget(btn_imaging);

  lay4->addLayout(lay5, glc++, 0, 1, 2);

  gridLayout->addLayout(lay4, 2, 0, 1, 1);
  // End gridLayout:Row(2)

  mainLayout->addLayout(gridLayout);

  // TabWidget
  m_tabWidget = new QTabWidget(this);
  m_tabWidget->setObjectName("TabWidget");

  ib_description = new TextField(this);
  ib_description->setObjectName("ib_description");
  m_tabWidget->insertTab(0, ib_description, tr("Public Description"));
  m_tabWidget->setTabIcon(0, myIcon("edit"));
  m_tabWidget->setTabToolTip(0, tr("This text is displayed on web pages"));

  ib_internal_description = new TextField(this);
  ib_internal_description->setObjectName("ib_internal_description");
  m_tabWidget->insertTab(1, ib_internal_description,
                         tr("Internal Description"));
  m_tabWidget->setTabIcon(1, myIcon("edit"));
  m_tabWidget->setTabToolTip(1, tr("This text is for internal purposes"));

  isbnTabIndex = 2;
  m_listWidget = new QListWidget(this);
  m_listWidget->setObjectName("isbnqueryresult");
  m_tabWidget->insertTab(isbnTabIndex, m_listWidget, "OpenLibrary.org");
  m_tabWidget->setTabIcon(isbnTabIndex, myIcon("folder_txt"));

  mainLayout->addWidget(m_tabWidget);

  m_actionBar = new EditorActionBar(this);
  mainLayout->addWidget(m_actionBar);

  // TabOrder
  setTabOrder(ib_id, ib_count);
  setTabOrder(ib_count, ib_price);
  setTabOrder(ib_price, ib_pagecount);
  setTabOrder(ib_pagecount, ib_weight);
  setTabOrder(ib_weight, ib_year);
  setTabOrder(ib_year, ib_storage);
  setTabOrder(ib_storage, ib_edition);
  setTabOrder(ib_edition, ib_volume);
  setTabOrder(ib_volume, ib_title);
  setTabOrder(ib_title, ib_title_extended);
  setTabOrder(ib_title_extended, ib_author);
  setTabOrder(ib_author, ib_publisher);
  setTabOrder(ib_publisher, ib_keyword);
  setTabOrder(ib_keyword, ib_condition);
  setTabOrder(ib_condition, ib_designation);
  setTabOrder(ib_designation, ib_language);
  setTabOrder(ib_language, ib_isbn);

  connect(ib_isbn, SIGNAL(s_isbnIsValid(bool)), m_btnQueryISBN,
          SLOT(setEnabled(bool)));
  connect(m_btnQueryISBN, SIGNAL(clicked()), this, SLOT(triggerIsbnQuery()));
  connect(btn_imaging, SIGNAL(clicked()), this, SLOT(triggerImageEdit()));
  connect(m_listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this,
          SLOT(infoISBNDoubleClicked(QListWidgetItem *)));
  connect(m_actionBar, SIGNAL(s_restoreClicked()), this,
          SLOT(restoreDataset()));
  connect(m_actionBar, SIGNAL(s_saveClicked()), this, SLOT(saveData()));
  connect(m_actionBar, SIGNAL(s_finishClicked()), this,
          SLOT(checkLeaveEditor()));
}

void BookEditor::triggerImageEdit() {
  qulonglong id = ib_id->value().toLongLong();
  ApplSettings cfg;
  QString p = cfg.value("imaging/sourcepath", imgPath()).toString();
  ImageDialog *dialog = new ImageDialog(id, this);
  if (!dialog->setSourceTarget(p)) {
    qDebug() << Q_FUNC_INFO << id << "Invalid Source Target:" << p;
    return;
  }
  if(id>=1)
    emit s_openImageEditor(id);

  if (dialog->exec()) {
    QImage img = dialog->getImage();
    if (!img.isNull())
      m_imageView->addNewImage(id, img);
  }
}

void BookEditor::sendQueryDatabase(const QString &sqlStatement) {
  // qDebug() << Q_FUNC_INFO << sqlStatement;
  QSqlDatabase db(QSqlDatabase::database(sqlConnectionName));
  if (db.isValid()) {
    // qDebug() << "BookEditor::sendToDatabase" << sqlStatement;
    QSqlQuery q = db.exec(sqlStatement);
    if (q.lastError().type() != QSqlError::NoError) {
      qDebug() << q.lastError();
    } else {
      MessageBox msgBox(this);
      msgBox.querySuccess(tr("Bookdata saved successfully!"), 1);
    }
    db.close();
  }
}

const QString BookEditor::stripValue(const QVariant &v) {
  QString s = v.toString().trimmed();
  if (s.isEmpty())
    return QString();

  QRegExp reg("[\\'\\\"]+");
  s = s.replace(reg, "");
  reg.setPattern("[\\s\\t]+");
  return s.replace(reg, " ");
}

void BookEditor::updateDataSet() {
  QString set;
  QString sql("UPDATE inventory_books SET ");
  for (int i = 0; i < dbDataSet.size(); ++i) {
    BookDataField f = dbDataSet.at(i);
    if (f.field.contains("ib_id"))
      continue;

    if (f.vtype == QVariant::Int) {
      IntSpinBox *sp =
          findChild<IntSpinBox *>(f.field, Qt::FindDirectChildrenOnly);
      if (sp != nullptr) {
        set.append(f.field);
        set.append("=");
        set.append(sp->value().toString());
        set.append(",");
      } else if (f.field.contains("ib_storage")) {
        set.append(f.field);
        set.append("=");
        set.append(ib_storage->value().toString());
        set.append(",");
      }
    } else if (f.vtype == QVariant::Bool) {
      BoolBox *sp = findChild<BoolBox *>(f.field, Qt::FindDirectChildrenOnly);
      if (sp != nullptr) {
        set.append(f.field);
        set.append("=");
        set.append((sp->isChecked() ? "true" : "false"));
        set.append(",");
      }
    } else if (f.vtype == QVariant::String) {
      if (f.field.contains("ib_description")) {
        set.append(f.field);
        set.append("='");
        QString plainText = ib_description->toPlainText();
        QRegExp reg("[\\']+");
        set.append(plainText.replace(reg, ""));
        set.append("',");
      } else if (f.field.contains("ib_internal_description")) {
        set.append(f.field);
        set.append("='");
        QString plainText = ib_internal_description->toPlainText();
        QRegExp reg("[\\']+");
        set.append(plainText.replace(reg, ""));
        set.append("',");
      }
      if (f.field.contains("ib_language")) {
        set.append(f.field);
        set.append("='");
        set.append(ib_language->value().toString());
        set.append("',");
      } else {
        // Jetzt alle StrLineEdit* durchgehen.
        StrLineEdit *sp =
            findChild<StrLineEdit *>(f.field, Qt::FindDirectChildrenOnly);
        if (sp != nullptr) {
          set.append(f.field);
          set.append("='");
          set.append(sp->value().toString());
          set.append("',");
        }
      }
    } else if (f.vtype == QVariant::Double) {
      if (f.field.contains("ib_price")) {
        set.append("ib_price=");
        set.append(ib_price->value().toString());
        set.append(",");
      } else if (f.field.contains("ib_year")) {
        set.append("ib_year=");
        set.append(ib_year->value().toString());
        set.append(",");
      }
    } else if ((f.vtype == QVariant::ULongLong) ||
               (f.vtype == QVariant::LongLong)) {
      if (f.field.contains("ib_isbn")) {
        set.append("ib_isbn=");
        if (ib_isbn->isValid()) {
          set.append(ib_isbn->text());
        } else {
          set.append("0");
        }
        set.append(",");
      }
    } else {
      qWarning("Unknown or Empty Datatype:");
      qDebug() << Q_FUNC_INFO << f.field << f.vtype << f.data;
    }
  }

  sql.append(set);
  sql.append("ib_changed=CURRENT_TIMESTAMP WHERE ib_id=");
  sql.append(ib_id->value().toString());
  sql.append(";");
  sendQueryDatabase(sql);
}

/**
   SQL INSERT Statement erstellen!

  struct BookDataField {
    QString field; @brief Fieldname
    int vtype;    @brief QVariant::Type
    QVariant data; @brief Datset
  };
*/
void BookEditor::insertDataSet() {
  qDebug() << "BookEditor::insertDataSet";
  QString fieldSet; /**< SQL Column */
  QString valueSet; /**< SQL Value */
  for (int i = 0; i < dbDataSet.size(); ++i) {
    BookDataField f = dbDataSet.at(i);
    if (f.field.contains("ib_id"))
      continue;

    fieldSet.append(f.field);
    valueSet.append("TODO");
  } // end for

  QString sql("INSERT INTO inventory_books (");
  sql.append(fieldSet);
  sql.append("= VALUES (");
  sql.append(valueSet);
  sql.append(");");

  qDebug() << sql << Qt::endl;
  // sendQueryDatabase(sql);
}

void BookEditor::createDataSet() {
  if (dbDataSet.size() < 15)
    return;

  blockSignals(true);

  for (int i = 0; i < dbDataSet.size(); ++i) {
    BookDataField f = dbDataSet.at(i);
    addDataFromQuery(f.field, f.data);
  }

  blockSignals(false);

  // Suche Bilddaten
  int id = ib_id->value().toInt();
  if (id > 0)
    m_imageView->searchImageById(id);
}

void BookEditor::clearDataFields() {
  QRegularExpression reg("^ib_[a-z_]+\\b$");
  QStringList widgetList;
  QList<QObject *> list =
      findChildren<QObject *>(reg, Qt::FindChildrenRecursively);
  for (int i = 0; i < list.size(); ++i) {
    if (list.at(i) != nullptr) {
      QMetaObject::invokeMethod(list.at(i), "reset", Qt::QueuedConnection);
    }
  }
  m_imageView->clear();
}

void BookEditor::checkLeaveEditor() {
  if (checkUnsafedData()) {
    // Nicht gespeicherte Änderungen, verlassen Sie diese Seite nicht,
    // bevor Sie sie gespeichert haben.
    emit s_sendMessage(
        tr("Unsaved Changes, don't leave this page before saved."));
    return;
  }
  // Aufräumen
  clearDataFields();
  dbDataSet.clear();
  m_actionBar->setRestoreable(false);
  // und tschüss
  emit s_leaveEditor();
}

void BookEditor::restoreDataset() {
  if (dbDataSet.isEmpty())
    return;

  createDataSet();
}

void BookEditor::addDataFromQuery(const QString &key, const QVariant &value) {
  // qDebug() << key << value;
  // "ib_id" QVariant(int)
  if (key.contains("ib_id")) {
    ib_id->setValue(value);
    return;
  }
  // "ib_isbn" QVariant(qulonglong)
  if (key.contains("ib_isbn")) {
    ib_isbn->setValue(value);
    return;
  }
  // "ib_year" QVariant(double)
  if (key.contains("ib_year")) {
    ib_year->setValue(value);
    return;
  }
  // "ib_storage" QVariant(int)
  if (key.contains("ib_storage")) {
    ib_storage->setValue(value);
    return;
  }
  // "ib_price" QVariant(double)
  if (key.contains("ib_price")) {
    ib_price->setValue(value);
    return;
  }
  // "ib_edition"
  if (key.contains("ib_edition")) {
    ib_edition->setValue(value);
    return;
  }
  // "ib_language" QVariant(QString, "de_DE")
  if (key.contains("ib_language")) {
    ib_language->setValue(value);
    return;
  }
  // ib_description
  if (key.contains("ib_description")) {
    ib_description->setValue(value);
    return;
  }
  // ib_internal_description
  if (key.contains("ib_internal_description")) {
    ib_internal_description->setValue(value);
    return;
  }
  // "ib_signed" QVariant(bool)
  // "ib_restricted" QVariant(bool)
  if (value.type() == QVariant::Bool) {
    BoolBox *v = findChild<BoolBox *>(key, Qt::FindDirectChildrenOnly);
    if (v != nullptr)
      v->setValue(value);

    return;
  }
  // "ib_volume" QVariant(int)
  // "ib_count" QVariant(int)
  // "ib_pagecount" QVariant(int)
  // "ib_weight" QVariant(int)
  if (value.type() == QVariant::Int) {
    IntSpinBox *v = findChild<IntSpinBox *>(key, Qt::FindDirectChildrenOnly);
    if (v != nullptr)
      v->setValue(value);

    return;
  }
  // "ib_title" QVariant(QString)
  // "ib_title_extended" QVariant(QString)
  // "ib_author" QVariant(QString)
  // "ib_publisher" QVariant(QString)
  // "ib_keyword" QVariant(QString)
  // "ib_condition" QVariant(QString)
  // "ib_designation" QVariant(QString)
  if (value.type() == QVariant::String) {
    StrLineEdit *v = findChild<StrLineEdit *>(key, Qt::FindDirectChildrenOnly);
    if (v != nullptr) {
      if (!value.toString().contains("NOT_SET")) {
        // TODO FIXME SET RED COLOR
        v->setValue(value);
      }
      return;
    }
  }
  qDebug() << "Missing" << key << value << value.type();
}

bool BookEditor::checkUnsafedData() {
  for (int i = 0; i < dbDataSet.size(); ++i) {
    BookDataField f = dbDataSet.at(i);
    QObject *obj = findChild<QObject *>(f.field, Qt::FindChildrenRecursively);
    if (obj != nullptr) {
      bool b = false;
      if (QMetaObject::invokeMethod(obj, "hasModified", Qt::DirectConnection,
                                    Q_RETURN_ARG(bool, b))) {
        if (b) {
          qDebug() << Q_FUNC_INFO << f.field << f.data << b << Qt::endl;
          return true;
        }
      }
    }
  }
  return false;
}

void BookEditor::readDataBaseEntry(const QString &sql) {
  if (sql.length() < 5)
    return;

  QString select("SELECT DISTINCT * FROM ");
  select.append("inventory_books WHERE ");
  select.append(sql);
  select.append(" ORDER BY ib_id LIMIT 1;");

  QRegularExpression reg("^ib_[a-z_]+\\b$");
  QStringList widgetList;
  QList<QObject *> list =
      findChildren<QObject *>(reg, Qt::FindChildrenRecursively);
  for (int i = 0; i < list.size(); ++i) {
    if (list.at(i) != nullptr) {
      if (!list.at(i)->objectName().isEmpty())
        widgetList << list.at(i)->objectName();
    }
  }
  // Fixme -- add Missing Fields
  if (!widgetList.contains("ib_description")) {
    widgetList << "ib_description";
  }
  if (!widgetList.contains("ib_internal_description")) {
    widgetList << "ib_internal_description";
  }
  if (!widgetList.contains("ib_title_extended")) {
    widgetList << "ib_title_extended";
  }

  QSqlDatabase db(QSqlDatabase::database(sqlConnectionName));
  // qDebug() "SELECT ->" << select << db.isValid();
  if (db.isValid()) {
    QSqlQuery q = db.exec(select);
    QSqlRecord r = db.record("inventory_books");
    dbDataSet.clear();
    while (q.next()) {
      foreach (QString key, widgetList) {
        QVariant val = q.value(r.indexOf(key));
        // qDebug() << "KEY -->" << key << val;
        BookDataField d;
        d.field = key;
        d.vtype = val.type();
        d.data = val;
        dbDataSet.append(d);
      }
    }
    db.close();
  }

  if (!dbDataSet.isEmpty() && !m_actionBar->isRestoreable())
    m_actionBar->setRestoreable(true);

  // Jetzt Daten einfügen ...
  createDataSet();
}

void BookEditor::saveData() {
  if (ib_id->text().isEmpty()) {
    insertDataSet();
  } else {
    updateDataSet();
  }
}

void BookEditor::setIsbnInfo(bool b) {
  m_listWidget->clear();

  Qt::ItemFlags flags(Qt::ItemIsEnabled | Qt::ItemIsSelectable |
                      Qt::ItemNeverHasChildren);

  if (!b) {
    QListWidgetItem *noData = new QListWidgetItem(m_listWidget);
    noData->setData(Qt::DisplayRole, tr("No datasets were found."));
    noData->setIcon(myIcon("messagebox_warning"));
    noData->setFlags(flags ^ Qt::ItemIsEnabled);
    m_listWidget->addItem(noData);
    m_tabWidget->setCurrentIndex(isbnTabIndex);
    return;
  }

  if (m_isbnRequest == nullptr)
    return;

  const QMap<QString, QVariant> isbnData = m_isbnRequest->getResponse();
  if (isbnData.size() < 1)
    return;

  /*
    QMapIterator<QString, QVariant> i(isbnData);
    while (i.hasNext()) {
       i.next();
       qDebug() << i.key() << i.value() << Qt::endl;
    }
  */

  // ib_title
  QListWidgetItem *title = new QListWidgetItem(m_listWidget);
  QString title_txt = isbnData.value("title").toString();
  title->setData(Qt::DisplayRole, tr("Title") + ": " + title_txt);
  title->setData(Qt::UserRole, "ib_title:" + title_txt);
  title->setIcon(myIcon("edit"));
  title->setToolTip(tr("Booktitle"));
  title->setFlags(flags);
  m_listWidget->addItem(title);

  if (!isbnData.value("authors").isNull()) {
    QListWidgetItem *author = new QListWidgetItem(m_listWidget);
    QString authors = isbnData.value("authors").toString();
    author->setData(Qt::DisplayRole, tr("Authors") + ": " + authors);
    author->setData(Qt::UserRole, "ib_author:" + authors);
    author->setIcon(myIcon("edit_group"));
    author->setToolTip(tr("Authors"));
    author->setFlags(flags);
    m_listWidget->addItem(author);
  }

  if (!isbnData.value("publish_date").isNull()) {
    QListWidgetItem *year = new QListWidgetItem(m_listWidget);
    QString year_txt = isbnData.value("publish_date").toString();
    year->setData(Qt::DisplayRole, tr("Year") + ": " + year_txt);
    year->setData(Qt::UserRole, "ib_year:" + year_txt);
    year->setIcon(myIcon("edit"));
    year->setToolTip(tr("Publisher Year"));
    year->setFlags(flags);
    m_listWidget->addItem(year);
  }

  if (!isbnData.value("publishers").isNull()) {
    QString publisher_txt = isbnData.value("publishers").toString();
    if (!isbnData.value("publish_places").isNull()) {
      publisher_txt.append("/");
      publisher_txt.append(isbnData.value("publish_places").toString());
    }
    QListWidgetItem *publisher = new QListWidgetItem(m_listWidget);
    publisher->setData(Qt::DisplayRole, tr("Publisher") + ": " + publisher_txt);
    publisher->setData(Qt::UserRole, "ib_publisher:" + publisher_txt);
    publisher->setIcon(myIcon("group"));
    publisher->setToolTip(tr("Publisher"));
    publisher->setFlags(flags);
    m_listWidget->addItem(publisher);
  }

  if (!isbnData.value("url").isNull()) {
    QListWidgetItem *website = new QListWidgetItem(m_listWidget);
    website->setData(Qt::DisplayRole,
                     tr("Open Webpage in Browser for full Description."));
    website->setData(Qt::UserRole,
                     "ib_website:" + isbnData.value("url").toString());
    website->setIcon(myIcon("html"));
    website->setToolTip(tr("External Book Description"));
    website->setFlags(flags);
    m_listWidget->addItem(website);
  }

  int images = 0;
  // Ein Bild existiert auf OpenLibrary.org
  if (!isbnData.value("medium_image").isNull()) {
    ++images;
  }
  if (!isbnData.value("large_image").isNull()) {
    ++images;
    ;
  }
  if (images > 0) {
    QListWidgetItem *graphs = new QListWidgetItem(m_listWidget);
    graphs->setData(Qt::DisplayRole, tr("An image exists on OpenLibrary.org"));
    graphs->setData(Qt::UserRole, images);
    graphs->setIcon(myIcon("image"));
    graphs->setToolTip(tr("Images"));
    graphs->setFlags(flags ^ Qt::ItemIsEnabled);
    m_listWidget->addItem(graphs);
  }

  QListWidgetItem *donate = new QListWidgetItem(m_listWidget);
  donate->setData(Qt::DisplayRole,
                  tr("OpenLibrary is free to use, but we need your Help!"));
  donate->setData(Qt::UserRole, "ib_website:https://archive.org/donate/");
  donate->setIcon(myIcon("html"));
  donate->setToolTip(tr("Donation"));
  donate->setFlags(flags);
  m_listWidget->addItem(donate);

  // Tab anzeigen
  m_tabWidget->setCurrentIndex(isbnTabIndex);
}

void BookEditor::infoISBNDoubleClicked(QListWidgetItem *item) {
  QRegExp regexp;
  QString data = item->data(Qt::UserRole).toString();
  if (data.contains("ib_title:")) {
    regexp.setPattern("^ib_title:\\b");
    data = data.replace(regexp, "");
    /**
     Überlange Einträge von externen
     Quelle unterbinden! Wenn der
     Vorschlag zu lang ist dann in
     "ib_title_extended" einfügen!
    */
    if (data.length() > 79) {
      ib_title_extended->setValue(data);
    } else {
      ib_title->setValue(data);
    }
  } else if (data.contains("ib_author:")) {
    regexp.setPattern("^ib_author:\\b");
    ib_author->setValue(data.replace(regexp, ""));
  } else if (data.contains("ib_year:")) {
    regexp.setPattern("^ib_year:");
    QString buffer = data.replace(regexp, "");
    bool b = true;
    ib_year->setValue(buffer.toDouble(&b));
  } else if (data.contains("ib_publisher:")) {
    regexp.setPattern("^ib_publisher:\\b");
    ib_publisher->setValue(data.replace(regexp, ""));
  } else if (data.contains("ib_website:")) {
    regexp.setPattern("^ib_website:\\b");
    QUrl url(data.replace(regexp, ""));
    QDesktopServices::openUrl(url);
  }
}

void BookEditor::changeEvent(QEvent *event) {
  if (event->type() == QEvent::EnabledChange) {
    ib_storage->loadStorageData();
    ib_condition->loadDataset("ib_condition");
    ib_designation->loadDataset("ib_designation");
  }
}

void BookEditor::triggerIsbnQuery() {
  QString isbn = ib_isbn->text().trimmed();
  int l = isbn.length();
  if (l > 13 && l != 10) {
    return;
  }
  m_isbnRequest = new IsbnRequest(isbn, this);
  connect(m_isbnRequest, SIGNAL(requestFinished(bool)), this,
          SLOT(setIsbnInfo(bool)));
  m_isbnRequest->triggerRequest();
}
