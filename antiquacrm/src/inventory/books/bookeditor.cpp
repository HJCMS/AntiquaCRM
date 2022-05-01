#include "bookeditor.h"
#include "antiqua_global.h"
#include "applsettings.h"
#include "isbnrequest.h"
#include "myicontheme.h"

#include <QtCore/QDebug>
#include <QtGui/QDesktopServices>
#include <QtWidgets>

// Schalte SQL ausgaben ein
#ifndef SHOW_SQL_QUERIES
#define SHOW_SQL_QUERIES false
#endif

BookEditor::BookEditor(QWidget *parent) : EditorMain{parent} {
  setObjectName("BookEditor");
  setWindowTitle(tr("Edit Book Title"));

  ApplSettings config;

  Qt::Alignment defaultAlignment = (Qt::AlignRight | Qt::AlignVCenter);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("mainLayout");

  QHBoxLayout *row0 = new QHBoxLayout();
  ib_id = new SerialID(this);
  ib_id->setObjectName("ib_id");
  ib_id->setInfo(tr("Article ID"));
  ib_id->setRequired(true);
  row0->addWidget(ib_id);

  ib_count = new IntSpinBox(this);
  ib_count->setObjectName("ib_count");
  ib_count->setInfo(tr("Count"));
  row0->addWidget(ib_count);

  double minPrice = config.value("books/min_price", 8.00).toDouble();
  ib_price = new PriceEdit(this);
  ib_price->setObjectName("ib_price");
  ib_price->setRequired(true);
  ib_price->setInfo(tr("Price"));
  ib_price->setMinimum(minPrice);
  row0->addWidget(ib_price);

  ib_signed = new BoolBox(this);
  ib_signed->setObjectName("ib_signed");
  ib_signed->setInfo(tr("Signed Version"));
  row0->addWidget(ib_signed);

  ib_restricted = new BoolBox(this);
  ib_restricted->setObjectName("ib_restricted");
  ib_restricted->setInfo(tr("Restricted Sale"));
  ib_restricted->setToolTip(
      tr("Is the title not for sale nationally or is it on a censorship list. "
         "This is relevant for the Shopsystem."));

  row0->addWidget(ib_restricted);
  row0->addStretch(1);
  mainLayout->addLayout(row0, 0);

  QHBoxLayout *row1 = new QHBoxLayout();

  ib_edition = new IntSpinBox(this);
  ib_edition->setObjectName("ib_edition");
  ib_edition->setRequired(true);
  ib_edition->setInfo(tr("Edition"));
  row1->addWidget(ib_edition);

  ib_pagecount = new IntSpinBox(this);
  ib_pagecount->setObjectName("ib_pagecount");
  ib_pagecount->setContextMenuPolicy(Qt::DefaultContextMenu);
  ib_pagecount->setRange(10, 100000);
  ib_pagecount->setSingleStep(10);
  ib_pagecount->setInfo(tr("Page count"));
  ib_pagecount->setRequired(true);
  row1->addWidget(ib_pagecount);

  ib_weight = new IntSpinBox(this);
  ib_weight->setObjectName("ib_weight");
  ib_weight->setRange(100, 100000000);
  ib_weight->setSuffix(tr("g"));
  ib_weight->setInfo(tr("Weight"));
  ib_weight->setRequired(true);
  row1->addWidget(ib_weight);

  ib_year = new YearEdit(this);
  ib_year->setObjectName("ib_year");
  ib_year->setRequired(true);
  ib_year->setInfo(tr("Year"));
  row1->addWidget(ib_year);

  ib_volume = new IntSpinBox(this);
  ib_volume->setObjectName("ib_volume");
  ib_volume->setPrefix(tr("Bd."));
  ib_volume->setInfo(tr("Volume"));
  row1->addWidget(ib_volume);
  row1->addStretch(1);
  mainLayout->addLayout(row1);

  QGridLayout *row2 = new QGridLayout();
  row2->setContentsMargins(2, 2, 2, 2);

  ib_storage = new StorageEdit(this);
  ib_storage->setObjectName("ib_storage");
  ib_storage->setInfo(tr("Storage"));
  row2->addWidget(ib_storage, 0, 0, 1, 2, Qt::AlignRight);

  QLabel *titleLabel = new QLabel(this);
  titleLabel->setObjectName("titleLabel");
  titleLabel->setAlignment(defaultAlignment);
  titleLabel->setText(tr("Book &Title:"));
  row2->addWidget(titleLabel, 1, 0, 1, 1);

  ib_title = new StrLineEdit(this);
  ib_title->setObjectName("ib_title");
  ib_title->setMaxAllowedLength(80);
  ib_title->setRequired(true);
  ib_title->setWindowTitle(tr("Title"));
  ib_title->setToolTip(tr("Required input field. Limited to 80 characters, "
                          "Webshop Systems require this."));
  titleLabel->setBuddy(ib_title);
  row2->addWidget(ib_title, 1, 1, 1, 1);

  QLabel *extendedLabel = new QLabel(this);
  extendedLabel->setObjectName("extendedLabel");
  extendedLabel->setAlignment(defaultAlignment);
  extendedLabel->setText(tr("Booktitle Extended:"));
  row2->addWidget(extendedLabel, 2, 0, 1, 1);

  ib_title_extended = new StrLineEdit(this);
  ib_title_extended->setObjectName("ib_title_extended");
  ib_title_extended->setMaxAllowedLength(148);
  ib_title_extended->setToolTip(tr("Extended Title or Subtitle."));
  ib_title_extended->setWindowTitle(tr("Extended Title"));
  row2->addWidget(ib_title_extended, 2, 1, 1, 1);

  QLabel *authorLabel = new QLabel(this);
  authorLabel->setObjectName("authorLabel");
  authorLabel->setAlignment(defaultAlignment);
  authorLabel->setText(tr("&Author:"));
  row2->addWidget(authorLabel, 3, 0, 1, 1);

  ib_author = new StrLineEdit(this);
  ib_author->setObjectName("ib_author");
  ib_author->setMaxAllowedLength(128);
  ib_author->setRequired(true);
  ib_author->setWindowTitle(tr("Author"));
  ib_author->setToolTip(
      tr("Format: Firstname lastname (Different Authors separated by comma)."));
  authorLabel->setBuddy(ib_author);
  row2->addWidget(ib_author, 3, 1, 1, 1);

  QLabel *publisherLabel = new QLabel(this);
  publisherLabel->setObjectName("publisherLabel");
  publisherLabel->setAlignment(defaultAlignment);
  publisherLabel->setText(tr("Publisher:"));
  publisherLabel->setToolTip(tr("Enter hier the Book Publisher."));
  row2->addWidget(publisherLabel, 4, 0, 1, 1);

  ib_publisher = new StrLineEdit(this);
  ib_publisher->setObjectName("ib_publisher");
  ib_publisher->setMaxAllowedLength(128);
  ib_publisher->setWindowTitle(tr("Publisher"));
  row2->addWidget(ib_publisher, 4, 1, 1, 1);

  QLabel *keywordLabel = new QLabel(this);
  keywordLabel->setObjectName("keywordLabel");
  keywordLabel->setAlignment(defaultAlignment);
  keywordLabel->setText(tr("Keyword:"));
  row2->addWidget(keywordLabel, 5, 0, 1, 1);

  ib_keyword = new StrLineEdit(this);
  ib_keyword->setObjectName("ib_keyword");
  ib_keyword->setMaxAllowedLength(60);
  ib_keyword->setToolTip(tr("Category Keywords for Shopsystems."));
  ib_keyword->setWindowTitle(tr("Chop Keyword"));
  row2->addWidget(ib_keyword, 5, 1, 1, 1);

  QLabel *conditionLabel = new QLabel(this);
  conditionLabel->setObjectName("conditionLabel");
  conditionLabel->setAlignment(defaultAlignment);
  conditionLabel->setText(tr("Condition:"));
  row2->addWidget(conditionLabel, 6, 0, 1, 1);

  ib_condition = new StrLineEdit(this);
  ib_condition->setObjectName("ib_condition");
  ib_condition->setMaxAllowedLength(128);
  ib_condition->setRequired(true);
  ib_condition->setWindowTitle(tr("Condition"));
  ib_condition->setToolTip(
      tr("Condition of this Book. See also Configuration conditions Table."));
  row2->addWidget(ib_condition, 6, 1, 1, 1);

  QLabel *designationLabel = new QLabel(this);
  designationLabel->setObjectName("designationLabel");
  designationLabel->setAlignment(defaultAlignment);
  designationLabel->setText(tr("Designation:"));
  row2->addWidget(designationLabel, 7, 0, 1, 1);

  ib_designation = new StrLineEdit(this);
  ib_designation->setObjectName("ib_designation");
  ib_designation->setMaxAllowedLength(128);
  ib_designation->setRequired(true);
  ib_designation->setWindowTitle(tr("Designation"));
  row2->addWidget(ib_designation, 7, 1, 1, 1);

  QLabel *languageLabel = new QLabel(this);
  languageLabel->setObjectName("languageLabel");
  languageLabel->setAlignment(defaultAlignment);
  languageLabel->setText(tr("Language:"));
  row2->addWidget(languageLabel, 8, 0, 1, 1);

  ib_language = new SetLanguage(this);
  ib_language->setObjectName("ib_language");
  row2->addWidget(ib_language, 8, 1, 1, 1);

  QLabel *isbnLabel = new QLabel(this);
  isbnLabel->setText("ISBN:");
  isbnLabel->setAlignment(defaultAlignment);
  row2->addWidget(isbnLabel, 9, 0, 1, 1);

  ib_isbn = new IsbnEdit(this);
  ib_isbn->setObjectName("ib_isbn");
  row2->addWidget(ib_isbn, 9, 1, 1, 1);

  m_imageToolBar = new ImageToolBar(this);
  m_imageToolBar->setObjectName("books_image_actions_bar");
  row2->addWidget(m_imageToolBar, 10, 0, 1, 2, Qt::AlignRight);

  m_imageView = new ImageWidget(this);
  row2->addWidget(m_imageView, 0, 2, 11, 1);

  mainLayout->addLayout(row2);

  m_tabWidget = new QTabWidget(this);
  m_tabWidget->setObjectName("tab_widget");
  m_tabWidget->setMinimumHeight(100);
  m_tabWidget->setContentsMargins(1, 1, 1, 1);

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

  isbnTabIndex = 2; /**< @note Wird von setIsbnInfo benötigt */
  m_listWidget = new QListWidget(this);
  m_listWidget->setObjectName("isbnqueryresult");
  m_tabWidget->insertTab(isbnTabIndex, m_listWidget, "OpenLibrary.org");
  m_tabWidget->setTabIcon(isbnTabIndex, myIcon("folder_txt"));

  mainLayout->addWidget(m_tabWidget);

  m_actionBar = new EditorActionBar(this);
  mainLayout->addWidget(m_actionBar);

  setLayout(mainLayout);

  connect(ib_isbn, SIGNAL(clicked()), this, SLOT(triggerIsbnQuery()));

  connect(m_imageToolBar, SIGNAL(s_openImage()), this, SLOT(openImageDialog()));
  connect(m_imageToolBar, SIGNAL(s_deleteImage(int)), this,
          SLOT(removeImageDialog(int)));

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

void BookEditor::removeImageDialog(int id) {
  if (ib_id->value().toInt() != id)
    return;

  QString image_id = QString::number(id);
  QString t(tr("Remove Image from Database"));
  QString ask(tr("Do you realy wan't to delete the Image fom Database?"));
  QString m = QString("%1\n\nImage - Article ID: %2").arg(ask, image_id);
  QMessageBox::StandardButton set = QMessageBox::question(this, t, m);
  if (set == QMessageBox::Yes) {
    QSqlQuery q = m_sql->query(
        "DELETE FROM inventory_images WHERE im_id=" + image_id + ";");
    if (q.lastError().isValid()) {
      qWarning("Delete Image SQL-Error:\n%s", qPrintable(m_sql->lastError()));
    } else {
      m_imageView->clear();
    }
  }
}

bool BookEditor::sendSqlQuery(const QString &sqlStatement) {
  if (SHOW_SQL_QUERIES) {
    qDebug() << Q_FUNC_INFO << sqlStatement;
  }
  MessageBox msgBox(this);
  QSqlQuery q = m_sql->query(sqlStatement);
  if (q.lastError().type() != QSqlError::NoError) {
    QString errorString = m_sql->fetchErrors();
    qDebug() << errorString << Qt::endl;
    msgBox.failed(sqlStatement, errorString);
    return false;
  } else {
    msgBox.success(tr("Bookdata saved successfully!"), 1);
    resetModified(inputList);
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
    data.insert(cur->objectName(), cur->value());
  }
  listInt.clear();
  // Fleder welche geprüft werden müssen
  if (ib_isbn->isValid()) {
    data.insert("ib_isbn", ib_isbn->value());
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
  // Felder die immer gesetzt werden
  data.insert("ib_description", ib_description->value());
  data.insert("ib_internal_description", ib_internal_description->value());
  data.insert("ib_language", ib_language->value());
  data.insert("ib_signed", ib_signed->value());
  data.insert("ib_restricted", ib_restricted->value());
  return data;
}

void BookEditor::createSqlUpdate() {
  // TODO Config Flags
  ib_edition->setRequired(false);

  if (!ib_id->isValid()) {
    emit s_postMessage(tr("Missing Article ID for Update."));
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

  /** Auf Aktivierung prüfen */
  if (ib_count->value().toInt() != 0) {
    for (int i = 0; i < sqlQueryResult.size(); ++i) {
      DataField f = sqlQueryResult.at(i);
      if (f.field() == "ib_count") {
        if (f.value().toInt() == 0) {
          /** Aktivierung */
          emit s_articleActivation(true);
          break;
        }
      }
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
    DataField f = sqlQueryResult.at(i);
    setData(f.field(), f.value(), f.isRequired());
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
  resetModified(inputList);
}

void BookEditor::checkLeaveEditor() {
  if (checkIsModified(p_objPattern)) {
    emit s_postMessage(
        tr("Unsaved Changes, don't leave this page before saved."));
    return;
  }
  finalLeaveEditor();
}

void BookEditor::finalLeaveEditor() {
  m_listWidget->clear();              /**< OpenLibrary.org Anzeige leeren */
  sqlQueryResult.clear();             /**< SQL History leeren */
  clearDataFields(p_objPattern);      /**< Alle Datenfelder leeren */
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  m_imageView->clear();               /**< Imaging clear */
  m_imageToolBar->setActive(false);   /**< Bilder Aktionsleiste zurücksetzen */
  emit s_leaveEditor();               /**< Zurück */
}

void BookEditor::restoreDataset() {
  if (sqlQueryResult.isEmpty())
    return;

  importSqlResult();
}

void BookEditor::setData(const QString &key, const QVariant &value,
                         bool required) {
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

bool BookEditor::realyDeactivateBookEntry() {
  QString body = tr("When setting the count of this book to 0. All existing "
                    "orders from this entry are also deactivated and shop "
                    "system entries are marked for deletion.");
  body.append("<br/><p>");
  body.append(tr("Are you sure to deactivate this entry?"));
  body.append("</p>");

  int ret = QMessageBox::question(this, tr("Book deactivation"), body);
  if (ret == QMessageBox::No) {
    for (int i = 0; i < sqlQueryResult.size(); ++i) {
      DataField f = sqlQueryResult.at(i);
      if (f.field() == "ib_count") {
        ib_count->setValue(f.value());
        break;
      }
    }
    return false;
  }
  emit s_articleActivation(false); /**< Deaktivierung */
  return true;
}

void BookEditor::saveData() {
  if (ib_id->value().toString().isEmpty()) {
    createSqlInsert();
  } else if (ib_count->value().toInt() == 0) {
    if (realyDeactivateBookEntry())
      createSqlUpdate();
  } else {
    createSqlUpdate();
  }
}

void BookEditor::setIsbnInfo(bool b) {
  m_listWidget->clear();

  Qt::ItemFlags flags(Qt::ItemIsEnabled | Qt::ItemIsSelectable |
                      Qt::ItemNeverHasChildren);

  if (!b) {
    QString ref_isbn = ib_isbn->value().toString();

    QString bookfinder_url("https://www.bookfinder.com/?mode=isbn&isbn=");
    bookfinder_url.append(ref_isbn);
    QListWidgetItem *bookfinder = new QListWidgetItem(m_listWidget);
    bookfinder->setData(Qt::DisplayRole,
                        tr("No Result: Search with %1").arg("Bookfinder.com"));
    bookfinder->setIcon(myIcon("html"));
    bookfinder->setFlags(flags);
    bookfinder->setData(Qt::UserRole, "ib_website:" + bookfinder_url);
    m_listWidget->addItem(bookfinder);

    QString gsearch;
    gsearch.append("https://books.google.com/advanced_book_search?lr=lang_de");
    gsearch.append("&hl=de&isbn=" + ref_isbn);

    QListWidgetItem *google = new QListWidgetItem(m_listWidget);
    google->setData(Qt::DisplayRole,
                    tr("No Result: Search with %1").arg("Google"));
    google->setIcon(myIcon("html"));
    google->setFlags(flags);
    google->setData(Qt::UserRole, "ib_website:" + gsearch);
    m_listWidget->addItem(google);

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
  if (event->type() == QEvent::EnabledChange && isEnabled()) {
    ib_storage->loadDataset();
    ib_keyword->loadStorageKeywords();
    ib_condition->loadDataset("condition");
    ib_designation->loadDataset("ib_designation");
  }
}

void BookEditor::triggerIsbnQuery() {
  QString isbn = ib_isbn->value().toString().trimmed();
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
  QSqlQuery q = m_sql->query(select);
  if (q.size() != 0) {
    QSqlRecord r = m_sql->record("inventory_books");
    sqlQueryResult.clear();
    while (q.next()) {
      foreach (QString key, inputList) {
        QVariant val = q.value(r.indexOf(key));
        bool required = (r.field(key).requiredStatus() == QSqlField::Required);
        DataField d;
        d.setField(key);
        d.setType(val.type());
        d.setRequired(required);
        d.setValue(val);
        sqlQueryResult.append(d);
      }
    }
  } else {
    MessageBox messanger(this);
    messanger.failed(m_sql->fetchErrors(), condition);
    return;
  }

  if (!sqlQueryResult.isEmpty() && !m_actionBar->isRestoreable())
    m_actionBar->setRestoreable(true);

  importSqlResult();
}

void BookEditor::createBookEntry() {
  setEnabled(true);
  m_imageToolBar->setActive(false);
  resetModified(inputList);
}
