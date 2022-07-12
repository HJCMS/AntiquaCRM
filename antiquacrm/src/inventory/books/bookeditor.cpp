// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookeditor.h"
#include "bookcard.h"
#include "isbnrequest.h"
#include "isbnresults.h"
#include "myicontheme.h"
#include <AntiquaCRM>

#include <QDebug>
#include <QDesktopServices>
#include <QtWidgets>

// Schalte SQL ausgaben ein
#ifndef SHOW_SQL_QUERIES
#define SHOW_SQL_QUERIES false
#endif

BookEditor::BookEditor(QWidget *parent) : EditorMain{parent} {
  setObjectName("BookEditor");
  setWindowTitle(tr("Edit Book Title"));
  setMinimumSize(800, 750);

  ApplSettings config;

  Qt::Alignment defaultAlignment = (Qt::AlignRight | Qt::AlignVCenter);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("bookeditor_main_layout");
  mainLayout->setSizeConstraint(QLayout::SetMaximumSize);

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

  double minPrice = config.value("payment/min_price", 8.00).toDouble();
  ib_price = new PriceEdit(this);
  ib_price->setObjectName("ib_price");
  ib_price->setRequired(true);
  ib_price->setInfo(tr("Price"));
  ib_price->setMinimum(minPrice);
  row0->addWidget(ib_price);

  ib_including_vat = new BoolBox(this);
  ib_including_vat->setObjectName("ib_including_vat");
  ib_including_vat->setInfo(tr("incl. vat"));
  ib_including_vat->setRequired(false);
  ib_including_vat->setToolTip(tr("VAT Settings must be set."));
  row0->addWidget(ib_including_vat);

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

  ib_since = new DateTimeDisplay(this);
  ib_since->setObjectName("ib_since");
  ib_since->setRequired(false);
  ib_since->setInfo(tr("book entry created at"));
  row1->addWidget(ib_since);

  row1->addStretch(1);
  mainLayout->addLayout(row1);

  QWidget *row2Widget = new QWidget(this);
  row2Widget->setMinimumHeight(380);
  int row2c = 0;
  QGridLayout *row2 = new QGridLayout(row2Widget);
  row2->setContentsMargins(2, 2, 2, 2);

  QLabel *titleLabel = new QLabel(this);
  titleLabel->setObjectName("titleLabel");
  titleLabel->setAlignment(defaultAlignment);
  titleLabel->setText(tr("Booktitle") + ":");
  row2->addWidget(titleLabel, row2c, 0, 1, 1);

  ib_title = new StrLineEdit(this);
  ib_title->setObjectName("ib_title");
  ib_title->setMaxAllowedLength(80);
  ib_title->setRequired(true);
  ib_title->setInfo(tr("Booktitle"));
  ib_title->setToolTip(tr("Required input field. Limited to 80 characters, "
                          "Webshop Systems require this."));
  titleLabel->setBuddy(ib_title);
  row2->addWidget(ib_title, row2c++, 1, 1, 1);

  QLabel *extendedLabel = new QLabel(this);
  extendedLabel->setObjectName("extendedLabel");
  extendedLabel->setAlignment(defaultAlignment);
  extendedLabel->setText(tr("Book Subtitle") + ":");
  row2->addWidget(extendedLabel, row2c, 0, 1, 1);

  ib_title_extended = new StrLineEdit(this);
  ib_title_extended->setObjectName("ib_title_extended");
  ib_title_extended->setMaxAllowedLength(148);
  ib_title_extended->setToolTip(tr("Extended Title or Subtitle."));
  ib_title_extended->setInfo(tr("Book Subtitle"));
  row2->addWidget(ib_title_extended, row2c++, 1, 1, 1);

  QLabel *authorLabel = new QLabel(this);
  authorLabel->setObjectName("authorLabel");
  authorLabel->setAlignment(defaultAlignment);
  authorLabel->setText(tr("Author") + ":");
  row2->addWidget(authorLabel, row2c, 0, 1, 1);

  ib_author = new StrLineEdit(this);
  ib_author->setObjectName("ib_author");
  ib_author->setMaxAllowedLength(128);
  ib_author->setRequired(true);
  ib_author->setInfo(tr("Author"));
  ib_author->setToolTip(
      tr("Format: Firstname lastname (Different Authors separated by comma)."));
  authorLabel->setBuddy(ib_author);
  row2->addWidget(ib_author, row2c++, 1, 1, 1);

  QLabel *publisherLabel = new QLabel(this);
  publisherLabel->setObjectName("publisherLabel");
  publisherLabel->setAlignment(defaultAlignment);
  publisherLabel->setText(tr("Publisher") + ":");
  publisherLabel->setToolTip(tr("Enter hier the Book Publisher."));
  row2->addWidget(publisherLabel, row2c, 0, 1, 1);

  ib_publisher = new StrLineEdit(this);
  ib_publisher->setObjectName("ib_publisher");
  ib_publisher->setMaxAllowedLength(128);
  ib_publisher->setInfo(tr("Publisher"));
  ib_publisher->setToolTip(tr("Publisher or Illustrator"));
  row2->addWidget(ib_publisher, row2c++, 1, 1, 1);

  QLabel *conditionLabel = new QLabel(this);
  conditionLabel->setObjectName("conditionLabel");
  conditionLabel->setAlignment(defaultAlignment);
  conditionLabel->setText(tr("Condition") + ":");
  row2->addWidget(conditionLabel, row2c, 0, 1, 1);

  ib_condition = new StrLineEdit(this);
  ib_condition->setObjectName("ib_condition");
  ib_condition->setMaxAllowedLength(128);
  ib_condition->setRequired(true);
  ib_condition->setInfo(tr("Condition"));
  ib_condition->setToolTip(
      tr("Condition of this Book. See also Configuration conditions Table."));
  row2->addWidget(ib_condition, row2c++, 1, 1, 1);

  QLabel *designationLabel = new QLabel(this);
  designationLabel->setObjectName("designationLabel");
  designationLabel->setAlignment(defaultAlignment);
  designationLabel->setText(tr("Designation") + ":");
  row2->addWidget(designationLabel, row2c, 0, 1, 1);

  ib_designation = new StrLineEdit(this);
  ib_designation->setObjectName("ib_designation");
  ib_designation->setMaxAllowedLength(128);
  ib_designation->setRequired(true);
  ib_designation->setInfo(tr("Designation"));
  row2->addWidget(ib_designation, row2c++, 1, 1, 1);

  QLabel *storageLabel = new QLabel(this);
  storageLabel->setObjectName("storageLabel");
  storageLabel->setAlignment(defaultAlignment);
  storageLabel->setText(tr("Storage") + ":");
  row2->addWidget(storageLabel, row2c, 0, 1, 1);

  ib_storage = new StorageEdit(this);
  ib_storage->setObjectName("ib_storage");
  ib_storage->setInfo(tr("Storage"));
  row2->addWidget(ib_storage, row2c++, 1, 1, 1);

  QLabel *keywordLabel = new QLabel(this);
  keywordLabel->setObjectName("keywordLabel");
  keywordLabel->setAlignment(defaultAlignment);
  keywordLabel->setText(tr("Keyword") + ":");
  row2->addWidget(keywordLabel, row2c, 0, 1, 1);

  ib_keyword = new StrLineEdit(this);
  ib_keyword->setObjectName("ib_keyword");
  ib_keyword->setMaxAllowedLength(60);
  ib_keyword->setInfo(tr("Keyword"));
  ib_keyword->setToolTip(tr("Category Keywords for Shopsystems."));
  row2->addWidget(ib_keyword, row2c++, 1, 1, 1);

  QLabel *subjectLabel = new QLabel(this);
  subjectLabel->setObjectName("subjectLabel");
  subjectLabel->setAlignment(defaultAlignment);
  subjectLabel->setText(tr("Provider Subject") + ":");
  row2->addWidget(subjectLabel, row2c, 0, 1, 1);

  ib_category_subject = new CategorySubject(this);
  ib_category_subject->setObjectName("ib_category_subject");
  ib_category_subject->setInfo(tr("Shop Category Keywords"));
  row2->addWidget(ib_category_subject, row2c++, 1, 1, 1);

  QLabel *languageLabel = new QLabel(this);
  languageLabel->setObjectName("languageLabel");
  languageLabel->setAlignment(defaultAlignment);
  languageLabel->setText(tr("Language:"));
  row2->addWidget(languageLabel, row2c, 0, 1, 1);

  ib_language = new SetLanguage(this);
  ib_language->setObjectName("ib_language");
  row2->addWidget(ib_language, row2c++, 1, 1, 1);

  QLabel *isbnLabel = new QLabel(this);
  isbnLabel->setText("ISBN:");
  isbnLabel->setAlignment(defaultAlignment);
  row2->addWidget(isbnLabel, row2c, 0, 1, 1);

  ib_isbn = new IsbnEdit(this);
  ib_isbn->setObjectName("ib_isbn");
  row2->addWidget(ib_isbn, row2c++, 1, 1, 1);

  m_imageToolBar = new ImageToolBar(this);
  m_imageToolBar->setObjectName("books_image_actions_bar");
  row2->addWidget(m_imageToolBar, row2c++, 0, 1, 2, Qt::AlignRight);

  QSize maximageSize = config.value("image/max_size", QSize(320, 320)).toSize();
  m_imageView = new ImageView(maximageSize, this);
  m_imageView->setMinimumHeight(maximageSize.height());
  m_imageView->setMaximumWidth(maximageSize.width());
  row2->addWidget(m_imageView, 0, 2, (row2c + 1), 1);

  row2Widget->setLayout(row2);
  mainLayout->addWidget(row2Widget);

  m_tabWidget = new QTabWidget(this);
  m_tabWidget->setObjectName("tab_widget");
  m_tabWidget->setMinimumHeight(180);
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

  m_isbnWidget = new ISBNResults(this); // ISBNResults
  m_isbnWidget->setObjectName("isbnqueryresult");
  m_tabWidget->insertTab(2, m_isbnWidget, "OpenLibrary.org");
  m_tabWidget->setTabIcon(2, myIcon("folder_txt"));

  mainLayout->addWidget(m_tabWidget);

  m_actionBar = new EditorActionBar(this);
  m_actionBar->viewPrintBookCardButton(true);
  mainLayout->addWidget(m_actionBar);

  setLayout(mainLayout);

  // SIGNALS
  QList<UtilsMain *> list =
      findChildren<UtilsMain *>(p_objPattern, Qt::FindChildrenRecursively);
  QList<UtilsMain *>::Iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    UtilsMain *e = *it;
    connect(e, SIGNAL(hasModified(bool)), this, SLOT(setWindowModified(bool)));
  }

  connect(ib_isbn, SIGNAL(clicked()), this, SLOT(createIsbnQuery()));

  connect(m_imageToolBar, SIGNAL(s_openImage()), this, SLOT(openImageDialog()));
  connect(m_imageToolBar, SIGNAL(s_deleteImage(int)), this,
          SLOT(removeImageDialog(int)));

  connect(m_isbnWidget, SIGNAL(requestFinished()), this, SLOT(viewIsbnTab()));

  connect(m_actionBar, SIGNAL(s_cancelClicked()), this,
          SLOT(finalLeaveEditor()));
  connect(m_actionBar, SIGNAL(s_restoreClicked()), this,
          SLOT(restoreDataset()));
  connect(m_actionBar, SIGNAL(s_saveClicked()), this, SLOT(saveData()));
  connect(m_actionBar, SIGNAL(s_finishClicked()), this,
          SLOT(checkLeaveEditor()));
  connect(m_actionBar, SIGNAL(s_printBookCard()), this,
          SLOT(printingBookCard()));
}

void BookEditor::setInputList() {
  inputList = m_sql->fields("inventory_books");
  if (inputList.isEmpty()) {
    qWarning("Books InputList is Empty!");
  }
  // Wird Manuel gesetzt!
  inputList.removeOne("ib_changed");
}

void BookEditor::openImageDialog() {
  int id = ib_id->value().toLongLong();
  ImageDialog *dialog = new ImageDialog(id, this);
  if (id >= 1)
    emit s_openImageEditor(id);

  if (dialog->exec()) {
    m_imageView->readFromDatabase(id);
  }
}

void BookEditor::removeImageDialog(int id) {
  if (ib_id->value().toInt() != id)
    return;

  QString image_id = QString::number(id);
  QString t(tr("Remove Image from Database"));
  QString ask(tr("Do you realy wan't to delete the Image?"));
  QString m = QString("%1\n\nImage - Article ID: %2").arg(ask, image_id);
  QMessageBox::StandardButton set = QMessageBox::question(this, t, m);
  if (set == QMessageBox::Yes) {
    if (m_imageView->removeFromDatabase(id)) {
      m_imageView->clear();
      sqlSuccessMessage(tr("Image delete successfully!"));
    }
  }
}

bool BookEditor::sendSqlQuery(const QString &sqlStatement) {
  if (SHOW_SQL_QUERIES) {
    qDebug() << Q_FUNC_INFO << sqlStatement;
  }
  QSqlQuery q = m_sql->query(sqlStatement);
  if (q.lastError().type() != QSqlError::NoError) {
    sqlErrnoMessage(sqlStatement, m_sql->fetchErrors());
    return false;
  } else {
    if (q.next()) {
      if (!q.isNull("ib_id")) {
        ib_id->setValue(q.value("ib_id"));
      }
    }
    sqlSuccessMessage(tr("Bookdata saved successfully!"));
    resetModified(inputList);
    return true;
  }
}

const QHash<QString, QVariant> BookEditor::createSqlDataset() {
  QHash<QString, QVariant> data;
  QList<UtilsMain *> list =
      findChildren<UtilsMain *>(p_objPattern, Qt::FindChildrenRecursively);
  QList<UtilsMain *>::Iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    UtilsMain *cur = *it;
    if (cur->isRequired() && !cur->isValid()) {
      sqlNoticeMessage(cur->notes());
      cur->setFocus();
      data.clear();
      return data;
    }
    // qDebug() << "Book:" << cur->objectName() << cur->value() << cur->notes();
    data.insert(cur->objectName(), cur->value());
  }
  list.clear();
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
    if (it.key() == "ib_id")
      continue;

    if (it.value().type() == QVariant::String) {
      set.append(it.key() + "='" + it.value().toString() + "'");
    } else {
      set.append(it.key() + "=" + it.value().toString());
    }
  }

  /** Auf Aktivierung prüfen */
  int articleId = ib_id->value().toInt();
  qint8 articleCount = ib_count->value().toInt();
  if (articleCount != 0 && (count_temp != articleCount)) {
    for (int i = 0; i < sqlQueryResult.size(); ++i) {
      DataField f = sqlQueryResult.at(i);
      /* Wenn sich die Anzahl geändert hat, ein Update senden! */
      if ((f.field() == "ib_count") && (f.value().toInt() != articleCount)) {
        count_temp = articleCount;
        emit s_articleCount(articleId, articleCount);
        break;
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
  if (ib_id->value().toInt() >= 1) {
    qWarning("No Book insert");
    createSqlUpdate();
    return;
  }

  /** Bei neu Einträgen immer erforderlich */
  ib_count->setRequired(true);
  ib_id->setRequired(false);

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
  sql.append(",CURRENT_TIMESTAMP) RETURNING ib_id;");
  if (sendSqlQuery(sql) && ib_id->value().toInt() >= 1) {
    m_imageToolBar->setActive(false);
  }
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
    m_imageView->readFromDatabase(id);
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
  count_temp = -1;                    /**< Zähler wieder zurücksetzen */
  m_isbnWidget->clear();              /**< OpenLibrary.org Anzeige leeren */
  sqlQueryResult.clear();             /**< SQL History leeren */
  clearDataFields(p_objPattern);      /**< Alle Datenfelder leeren */
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  m_imageView->clear();               /**< Imaging clear */
  m_imageToolBar->setActive(false);   /**< Bilder Aktionsleiste zurücksetzen */
  emit s_leaveEditor();               /**< Zurück zur Hauptsansicht */
}

void BookEditor::restoreDataset() {
  if (sqlQueryResult.isEmpty())
    return;

  importSqlResult();
}

void BookEditor::setData(const QString &key, const QVariant &value,
                         bool required) {
  if (key.isEmpty())
    return;

  UtilsMain *inp = findChild<UtilsMain *>(key, Qt::FindChildrenRecursively);
  if (inp != nullptr) {
    inp->setValue(value);
    if (required && !inp->isRequired())
      inp->setRequired(required);

    return;
  }
  qDebug() << "Missing:" << key << "|" << value << "|" << required;
}

bool BookEditor::realyDeactivateBookEntry() {
  int countNew = ib_count->value().toInt();
  int countOld = findResultValue("ib_count").toInt();
  if ((countNew == countOld) || (countNew == count_temp))
    return true; // alles ok

  QString body = tr("When setting the count of this book to 0. All existing "
                    "orders from this entry are also deactivated and shop "
                    "system entries are marked for deletion.");
  body.append("<br/><p>");
  body.append(tr("Are you sure to deactivate this entry?"));
  body.append("</p>");

  int ret = QMessageBox::question(this, tr("Book deactivation"), body);
  if (ret == QMessageBox::No) {
    // Zurücksetzen!
    for (int i = 0; i < sqlQueryResult.size(); ++i) {
      DataField f = sqlQueryResult.at(i);
      if (f.field() == "ib_count") {
        ib_count->setValue(f.value());
        break;
      }
    }
    return false;
  }

  /* Änderung senden */
  int articleId = ib_id->value().toInt();
  count_temp = countNew;
  emit s_articleCount(articleId, countNew);
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

void BookEditor::printingBookCard() {
  BookCard *dialog = new BookCard(this);
  dialog->setObjectName("book_card_printing");

  QHash<QString, QVariant> data;
  data.insert("id", ib_id->value());
  data.insert("title", ib_title->value());
  data.insert("author", ib_author->value());
  data.insert("year", ib_year->value());
  data.insert("storage", ib_storage->description());
  data.insert("since", ib_since->currentDate());
  data.insert("keywords", ib_keyword->value());

  if (dialog->exec(data) == QDialog::Accepted) {
    /* Im Moment nicht zugewiesen */
  }
}

void BookEditor::changeEvent(QEvent *event) {
  if (event->type() == QEvent::EnabledChange && isEnabled()) {
    /**
     * Bei einem erneuten Aufruf wieder zurück setzen!
     */
    count_temp = -1;

    /**
     * @warning Die Suche muss bei einem Öffnen Leer sein!
     * Danach kann loadDatset ausgeführt werden!
     */
    ib_storage->reset();
    ib_storage->loadDataset();

    /**
     * Dienstleister daten laden
     */
    ib_category_subject->loadDataset();

    /**
     * Lade Herausgeber XML
     */
    ib_publisher->loadXmlCompleter("ib_publisher");

    /**
     * Lese aus SQL Datenbank Autovervollständigungen
     */
    ib_condition->loadDataset("condition", StrLineEdit::BOOK);
    ib_designation->loadDataset("designation", StrLineEdit::BOOK);
    ib_keyword->loadStorageKeywords();
  }
}

void BookEditor::createIsbnQuery() {
  m_isbnWidget->fetchIsbnData(ib_isbn->value());
}

void BookEditor::viewIsbnTab() {
  int index = m_tabWidget->indexOf(m_isbnWidget);
  if (index >= 0)
    m_tabWidget->setCurrentIndex(index);
}

void BookEditor::editBookEntry(const QString &condition) {
  setInputList();
  if (condition.length() < 5)
    return;

  QString select("SELECT * FROM inventory_books WHERE ");
  select.append(condition);
  select.append(" ORDER BY ib_id LIMIT 1;");
  // qDebug() << Q_FUNC_INFO << "SELECT ->" << select;
  QSqlQuery q = m_sql->query(select);
  if (q.size() != 0) {
    QSqlRecord r = m_sql->record("inventory_books");
    sqlQueryResult.clear();
    while (q.next()) {
      foreach (QString key, inputList) {
        QVariant val = q.value(r.indexOf(key));
        bool required = (r.field(key).requiredStatus() == QSqlField::Required);
        // qDebug() << Q_FUNC_INFO << key << val << required;
        DataField d;
        d.setField(key);
        d.setType(val.type());
        d.setRequired(required);
        d.setValue(val);
        sqlQueryResult.append(d);
      }
    }
  } else {
    sqlErrnoMessage(m_sql->fetchErrors(), condition);
    return;
  }

  if (!sqlQueryResult.isEmpty() && !m_actionBar->isRestoreable())
    m_actionBar->setRestoreable(true);

  importSqlResult();
}

void BookEditor::createBookEntry() {
  setInputList();
  setEnabled(true);
  m_imageToolBar->setActive(false);
  resetModified(inputList);
}
