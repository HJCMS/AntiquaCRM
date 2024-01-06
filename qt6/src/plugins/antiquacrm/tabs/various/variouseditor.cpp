// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "variouseditor.h"
#include "variousconfig.h"

#include <AntiquaCRM>
#include <AntiquaImageDialog>
#include <AntiquaPrinting>
#include <QDate>
#include <QLayout>

VariousEditor::VariousEditor(QWidget *parent)
    : AntiquaCRM::TabsEditor{VARIOUS_SQL_EDITOR_PATTERN, parent} {
  setWindowTitle(tr("Edit entry"));
  setObjectName("tab_various_editor");

  QString _whatsThis;
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setSizeConstraint(QLayout::SetMaximumSize);

  // BEGIN : Row0
  QHBoxLayout *row0 = new QHBoxLayout();
  va_id = new AntiquaCRM::SerialId(this);
  va_id->setObjectName("va_id");
  va_id->setBuddyLabel(tr("Article ID"));
  va_id->setRequired(true);
  _whatsThis = tr("Unique article number for this listing.");
  va_id->setWhatsThisText(_whatsThis);
  row0->addWidget(va_id);
  // va_count
  va_count = new AntiquaCRM::CrowdEdit(this);
  va_count->setObjectName("va_count");
  va_count->setBuddyLabel(tr("Count"));
  _whatsThis = tr("Current number of articles in stock. If the number is 0, "
                  "the item will be deleted from the sales portals!");
  va_count->setWhatsThisText(_whatsThis);
  row0->addWidget(va_count);
  // va_price
  double minPrice = m_cfg->value("payment/min_price_media", 1.00).toDouble();
  va_price = new AntiquaCRM::PriceEdit(this);
  va_price->setObjectName("va_price");
  va_price->setRequired(true);
  va_price->setBuddyLabel(tr("Price"));
  va_price->setMinimum(minPrice);
  _whatsThis = tr("Enter your selling price for the article here. This is the "
                  "net price, VAT and delivery costs will be added later.");
  va_price->setWhatsThisText(_whatsThis);
  row0->addWidget(va_price);
  // va_year
  va_year = new AntiquaCRM::YearEdit(this);
  va_year->setObjectName("va_year");
  va_year->setRequired(true);
  va_year->setBuddyLabel(tr("Year"));
  va_year->setValue(1800);
  _whatsThis = tr("Optional year, a date is required for articles, but the "
                  "year may differ from the actual year of manufacture. You "
                  "then write this information in the description section.");
  va_year->setWhatsThisText(_whatsThis);
  row0->addWidget(va_year);
  // va_restricted
  va_restricted = new AntiquaCRM::BoolBox(this);
  va_restricted->setObjectName("va_restricted");
  va_restricted->setBuddyLabel(tr("Local Usage only"));
  _whatsThis = tr("If checked, the article will not be exported to your "
                  "sellers.<br><b>Warning:</b> If you activate this option "
                  "later this will have no impact to your providers.<br>The "
                  "entries must then be removed manually!");
  va_restricted->setWhatsThisText(_whatsThis);
  va_restricted->setValue(true);
  row0->addWidget(va_restricted);
  row0->addStretch(1);
  mainLayout->addLayout(row0, 0);
  // END : Row0

  // Zellenbeschreibung
  AntiquaCRM::ALabel *infolabel;

  // BEGIN : Row1
  m_splitter = new AntiquaCRM::Splitter(this);
  // Eingabemasken
  QWidget *row1Widget = new QWidget(m_splitter);
  row1Widget->setContentsMargins(0, 0, 0, 0);
  int row1c = 0;
  QGridLayout *row1 = new QGridLayout(row1Widget);
  row1->setContentsMargins(0, 0, 0, 0);
  row1->setColumnStretch(1, 1);
  // va_title
  infolabel = new AntiquaCRM::ALabel(tr("Title"), row1Widget);
  row1->addWidget(infolabel, row1c, 0, 1, 1);
  va_title = new AntiquaCRM::TextLine(this);
  va_title->setObjectName("va_title");
  va_title->setToolTip(infolabel->text());
  _whatsThis = tr("Current title of the article. This is displayed first in "
                  "the sales portal and is limited to 80 characters.");
  va_title->setWhatsThisText(_whatsThis);
  row1->addWidget(va_title, row1c++, 1, 1, 1);
  // va_title_extended
  infolabel = new AntiquaCRM::ALabel(tr("Extended"), row1Widget);
  row1->addWidget(infolabel, row1c, 0, 1, 1);
  va_title_extended = new AntiquaCRM::TextLine(this);
  va_title_extended->setObjectName("va_title_extended");
  va_title_extended->setToolTip(infolabel->text());
  _whatsThis = tr("Additional sales title of the article. A supplement to the "
                  "primary title.");
  va_title_extended->setWhatsThisText(_whatsThis);
  row1->addWidget(va_title_extended, row1c++, 1, 1, 1);
  // va_designation
  infolabel = new AntiquaCRM::ALabel(tr("Designation"), row1Widget);
  row1->addWidget(infolabel, row1c, 0, 1, 1);
  va_designation = new AntiquaCRM::TextLine(this);
  va_designation->setObjectName("va_designation");
  va_designation->setToolTip(infolabel->text());
  _whatsThis = tr("Article designation for sales.");
  va_designation->setWhatsThisText(_whatsThis);
  row1->addWidget(va_designation, row1c++, 1, 1, 1);
  // va_condition
  infolabel = new AntiquaCRM::ALabel(tr("Condition"), row1Widget);
  row1->addWidget(infolabel, row1c, 0, 1, 1);
  va_condition = new AntiquaCRM::ConditionEdit(this);
  va_condition->setObjectName("va_condition");
  va_condition->setToolTip(tr("Predefined item condition."));
  va_condition->appendStretch(0);
  row1->addWidget(va_condition, row1c++, 1, 1, 1);
  // va_storage
  infolabel = new AntiquaCRM::ALabel(tr("Storage"), row1Widget);
  row1->addWidget(infolabel, row1c, 0, 1, 1);
  va_storage = new AntiquaCRM::SelectStorage(this);
  va_storage->setObjectName("va_storage");
  va_storage->setToolTip(infolabel->text());
  _whatsThis = tr("The Storage location where this %1 has been stored. "
                  "You need to Configure Storage locations first in your "
                  "Database Configuration Menu before you can use it.")
                   .arg(tr("Article"));
  va_storage->setWhatsThisText(_whatsThis);
  row1->addWidget(va_storage, row1c++, 1, 1, 1);
  // image toolbar
  row1->setRowStretch(row1c++, 1);
  QHBoxLayout *imageBarLayout = new QHBoxLayout;
  imageBarLayout->addStretch(1);
  m_imageToolBar = new AntiquaCRM::ImageToolBar(this);
  imageBarLayout->addWidget(m_imageToolBar);
  row1->addLayout(imageBarLayout, row1c++, 1, 1, 1);
  row1Widget->setLayout(row1);
  m_splitter->addLeft(row1Widget);
  // Image Viewer
  m_imageView = new AntiquaCRM::ImageThumbnail(m_splitter);
  m_splitter->addRight(m_imageView);
  mainLayout->addWidget(m_splitter, 1);
  mainLayout->setStretch(1, 1);
  // END : Row1
  // BEGIN : Row2
  m_tabWidget = new AntiquaCRM::TabsWidget(this);
  m_tabWidget->setObjectName("tab_widget");
  // va_description
  va_description = new AntiquaCRM::TextField(this);
  va_description->setObjectName("va_description");
  m_tabWidget->insertTab(0, va_description, windowIcon(), tr("Description"));
  // va_internal_description
  va_int_description = new AntiquaCRM::TextField(this);
  va_int_description->setObjectName("va_internal_description");
  m_tabWidget->insertTab(1, va_int_description, windowIcon(),
                         tr("Internal Description"));
  // Info Tab
  QWidget *m_infos = new QWidget(this);
  QVBoxLayout *m_infoLayout = new QVBoxLayout(m_infos);
  // va_since
  va_since = new AntiquaCRM::DateTimeInfo(this);
  va_since->setObjectName("va_since");
  va_since->setBuddyLabel(tr("Created at"));
  m_infoLayout->addWidget(va_since);
  // va_changed
  va_changed = new AntiquaCRM::DateTimeInfo(this);
  va_changed->setObjectName("va_changed");
  va_changed->setBuddyLabel(tr("Last changed"));
  m_infoLayout->addWidget(va_changed);
  m_infoLayout->addStretch(1);
  m_infos->setLayout(m_infoLayout);
  m_tabWidget->insertTab(2, m_infos, windowIcon(), tr("Information"));
  mainLayout->addWidget(m_tabWidget);
  // END : Row2
  // BEGIN : Row3
  m_actionBar = new AntiquaCRM::TabsEditActionBar(this);
  m_actionBar->setViewMailButton(false);
  // m_actionBar->setPrinterMenu(AntiquaCRM::PRINT_BOOKCARD);
  mainLayout->addWidget(m_actionBar);
  // END : Row3
  setLayout(mainLayout);

  // Register modified changes
  registerInputChanged();

  // Signals
  // Signals:ImageToolBar
  connect(m_imageToolBar, SIGNAL(sendDeleteImage(qint64)),
          SLOT(setRemoveThumbnail(qint64)));
  connect(m_imageToolBar, SIGNAL(sendOpenImage()), SLOT(setImportEditImage()));

  // Signals::Storage
  connect(va_storage, SIGNAL(sendValueChanged()),
          SLOT(setStorageCompartments()));

  // Signals:ActionBar
  connect(m_actionBar, SIGNAL(sendCancelClicked()),
          SLOT(setFinalLeaveEditor()));
  connect(m_actionBar, SIGNAL(sendRestoreClicked()), SLOT(setRestore()));
  connect(m_actionBar, SIGNAL(sendSaveClicked()), SLOT(setSaveData()));
  connect(m_actionBar, SIGNAL(sendFinishClicked()),
          SLOT(setCheckLeaveEditor()));
  // connect(m_actionBar, SIGNAL(sendPrintCard()), SLOT(setPrintCard()));
}

VariousEditor::~VariousEditor() {
  ignoreFields.clear();
  inputFields.clear();
  if (m_sql != nullptr)
    m_sql->deleteLater();

  if (m_cfg != nullptr)
    m_cfg->deleteLater();
}

void VariousEditor::setInputFields() {
  // Bei UPDATE/INSERT Ignorieren
  ignoreFields << "va_since";
  ignoreFields << "va_changed";
  ignoreFields << "va_type"; // only for orders!
  ignoreFields << "va_including_vat";

  // Load default table data
  m_tableData = initTableData(VARIOUS_SQL_TABLE_NAME);
  if (m_tableData == nullptr)
    return;

  // set input defaults
  AntiquaCRM::ASharedDataFiles _dataFiles;
  QStringList _completer_data;

  const QJsonObject _jobj = loadSqlConfig(VARIOUS_CONFIG_POINTER);
  double _price_lowest = _jobj.value("various_price_lowest").toDouble();
  if (_price_lowest > 1.0)
    va_price->setMinimum(_price_lowest);

  double _price_default = _jobj.value("various_price_normal").toDouble();
  if (_price_default > 2.0)
    va_price->setValue(_price_default);

  // designations
  _completer_data = _dataFiles.getCompleterList("designations", "name");
  va_designation->setCompleterList(_completer_data);

  // storage
  va_storage->initData();

  _completer_data.clear();
}

bool VariousEditor::setDataField(const QSqlField &field,
                                 const QVariant &value) {
  if (!field.isValid())
    return false;

  QString key = field.name();
  // qDebug() << "setDataField:" << key << value << ignoreFields.contains(key);
  bool required = (field.requiredStatus() == QSqlField::Required);
  AntiquaCRM::AInputWidget *inp =
      findChild<AntiquaCRM::AInputWidget *>(key, Qt::FindChildrenRecursively);
  if (inp != nullptr) {
    inp->setRestrictions(field);
    // Muss nach setRestrictions kommen!
    inp->setValue(value);
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

void VariousEditor::importSqlResult() {
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
  qint64 id = va_id->getValue().toLongLong();
  if (id > 0)
    m_imageToolBar->setArticleId(id);

  m_actionBar->setRestoreable(m_tableData->isValid());
  setResetModified(inputFields);
}

bool VariousEditor::sendSqlQuery(const QString &query) {
  // qDebug() << Q_FUNC_INFO << query;
  // return true;
  QSqlQuery q = m_sql->query(query);
  if (q.lastError().type() != QSqlError::NoError) {
    qDebug() << Q_FUNC_INFO << query << m_sql->lastError();
    return false;
  }

  if (q.next()) {
    if (!q.isNull("va_id")) {
      QSqlField field = m_tableData->getProperties("va_id");
      setDataField(field, q.value("va_id"));
    }
  }

  openSuccessMessage(tr("Data saved successfully!"));
  setResetModified(inputFields);
  return true;
}

const QHash<QString, QVariant> VariousEditor::createSqlDataset() {
  QHash<QString, QVariant> data;
  QList<AntiquaCRM::AInputWidget *> list =
      findChildren<AntiquaCRM::AInputWidget *>(fieldPattern,
                                               Qt::FindChildrenRecursively);
  QList<AntiquaCRM::AInputWidget *>::Iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    AntiquaCRM::AInputWidget *cur = *it;
    QString objName = cur->objectName();
    if (ignoreFields.contains(objName))
      continue;

    // qDebug() << objName << cur->isRequired() << cur->isValid() <<
    // cur->getValue();

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

void VariousEditor::createSqlUpdate() {
  int articleId = va_id->getValue().toInt();
  if (articleId < 0) {
    openNoticeMessage(tr("Missing Article ID for Update."));
    return;
  }
  // UPDATE Anforderungen
  va_id->setRequired(true);

  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 1)
    return;

  QStringList set;
  QHash<QString, QVariant>::iterator it;
  int changes = 0;
  for (it = data.begin(); it != data.end(); ++it) {
    if (it.key() == "va_id")
      continue;

    const QString _key = it.key();
    // Nur geänderte Felder in das Update aufnehmen!
    if (!isModifiedCompare(_key, m_tableData->getValue(_key)))
      continue;

    if (m_tableData->getType(_key).id() == QMetaType::QString) {
      set.append(_key + "='" + it.value().toString() + "'");
      changes++;
    } else {
      set.append(_key + "=" + it.value().toString());
      changes++;
    }
  }

  if (changes == 0) {
    pushStatusMessage(tr("No Modifications found, Update aborted!"));
    setWindowModified(false);
    return;
  }

  // Artikel auf Deaktivierung prüfen!
  // Wenn sich die Anzahl geändert hat, ein Update senden!
  // NOTE Die Nutzerabfrage erfolgt vorher in setSaveData()!
  int cur_count = va_count->getValue().toInt();
  int old_count = m_tableData->getValue("va_count").toInt();
  if (old_count != cur_count && cur_count == 0) {
    // Den Daten Zwischenspeicher anpassen damit das Signal
    // an die Dienstleister nur einmal gesendet wird!
    m_tableData->setValue("va_count", cur_count);
    // Ab diesen Zeitpunkt ist das Zurücksetzen erst mal nicht mehr gültig!
    m_actionBar->setRestoreable(false);
    // Sende Bestands Mitteilung an den Socket
    QJsonObject action;
    action.insert("type", QJsonValue("article_count"));
    action.insert("aid", QJsonValue(articleId));
    action.insert("count", QJsonValue(cur_count));

    QJsonObject obj;
    obj.insert("ACTION", QJsonValue("provider_update"));
    obj.insert("TARGET", QJsonValue("provider_tab"));
    obj.insert("VALUE", QJsonValue(action));
    pushPluginOperation(obj);
  }

  QString sql("UPDATE " + QString(VARIOUS_SQL_TABLE_NAME) + " SET ");
  sql.append(set.join(","));
  sql.append(",va_changed=CURRENT_TIMESTAMP WHERE va_id=");
  sql.append(va_id->getValue().toString());
  sql.append(";");
  if (sendSqlQuery(sql)) {
    pushStatusMessage(tr("Update success!"));
    setWindowModified(false);
  }
}

void VariousEditor::createSqlInsert() {
  int articleId = va_id->getValue().toInt();
  if (articleId >= 1) {
    qWarning("Skip INSERT, switch to UPDATE with (ArticleID > 0)!");
    createSqlUpdate();
    return;
  }
  // Bei einem INSERT die Anforderungen anpassen.
  va_count->setRequired(true);
  va_id->setRequired(false);

  // Bei einem INSERT wird diese hier befüllt!
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
    // Daten einfügen
    m_tableData->setValue(field, it.value());

    column.append(field);
    if (m_tableData->getType(field).id() == QMetaType::QString) {
      values.append("'" + it.value().toString() + "'");
    } else {
      values.append(it.value().toString());
    }
  }

  QString _sql("INSERT INTO " + QString(VARIOUS_SQL_TABLE_NAME) + " (");
  _sql.append(column.join(","));
  _sql.append(",va_changed) VALUES (");
  _sql.append(values.join(","));
  _sql.append(",CURRENT_TIMESTAMP) RETURNING va_id;");
  if (sendSqlQuery(_sql) && va_id->getValue().toInt() >= 1) {
    // Zurücksetzen Knopf Aktivieren?
    m_actionBar->setRestoreable(m_tableData->isValid());
    // Bildaktionen erst bei vorhandener Artikel Nummer freischalten!
    m_imageToolBar->setArticleId(va_id->getValue().toInt());
    va_id->setRequired(true);
    pushStatusMessage(tr("Entry created!"));
    setWindowModified(false);
  }
}

void VariousEditor::setSaveData() {
  if (va_id->getValue().toString().isEmpty()) {
    createSqlInsert();
    return;
  }
  createSqlUpdate();
}

void VariousEditor::setCheckLeaveEditor() {
  if (checkIsModified()) {
    unsavedChangesPopup();
    return;
  }
  setFinalLeaveEditor();
}

void VariousEditor::setFinalLeaveEditor(bool force) {
  if (force) // Wenn auf Abbrechen geklickt wurde!
    setWindowModified(false);

  setResetInputFields();
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  m_imageView->clear();               /**< Bildvorschau leeren */
  emit sendLeaveEditor();             /**< Back to MainView */
}

void VariousEditor::setLoadThumbnail(qint64 articleId) {
  if (articleId < 1)
    return;

  m_imageToolBar->setArticleId(articleId);

  AntiquaCRM::ImageFileSource image_preview;
  if (image_preview.findInDatabase(m_sql, articleId))
    m_imageView->setPixmap(image_preview.getThumbnail());
}

void VariousEditor::setRemoveThumbnail(qint64 articleId) {
  qint64 _id = va_id->getValue().toLongLong();
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
      m_imageView->clear();
      pushStatusMessage(tr("Image delete successfully!"));
    }
  }
}

void VariousEditor::setImportEditImage() {
  qint64 _id = va_id->getValue().toLongLong();
  if (_id < 1) {
    pushStatusMessage(tr("No Article number for Image import!"));
    return;
  }

  AntiquaCRM::ImageImportDialog *d =
      new AntiquaCRM::ImageImportDialog(_id, "Various", this);
  connect(d, SIGNAL(sendThumbnail(const QPixmap &)), m_imageView,
          SLOT(setPixmap(const QPixmap &)));

  d->exec();
  d->deleteLater();
}

void VariousEditor::setRestore() {
  importSqlResult();
  va_count->setFocus();
}

bool VariousEditor::openEditEntry(qint64 articleId) {
  bool status = false;
  if (articleId < 1)
    return status;

  QString a_id = QString::number(articleId);
  if (a_id.isEmpty())
    return status;

  setInputFields();
  const QString table(VARIOUS_SQL_TABLE_NAME);
  QString query("SELECT * FROM " + table + " WHERE va_id=" + a_id + ";");
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
  }

  if (status) {
    // Die aktuelle Abfolge ist Identisch mit setRestore!
    setRestore();
  }
  return status;
}

bool VariousEditor::createNewEntry() {
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

bool VariousEditor::createCustomEntry(const QJsonObject &object) {
  Q_UNUSED(object);
  qInfo("Unused function for this plugin, skipped!");
  return true;
}
