// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookeditor.h"
#include "editoractionbar.h"
#include "keywordlineedit.h"

#include <AntiquaCRM>
#include <AntiquaPrinting>
#include <QDebug>
#include <QMessageBox>

BookEditor::BookEditor(QWidget *parent)
    : InventoryEditor{"^ib_[a-z_]+\\b$", parent} {
  setObjectName("book_editor");
  setWindowTitle(tr("Edit Book"));

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("bookeditor_main_layout");
  mainLayout->setSizeConstraint(QLayout::SetMaximumSize);

  // Row 0
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

  double minPrice = m_cfg->value("payment/min_price", 5.00).toDouble();
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
  row0->addWidget(ib_including_vat);

  ib_signed = new BoolBox(this);
  ib_signed->setObjectName("ib_signed");
  ib_signed->setInfo(tr("Signed Version"));
  row0->addWidget(ib_signed);

  ib_restricted = new BoolBox(this);
  ib_restricted->setObjectName("ib_restricted");
  ib_restricted->setInfo(tr("Local Usage only"));
  ib_restricted->setToolTip(tr("When this Options is marked. Then this Article "
                               "will not exported to your Providers."));

  row0->addWidget(ib_restricted);
  row0->addStretch(1);
  mainLayout->addLayout(row0, 0);

  // Row 1
  QHBoxLayout *row1 = new QHBoxLayout();

  ib_edition = new IntSpinBox(this);
  ib_edition->setObjectName("ib_edition");
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

  // Row 2
  m_splitter = new AntiquaSplitter(this);
  QWidget *row2Widget = new QWidget(m_splitter);
  row2Widget->setContentsMargins(0, 0, 0, 0);
  int row2c = 0;
  QGridLayout *row2 = new QGridLayout(row2Widget);
  row2->setContentsMargins(0, 0, 0, 0);

  AntiquaILabel *infoText;

  infoText = new AntiquaILabel(tr("Booktitle"), row2Widget);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ib_title = new LineEdit(this);
  ib_title->setObjectName("ib_title");
  ib_title->setToolTip(infoText->text());
  row2->addWidget(ib_title, row2c++, 1, 1, 1);

  infoText = new AntiquaILabel(tr("Book Subtitle"), this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ib_title_extended = new LineEdit(this);
  ib_title_extended->setObjectName("ib_title_extended");
  ib_title_extended->setToolTip(infoText->text());
  row2->addWidget(ib_title_extended, row2c++, 1, 1, 1);

  infoText = new AntiquaILabel(tr("Author"), this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ib_author = new LineEdit(this);
  ib_author->setObjectName("ib_author");
  ib_author->setToolTip(infoText->text());
  row2->addWidget(ib_author, row2c++, 1, 1, 1);

  infoText = new AntiquaILabel(tr("Publisher"), this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ib_publisher = new LineEdit(this);
  ib_publisher->setObjectName("ib_publisher");
  ib_publisher->setToolTip(infoText->text());
  row2->addWidget(ib_publisher, row2c++, 1, 1, 1);

  infoText = new AntiquaILabel(tr("Condition"), this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  QHBoxLayout *conditionlayout = new QHBoxLayout();
  ib_condition = new ConditionEdit(this);
  ib_condition->setObjectName("ib_condition");
  ib_condition->setToolTip(tr("Condition of this Book."));
  conditionlayout->addWidget(ib_condition, Qt::AlignLeft);

  infoText = new AntiquaILabel(tr("Language"), this);
  conditionlayout->addWidget(infoText, Qt::AlignRight);
  ib_language = new Language(this);
  ib_language->setObjectName("ib_language");
  conditionlayout->addWidget(ib_language);
  row2->addLayout(conditionlayout, row2c++, 1, 1, 1);

  infoText = new AntiquaILabel(tr("Designation"), this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  // Bucheinband und Bindungsart
  QHBoxLayout *binding_layout = new QHBoxLayout();
  binding_layout->setContentsMargins(0, 0, 0, 0);
  ib_binding = new BookBinding(this);
  ib_binding->setObjectName("ib_binding");
  ib_binding->setToolTip(tr("Cover"));
  binding_layout->addWidget(ib_binding);
  ib_designation = new LineEdit(this);
  ib_designation->setObjectName("ib_designation");
  ib_designation->setToolTip(infoText->text());
  binding_layout->addWidget(ib_designation);
  row2->addLayout(binding_layout, row2c++, 1, 1, 1);

  // BEGIN:Storage
  infoText = new AntiquaILabel(tr("Storage"), this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  QHBoxLayout *storageLayout = new QHBoxLayout;
  ib_storage = new StorageLocation(this);
  ib_storage->setObjectName("ib_storage");
  ib_storage->setToolTip(infoText->text());
  storageLayout->addWidget(ib_storage);
  infoText = new AntiquaILabel(tr("Compartment"), this);
  storageLayout->addWidget(infoText);
  ib_storage_compartment = new LineEdit(this);
  ib_storage_compartment->setObjectName("ib_storage_compartment");
  ib_storage_compartment->setToolTip(tr("Storage compartment or group"));
  storageLayout->addWidget(ib_storage_compartment);
  row2->addLayout(storageLayout, row2c++, 1, 1, 1);
  // END:Storage

  infoText = new AntiquaILabel(tr("Keywords"), this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ib_keyword = new KeywordLineEdit(this);
  ib_keyword->setObjectName("ib_keyword");
  ib_keyword->setInfo(infoText->text());
  row2->addWidget(ib_keyword, row2c++, 1, 1, 1);
  row2->setRowStretch(row2c++, 1);

  // @BEGIN_GROUP
  infoText = new AntiquaILabel("ISBN/EAN", this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  QHBoxLayout *tbLayout = new QHBoxLayout;
  ib_isbn = new IsbnEdit(this);
  ib_isbn->setObjectName("ib_isbn");
  ib_isbn->setToolTip("ISBN/EAN");
  tbLayout->addWidget(ib_isbn);

  m_dnbQuery = new QLabel(this);
  m_dnbQuery->setOpenExternalLinks(true);
  m_dnbQuery->setToolTip(tr("German National Library"));
  tbLayout->addWidget(m_dnbQuery);

  tbLayout->addStretch(1);
  m_imageToolBar = new ImageToolBar(this);
  tbLayout->addWidget(m_imageToolBar);
  // @END_GROUP
  row2->addLayout(tbLayout, row2c++, 1, 1, 1);
  row2Widget->setLayout(row2);

  // Image Viewer
  QSize maxSize = m_cfg->value("image/max_size", QSize(320, 320)).toSize();
  m_imageView = new ImageView(maxSize, this);

  m_splitter->addLeft(row2Widget);
  m_splitter->addRight(m_imageView);
  mainLayout->addWidget(m_splitter);
  mainLayout->setStretch(2, 1);

  // Row3
  m_tabWidget = new EditorTab(this);
  m_tabWidget->setObjectName("tab_widget");
  QIcon tabIcons = m_tabWidget->defaultIcon();
  // Description
  ib_description = new DescripeEdit(m_tabWidget);
  ib_description->setObjectName("ib_description");
  m_tabWidget->insertTab(0, ib_description, tabIcons, tr("Public Description"));
  // Internal Description
  ib_internal_description = new TextField(m_tabWidget);
  ib_internal_description->setObjectName("ib_internal_description");
  m_tabWidget->insertTab(1, ib_internal_description, tabIcons,
                         tr("Internal Description"));
  // Info Tab
  QWidget *m_infos = new QWidget(this);
  QVBoxLayout *m_infoLayout = new QVBoxLayout(m_infos);
  ib_since = new AntiquaDateInfo(this);
  ib_since->setObjectName("ib_since");
  ib_since->setInfo(tr("Created at"));
  m_infoLayout->addWidget(ib_since);
  ib_changed = new AntiquaDateInfo(this);
  ib_changed->setObjectName("ib_changed");
  ib_changed->setInfo(tr("Last changed"));
  m_infoLayout->addWidget(ib_changed);
  m_infoLayout->addStretch(1);
  m_infos->setLayout(m_infoLayout);
  m_tabWidget->insertTab(2, m_infos, tabIcons, tr("Information"));
  mainLayout->addWidget(m_tabWidget);

  m_actionBar = new EditorActionBar(this);
  m_actionBar->setPrinterMenu(PrinterButton::Bookcard);
  mainLayout->addWidget(m_actionBar);

  setLayout(mainLayout);

  // Signals:ImageToolBar
  connect(m_imageToolBar, SIGNAL(sendDeleteImage(qint64)),
          SLOT(actionRemoveImage(qint64)));
  connect(m_imageToolBar, SIGNAL(sendOpenImage()), SLOT(actionEditImages()));

  // Signals:ImageViewer
  connect(m_imageView, SIGNAL(sendImageLoadSuccess(bool)), m_imageToolBar,
          SLOT(enableActions(bool)));

  // Signals:ActionBar
  connect(m_actionBar, SIGNAL(sendCancelClicked()),
          SLOT(setFinalLeaveEditor()));
  connect(m_actionBar, SIGNAL(sendRestoreClicked()), SLOT(setRestore()));
  connect(m_actionBar, SIGNAL(sendSaveClicked()), SLOT(setSaveData()));
  connect(m_actionBar, SIGNAL(sendFinishClicked()),
          SLOT(setCheckLeaveEditor()));
  connect(m_actionBar, SIGNAL(sendPrintBookCard()), SLOT(setPrintBookCard()));
}

void BookEditor::setInputFields() {
  // Bei UPDATE/INSERT Ignorieren
  ignoreFields << "ib_since";
  ignoreFields << "ib_type";
  ignoreFields << "ib_changed";

  QUrl _url("https://katalog.dnb.de/DE/home.html");
  m_dnbQuery->setText(
      tr("<a href='%1'>German National Library</a>").arg(_url.toString()));

  m_tableData = new AntiquaCRM::ASqlDataQuery("inventory_books");
  inputFields = m_tableData->columnNames();
  if (inputFields.isEmpty()) {
    QStringList warn(tr("An error has occurred!"));
    warn << tr("Can't load input datafields!");
    warn << tr("When getting this Message, please check your Network and "
               "Database connection!");
    openNoticeMessage(warn.join("\n"));
  }

  // Autoren
  QStringList authors({tr("Authors group"), tr("Authors team")});
  ib_author->setCompleter(authors);

  // Herausgeber
  AntiquaCRM::ACompleterData publishers("publishers");
  ib_publisher->setCompleter(publishers.completition("name"));

  // Lager
  ib_storage->reset();
  ib_storage->loadDataset();
  // Lagerfach
  QSqlField ib_st_field;
  ib_st_field.setName("ib_storage_compartment");
  ib_st_field.setType(QVariant::String);
  ib_st_field.setRequiredStatus(QSqlField::Required);
  ib_storage_compartment->setProperties(ib_st_field);

  // Buch Zustand
  ib_binding->loadDataset();
  AntiquaCRM::ACompleterData designations("designations");
  ib_designation->setCompleter(designations.completition("name"));

  // Schlüsselwörter
  ib_keyword->loadDataset();
}

void BookEditor::setCatalogSearch() {
  QString _key("num");
  QString _search = m_tableData->getValue("ib_isbn").toString();
  if (_search.length() < 10) {
    _key = QString("all");
    _search = AntiquaCRM::AUtil::urlSearchString(ib_title->value().toString());
    if (_search.isEmpty())
      return;
  }

  QUrl _url;
  _url.setScheme("https");
  _url.setHost("katalog.dnb.de");
  _url.setPath("/DE/list.html");
  QUrlQuery _query;
  _query.addQueryItem("key", _key);
  _query.addQueryItem("t", _search);
  _query.addQueryItem("v", "plist");
  _url.setQuery(_query);

  if (_url.isValid()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << _url.toString();
#endif
    m_dnbQuery->setText(
        tr("<a href='%1'>German National Library</a>").arg(_url.toString()));
  }
}

bool BookEditor::setDataField(const QSqlField &field, const QVariant &value) {
  if (!field.isValid())
    return false;

  QString key = field.name();
  bool required = (field.requiredStatus() == QSqlField::Required);
  InputEdit *inp = findChild<InputEdit *>(key, Qt::FindChildrenRecursively);
  if (inp != nullptr) {
    inp->setValue(value);
    inp->setProperties(field);
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

void BookEditor::importSqlResult() {
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
  qint64 id = ib_id->value().toLongLong();
  if (id > 0) {
    m_imageToolBar->setArticleId(id);
    m_imageToolBar->setActive(true);
    m_imageView->readFromDatabase(id);
  }

  m_actionBar->setRestoreable(m_tableData->isValid());
  setResetModified(inputFields);
  setCatalogSearch();
}

bool BookEditor::sendSqlQuery(const QString &query) {
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

const QHash<QString, QVariant> BookEditor::createSqlDataset() {
  QHash<QString, QVariant> data;
  QList<InputEdit *> list =
      findChildren<InputEdit *>(fieldPattern, Qt::FindChildrenRecursively);
  QList<InputEdit *>::Iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    InputEdit *cur = *it;
    QString objName = cur->objectName();
    if (ignoreFields.contains(objName))
      continue;

    // qDebug() << objName << cur->isRequired() << cur->isValid() <<
    // cur->value();
    if (cur->isRequired() && !cur->isValid()) {
      openNoticeMessage(cur->notes());
      cur->setFocus();
      data.clear();
      return data;
    }
    data.insert(objName, cur->value());
  }
  list.clear();

  return data;
}

void BookEditor::createSqlUpdate() {
  int articleId = ib_id->value().toInt();
  if (articleId < 0) {
    openNoticeMessage(tr("Missing Article ID for Update."));
    return;
  }
  // UPDATE Anforderungen
  ib_id->setRequired(true);

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
  int cur_count = ib_count->value().toInt();
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
  sql.append(ib_id->value().toString());
  sql.append(";");
  if (sendSqlQuery(sql)) {
    qInfo("SQL UPDATE Inventory Books success!");
  }
}

void BookEditor::createSqlInsert() {
  int articleId = ib_id->value().toInt();
  if (articleId >= 1) {
    qWarning("Skip INSERT, switch to UPDATE with (ArticleID > 0)!");
    createSqlUpdate();
    return;
  }
  // Bei einem INSERT die Anforderungen anpassen.
  ib_count->setRequired(true);
  ib_id->setRequired(false);

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
  if (sendSqlQuery(sql) && ib_id->value().toInt() >= 1) {
    qInfo("SQL INSERT Inventory Books success!");
    // Zurücksetzen Knopf Aktivieren?
    m_actionBar->setRestoreable(m_tableData->isValid());
    // Bildaktionen erst bei vorhandener Artikel Nummer freischalten!
    m_imageToolBar->setActive(true);
    ib_id->setRequired(true);
  }
}

bool BookEditor::realyDeactivateEntry() {
  qint8 _curCount = ib_count->value().toInt();
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
    ib_count->setRequired(true);
    return false;
  }

  ib_count->setRequired(false);
  return true;
}

void BookEditor::setSaveData() {
  if (ib_id->value().toString().isEmpty()) {
    createSqlInsert();
    return;
  }

  if (ib_count->value().toInt() == 0 && !realyDeactivateEntry())
    return;

  createSqlUpdate();
}

void BookEditor::setCheckLeaveEditor() {
  if (checkIsModified()) {
    unsavedChangesPopup();
    return;
  }
  setFinalLeaveEditor(false);
}

void BookEditor::setFinalLeaveEditor(bool force) {
  if (force) // Wenn auf Abbrechen geklickt wurde!
    setWindowModified(false);

  setResetInputFields();
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  m_imageView->clear();               /**< Bildvorschau leeren */
  emit sendLeaveEditor();             /**< Back to MainView */
}

void BookEditor::setPrintBookCard() {
  BookCard *m_d = new BookCard(this);
  m_d->setObjectName("book_card_printing");
  QHash<QString, QVariant> data;
  data.insert("id", getDataValue("ib_id"));
  data.insert("title", getDataValue("ib_title"));
  data.insert("author", getDataValue("ib_author"));
  data.insert("year", getDataValue("ib_year"));
  data.insert("storage", getDataValue("ib_storage"));
  data.insert("since", getDataValue("ib_since"));
  QString _compartment = getDataValue("ib_storage_compartment").toString();
  if (_compartment.isEmpty())
    _compartment = getDataValue("ib_keyword").toString();

  data.insert("keywords", _compartment);
  m_d->exec(data);
}

void BookEditor::actionRemoveImage(qint64 articleId) {
  qint64 id = ib_id->value().toLongLong();
  if (articleId != id)
    return;

  QString image_id = QString::number(id);
  QString t(tr("Remove Image from Database"));
  QString ask(tr("Do you really want to delete the Image?"));
  QString m = tr("%1\n\nImage - Article Id: %2").arg(ask, image_id);
  QMessageBox::StandardButton set = QMessageBox::question(this, t, m);
  if (set == QMessageBox::Yes) {
    if (m_imageView->removeFromDatabase(id)) {
      m_imageView->clear();
      sendStatusMessage(tr("Image delete successfully!"));
    }
  }
}

void BookEditor::actionEditImages() {
  qint64 id = ib_id->value().toLongLong();
  ImageDialog *d = new ImageDialog(id, this);
  d->setSubCategory("Books");
  if (d->exec()) {
    m_imageView->readFromDatabase(id);
  }
}

void BookEditor::setRestore() {
  importSqlResult();
  ib_count->setFocus();
}

bool BookEditor::openEditEntry(qint64 articleId) {
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
    status = true;
  } else {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
#endif
    status = false;
  }

  if (status) {
    // Die aktuelle Abfolge ist Identisch mit setRestore!
    setRestore();
  }

  return status;
}

bool BookEditor::createNewEntry() {
  setInputFields();
  m_imageView->clear();
  foreach (QString column, m_tableData->columnNames()) {
    QSqlField field = m_tableData->getProperties(column);
    if (column == "ib_id")
      field.setRequired(false);

    setDefaultInput(field);
  }
  setResetModified(inputFields);
  return isEnabled();
}
