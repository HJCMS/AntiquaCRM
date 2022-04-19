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
#include "sqlcore.h"
#include "storageedit.h"
#include "strlineedit.h"
#include "textfield.h"
#include "version.h"
#include "yearedit.h"

#include <QtCore>
#include <QtGui/QDesktopServices>
#include <QtWidgets>

BookEditor::BookEditor(QWidget *parent) : QWidget{parent} {
  setObjectName("BookEditor");
  setWindowTitle(tr("Edit Book Title"));
  setMinimumSize(800, 600);

  ApplSettings config;

  db = new HJCMS::SqlCore(this);

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
  ib_id->setRequired(true);

  lay1->addWidget(ib_id);

  QLabel *ibCountLabel = new QLabel(this);
  ibCountLabel->setObjectName("ib_countLabel");
  ibCountLabel->setAlignment(defaultAlignment);
  ibCountLabel->setText(tr("Count:"));

  lay1->addWidget(ibCountLabel);

  ib_count = new IntSpinBox(this);
  ib_count->setObjectName("ib_count");
  ib_count->setWindowTitle(tr("Count"));

  lay1->addWidget(ib_count);

  QLabel *ibPriceLabel = new QLabel(this);
  ibPriceLabel->setObjectName("ib_priceLabel");
  ibPriceLabel->setText(tr("Price:"));

  lay1->addWidget(ibPriceLabel);

  double minPrice = config.value("books/min_price", 8.00).toDouble();
  ib_price = new PriceEdit(this);
  ib_price->setObjectName("ib_price");
  ib_price->setRequired(true);
  ib_price->setMinimum(minPrice);

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
  ib_pagecount->setWindowTitle(tr("Pagecount"));
  ib_pagecount->setRequired(true);

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
  ib_weight->setWindowTitle(tr("Weight"));
  ib_weight->setRequired(true);

  lay2->addWidget(ib_weight);

  QLabel *ibYearLabel = new QLabel(this);
  ibYearLabel->setObjectName("ib_yearLabel");
  ibYearLabel->setAlignment(defaultAlignment);
  ibYearLabel->setText(tr("Year:"));

  lay2->addWidget(ibYearLabel);

  ib_year = new YearEdit(this);
  ib_year->setObjectName("ib_year");
  ib_year->setWindowTitle(tr("Year"));
  ib_year->setRequired(true);

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
  ib_storage->setRequired(true);

  lay3->addWidget(ib_storage);

  QLabel *ibEditionLabel = new QLabel(this);
  ibEditionLabel->setObjectName("ibeditionlabel");
  ibEditionLabel->setAlignment(defaultAlignment);
  ibEditionLabel->setText(tr("Edition:"));
  lay3->addWidget(ibEditionLabel);

  ib_edition = new IntSpinBox(this);
  ib_edition->setObjectName("ib_edition");
  ib_edition->setRequired(true);
  ib_edition->setWindowTitle(tr("Edition"));
  lay3->addWidget(ib_edition);

  QLabel *ibVolumeLabel = new QLabel(this);
  ibVolumeLabel->setObjectName("ib_volumeLabel");
  ibVolumeLabel->setAlignment(defaultAlignment);
  ibVolumeLabel->setText(tr("Volume:"));

  lay3->addWidget(ibVolumeLabel);

  ib_volume = new IntSpinBox(this);
  ib_volume->setObjectName("ib_volume");
  ib_volume->setPrefix(tr("Bd."));
  ib_volume->setWindowTitle(tr("Volume"));

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
  ib_title->setRequired(true);
  ib_title->setWindowTitle(tr("Booktitle"));
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
  ib_title_extended->setWindowTitle(tr("Extended Title"));

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
  ib_author->setRequired(true);
  ib_author->setWindowTitle(tr("Author"));
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
  ib_publisher->setWindowTitle(tr("Publisher"));

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
  ib_keyword->setWindowTitle(tr("Chop Keyword"));

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
  ib_condition->setRequired(true);
  ib_condition->setWindowTitle(tr("Condition"));
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
  ib_designation->setRequired(true);
  ib_designation->setWindowTitle(tr("Designation"));

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

  m_tabWidget = new QTabWidget(this);
  m_tabWidget->setObjectName("TabWidget");

  ib_description = new TextField(this);
  ib_description->setObjectName("ib_description");
  ib_description->setWindowTitle(tr("Public Description"));
  m_tabWidget->insertTab(0, ib_description, tr("Public Description"));
  m_tabWidget->setTabIcon(0, myIcon("edit"));
  m_tabWidget->setTabToolTip(0, tr("This text is displayed on web pages"));

  ib_internal_description = new TextField(this);
  ib_internal_description->setObjectName("ib_internal_description");
  ib_description->setWindowTitle(tr("Internal Description"));
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
  connect(btn_imaging, SIGNAL(clicked()), this, SLOT(openImageDialog()));
  connect(m_listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this,
          SLOT(infoISBNDoubleClicked(QListWidgetItem *)));
  connect(m_actionBar, SIGNAL(s_cancelClicked()), this, SLOT(andLeaveEditor()));
  connect(m_actionBar, SIGNAL(s_restoreClicked()), this,
          SLOT(restoreDataset()));
  connect(m_actionBar, SIGNAL(s_saveClicked()), this, SLOT(saveData()));
  connect(m_actionBar, SIGNAL(s_finishClicked()), this,
          SLOT(checkLeaveEditor()));

  setInputList();
}

void BookEditor::setInputList() {
  QList<QObject *> list =
      findChildren<QObject *>(p_objPattern, Qt::FindChildrenRecursively);
  for (int i = 0; i < list.size(); ++i) {
    if (list.at(i) != nullptr) {
      if (!list.at(i)->objectName().isEmpty())
        inputList << list.at(i)->objectName();
    }
  }
}

void BookEditor::openImageDialog() {
  qulonglong id = ib_id->value().toLongLong();
  ApplSettings cfg;
  QString p = cfg.value("imaging/sourcepath").toString();
  ImageDialog *dialog = new ImageDialog(id, this);
  if (!dialog->setSourceTarget(p)) {
    qDebug() << Q_FUNC_INFO << id << "Invalid Source Target:" << p;
    return;
  }
  if (id >= 1)
    emit s_openImageEditor(id);

  if (dialog->exec()) {
    QImage img = dialog->getImage();
    if (!img.isNull())
      m_imageView->addNewImage(id, img);
  }
}

void BookEditor::resetModified() {
  foreach (QString name, inputList) {
    QObject *child = findChild<QObject *>(name, Qt::FindChildrenRecursively);
    if (child != nullptr) {
      QMetaObject::invokeMethod(child, "setModified", Qt::DirectConnection,
                                Q_ARG(bool, false));
    }
  }
}

bool BookEditor::sendSqlQuery(const QString &sqlStatement) {
  // qDebug() << Q_FUNC_INFO << sqlStatement;
  MessageBox msgBox(this);
  QSqlQuery q = db->query(sqlStatement);
  if (q.lastError().type() != QSqlError::NoError) {
    QString errorString = db->fetchErrors();
    qDebug() << errorString << Qt::endl;
    msgBox.queryFail(errorString);
    return false;
  } else {
    msgBox.querySuccess(tr("Bookdata saved successfully!"), 1);
    resetModified();
    return true;
  }
}

const QHash<QString, QVariant> BookEditor::createSqlDataset() {
  QHash<QString, QVariant> data;
  MessageBox messanger;
  QList<StrLineEdit *> listStr =
      findChildren<StrLineEdit *>(p_objPattern, Qt::FindDirectChildrenOnly);
  QList<StrLineEdit *>::Iterator i_str;
  for (i_str = listStr.begin(); i_str != listStr.end(); ++i_str) {
    StrLineEdit *cur = *i_str;
    if (cur->isRequired() && !cur->isValid()) {
      messanger.noticeMessage(cur->notes());
      cur->setFocus();
      data.clear();
      return data;
    }
    if (cur->value().toString().isEmpty())
      continue;

    data.insert(cur->objectName(), cur->value());
  }
  listStr.clear();
  QList<IntSpinBox *> listInt =
      findChildren<IntSpinBox *>(p_objPattern, Qt::FindDirectChildrenOnly);
  QList<IntSpinBox *>::Iterator i_int;
  for (i_int = listInt.begin(); i_int != listInt.end(); ++i_int) {
    IntSpinBox *cur = *i_int;
    if (cur->isRequired() && !cur->isValid()) {
      messanger.noticeMessage(cur->notes());
      cur->setFocus();
      data.clear();
      return data;
    }
    if (cur->value().toInt() == 0)
      continue;

    data.insert(cur->objectName(), cur->value());
  }
  listInt.clear();
  if (ib_storage->isValid()) {
    data.insert("ib_storage", ib_storage->value());
  }
  if (ib_language->isValid()) {
    data.insert("ib_language", ib_language->value());
  }
  if (ib_signed->isChecked()) {
    data.insert("ib_signed", ib_signed->value());
  }
  if (ib_restricted->isChecked()) {
    data.insert("ib_restricted", ib_restricted->value());
  }
  if (ib_isbn->isValid()) {
    data.insert("ib_isbn", ib_isbn->value());
  }
  if (ib_description->isValid()) {
    data.insert("ib_description", ib_description->value());
  }
  if (ib_internal_description->isValid()) {
    data.insert("ib_internal_description", ib_internal_description->value());
  }
  if (!ib_year->isValid()) {
    messanger.noticeMessage(ib_year->notes());
    ib_year->setFocus();
    data.clear();
  } else {
    data.insert("ib_year", ib_year->value());
  }
  if (!ib_price->isValid()) {
    messanger.noticeMessage(ib_price->notes());
    ib_price->setFocus();
    data.clear();
  } else {
    data.insert("ib_price", ib_price->value());
  }
  return data;
}

void BookEditor::createSqlUpdate() {
  QString set;
  for (int i = 0; i < sqlQueryResult.size(); ++i) {
    BookData f = sqlQueryResult.at(i);
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
        set.append(ib_description->value().toString());
        set.append("',");
      } else if (f.field.contains("ib_internal_description")) {
        set.append(f.field);
        set.append("='");
        set.append(ib_internal_description->value().toString());
        set.append("',");
      }
      if (f.field.contains("ib_language")) {
        set.append(f.field);
        set.append("='");
        set.append(ib_language->value().toString());
        set.append("',");
      } else {
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
      qWarning("Unknown Field '%s' in UPDATE statement.", qPrintable(f.field));
    }
  }

  QString sql("UPDATE inventory_books SET ");
  sql.append(set);
  sql.append("ib_changed=CURRENT_TIMESTAMP WHERE ib_id=");
  sql.append(ib_id->value().toString());
  sql.append(";");
  sendSqlQuery(sql);
}

void BookEditor::createSqlInsert() {
  /** Bei neu Einträgen immer erforderlich */
  ib_count->setRequired(true);

  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 1)
    return;

  QStringList column; /**< SQL Columns */
  QStringList values; /**< SQL Values */
  QHash<QString, QVariant>::iterator it;
  for (it = data.begin(); it != data.end(); ++it) {
    if (it.value().toString().isEmpty())
      continue;

    column.append(it.key());
    if (it.value().type() == QVariant::String) {
      values.append("'" + it.value().toString() + "'");
    } else {
      values.append(it.value().toString());
    }
  }

  QString sql("INSERT INTO inventory_books (");
  sql.append(column.join(","));
  sql.append(",ib_changed) VALUES (");
  sql.append(values.join(","));
  sql.append(",CURRENT_TIMESTAMP);");
  // qDebug() << Q_FUNC_INFO << sql << Qt::endl;
  if (sendSqlQuery(sql))
    checkLeaveEditor();
}

void BookEditor::importSqlResult() {
  if (sqlQueryResult.size() < 15)
    return;

  blockSignals(true);
  for (int i = 0; i < sqlQueryResult.size(); ++i) {
    BookData f = sqlQueryResult.at(i);
    setSqlQueryData(f.field, f.data);
  }
  blockSignals(false);

  // Nach Ersteintrag zurück setzen!
  resetModified();

  // Suche Bilddaten
  int id = ib_id->value().toInt();
  if (id > 0)
    m_imageView->searchImageById(id);
}

void BookEditor::clearDataFields() {
  QList<QObject *> list =
      findChildren<QObject *>(p_objPattern, Qt::FindChildrenRecursively);
  for (int i = 0; i < list.size(); ++i) {
    if (list.at(i) != nullptr) {
      QMetaObject::invokeMethod(list.at(i), "reset", Qt::QueuedConnection);
    }
  }
  m_imageView->clear();
}

bool BookEditor::checkIsModified() {
  QList<QObject *> list =
      findChildren<QObject *>(p_objPattern, Qt::FindChildrenRecursively);
  for (int i = 0; i < list.size(); ++i) {
    if (list.at(i) != nullptr) {
      bool b = false;
      if (QMetaObject::invokeMethod(list.at(i), "hasModified",
                                    Qt::DirectConnection,
                                    Q_RETURN_ARG(bool, b))) {

        if (list.at(i)->objectName().contains("ib_price"))
          qDebug() << Q_FUNC_INFO << list.at(i)->objectName() << b;

        if (b) {
          return true;
        }
      }
    }
  }
  return false;
}

void BookEditor::checkLeaveEditor() {
  if (checkIsModified()) {
    emit s_sendMessage(
        tr("Unsaved Changes, don't leave this page before saved."));
    return;
  }
  andLeaveEditor();
}

void BookEditor::andLeaveEditor() {
  m_listWidget->clear();              /**< OpenLibrary.org Anzeige leeren */
  sqlQueryResult.clear();             /**< SQL History leeren */
  clearDataFields();                  /**< Alle Datenfelder leeren */
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  emit s_leaveEditor();               /**< Zurück */
}

void BookEditor::restoreDataset() {
  if (sqlQueryResult.isEmpty())
    return;

  clearDataFields();
  importSqlResult();
}

void BookEditor::setSqlQueryData(const QString &key, const QVariant &value) {
  // qDebug() << key << value;
  if (key.contains("ib_id")) {
    ib_id->setValue(value);
    return;
  }
  if (key.contains("ib_isbn")) {
    ib_isbn->setValue(value);
    return;
  }
  if (key.contains("ib_year")) {
    ib_year->setValue(value);
    return;
  }
  if (key.contains("ib_storage")) {
    ib_storage->setValue(value);
    return;
  }
  if (key.contains("ib_price")) {
    ib_price->setValue(value);
    return;
  }
  if (key.contains("ib_edition")) {
    ib_edition->setValue(value);
    return;
  }
  if (key.contains("ib_language")) {
    ib_language->setValue(value);
    return;
  }
  if (value.type() == QVariant::Bool) {
    BoolBox *v = findChild<BoolBox *>(key, Qt::FindDirectChildrenOnly);
    if (v != nullptr)
      v->setValue(value);

    return;
  }
  if (value.type() == QVariant::Int) {
    IntSpinBox *v = findChild<IntSpinBox *>(key, Qt::FindDirectChildrenOnly);
    if (v != nullptr)
      v->setValue(value);

    return;
  }
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
  if (key.contains("ib_description")) {
    ib_description->setValue(value);
    return;
  }
  if (key.contains("ib_internal_description")) {
    ib_internal_description->setValue(value);
    return;
  }
  qDebug() << "Missing" << key << value << value.type();
}

void BookEditor::saveData() {
  if (ib_id->text().isEmpty()) {
    createSqlInsert();
  } else {
    createSqlUpdate();
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

  // ib_title
  QListWidgetItem *title = new QListWidgetItem(m_listWidget);
  QString title_txt = isbnData.value("title").toString();
  title->setData(Qt::DisplayRole, tr("Booktitle") + ": " + title_txt);
  title->setData(Qt::UserRole, "ib_title:" + title_txt);
  title->setIcon(myIcon("edit"));
  title->setFlags(flags);
  m_listWidget->addItem(title);

  if (!isbnData.value("title_extended").isNull()) {
    QListWidgetItem *title_ex = new QListWidgetItem(m_listWidget);
    QString title_ext_txt = isbnData.value("title_extended").toString();
    title_ex->setData(Qt::DisplayRole, tr("Subtitle") + ": " + title_ext_txt);
    title_ex->setData(Qt::UserRole, "ib_title_extended:" + title_ext_txt);
    title_ex->setIcon(myIcon("edit"));
    title_ex->setFlags(flags);
    m_listWidget->addItem(title_ex);
  }

  if (!isbnData.value("authors").isNull()) {
    QListWidgetItem *author = new QListWidgetItem(m_listWidget);
    QString authors = isbnData.value("authors").toString();
    author->setData(Qt::DisplayRole, tr("Authors") + ": " + authors);
    author->setData(Qt::UserRole, "ib_author:" + authors);
    author->setIcon(myIcon("edit_group"));
    author->setFlags(flags);
    m_listWidget->addItem(author);
  }

  if (!isbnData.value("year").isNull()) {
    QListWidgetItem *year = new QListWidgetItem(m_listWidget);
    QString year_txt = isbnData.value("year").toString();
    year->setData(Qt::DisplayRole, tr("Year") + ": " + year_txt);
    year->setData(Qt::UserRole, "ib_year:" + year_txt);
    year->setIcon(myIcon("edit"));
    year->setFlags(flags);
    m_listWidget->addItem(year);
  }

  if (!isbnData.value("publisher").isNull()) {
    QString publisher_txt = isbnData.value("publisher").toString();
    QListWidgetItem *publisher = new QListWidgetItem(m_listWidget);
    publisher->setData(Qt::DisplayRole, tr("Publisher") + ": " + publisher_txt);
    publisher->setData(Qt::UserRole, "ib_publisher:" + publisher_txt);
    publisher->setIcon(myIcon("group"));
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

  if (!isbnData.value("pages").isNull()) {
    QListWidgetItem *pages = new QListWidgetItem(m_listWidget);
    QString pages_txt = isbnData.value("pages").toString();
    pages->setData(Qt::DisplayRole, tr("Pages") + ": " + pages_txt);
    pages->setData(Qt::UserRole, "ib_pagecount:" + pages_txt);
    pages->setIcon(myIcon("edit"));
    pages->setFlags(flags ^ Qt::ItemIsEnabled);
    m_listWidget->addItem(pages);
  }

  if (isbnData.value("images").toBool()) {
    QListWidgetItem *graphs = new QListWidgetItem(m_listWidget);
    graphs->setData(Qt::DisplayRole, tr("An image exists on OpenLibrary.org"));
    graphs->setData(Qt::UserRole, true);
    graphs->setIcon(myIcon("image"));
    graphs->setFlags(flags ^ Qt::ItemIsEnabled);
    m_listWidget->addItem(graphs);
  }

  QListWidgetItem *donate = new QListWidgetItem(m_listWidget);
  donate->setData(Qt::DisplayRole,
                  tr("OpenLibrary is free to use, but we need your Help!"));
  donate->setData(Qt::UserRole, "ib_website:https://archive.org/donate/");
  donate->setIcon(myIcon("html"));
  donate->setFlags(flags);
  m_listWidget->addItem(donate);

  m_tabWidget->setCurrentIndex(isbnTabIndex);
}

void BookEditor::infoISBNDoubleClicked(QListWidgetItem *item) {
  QRegExp regexp;
  QString data = item->data(Qt::UserRole).toString();
  if (data.contains("ib_title:")) {
    regexp.setPattern("^ib_title:\\b");
    data = data.replace(regexp, "");
    // Überlange Einträge von externen Quelle unterbinden! Wenn der
    // Vorschlag zu lang ist dann in "ib_title_extended" einfügen!
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

void BookEditor::openBookEntry(const QString &condition) {
  if (condition.length() < 5)
    return;

  QString select("SELECT * FROM inventory_books WHERE ");
  select.append(condition);
  select.append(" ORDER BY ib_id LIMIT 1;");

  // qDebug() "SELECT ->" << select << db.isValid();
  QSqlQuery q = db->query(select);
  if (q.size() != 0) {
    QSqlRecord r = db->record("inventory_books");
    sqlQueryResult.clear();
    while (q.next()) {
      foreach (QString key, inputList) {
        QVariant val = q.value(r.indexOf(key));
        BookData d;
        d.field = key;
        d.vtype = val.type();
        d.data = val;
        sqlQueryResult.append(d);
      }
    }
  } else {
    qDebug() << Q_FUNC_INFO << condition << db->fetchErrors() << Qt::endl;
  }

  if (!sqlQueryResult.isEmpty() && !m_actionBar->isRestoreable())
    m_actionBar->setRestoreable(true);

  btn_imaging->setEnabled(true);
  importSqlResult();
}

void BookEditor::createBookEntry() {
  setEnabled(true);
  btn_imaging->setEnabled(false);
  resetModified();
}
