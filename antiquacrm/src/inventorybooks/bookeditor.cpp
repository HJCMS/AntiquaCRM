#include "bookeditor.h"
#include "applsettings.h"
#include "serialid.h"
#include "imagewidget.h"
#include "imagetoolbar.h"
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

  ib_id = new SerialID(this);
  ib_id->setObjectName("ib_id");
  ib_id->setRequired(true);

  lay1->addWidget(ib_id);

  QLabel *countLabel = new QLabel(this);
  countLabel->setObjectName("countLabel");
  countLabel->setAlignment(defaultAlignment);
  countLabel->setText(tr("Count:"));

  lay1->addWidget(countLabel);

  ib_count = new IntSpinBox(this);
  ib_count->setObjectName("ib_count");
  ib_count->setWindowTitle(tr("Count"));

  lay1->addWidget(ib_count);

  QLabel *priceLabel = new QLabel(this);
  priceLabel->setObjectName("priceLabel");
  priceLabel->setText(tr("Price:"));

  lay1->addWidget(priceLabel);

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

  QLabel *pagecountLabel = new QLabel(this);
  pagecountLabel->setObjectName("pagecountLabel");
  pagecountLabel->setAlignment(defaultAlignment);
  pagecountLabel->setText(tr("Page Count:"));

  lay2->addWidget(pagecountLabel);

  ib_pagecount = new IntSpinBox(this);
  ib_pagecount->setObjectName("ib_pagecount");
  ib_pagecount->setContextMenuPolicy(Qt::DefaultContextMenu);
  ib_pagecount->setMinimum(10);
  ib_pagecount->setMaximum(100000);
  ib_pagecount->setSingleStep(10);
  ib_pagecount->setWindowTitle(tr("Pagecount"));
  ib_pagecount->setRequired(true);

  lay2->addWidget(ib_pagecount);

  QLabel *weightLabel = new QLabel(this);
  weightLabel->setObjectName("weightLabel");
  weightLabel->setAlignment(defaultAlignment);
  weightLabel->setText(tr("Weight:"));

  lay2->addWidget(weightLabel);

  ib_weight = new IntSpinBox(this);
  ib_weight->setObjectName("ib_weight");
  ib_weight->setMinimum(100);
  ib_weight->setMaximum(100000000);
  ib_weight->setSuffix(tr("g"));
  ib_weight->setWindowTitle(tr("Weight"));
  ib_weight->setRequired(true);

  lay2->addWidget(ib_weight);

  QLabel *yearLabel = new QLabel(this);
  yearLabel->setObjectName("yearLabel");
  yearLabel->setAlignment(defaultAlignment);
  yearLabel->setText(tr("Year:"));

  lay2->addWidget(yearLabel);

  ib_year = new YearEdit(this);
  ib_year->setObjectName("ib_year");
  ib_year->setWindowTitle(tr("Year"));
  ib_year->setRequired(true);

  lay2->addWidget(ib_year);

  gridLayout->addLayout(lay2, 0, 0, 1, 1);

  m_imageView = new ImageWidget(this);
  gridLayout->addWidget(m_imageView, 0, 1, 3, 1);
  // END gridLayout:Row(0)

  // Begin gridLayout:Row(1)
  QHBoxLayout *lay3 = new QHBoxLayout();
  lay3->setObjectName("lay3");

  ib_storage = new StorageEdit(this);
  ib_storage->setObjectName("ib_storage");

  lay3->addWidget(ib_storage);
  lay3->addStretch(1);

  QLabel *editionlabel = new QLabel(this);
  editionlabel->setObjectName("editionlabel");
  editionlabel->setAlignment(defaultAlignment);
  editionlabel->setText(tr("Edition:"));
  lay3->addWidget(editionlabel);

  ib_edition = new IntSpinBox(this);
  ib_edition->setObjectName("ib_edition");
  ib_edition->setRequired(true);
  ib_edition->setWindowTitle(tr("Edition"));
  lay3->addWidget(ib_edition);

  QLabel *volumeLabel = new QLabel(this);
  volumeLabel->setObjectName("volumeLabel");
  volumeLabel->setAlignment(defaultAlignment);
  volumeLabel->setText(tr("Volume:"));

  lay3->addWidget(volumeLabel);

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

  QLabel *titleLabel = new QLabel(this);
  titleLabel->setObjectName("titleLabel");
  titleLabel->setAlignment(defaultAlignment);
  titleLabel->setText(tr("Book &Title:"));

  lay4->addWidget(titleLabel, glc, 0, 1, 1);

  ib_title = new StrLineEdit(this);
  ib_title->setObjectName("ib_title");
  ib_title->setMaxAllowedLength(80);
  ib_title->setRequired(true);
  ib_title->setWindowTitle(tr("Title"));
  ib_title->setToolTip(tr("Required input field. Limited to 80 characters, "
                          "Webshop Systems require this."));
  titleLabel->setBuddy(ib_title);

  lay4->addWidget(ib_title, glc++, 1, 1, 1);

  // Zeile 1
  QLabel *extendedLabel = new QLabel(this);
  extendedLabel->setObjectName("extendedLabel");
  extendedLabel->setAlignment(defaultAlignment);
  extendedLabel->setText(tr("Booktitle Extended:"));

  lay4->addWidget(extendedLabel, glc, 0, 1, 1);

  ib_title_extended = new StrLineEdit(this);
  ib_title_extended->setObjectName("ib_title_extended");
  ib_title_extended->setMaxAllowedLength(148);
  ib_title_extended->setToolTip(tr("Extended Title or Subtitle."));
  ib_title_extended->setWindowTitle(tr("Extended Title"));

  lay4->addWidget(ib_title_extended, glc++, 1, 1, 1);

  // Zeile 2
  QLabel *authorLabel = new QLabel(this);
  authorLabel->setObjectName("authorLabel");
  authorLabel->setAlignment(defaultAlignment);
  authorLabel->setText(tr("&Author:"));

  lay4->addWidget(authorLabel, glc, 0, 1, 1);

  ib_author = new StrLineEdit(this);
  ib_author->setObjectName("ib_author");
  ib_author->setMaxAllowedLength(128);
  ib_author->setRequired(true);
  ib_author->setWindowTitle(tr("Author"));
  ib_author->setToolTip(
      tr("Format: Firstname lastname (Different Authors separated by comma)."));
  authorLabel->setBuddy(ib_author);

  lay4->addWidget(ib_author, glc++, 1, 1, 1);

  // Zeile 3
  QLabel *publisherLabel = new QLabel(this);
  publisherLabel->setObjectName("publisherLabel");
  publisherLabel->setAlignment(defaultAlignment);
  publisherLabel->setText(tr("Publisher:"));
  publisherLabel->setToolTip(tr("Enter hier the Book Publisher."));

  lay4->addWidget(publisherLabel, glc, 0, 1, 1);

  ib_publisher = new StrLineEdit(this);
  ib_publisher->setObjectName("ib_publisher");
  ib_publisher->setMaxAllowedLength(128);
  ib_publisher->setWindowTitle(tr("Publisher"));

  lay4->addWidget(ib_publisher, glc++, 1, 1, 1);

  // Zeile 4
  QLabel *keywordLabel = new QLabel(this);
  keywordLabel->setObjectName("keywordLabel");
  keywordLabel->setAlignment(defaultAlignment);
  keywordLabel->setText(tr("Keyword:"));

  lay4->addWidget(keywordLabel, glc, 0, 1, 1);

  ib_keyword = new StrLineEdit(this);
  ib_keyword->setObjectName("ib_keyword");
  ib_keyword->setMaxAllowedLength(60);
  ib_keyword->setToolTip(tr("Category Keywords for Shopsystems."));
  ib_keyword->setWindowTitle(tr("Chop Keyword"));

  lay4->addWidget(ib_keyword, glc++, 1, 1, 1);

  // Zeile 5
  QLabel *conditionLabel = new QLabel(this);
  conditionLabel->setObjectName("conditionLabel");
  conditionLabel->setAlignment(defaultAlignment);
  conditionLabel->setText(tr("Condition:"));

  lay4->addWidget(conditionLabel, glc, 0, 1, 1);

  ib_condition = new StrLineEdit(this);
  ib_condition->setObjectName("ib_condition");
  ib_condition->setMaxAllowedLength(128);
  ib_condition->setRequired(true);
  ib_condition->setWindowTitle(tr("Condition"));
  ib_condition->setToolTip(
      tr("Condition of this Book. See also Configuration conditions Table."));

  lay4->addWidget(ib_condition, glc++, 1, 1, 1);

  // Zeile 6
  QLabel *designationLabel = new QLabel(this);
  designationLabel->setObjectName("designationLabel");
  designationLabel->setAlignment(defaultAlignment);
  designationLabel->setText(tr("Designation:"));

  lay4->addWidget(designationLabel, glc, 0, 1, 1);

  ib_designation = new StrLineEdit(this);
  ib_designation->setObjectName("ib_designation");
  ib_designation->setMaxAllowedLength(128);
  ib_designation->setRequired(true);
  ib_designation->setWindowTitle(tr("Designation"));

  lay4->addWidget(ib_designation, glc++, 1, 1, 1);

  // Zeile 7
  QLabel *languageLabel = new QLabel(this);
  languageLabel->setObjectName("languageLabel");
  languageLabel->setAlignment(defaultAlignment);
  languageLabel->setText(tr("Language:"));

  lay4->addWidget(languageLabel, glc, 0, 1, 1);

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

  lay5->addStretch(1);

  m_imageToolBar = new ImageToolBar(this);
  m_imageToolBar->setObjectName("books_image_actions_bar");
  lay5->addWidget(m_imageToolBar);

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

  // TODO
  // setTabOrder(ib_id, ib_count);
  // setTabOrder(ib_count, ib_price);

  connect(ib_isbn, SIGNAL(s_isbnIsValid(bool)), m_btnQueryISBN,
          SLOT(setEnabled(bool)));
  connect(m_btnQueryISBN, SIGNAL(clicked()), this, SLOT(triggerIsbnQuery()));

  connect(m_imageToolBar, SIGNAL(s_openImage()), this, SLOT(openImageDialog()));
  // connect(m_imageToolBar, SIGNAL(s_deleteImage(int)), this, SLOT(__TODO__(int)));

  connect(m_listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem *)), this,
          SLOT(infoISBNDoubleClicked(QListWidgetItem *)));
  connect(m_actionBar, SIGNAL(s_cancelClicked()), this,
          SLOT(finalLeaveEditor()));
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
    msgBox.failed(sqlStatement, errorString);
    return false;
  } else {
    msgBox.success(tr("Bookdata saved successfully!"), 1);
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
      messanger.notice(cur->notes());
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
      messanger.notice(cur->notes());
      cur->setFocus();
      data.clear();
      return data;
    }
    if (cur->value().toInt() == 0)
      continue;

    data.insert(cur->objectName(), cur->value());
  }
  listInt.clear();
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
    messanger.notice(ib_year->notes());
    ib_year->setFocus();
    data.clear();
    return data;
  } else {
    data.insert("ib_year", ib_year->value());
  }
  if (ib_storage->isValid()) {
    data.insert("ib_storage", ib_storage->value());
  } else {
    messanger.notice(ib_storage->notes());
    ib_storage->setFocus();
    data.clear();
    return data;
  }
  if (ib_price->isValid()) {
    data.insert("ib_price", ib_price->value());
  } else {
    messanger.notice(ib_price->notes());
    ib_price->setFocus();
    data.clear();
    return data;
  }
  return data;
}

void BookEditor::createSqlUpdate() {
  // TODO Config Flags
  ib_edition->setRequired(false);

  if (!ib_id->isValid()) {
    emit s_sendMessage(tr("Missing Article ID for Update."));
    return;
  }

  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 1)
    return;

  QStringList set;
  QHash<QString, QVariant>::iterator it;
  for (it = data.begin(); it != data.end(); ++it) {
    if (it.value().type() == QVariant::String) {
      set.append(it.key() + "='" + it.value().toString() + "'");
    } else {
      set.append(it.key() + "=" + it.value().toString());
    }
  }

  QString sql("UPDATE inventory_books SET ");
  sql.append(set.join(","));
  sql.append(",ib_changed=CURRENT_TIMESTAMP WHERE ib_id=");
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
    DataEntries f = sqlQueryResult.at(i);
    setSqlQueryData(f.field, f.data);
  }
  blockSignals(false);

  // Suche Bilddaten
  int id = ib_id->value().toInt();
  if (id > 0) {
    m_imageToolBar->setArticleId(id);
    m_imageToolBar->setActive(true);
    m_imageView->searchImageById(id);
  }
  // Nach Ersteintrag zurück setzen!
  resetModified();
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
  finalLeaveEditor();
}

void BookEditor::finalLeaveEditor() {
  m_listWidget->clear();              /**< OpenLibrary.org Anzeige leeren */
  sqlQueryResult.clear();             /**< SQL History leeren */
  clearDataFields();                  /**< Alle Datenfelder leeren */
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  m_imageToolBar->setActive(false);   /**< Bilder Aktionsleiste zurücksetzen */
  emit s_leaveEditor();               /**< Zurück */
}

void BookEditor::restoreDataset() {
  if (sqlQueryResult.isEmpty())
    return;

  importSqlResult();
}

void BookEditor::setSqlQueryData(const QString &key, const QVariant &value) {
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
  if (ib_id->value().toString().isEmpty()) {
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
    ib_storage->loadDataset();
    ib_condition->loadDataset("condition");
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

void BookEditor::editBookEntry(const QString &condition) {
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
        DataEntries d;
        d.field = key;
        d.vtype = val.type();
        d.data = val;
        sqlQueryResult.append(d);
      }
    }
  } else {
    MessageBox messanger(this);
    messanger.failed(db->fetchErrors(), condition);
    return;
  }

  if (!sqlQueryResult.isEmpty() && !m_actionBar->isRestoreable())
    m_actionBar->setRestoreable(true);

  importSqlResult();
}

void BookEditor::createBookEntry() {
  setEnabled(true);
  m_imageToolBar->setActive(false);
  resetModified();
}
