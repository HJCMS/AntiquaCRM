// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookseditor.h"
#include "booksconfig.h"

#include <AntiquaCRM>
#include <QDate>
#include <QLayout>

BooksEditor::BooksEditor(QWidget *parent)
    : AntiquaCRM::TabsEditor{BOOKS_SQL_EDITOR_PATTERN, parent} {
  setWindowTitle(tr("Edit Book"));
  setObjectName("tab_books_editor");

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("bookeditor_main_layout");
  mainLayout->setSizeConstraint(QLayout::SetMaximumSize);

  QString tempWhatsThis;
  AntiquaCRM::ALabel::Align _lbAlign = AntiquaCRM::ALabel::Align::MiddleRight;

  // Row 0
  QHBoxLayout *row0 = new QHBoxLayout();
  ib_id = new AntiquaCRM::SerialId(this);
  ib_id->setObjectName("ib_id");
  ib_id->setBuddyLabel(tr("Article ID"));
  row0->addWidget(ib_id);

  ib_count = new AntiquaCRM::CrowdEdit(this);
  ib_count->setObjectName("ib_count");
  ib_count->setBuddyLabel(tr("Count"));
  row0->addWidget(ib_count);

  double minPrice = m_cfg->value("payment/min_price", 5.00).toDouble();
  ib_price = new AntiquaCRM::PriceEdit(this);
  ib_price->setObjectName("ib_price");
  ib_price->setBuddyLabel(tr("Price"));
  ib_price->setMinimum(minPrice);
  row0->addWidget(ib_price);

  ib_including_vat = new AntiquaCRM::BoolBox(this);
  ib_including_vat->setObjectName("ib_including_vat");
  ib_including_vat->setBuddyLabel(tr("incl. vat"));
  row0->addWidget(ib_including_vat);

  ib_signed = new AntiquaCRM::BoolBox(this);
  ib_signed->setObjectName("ib_signed");
  ib_signed->setBuddyLabel(tr("Signed Version"));
  row0->addWidget(ib_signed);

  ib_restricted = new AntiquaCRM::BoolBox(this);
  ib_restricted->setObjectName("ib_restricted");
  ib_restricted->setBuddyLabel(tr("Local Usage only"));
  ib_restricted->setToolTip(tr("When this Options is marked. Then this Article "
                               "will not exported to your Providers."));

  row0->addWidget(ib_restricted);
  row0->addStretch(1);
  mainLayout->addLayout(row0, 0);

  // Row 1
  QHBoxLayout *row1 = new QHBoxLayout();
  ib_edition = new AntiquaCRM::NumEdit(this);
  ib_edition->setObjectName("ib_edition");
  ib_edition->setBuddyLabel(tr("Edition"));
  row1->addWidget(ib_edition);

  ib_pagecount = new AntiquaCRM::NumEdit(this);
  ib_pagecount->setObjectName("ib_pagecount");
  ib_pagecount->setContextMenuPolicy(Qt::DefaultContextMenu);
  ib_pagecount->setRange(10, 100000);
  ib_pagecount->setSingleStep(10);
  ib_pagecount->setBuddyLabel(tr("Page count"));
  row1->addWidget(ib_pagecount);

  ib_weight = new AntiquaCRM::NumEdit(this);
  ib_weight->setObjectName("ib_weight");
  ib_weight->setRange(100, 100000000);
  ib_weight->setSuffix(tr("g"));
  ib_weight->setBuddyLabel(tr("Weight"));
  row1->addWidget(ib_weight);

  QDate _curDate = QDate::currentDate();
  ib_year = new AntiquaCRM::NumEdit(this);
  ib_year->setObjectName("ib_year");
  ib_year->setRange(868, _curDate.year());
  ib_year->setBuddyLabel(tr("Year"));
  row1->addWidget(ib_year);

  ib_volume = new AntiquaCRM::NumEdit(this);
  ib_volume->setObjectName("ib_volume");
  ib_volume->setPrefix(tr("Bd."));
  ib_volume->setBuddyLabel(tr("Volume"));
  row1->addWidget(ib_volume);
  row1->addStretch(1);
  mainLayout->addLayout(row1);

  // Row 2
  m_splitter = new AntiquaCRM::Splitter(this);
  QWidget *row2Widget = new QWidget(m_splitter);
  row2Widget->setContentsMargins(0, 0, 0, 0);
  int row2c = 0;
  QGridLayout *row2 = new QGridLayout(row2Widget);
  row2->setContentsMargins(0, 0, 0, 0);

  AntiquaCRM::ALabel *infoText;
  infoText = new AntiquaCRM::ALabel(tr("Booktitle"), _lbAlign, row2Widget);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ib_title = new AntiquaCRM::TextLine(this);
  ib_title->setObjectName("ib_title");
  ib_title->setInputToolTip(infoText->text());
  tempWhatsThis =
      tr("A Book title is restricted to 80 Characters. Some Online Providers "
         "only support this title length. Other Providers allowing more "
         "characters in Book titles. To get around this, the following "
         "Subtitle field will appended seperated by „-“ to the Book title when "
         "exporting the books data to your online shop providers.");
  ib_title->setWhatsThisText(tempWhatsThis);
  row2->addWidget(ib_title, row2c++, 1, 1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Book Subtitle"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ib_title_extended = new AntiquaCRM::TextLine(this);
  ib_title_extended->setObjectName("ib_title_extended");
  ib_title_extended->setInputToolTip(infoText->text());
  // WARNING - ib_title->setWhatsThisText is used!
  ib_title_extended->setWhatsThisText(tempWhatsThis);
  row2->addWidget(ib_title_extended, row2c++, 1, 1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Author"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ib_author = new AntiquaCRM::TextLine(this);
  ib_author->setObjectName("ib_author");
  ib_author->setInputToolTip(tr("Book Authors"));
  tempWhatsThis = tr("This field is reserved for Book Authors.\n"
                     "You can add more separated by comma.\n"
                     "e.g.: Karl Valentin, Franz Kafka\n"
                     "Some provider Platforms supporting spezial Keywords.\n"
                     "AntiquaCRM suggests the most common author group names, "
                     "when editing this field.");
  ib_author->setWhatsThisText(tempWhatsThis);
  row2->addWidget(ib_author, row2c++, 1, 1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Publisher"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ib_publisher = new AntiquaCRM::TextLine(this);
  ib_publisher->setObjectName("ib_publisher");
  ib_publisher->setInputToolTip(infoText->text());
  tempWhatsThis = tr("This field is reserved to insert Book Publishers.\n"
                     "Used Format is: „Publisher, Location“\n"
                     "Example: J.F. Lehmanns Verlag, Munich\n"
                     "AntiquaCRM suggests the most common publisher names, "
                     "when editing this field.");
  ib_publisher->setWhatsThisText(tempWhatsThis);
  row2->addWidget(ib_publisher, row2c++, 1, 1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Condition"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  QHBoxLayout *conditionlayout = new QHBoxLayout();
  ib_condition = new AntiquaCRM::ConditionEdit(this);
  ib_condition->setObjectName("ib_condition");
  tempWhatsThis =
      tr("The Book condition is predefined by your Book Seller online shop.\n"
         "Mostly defined with 1-4, very good to heavily worn.");
  ib_condition->setWhatsThisText(tempWhatsThis);
  ib_condition->appendStretch();
  conditionlayout->addWidget(ib_condition, Qt::AlignLeft);

  infoText = new AntiquaCRM::ALabel(tr("Language"), _lbAlign, this);
  conditionlayout->addWidget(infoText, Qt::AlignRight);
  ib_language = new AntiquaCRM::SelectLanguage(this);
  ib_language->setObjectName("ib_language");
  tempWhatsThis = tr("This field is reserved to the content language of the "
                     "book. If Multilingual content select Europe.");
  ib_language->setWhatsThisText(tempWhatsThis);
  conditionlayout->addWidget(ib_language);
  row2->addLayout(conditionlayout, row2c++, 1, 1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Designation"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  // Bucheinband und Bindungsart
  QHBoxLayout *binding_layout = new QHBoxLayout();
  binding_layout->setContentsMargins(0, 0, 0, 0);
  ib_binding = new AntiquaCRM::BookBindingEdit(this);
  ib_binding->setObjectName("ib_binding");
  ib_binding->setToolTip(tr("Bookbinding"));
  tempWhatsThis =
      tr("The Bookbinding selecter  contains the most prefinied Book bindings. "
         "Please select the best Category for this Book.");
  ib_binding->setWhatsThisText(tempWhatsThis);
  binding_layout->addWidget(ib_binding);

  ib_designation = new AntiquaCRM::TextLine(this);
  ib_designation->setObjectName("ib_designation");
  ib_designation->setToolTip(infoText->text());
  tempWhatsThis = tr(
      "This Field is reserved to add Book Designations. It is a addition for "
      "detailed Description of the Book cover.\n"
      "You can edit predefined Designations in your Database Configuration "
      "Menu.");
  ib_designation->setWhatsThisText(tempWhatsThis);
  binding_layout->addWidget(ib_designation);
  row2->addLayout(binding_layout, row2c++, 1, 1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Storage"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ib_storage = new AntiquaCRM::SelectStorage(this);
  ib_storage->setObjectName("ib_storage");
  ib_storage->setToolTip(infoText->text());
  tempWhatsThis =
      tr("Storage location where this Book has been stored.\n"
         "You can set Storage locations in your Database Configuration Menu.");
  ib_storage->setWhatsThisText(tempWhatsThis);
  row2->addWidget(ib_storage, row2c++, 1, 1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Keywords"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ib_keyword = new AntiquaCRM::KeywordsEdit(this);
  ib_keyword->setObjectName("ib_keyword");
  tempWhatsThis = tr(
      "Keywords will help Buyers, to find your Article on Provider pages. This "
      "Articles keyword management can also be compared to „Search Engine "
      "Optimization“ (SEO).\n"
      "Restrictions:\n"
      "- A Keyword must have a minimum length from 3 characters.\n"
      "- A Keywords list is restricted to a Maximum length of 60 Characters.\n"
      "- A keyword cannot contain spaces or non unicode special characters.\n"
      "You can edit predefined Keywords in your Database Configuration Menu.");
  ib_keyword->setWhatsThisText(tempWhatsThis);
  row2->addWidget(ib_keyword, row2c++, 1, 1, 1);
  row2->setRowStretch(row2c++, 1);

  // @BEGIN_GROUP
  infoText = new AntiquaCRM::ALabel("ISBN/EAN", _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  QHBoxLayout *tbLayout = new QHBoxLayout;
  ib_isbn = new AntiquaCRM::IsbnEdit(this);
  ib_isbn->setObjectName("ib_isbn");
  tbLayout->addWidget(ib_isbn);
  tbLayout->addStretch(1);
  m_imageToolBar = new AntiquaCRM::ImageToolBar(this);
  tbLayout->addWidget(m_imageToolBar);
  // @END_GROUP
  row2->addLayout(tbLayout, row2c++, 1, 1, 1);
  row2Widget->setLayout(row2);

  // Image Viewer
  QSize _max_size = m_cfg->value("image/max_size", QSize(320, 320)).toSize();
  m_thumbnail = new AntiquaCRM::ImageThumbnail(this);
  m_thumbnail->setMinimumWidth(100);
  m_thumbnail->setMaximumWidth(_max_size.width());
  m_splitter->addLeft(row2Widget);
  m_splitter->addRight(m_thumbnail);
  mainLayout->addWidget(m_splitter);
  mainLayout->setStretch(2, 1);

  // Row3
  m_tabWidget = new AntiquaCRM::TabWidget(this);
  QIcon tabIcons = m_tabWidget->defaultIcon();

  // Description
  ib_description = new AntiquaCRM::TextField(m_tabWidget);
  ib_description->setObjectName("ib_description");
  m_tabWidget->insertTab(0, ib_description, tabIcons, tr("Public Description"));
  // Internal Description
  ib_internal_description = new AntiquaCRM::TextField(m_tabWidget);
  ib_internal_description->setObjectName("ib_internal_description");
  m_tabWidget->insertTab(1, ib_internal_description, tabIcons,
                         tr("Internal Description"));
  // Info Tab
  QWidget *m_infos = new QWidget(this);
  // __TODO__

  m_tabWidget->insertTab(2, m_infos, tabIcons, tr("Information"));
  mainLayout->addWidget(m_tabWidget);

  m_actionBar = new AntiquaCRM::TabsEditActionBar(this);
  // __TODO__ m_actionBar->setPrinterMenu(PrinterButton::Bookcard);
  mainLayout->addWidget(m_actionBar);

  setLayout(mainLayout);

  // Signals:ImageToolBar
  connect(m_imageToolBar, SIGNAL(sendDeleteImage(qint64)),
          SLOT(setRemoveThumbnail(qint64)));
  connect(m_imageToolBar, SIGNAL(sendOpenImage()), SLOT(setActionEditImages()));

  // Signals:ActionBar
  connect(m_actionBar, SIGNAL(sendCancelClicked()),
          SLOT(setFinalLeaveEditor()));
  connect(m_actionBar, SIGNAL(sendRestoreClicked()), SLOT(setRestore()));
  connect(m_actionBar, SIGNAL(sendSaveClicked()), SLOT(setSaveData()));
  connect(m_actionBar, SIGNAL(sendFinishClicked()),
          SLOT(setCheckLeaveEditor()));
  connect(m_actionBar, SIGNAL(sendPrintBookCard()), SLOT(setPrintBookCard()));
}

void BooksEditor::setInputFields() {
  // Bei UPDATE/INSERT Ignorieren
  ignoreFields << "ib_since";
  ignoreFields << "ib_changed";

  m_tableData = new AntiquaCRM::ASqlDataQuery(BOOKS_SQL_TABLE_NAME);
  inputFields = m_tableData->columnNames();
  if (inputFields.isEmpty()) {
    QStringList warn(tr("An error has occurred!"));
    warn << tr("Can't load input datafields!");
    warn << tr("When getting this Message, please check your Network and "
               "Database connection!");
    openNoticeMessage(warn.join("\n"));
  }

  // Autoren
  QStringList authors(tr("Authors group"));
  authors << tr("Authors team");
  authors << tr("Various authors");
  ib_author->setCompleterList(authors);

  AntiquaCRM::ASharedDataFiles _dataFiles;
  QStringList _completer_data;

  // Herausgeber
  _completer_data = _dataFiles.getCompleterList("publishers", "name");
  ib_publisher->setCompleterList(_completer_data);

  // Buch Zustand
  ib_binding->initData();
  _completer_data = _dataFiles.getCompleterList("designations", "name");
  ib_designation->setCompleterList(_completer_data);

  // Lager
  ib_storage->initData();

  // Schlüsselwörter
  QStringList _keywords_data({"TODO", "Keywords"});
  ib_keyword->setCompleterList(_keywords_data);
}

bool BooksEditor::setDataField(const QSqlField &field, const QVariant &value) {
  if (!field.isValid())
    return false;

  QString key = field.name();

  // qDebug() << __LINE__ << "Search:" << field.name() << value;

  bool required = (field.requiredStatus() == QSqlField::Required);
  AntiquaCRM::AbstractInput *inp =
      findChild<AntiquaCRM::AbstractInput *>(key, Qt::FindChildrenRecursively);
  if (inp != nullptr) {
    // qDebug() << __LINE__ << inp->objectName() << key << value;
    inp->setValue(value);
    inp->setRestrictions(field);
    return true;
  }

  if (ignoreFields.contains(key))
    return true;

  if (required) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Unknown:" << key << "|" << value;
#else
    qWarning("Unknown Key (%s) found.", qPrintable(key));
#endif
  }
  return false;
}

void BooksEditor::importSqlResult() {
  if (m_tableData == nullptr)
    return;

  QHashIterator<QString, QVariant> it(m_tableData->getDataset());
  blockSignals(true);
  while (it.hasNext()) {
    it.next();
    QSqlField field = m_tableData->getProperties(it.key());
    setDataField(field, it.value());
  }
  blockSignals(false);

  // Suche Bilddaten
  qint64 id = ib_id->getValue().toLongLong();
  if (id > 0) {
    // m_imageToolBar->setArticleId(id);
    // m_imageToolBar->setActive(true);
    // m_imageView->readFromDatabase(id);
  }

  m_actionBar->setRestoreable(m_tableData->isValid());
  setResetModified(inputFields);
}

bool BooksEditor::sendSqlQuery(const QString &query) {
  //  qDebug() << Q_FUNC_INFO << query;
  //  return true;
  QSqlQuery q = m_sql->query(query);
  if (q.lastError().type() != QSqlError::NoError) {
    qDebug() << Q_FUNC_INFO << query << m_sql->lastError();
    return false;
  }

  if (q.next()) {
    if (!q.isNull("ib_id")) {
      QSqlField field = m_tableData->getProperties("ib_id");
      setDataField(field, q.value("ib_id"));
    }
  }

  openSuccessMessage(tr("Bookdata saved successfully!"));
  setResetModified(inputFields);
  return true;
}

const QHash<QString, QVariant> BooksEditor::createSqlDataset() {
  QHash<QString, QVariant> data;
  QList<AntiquaCRM::AbstractInput *> list =
      findChildren<AntiquaCRM::AbstractInput *>(fieldPattern,
                                                Qt::FindChildrenRecursively);
  QList<AntiquaCRM::AbstractInput *>::Iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    AntiquaCRM::AbstractInput *cur = *it;
    QString objName = cur->objectName();
    if (ignoreFields.contains(objName))
      continue;

    // qDebug() << objName << cur->isRequired() << cur->isValid() <<
    // cur->value();
    if (cur->isRequired() && !cur->isValid()) {
      openNoticeMessage(cur->popUpHints());
      cur->setFocus();
      data.clear();
      return data;
    }
    data.insert(objName, cur->getValue());
  }
  list.clear();

  return data;
}

void BooksEditor::createSqlUpdate() {
  int articleId = ib_id->getValue().toInt();
  if (articleId < 0) {
    openNoticeMessage(tr("Missing Article ID for Update."));
    return;
  }
  // UPDATE Anforderungen
  qDebug() << Q_FUNC_INFO << __LINE__ << "TODO";
  // ib_id->setRequired(true);

  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 1)
    return;

  QStringList set;
  QHash<QString, QVariant>::iterator it;
  int changes = 0;
  for (it = data.begin(); it != data.end(); ++it) {
    if (it.key() == "ib_id")
      continue;

    // Nur geänderte Felder in das Update aufnehmen!
    if (!isModifiedCompare(it.key(), m_tableData->getValue(it.key())))
      continue;

    if (m_tableData->getType(it.key()).id() == QMetaType::QString) {
      set.append(it.key() + "='" + it.value().toString() + "'");
      changes++;
    } else {
      set.append(it.key() + "=" + it.value().toString());
      changes++;
    }
  }

  if (changes == 0) {
    sendStatusMessage(tr("No Modifications found, Update aborted!"));
    setWindowModified(false);
    return;
  }

  // Artikel auf Deaktivierung prüfen!
  // Wenn sich die Anzahl geändert hat, ein Update senden!
  // NOTE Die Nutzerabfrage erfolgt vorher in setSaveData()!
  int cur_count = ib_count->getValue().toInt();
  int old_count = m_tableData->getValue("ib_count").toInt();
  if (old_count != cur_count && cur_count == 0) {
    // Den Buchdaten Zwischenspeicher anpassen damit das Signal
    // an die Dienstleister nur einmal gesendet wird!
    m_tableData->setValue("ib_count", cur_count);
    // Ab diesen Zeitpunkt ist das Zurücksetzen erst mal nicht mehr gültig!
    m_actionBar->setRestoreable(false);
    // Sende Bestands Mitteilung an den Socket
    QJsonObject obj;
    QJsonObject action;
    action.insert("type", QJsonValue("article_update"));
    action.insert("articleId", QJsonValue(articleId));
    action.insert("count", QJsonValue(cur_count));
    obj.insert("plugin_operation", QJsonValue(action));
    pushPluginOperation(obj);
  }

  QString sql("UPDATE inventory_books SET ");
  sql.append(set.join(","));
  sql.append(",ib_changed=CURRENT_TIMESTAMP WHERE ib_id=");
  sql.append(ib_id->getValue().toString());
  sql.append(";");
  if (sendSqlQuery(sql)) {
    qInfo("SQL UPDATE Inventory Books success!");
  }
}

void BooksEditor::createSqlInsert() {
  int articleId = ib_id->getValue().toInt();
  if (articleId >= 1) {
    qWarning("Skip INSERT, switch to UPDATE with (ArticleID > 0)!");
    createSqlUpdate();
    return;
  }
  // Bei einem INSERT die Anforderungen anpassen.
  qDebug() << Q_FUNC_INFO << __LINE__ << "TODO";
  // ib_count->setRequired(true);
  // ib_id->setRequired(false);

  // Prüfung der Buchdaten Klasse
  // Die Initialisierung erfolgt in setInputFields!
  // Bei einem INSERT wir diese hier befüllt!
  if (m_tableData == nullptr || !m_tableData->isValid()) {
    qWarning("Invalid AntiquaCRM::ASqlDataQuery detected!");
    return;
  }

  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 1)
    return;

  QStringList column; // SQL Columns
  QStringList values; // SQL Values
  QHash<QString, QVariant>::iterator it;
  for (it = data.begin(); it != data.end(); ++it) {
    if (it.value().isNull())
      continue;

    QString field = it.key();
    // Buchdaten einfügen
    m_tableData->setValue(field, it.value());

    column.append(field);
    if (m_tableData->getType(field).id() == QMetaType::QString) {
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
  if (sendSqlQuery(sql) && ib_id->getValue().toInt() >= 1) {
    qInfo("SQL INSERT Inventory Books success!");
    // Zurücksetzen Knopf Aktivieren?
    qDebug() << Q_FUNC_INFO << __LINE__ << "TODO";
    // m_actionBar->setRestoreable(m_tableData->isValid());
    // Bildaktionen erst bei vorhandener Artikel Nummer freischalten!
    // m_imageToolBar->setActive(true);
    // ib_id->setRequired(true);
  }
}

bool BooksEditor::realyDeactivateEntry() {
  qint8 _curCount = ib_count->getValue().toInt();
  qint8 _oldcount = m_tableData->getValue("ib_count").toInt();
  if (_curCount == _oldcount)
    return true; // alles ok

  QStringList body;
  body << QString("<b>");
  body << tr("When setting the Article Count to 0.");
  body << QString("</b><p>");
  body << tr("This marked the Article in all Shopsystem for deletion!");
  body << QString("</p><p>");
  body << tr("Are you sure to finish this operation?");
  body << QString("</p>");

  int ret = QMessageBox::question(this, tr("Book deactivation"), body.join(""));
  if (ret == QMessageBox::No) {
    ib_count->setValue(m_tableData->getValue("ib_count"));
    qDebug() << Q_FUNC_INFO << __LINE__ << "TODO";
    // ib_count->setRequired(true);
    return false;
  }

  qDebug() << Q_FUNC_INFO << __LINE__ << "TODO";
  // ib_count->setRequired(false);
  return true;
}

void BooksEditor::setSaveData() {
  if (ib_id->getValue().toString().isEmpty()) {
    createSqlInsert();
    return;
  }

  if (ib_count->getValue().toInt() == 0 && !realyDeactivateEntry())
    return;

  createSqlUpdate();
}

void BooksEditor::setCheckLeaveEditor() {
  if (checkIsModified()) {
    unsavedChangesPopup();
    return;
  }
  setFinalLeaveEditor(false);
}

void BooksEditor::setFinalLeaveEditor(bool force) {
  if (force) // Wenn auf Abbrechen geklickt wurde!
    setWindowModified(false);

  setResetInputFields();
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  m_thumbnail->clear();               /**< Bildvorschau leeren */
  emit sendLeaveEditor();             /**< Back to MainView */
}

void BooksEditor::setPrintBookCard() {
  qDebug() << Q_FUNC_INFO << "__TODO__";
  // BookCard *m_d = new BookCard(this);
  // m_d->setObjectName("book_card_printing");
  // QHash<QString, QVariant> data;
  // data.insert("id", getDataValue("ib_id"));
  // data.insert("title", getDataValue("ib_title"));
  // data.insert("author", getDataValue("ib_author"));
  // data.insert("year", getDataValue("ib_year"));
  // data.insert("storage", getDataValue("ib_storage"));
  // data.insert("since", getDataValue("ib_since"));
  // data.insert("keywords", getDataValue("ib_keyword"));
  // m_d->exec(data);
}

void BooksEditor::setLoadThumbnail(qint64 articleId) {
  if (articleId < 1)
    return;

  m_imageToolBar->setArticleId(articleId);

  AntiquaCRM::ImageFileSource thumbnail;
  thumbnail.setFileId(articleId);
  if (thumbnail.findInDatabase(m_sql, articleId))
    m_thumbnail->setPixmap(thumbnail.getCachedPixmap());
}

void BooksEditor::setRemoveThumbnail(qint64 articleId) {
  qint64 _id = ib_id->getValue().toLongLong();
  if (articleId != _id) {
    qWarning("Remove Thumbnail ids not equal!");
    return;
  }

  QMessageBox::StandardButton set = QMessageBox::question(
      this, tr("Remove Image from Database"),
      tr("%1\n\nImage - Article Id: %2")
          .arg(tr("Do you really want to delete the Image?"),
               QString::number(_id)));
  if (set == QMessageBox::Yes) {
    AntiquaCRM::ImageFileSource thumbnail;
    thumbnail.setFileId(_id);
    if (thumbnail.removeFromDatabase(m_sql, _id)) {
      m_thumbnail->clear();
      sendStatusMessage(tr("Image delete successfully!"));
    }
  }
}

void BooksEditor::setActionEditImages() {
  qint64 _id = ib_id->getValue().toLongLong();
  qDebug() << Q_FUNC_INFO << "__TODO__" << _id;
  // ImageDialog *d = new ImageDialog(_id, this);
  // if (d->exec()) {
  //   m_imageView->readFromDatabase(id);
  // }
}

void BooksEditor::setRestore() {
  importSqlResult();
  ib_count->setFocus();
}

bool BooksEditor::openEditEntry(qint64 articleId) {
  bool status = false;
  if (articleId < 1)
    return status;

  QString ib_id = QString::number(articleId);
  if (ib_id.isEmpty())
    return status;

  setInputFields();
  QString table = m_tableData->tableName();
  QString query("SELECT * FROM " + table + " WHERE ib_id=" + ib_id + ";");
  QSqlQuery q = m_sql->query(query);
  if (q.size() != 0) {
    QSqlRecord r = m_tableData->record();
    while (q.next()) {
      foreach (QString key, inputFields) {
        m_tableData->setValue(key, q.value(r.indexOf(key)));
      }
    }
    setLoadThumbnail(articleId);
    status = true;
  } else {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
    status = false;
  }

  if (status) {
    // Die aktuelle Abfolge ist Identisch mit setRestore!
    setRestore();
  }

  return status;
}

bool BooksEditor::createNewEntry() {
  setInputFields();
  qDebug() << Q_FUNC_INFO << "__TODO__";
  // m_imageView->clear();
  foreach (QString column, m_tableData->columnNames()) {
    QSqlField field = m_tableData->getProperties(column);
    if (column == "ib_id")
      field.setRequired(false);

    setDefaultInput(field);
  }
  setResetModified(inputFields);
  return isEnabled();
}
