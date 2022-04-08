#include "bookeditor.h"
#include "articleid.h"
#include "editionedit.h"
#include "isbnedit.h"
#include "isbnrequest.h"
#include "messagebox.h"
#include "priceedit.h"
#include "setlanguage.h"
#include "storageedit.h"
#include "strlineedit.h"
#include "version.h"
#include "yearedit.h"

#include <QtCore/QDebug>
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
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>

BookEditor::BookEditor(QDialog *parent) : QWidget{parent} {
  setObjectName("BookEditor");
  setWindowTitle(tr("Edit Book Title"));

  Qt::Alignment defaultAlignment =
      (Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);

  QSpacerItem *m_horizontalSpacer =
      new QSpacerItem(8, 8, QSizePolicy::Expanding, QSizePolicy::Minimum);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("mainLayout");

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

  ib_count = new QSpinBox(this);
  ib_count->setObjectName("ib_count");

  lay1->addWidget(ib_count);

  QLabel *ibPriceLabel = new QLabel(this);
  ibPriceLabel->setObjectName("ib_priceLabel");
  ibPriceLabel->setText(tr("Price:"));

  lay1->addWidget(ibPriceLabel);

  ib_price = new PriceEdit(this);
  ib_price->setObjectName("ib_price");

  lay1->addWidget(ib_price);

  ib_signed = new QCheckBox(this);
  ib_signed->setObjectName("ib_signed");
  ib_signed->setText(tr("Signed Version"));

  lay1->addWidget(ib_signed);

  ib_restricted = new QCheckBox(tr("Restricted Sale"), this);
  ib_restricted->setObjectName("ib_restricted");
  ib_restricted->setToolTip(
      tr("Is the title not for sale nationally or is it on a censorship list. "
         "This is relevant for the Shopsystem."));

  lay1->addWidget(ib_restricted);

  lay1->addItem(m_horizontalSpacer);

  mainLayout->addLayout(lay1);

  QGridLayout *gridLayout = new QGridLayout();
  gridLayout->setObjectName("gridLayout");

  QHBoxLayout *lay2 = new QHBoxLayout();
  lay2->setObjectName("lay2");

  QLabel *ibPagecountLabel = new QLabel(this);
  ibPagecountLabel->setObjectName("ib_pagecountLabel");
  ibPagecountLabel->setAlignment(defaultAlignment);
  ibPagecountLabel->setText(tr("Page Count:"));

  lay2->addWidget(ibPagecountLabel);

  ib_pagecount = new QSpinBox(this);
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

  ib_weight = new QSpinBox(this);
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

  image_preview = new QWidget(this);
  image_preview->setObjectName("image_preview");
  image_preview->setMinimumSize(QSize(320, 400));
  image_preview->setMaximumSize(QSize(320, 450));

  gridLayout->addWidget(image_preview, 0, 1, 3, 1);

  QHBoxLayout *lay3 = new QHBoxLayout();
  lay3->setObjectName("lay3");

  ib_storage = new StorageEdit(this);
  ib_storage->setObjectName("ib_storage");

  lay3->addWidget(ib_storage);

  QLabel *ibVolumeLabel = new QLabel(this);
  ibVolumeLabel->setObjectName("ib_volumeLabel");
  ibVolumeLabel->setAlignment(defaultAlignment);
  ibVolumeLabel->setText(tr("Volume:"));

  lay3->addWidget(ibVolumeLabel);

  ib_volume = new QSpinBox(this);
  ib_volume->setObjectName("ib_volume");
  ib_volume->setPrefix(tr("Bd."));

  lay3->addWidget(ib_volume);

  gridLayout->addLayout(lay3, 1, 0, 1, 1);

  QGridLayout *lay4 = new QGridLayout();
  lay4->setObjectName("lay4");

  QLabel *ibTitleLabel = new QLabel(this);
  ibTitleLabel->setObjectName("ib_titleLabel");
  ibTitleLabel->setAlignment(defaultAlignment);
  ibTitleLabel->setText(tr("Book &Title:"));

  lay4->addWidget(ibTitleLabel, 0, 0, 1, 1);

  ib_title = new StrLineEdit(this);
  ib_title->setObjectName("ib_title");
  ib_title->setMaxAllowedLength(80);
  ibTitleLabel->setBuddy(ib_title);

  lay4->addWidget(ib_title, 0, 1, 1, 1);

  QLabel *ibExtendedLabel = new QLabel(this);
  ibExtendedLabel->setObjectName("ib_extendedLabel");
  ibExtendedLabel->setAlignment(defaultAlignment);
  ibExtendedLabel->setText(tr("Book Title Extended:"));

  lay4->addWidget(ibExtendedLabel, 1, 0, 1, 1);

  ib_title_extended = new StrLineEdit(this);
  ib_title_extended->setObjectName("ib_title_extended");
  ib_title_extended->setMaxAllowedLength(148);

  lay4->addWidget(ib_title_extended, 1, 1, 1, 1);

  QLabel *ibAuthorLabel = new QLabel(this);
  ibAuthorLabel->setObjectName("ib_authorLabel");
  ibAuthorLabel->setAlignment(defaultAlignment);
  ibAuthorLabel->setText(tr("&Author:"));

  lay4->addWidget(ibAuthorLabel, 2, 0, 1, 1);

  ib_author = new StrLineEdit(this);
  ib_author->setObjectName("ib_author");
  ib_author->setMaxAllowedLength(128);
  ib_author->setToolTip(tr("Requiered input field"));
  ibAuthorLabel->setBuddy(ib_author);

  lay4->addWidget(ib_author, 2, 1, 1, 1);

  QLabel *ibPublisherLabel = new QLabel(this);
  ibPublisherLabel->setObjectName("ib_publisherLabel");
  ibPublisherLabel->setAlignment(defaultAlignment);
  ibPublisherLabel->setText(tr("Publisher:"));

  lay4->addWidget(ibPublisherLabel, 3, 0, 1, 1);

  ib_publisher = new StrLineEdit(this);
  ib_publisher->setObjectName("ib_publisher");
  ib_publisher->setMaxAllowedLength(128);

  lay4->addWidget(ib_publisher, 3, 1, 1, 1);

  QLabel *ibKeywordLabel = new QLabel(this);
  ibKeywordLabel->setObjectName("ib_keywordLabel");
  ibKeywordLabel->setAlignment(defaultAlignment);
  ibKeywordLabel->setText(tr("Keyword:"));

  lay4->addWidget(ibKeywordLabel, 4, 0, 1, 1);

  ib_keyword = new StrLineEdit(this);
  ib_keyword->setObjectName("ib_keyword");
  ib_keyword->setMaxAllowedLength(60);

  lay4->addWidget(ib_keyword, 4, 1, 1, 1);

  QLabel *ibConditionLabel = new QLabel(this);
  ibConditionLabel->setObjectName("ib_conditionLabel");
  ibConditionLabel->setAlignment(defaultAlignment);
  ibConditionLabel->setText(tr("Condition:"));

  lay4->addWidget(ibConditionLabel, 5, 0, 1, 1);

  ib_condition = new StrLineEdit(this);
  ib_condition->setObjectName("ib_condition");
  ib_condition->setKeyword("ib_condition");
  ib_condition->setMaxAllowedLength(128);

  lay4->addWidget(ib_condition, 5, 1, 1, 1);

  QLabel *ibDesignationLabel = new QLabel(this);
  ibDesignationLabel->setObjectName("ib_designationLabel");
  ibDesignationLabel->setAlignment(defaultAlignment);
  ibDesignationLabel->setText(tr("Designation:"));

  lay4->addWidget(ibDesignationLabel, 6, 0, 1, 1);

  ib_designation = new StrLineEdit(this);
  ib_designation->setObjectName("ib_designation");
  ib_designation->setKeyword("ib_designation");
  ib_designation->setMaxAllowedLength(128);

  lay4->addWidget(ib_designation, 6, 1, 1, 1);

  QLabel *ibEditionLabel = new QLabel(this);
  ibEditionLabel->setObjectName("ib_editionLabel");
  ibEditionLabel->setAlignment(defaultAlignment);
  ibEditionLabel->setText(tr("Edition:"));

  lay4->addWidget(ibEditionLabel, 7, 0, 1, 1);

  ib_edition = new EditionEdit(this);
  ib_edition->setObjectName("ib_edition");

  lay4->addWidget(ib_edition, 7, 1, 1, 1);

  QPushButton *m_btnQueryISBN = new QPushButton(this);
  m_btnQueryISBN->setText("OpenLibrary ISBN");
  m_btnQueryISBN->setToolTip(tr("Send ISBN request to openlibrary.org"));
  m_btnQueryISBN->setEnabled(false);
  m_btnQueryISBN->setIcon(myIcon("autostart"));
  lay4->addWidget(m_btnQueryISBN, 8, 0, 1, 1);

  ib_isbn = new IsbnEdit(this);
  ib_isbn->setObjectName("ib_isbn");

  connect(ib_isbn, SIGNAL(s_isbnIsValid(bool)), m_btnQueryISBN,
          SLOT(setEnabled(bool)));

  lay4->addWidget(ib_isbn, 8, 1, 1, 1);

  QLabel *ibLanguageLabel = new QLabel(this);
  ibLanguageLabel->setObjectName("ib_languageLabel");
  ibLanguageLabel->setAlignment(defaultAlignment);
  ibLanguageLabel->setText(tr("Language:"));

  lay4->addWidget(ibLanguageLabel, 9, 0, 1, 1);

  ib_language = new SetLanguage(this);
  ib_language->setObjectName("ib_language");

  lay4->addWidget(ib_language, 9, 1, 1, 1);

  gridLayout->addLayout(lay4, 2, 0, 1, 1);

  mainLayout->addLayout(gridLayout);

  QHBoxLayout *lay5 = new QHBoxLayout();

  lay5->addItem(m_horizontalSpacer);
  // TODO PushButton
  btn_imaging = new QPushButton(myIcon("image"), tr("Picture"), this);
  btn_imaging->setObjectName("OpenImagingButton");
  btn_imaging->setToolTip(
      tr("Open the Imaging Dialog for Import and Edit Pictures."));

  lay5->addWidget(btn_imaging);

  mainLayout->addLayout(lay5);

  // TabWidget
  m_tabWidget = new QTabWidget(this);
  m_tabWidget->setObjectName("TabWidget");

  ib_description = new QTextEdit(this);
  ib_description->setObjectName("textEdit");
  ib_description->setTextInteractionFlags(Qt::TextEditorInteraction);
  ib_description->setAcceptRichText(false); // Qt::PlainText
  m_tabWidget->insertTab(0, ib_description, tr("Additional Description"));
  m_tabWidget->setTabIcon(0, myIcon("edit"));

  m_listWidget = new QListWidget(this);
  m_listWidget->setObjectName("ISBNInfoRichText");
  m_tabWidget->insertTab(1, m_listWidget, "OpenLibrary.org");
  m_tabWidget->setTabIcon(1, myIcon("folder_txt"));

  mainLayout->addWidget(m_tabWidget);

  // TabOrder
  setTabOrder(ib_id, ib_count);
  setTabOrder(ib_count, ib_price);
  setTabOrder(ib_price, ib_pagecount);
  setTabOrder(ib_pagecount, ib_weight);
  setTabOrder(ib_weight, ib_year);
  setTabOrder(ib_year, ib_storage);
  setTabOrder(ib_storage, ib_volume);
  setTabOrder(ib_volume, ib_title);
  setTabOrder(ib_title, ib_title_extended);
  setTabOrder(ib_title_extended, ib_author);
  setTabOrder(ib_author, ib_publisher);
  setTabOrder(ib_publisher, ib_keyword);
  setTabOrder(ib_keyword, ib_condition);
  setTabOrder(ib_condition, ib_designation);
  setTabOrder(ib_designation, ib_edition);
  setTabOrder(ib_edition, ib_isbn);
  setTabOrder(ib_isbn, ib_language);

  connect(m_btnQueryISBN, SIGNAL(clicked()), this, SLOT(triggerIsbnQuery()));
  connect(btn_imaging, SIGNAL(clicked()), this, SLOT(triggerImageEdit()));
  connect(m_listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this,
          SLOT(infoISBNDoubleClicked(QListWidgetItem *)));
}

void BookEditor::triggerImageEdit() {
  double id = ib_id->value().toDouble();
  emit s_openImageEditor(id);
}

/**
   Datenbankeingabe ausführen
 */
void BookEditor::sendQueryDatabase(const QString &sqlStatement) {
  QSqlDatabase db(QSqlDatabase::database(sqlConnectionName));
  if (db.isValid()) {
    // qDebug() << "BookEditor::sendToDatabase" << sqlStatement;
    QSqlQuery q = db.exec(sqlStatement);
    if (q.lastError().type() != QSqlError::NoError) {
      qDebug() << q.lastError();
    } else {
      MessageBox msgBox(this);
      msgBox.querySuccess(tr("Bookdata saved successfully!"),2);
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

/**
   SQL UPDATE Statement erstellen!
   \d inventory_books
 */
void BookEditor::updateDataSet() {
  QString set;
  QString sql("UPDATE inventory_books SET ");
  for (int i = 0; i < dbDataSet.size(); ++i) {
    BookDataField f = dbDataSet.at(i);
    if (f.field.contains("ib_id"))
      continue;

    if (f.vtype == QVariant::Int) {
      QSpinBox *sp = findChild<QSpinBox *>(f.field, Qt::FindDirectChildrenOnly);
      if (sp != nullptr) {
        set.append(f.field);
        set.append("=");
        set.append(QString::number(sp->value()));
        set.append(",");
      } else if (f.field.contains("ib_edition")) {
        set.append(f.field);
        set.append("=");
        set.append(QString::number(ib_edition->value().toInt()));
        set.append(",");
      }
    } else if (f.vtype == QVariant::Bool) {
      QCheckBox *sp =
          findChild<QCheckBox *>(f.field, Qt::FindDirectChildrenOnly);
      if (sp != nullptr) {
        set.append(f.field);
        set.append("=");
        set.append((sp->isChecked() ? "true" : "false"));
        set.append(",");
      }
    } else if (f.vtype == QVariant::String) {
      StrLineEdit *sp =
          findChild<StrLineEdit *>(f.field, Qt::FindDirectChildrenOnly);
      if (sp != nullptr) {
        set.append(f.field);
        set.append("='");
        set.append(sp->value().toString());
        set.append("',");
      } else if (f.field.contains("ib_description")) {
        set.append(f.field);
        set.append("='");
        QString plainText = ib_description->toPlainText();
        QRegExp reg("[\\']+");
        set.append(plainText.replace(reg, ""));
        set.append("',");
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
    } else if (f.vtype == QVariant::ULongLong) {
      if (f.field.contains("ib_isbn")) {
        set.append("ib_isbn=");
        set.append(ib_isbn->text());
        set.append(",");
      }
    } else {
      qDebug() << "Unknown Datatype:" << f.field << f.vtype << f.data;
    }
  }

  sql.append(set);
  sql.append("ib_changed=now() WHERE ib_id=");
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
  }
  // "ib_language" QVariant(QString, "de_DE")
  if (key.contains("ib_language")) {
    ib_language->setValue(value);
  }
  // ib_description
  if (key.contains("ib_description")) {
    ib_description->setPlainText(value.toString());
  }
  // "ib_signed" QVariant(bool)
  // "ib_restricted" QVariant(bool)
  if (value.type() == QVariant::Bool) {
    QCheckBox *v = findChild<QCheckBox *>(key, Qt::FindDirectChildrenOnly);
    if (v != nullptr)
      v->setChecked(value.toBool());
  }
  // "ib_volume" QVariant(int)
  // "ib_count" QVariant(int)
  // "ib_pagecount" QVariant(int)
  // "ib_weight" QVariant(int)
  if (value.type() == QVariant::Int) {
    QSpinBox *v = findChild<QSpinBox *>(key, Qt::FindDirectChildrenOnly);
    if (v != nullptr)
      v->setValue(value.toInt());
    return;
  }
  // QVariant(double)
  if (value.type() == QVariant::Double) {
    QDoubleSpinBox *v =
        findChild<QDoubleSpinBox *>(key, Qt::FindDirectChildrenOnly);
    if (v != nullptr)
      v->setValue(value.toDouble());
    return;
  }
  // "ib_title" QVariant(QString)
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
    }
  }
}

void BookEditor::readDataBaseEntry(const QString &sql) {
  if (sql.length() < 5)
    return;

  QString select("SELECT DISTINCT * FROM ");
  select.append("inventory_books WHERE ");
  select.append(sql);
  select.append(" ORDER BY ib_id LIMIT 1;");

  QRegularExpression reg("^ib_[a-z]+\\b");
  QStringList widgetList;
  QList<QObject *> list =
      findChildren<QObject *>(reg, Qt::FindDirectChildrenOnly);
  for (int i = 0; i < list.size(); ++i) {
    if (list.at(i) != nullptr) {
      if (!list.at(i)->objectName().isEmpty())
        widgetList << list.at(i)->objectName();
    }
  }
  // TextEdit Feld
  widgetList << "ib_description";

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
    m_tabWidget->setCurrentIndex(1);
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
  // Tab anzeigen
  m_tabWidget->setCurrentIndex(1);
}

void BookEditor::infoISBNDoubleClicked(QListWidgetItem *item) {
  QRegExp regexp;
  QString data = item->data(Qt::UserRole).toString();
  if (data.contains("ib_title:")) {
    regexp.setPattern("^ib_title:\\b");
    ib_title->setValue(data.replace(regexp, ""));
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

void BookEditor::createDataBaseEntry() { qDebug() << __FUNCTION__ << "TODO"; }
