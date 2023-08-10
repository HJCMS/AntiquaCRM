// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "stitcheseditor.h"
#include "stitchesconfig.h"

#include <AntiquaCRM>
#include <AntiquaPrinting>
#include <QDate>
#include <QLayout>

StitchesEditor::StitchesEditor(QWidget *parent)
    : AntiquaCRM::TabsEditor{STITCHES_SQL_EDITOR_PATTERN, parent} {
  setWindowTitle(tr("Edit entry"));
  setObjectName("tab_stitches_editor");

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("printseditor_main_layout");
  mainLayout->setSizeConstraint(QLayout::SetMaximumSize);

  QString tempWhatsThis;
  AntiquaCRM::ALabel::Align _lbAlign = AntiquaCRM::ALabel::Align::MiddleRight;

  // Row 0
  QHBoxLayout *row0 = new QHBoxLayout();
  ip_id = new AntiquaCRM::SerialId(this);
  ip_id->setObjectName("ip_id");
  ip_id->setBuddyLabel(tr("Article ID"));
  tempWhatsThis = tr("Probably the most important parameter for the article "
                     "processing and is generated automatically when creating "
                     "a entry. AntiquaCRM use a running numbering system.");
  ip_id->setWhatsThisText(tempWhatsThis);
  row0->addWidget(ip_id);

  ip_count = new AntiquaCRM::CrowdEdit(this);
  ip_count->setObjectName("ip_count");
  ip_count->setBuddyLabel(tr("Count"));
  tempWhatsThis = tr(
      "The Article count is the main value for activating and deactivating "
      "stitches in your inventory. If you set it to 0, it will be disabled on "
      "your vendor pages and no longer visible for sale.\nNotes: These "
      "updates are not done directly, however daily performed from "
      "AntiquaCRM Server after working time.");
  ip_count->setWhatsThisText(tempWhatsThis);
  row0->addWidget(ip_count);

  ip_price = new AntiquaCRM::PriceEdit(this);
  ip_price->setObjectName("ip_price");
  ip_price->setBuddyLabel(tr("Price"));
  tempWhatsThis =
      tr("The price you want to have for it.\nNotes: You can always set "
         "limits for Prices in the main Configuration dialog.");
  ip_price->setWhatsThisText(tempWhatsThis);
  row0->addWidget(ip_price);

  ip_views = new AntiquaCRM::BoolBox(this);
  ip_views->setObjectName("ip_views");
  ip_views->setBuddyLabel(tr("View"));
  tempWhatsThis = tr("Is it a portrait or landscape print.");
  ip_views->setWhatsThisText(tempWhatsThis);
  row0->addWidget(ip_views);

  ip_kolorit = new AntiquaCRM::BoolBox(this);
  ip_kolorit->setObjectName("ip_kolorit");
  ip_kolorit->setBuddyLabel(tr("Kolorit"));
  tempWhatsThis = tr("In painting, color means the selection, harmony, shading "
                     "and composition of the different colors in a painting.");
  ip_kolorit->setWhatsThisText(tempWhatsThis);
  row0->addWidget(ip_kolorit);

  ip_landscape = new AntiquaCRM::BoolBox(this);
  ip_landscape->setObjectName("ip_landscape");
  ip_landscape->setBuddyLabel(tr("Landscape"));
  tempWhatsThis = tr("Is it a landscape format?");
  ip_landscape->setWhatsThisText(tempWhatsThis);
  row0->addWidget(ip_landscape);

  ip_year = new AntiquaCRM::YearEdit(this);
  ip_year->setObjectName("ip_year");
  ip_year->setBuddyLabel(tr("Year"));
  tempWhatsThis =
      tr("The Article Year input mask starts with 1400 and ending one Year in "
         "the future. If the Edition Year is unknown, set an estimate and add "
         "a note in the description section.");
  ip_year->setWhatsThisText(tempWhatsThis);
  row0->addWidget(ip_year);

  ip_restricted = new AntiquaCRM::BoolBox(this);
  ip_restricted->setObjectName("ip_restricted");
  ip_restricted->setBuddyLabel(tr("Local Usage only"));
  tempWhatsThis =
      tr("The Article will not exported to your Online Sellers when it is "
         "marked.\nNotes: You cannot do this afterwards, there are no remote "
         "deletion processes with your online retailer for this.");
  ip_restricted->setWhatsThisText(tempWhatsThis);
  row0->addWidget(ip_restricted);
  row0->addStretch(1);
  mainLayout->addLayout(row0, 0);

  // Row 1
  m_splitter = new AntiquaCRM::Splitter(this);
  QWidget *row2Widget = new QWidget(m_splitter);
  row2Widget->setContentsMargins(0, 0, 0, 0);
  int row2c = 0;
  QGridLayout *row2 = new QGridLayout(row2Widget);
  row2->setContentsMargins(0, 0, 0, 0);

  AntiquaCRM::ALabel *infoText;
  infoText = new AntiquaCRM::ALabel(tr("Title"), _lbAlign, row2Widget);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ip_title = new AntiquaCRM::TextLine(this);
  ip_title->setObjectName("ip_title");
  ip_title->setInputToolTip(tr("Title"));
  tempWhatsThis =
      tr("A Title is restricted to 80 Characters. Some Online Providers only "
         "support this title length. Other Providers allowing more characters. "
         "To get around this, the following Subtitle field will appended "
         "seperated by „-“ to the title when exporting the article data to "
         "your online shop providers.");
  ip_title->setWhatsThisText(tempWhatsThis);
  row2->addWidget(ip_title, row2c++, 1, 1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Subtitle"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ip_title_extended = new AntiquaCRM::TextLine(this);
  ip_title_extended->setObjectName("ip_title_extended");
  ip_title_extended->setInputToolTip(tr("Subtitle"));
  // Warnung: Verwende „tempWhatsThis“ von ip_title!
  ip_title_extended->setWhatsThisText(tempWhatsThis);
  row2->addWidget(ip_title_extended, row2c++, 1, 1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Author"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ip_author = new AntiquaCRM::TextLine(this);
  ip_author->setObjectName("ip_author");
  ip_author->setInputToolTip(tr("Authors"));
  tempWhatsThis = tr("This field is reserved for Authors.\nYou can add more "
                     "separated by comma.\nSome provider Platforms supporting "
                     "spezial Keywords.\nAntiquaCRM suggests the most common "
                     "author group names, when editing this field.");
  ip_author->setWhatsThisText(tempWhatsThis);
  row2->addWidget(ip_author, row2c++, 1, 1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Format"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ip_format = new AntiquaCRM::TextLine(this);
  ip_format->setObjectName("ip_format");
  ip_format->setInputToolTip(infoText->text());
  tempWhatsThis = tr("FORMAT __TODO__");
  ip_format->setWhatsThisText(tempWhatsThis);
  row2->addWidget(ip_format, row2c++, 1, 1, 1);

  // Begin:Storage
  infoText = new AntiquaCRM::ALabel(tr("Storage"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);

  QHBoxLayout *storageLayout = new QHBoxLayout;

  ip_storage = new AntiquaCRM::SelectStorage(this);
  ip_storage->setObjectName("ip_storage");
  ip_storage->setInputToolTip(tr("Storage location"));
  tempWhatsThis = tr("The Storage location where this Article has been stored. "
                     "You need to Configure Storage locations first in your "
                     "Database Configuration Menu before you can use it.");
  ip_storage->setWhatsThisText(tempWhatsThis);
  ip_storage->appendStretch();
  storageLayout->addWidget(ip_storage);

  ip_storage_compartment = new AntiquaCRM::TextLine(this);
  ip_storage_compartment->setObjectName("ip_storage_compartment");
  ip_storage_compartment->setInputToolTip(tr("Storage compartment"));
  ip_storage_compartment->setValue(tr("Office"));
  ip_storage_compartment->setBuddyLabel(tr("compartment"));
  tempWhatsThis =
      tr("Define Storage compartment in this Input field. It depends on the "
         "Storage location and will printed on top of the id card. If you "
         "select Storage location first a completer will add for this input "
         "field.");
  ip_storage_compartment->setWhatsThisText(tempWhatsThis);
  storageLayout->addWidget(ip_storage_compartment);
  row2->addLayout(storageLayout, row2c++, 1, 1, 1);
  // End:Storage

  infoText = new AntiquaCRM::ALabel(tr("Keywords"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ip_keyword = new AntiquaCRM::KeywordsEdit(this);
  ip_keyword->setObjectName("ip_keyword");
  tempWhatsThis = tr(
      "Keywords will help Buyers, to find your Article on Provider pages. This "
      "Articles keyword management can also be compared to „Search Engine "
      "Optimization“ (SEO).\n"
      "Restrictions:\n"
      "- A Keyword must have a minimum length from 3 characters.\n"
      "- A Keywords list is restricted to a Maximum length of 60 Characters.\n"
      "- A keyword cannot contain spaces or non unicode special characters.\n"
      "You can edit predefined Keywords in your Database Configuration Menu.");
  ip_keyword->setWhatsThisText(tempWhatsThis);
  row2->addWidget(ip_keyword, row2c++, 1, 1, 1);
  row2->setRowStretch(row2c++, 1);

  // infoText = new AntiquaCRM::ALabel("ISBN/EAN", _lbAlign, this);
  // row2->addWidget(infoText, row2c, 0, 1, 1);
  m_imageToolBar = new AntiquaCRM::ImageToolBar(this);
  row2->addWidget(m_imageToolBar, row2c++, 1, 1, 1, Qt::AlignRight);

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
  m_tabWidget = new AntiquaCRM::TabsWidget(this);
  QIcon tabIcons = m_tabWidget->defaultIcon();

  // Description
  ip_description = new AntiquaCRM::DescripeEditor(m_tabWidget);
  ip_description->setObjectName("ip_description");
  m_tabWidget->insertTab(0, ip_description, tabIcons, tr("Public Description"));
  // Internal Description
  ip_internal_description = new AntiquaCRM::TextField(m_tabWidget);
  ip_internal_description->setObjectName("ip_internal_description");
  m_tabWidget->insertTab(1, ip_internal_description, tabIcons,
                         tr("Internal Description"));
  // Info Tab
  QWidget *m_infos = new QWidget(this);
  QGridLayout *infoLayout = new QGridLayout(m_infos);
  infoLayout->setColumnStretch(1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Created"), _lbAlign, m_infos);
  infoLayout->addWidget(infoText, 0, 0, 1, 1);
  ip_since = new AntiquaCRM::DateTimeInfo(m_infos);
  ip_since->setObjectName("ip_since");
  infoLayout->addWidget(ip_since, 0, 1, 1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Changed"), _lbAlign, m_infos);
  infoLayout->addWidget(infoText, 1, 0, 1, 1);
  ip_changed = new AntiquaCRM::DateTimeInfo(m_infos);
  ip_changed->setObjectName("ip_changed");
  infoLayout->addWidget(ip_changed, 1, 1, 1, 1);

  infoLayout->setRowStretch(2, 1);
  m_infos->setLayout(infoLayout);
  m_tabWidget->insertTab(2, m_infos, tabIcons, tr("Information"));
  mainLayout->addWidget(m_tabWidget);

  m_actionBar = new AntiquaCRM::TabsEditActionBar(this);
  m_actionBar->setViewMailButton(false);
  m_actionBar->setPrinterMenu(AntiquaCRM::PRINT_BOOKCARD);
  mainLayout->addWidget(m_actionBar);

  setLayout(mainLayout);

  // Register modified changes
  registerInputChanged();

  // Signals:ImageToolBar
  connect(m_imageToolBar, SIGNAL(sendDeleteImage(qint64)),
          SLOT(setRemoveThumbnail(qint64)));
  connect(m_imageToolBar, SIGNAL(sendOpenImage()), SLOT(setImportEditImage()));

  // Signals::Storage
  connect(ip_storage, SIGNAL(sendValueChanged()),
          SLOT(setStorageCompartments()));

  // Signals:ActionBar
  connect(m_actionBar, SIGNAL(sendCancelClicked()),
          SLOT(setFinalLeaveEditor()));
  connect(m_actionBar, SIGNAL(sendRestoreClicked()), SLOT(setRestore()));
  connect(m_actionBar, SIGNAL(sendSaveClicked()), SLOT(setSaveData()));
  connect(m_actionBar, SIGNAL(sendFinishClicked()),
          SLOT(setCheckLeaveEditor()));
  connect(m_actionBar, SIGNAL(sendPrintBookCard()), SLOT(setPrintBookCard()));
}

StitchesEditor::~StitchesEditor() {
  ignoreFields.clear();
  inputFields.clear();
  if (m_sql != nullptr)
    m_sql->deleteLater();

  if (m_cfg != nullptr)
    m_cfg->deleteLater();
}

void StitchesEditor::setInputFields() {
  // Bei UPDATE/INSERT Ignorieren
  ignoreFields << "ip_since";
  ignoreFields << "ip_changed";
  ignoreFields << "ip_including_vat"; /* DEPRECATED */

  m_tableData = initTableData(STITCHES_TABLE_NAME);
  if (m_tableData == nullptr)
    return;

  // Set input defaults
  AntiquaCRM::ASharedDataFiles _dataFiles;
  QStringList _completer_data;

  const QJsonObject _jobj = loadSqlConfig(STITCHES_CONFIG_POINTER);
  double _price_lowest = _jobj.value("stitches_price_lowest").toDouble();
  if (_price_lowest > 1.0)
    ip_price->setMinimum(_price_lowest);

  double _price_default = _jobj.value("stitches_price_normal").toDouble();
  if (_price_default > 2.0)
    ip_price->setValue(_price_default);

  // authors
  QStringList authors(tr("Authors group"));
  authors << tr("Authors team");
  authors << tr("Various authors");
  ip_author->setCompleterList(authors);

  // storage
  ip_storage->initData();

  // keywords
  _completer_data = _dataFiles.getCompleterList("keywords", "name");
  ip_keyword->setCompleterList(_completer_data);

  _completer_data.clear();

  // description
  QStringList _list;
  _list << tr("Nice clean copy");
  _list << tr("Clean copy");
  _list << tr("Copy a bit dusty");
  _list << tr("Year may differ from entry");
  _list << tr("Age-related traces");
  _list << tr("With bookplate");
  ip_description->setWordsList(_list);
}

bool StitchesEditor::setDataField(const QSqlField &field,
                                  const QVariant &value) {
  if (!field.isValid())
    return false;

  QString key = field.name();
  // qDebug() << "setDataField:" << field.name() << value;
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

void StitchesEditor::importSqlResult() {
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
  qint64 id = ip_id->getValue().toLongLong();
  if (id > 0)
    m_imageToolBar->setArticleId(id);

  m_actionBar->setRestoreable(m_tableData->isValid());
  setResetModified(inputFields);
}

bool StitchesEditor::sendSqlQuery(const QString &query) {
  //  qDebug() << Q_FUNC_INFO << query;
  //  return true;
  QSqlQuery q = m_sql->query(query);
  if (q.lastError().type() != QSqlError::NoError) {
    qDebug() << Q_FUNC_INFO << query << m_sql->lastError();
    return false;
  }

  if (q.next()) {
    if (!q.isNull("ip_id")) {
      QSqlField field = m_tableData->getProperties("ip_id");
      setDataField(field, q.value("ip_id"));
    }
  }

  openSuccessMessage(tr("Data saved successfully!"));
  setResetModified(inputFields);
  return true;
}

const QHash<QString, QVariant> StitchesEditor::createSqlDataset() {
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

void StitchesEditor::createSqlUpdate() {
  int articleId = ip_id->getValue().toInt();
  if (articleId < 0) {
    openNoticeMessage(tr("Missing Article ID for Update."));
    return;
  }
  // UPDATE Anforderungen
  ip_id->setRequired(true);

  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 1)
    return;

  QStringList set;
  QHash<QString, QVariant>::iterator it;
  int changes = 0;
  for (it = data.begin(); it != data.end(); ++it) {
    if (it.key() == "ip_id")
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
  int cur_count = ip_count->getValue().toInt();
  int old_count = m_tableData->getValue("ip_count").toInt();
  if (old_count != cur_count && cur_count == 0) {
    // Den Buchdaten Zwischenspeicher anpassen damit das Signal
    // an die Dienstleister nur einmal gesendet wird!
    m_tableData->setValue("ip_count", cur_count);
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

  QString sql("UPDATE inventory_prints SET ");
  sql.append(set.join(","));
  sql.append(",ip_changed=CURRENT_TIMESTAMP WHERE ip_id=");
  sql.append(ip_id->getValue().toString());
  sql.append(";");
  if (sendSqlQuery(sql)) {
    qInfo("SQL UPDATE Inventory Stitches success!");
    setWindowModified(false);
  }
}

void StitchesEditor::createSqlInsert() {
  int articleId = ip_id->getValue().toInt();
  if (articleId >= 1) {
    qWarning("Skip INSERT, switch to UPDATE with (ArticleID > 0)!");
    createSqlUpdate();
    return;
  }
  // Bei einem INSERT die Anforderungen anpassen.
  ip_count->setRequired(true);
  ip_id->setRequired(false);

  // Die Initialisierung erfolgt in setInputFields!
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
    // druck einfügen
    m_tableData->setValue(field, it.value());

    column.append(field);
    if (m_tableData->getType(field).id() == QMetaType::QString) {
      values.append("'" + it.value().toString() + "'");
    } else {
      values.append(it.value().toString());
    }
  }

  QString sql("INSERT INTO inventory_prints (");
  sql.append(column.join(","));
  sql.append(",ip_changed) VALUES (");
  sql.append(values.join(","));
  sql.append(",CURRENT_TIMESTAMP) RETURNING ip_id;");
  if (sendSqlQuery(sql) && ip_id->getValue().toInt() >= 1) {
    qInfo("SQL INSERT Inventory Prints & Stitches success!");
    // Zurücksetzen Knopf Aktivieren?
    m_actionBar->setRestoreable(m_tableData->isValid());
    // Bildaktionen erst bei vorhandener Artikel Nummer freischalten!
    m_imageToolBar->setArticleId(ip_id->getValue().toInt());
    ip_id->setRequired(true);
    setWindowModified(false);
  }
}

bool StitchesEditor::realyDeactivateEntry() {
  qint8 _curCount = ip_count->getValue().toInt();
  qint8 _oldcount = m_tableData->getValue("ip_count").toInt();
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

  int ret =
      QMessageBox::question(this, tr("Entry deactivation"), body.join(""));
  if (ret == QMessageBox::No) {
    ip_count->setValue(m_tableData->getValue("ip_count"));
    ip_count->setRequired(true);
    return false;
  }

  ip_count->setRequired(false);
  return true;
}

void StitchesEditor::setSaveData() {
  if (ip_id->getValue().toString().isEmpty()) {
    createSqlInsert();
    return;
  }

  if (ip_count->getValue().toInt() == 0 && !realyDeactivateEntry())
    return;

  createSqlUpdate();
}

void StitchesEditor::setCheckLeaveEditor() {
  if (checkIsModified()) {
    unsavedChangesPopup();
    return;
  }
  setFinalLeaveEditor();
}

void StitchesEditor::setFinalLeaveEditor(bool force) {
  if (force) // Wenn auf Abbrechen geklickt wurde!
    setWindowModified(false);

  setResetInputFields();
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  m_thumbnail->clear();               /**< Bildvorschau leeren */
  emit sendLeaveEditor();             /**< Back to MainView */
}

void StitchesEditor::setStorageCompartments() {
  QStringList _list = ip_storage->getCompartments();
  if (_list.size() > 0)
    ip_storage_compartment->setCompleterList(_list);

  _list.clear();
}

void StitchesEditor::setPrintBookCard() {
  qint64 _aid = getDataValue("ip_id").toLongLong();
  if (_aid < 1) {
    pushStatusMessage(tr("Missing valid Article Id!"));
    return;
  }

  QJsonObject _config = ip_storage->getBookcardData();
  _config.insert("aid", _aid);
  _config.insert("basename", AntiquaCRM::AUtil::zerofill(_aid));
  _config.insert("title", getDataValue("ip_title").toString());
  _config.insert("year", getDataValue("ip_year").toString());
  _config.insert("author", getDataValue("ip_author").toString());

  QUrl _qr_url;
  m_cfg->beginGroup("printer");
  _qr_url.setUrl(m_cfg->value("qrcode_url").toString());

  QString _query(m_cfg->value("qrcode_query").toString());
  _query.append("=");
  _query.append(_config.value("aid").toString());
  _qr_url.setQuery(_query);

  _config.insert("qrquery", _qr_url.toString());
  m_cfg->endGroup();

  QString _buffer = getDataValue("ip_storage_compartment").toString();
  _config.insert("compartment", _buffer.trimmed());
  _buffer.clear();

  _buffer = getDataValue("ip_changed")
                .toDate()
                .toString(ANTIQUACRM_SHORT_DATE_DISPLAY);
  _config.insert("changed", _buffer.trimmed());
  _buffer.clear();

  AntiquaCRM::PrintBookCard *m_d = new AntiquaCRM::PrintBookCard(this);
  if (m_d->exec(_config) == QDialog::Accepted) {
    pushStatusMessage(tr("Card print successfully."));
  } else {
    pushStatusMessage(tr("Card print canceled."));
  }
}

void StitchesEditor::setLoadThumbnail(qint64 articleId) {
  if (articleId < 1)
    return;

  m_imageToolBar->setArticleId(articleId);

  AntiquaCRM::ImageFileSource image_preview;
  if (image_preview.findInDatabase(m_sql, articleId))
    m_thumbnail->setPixmap(image_preview.getThumbnail());
}

void StitchesEditor::setRemoveThumbnail(qint64 articleId) {
  qint64 _id = ip_id->getValue().toLongLong();
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
      pushStatusMessage(tr("Image delete successfully!"));
    }
  }
}

void StitchesEditor::setImportEditImage() {
  qint64 _id = ip_id->getValue().toLongLong();
  if (_id < 1) {
    pushStatusMessage(tr("No Article number for Image import!"));
    return;
  }

  AntiquaCRM::ImageImportDialog *d =
      new AntiquaCRM::ImageImportDialog(_id, "Stitches", this);
  connect(d, SIGNAL(sendThumbnail(const QPixmap &)), m_thumbnail,
          SLOT(setPixmap(const QPixmap &)));

  d->exec();
  d->deleteLater();
}

void StitchesEditor::setRestore() {
  importSqlResult();
  ip_count->setFocus();
}

bool StitchesEditor::openEditEntry(qint64 articleId) {
  bool status = false;
  if (articleId < 1)
    return status;

  QString ip_id = QString::number(articleId);
  if (ip_id.isEmpty())
    return status;

  setInputFields();
  QString table = m_tableData->tableName();
  QString query("SELECT * FROM " + table + " WHERE ip_id=" + ip_id + ";");
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

bool StitchesEditor::createNewEntry() {
  setInputFields();
  m_thumbnail->clear();
  foreach (QString column, m_tableData->columnNames()) {
    QSqlField field = m_tableData->getProperties(column);
    if (column == "ip_id")
      field.setRequired(false);

    setDefaultInput(field);
  }
  setResetModified(inputFields);
  return isEnabled();
}

bool StitchesEditor::createCustomEntry(const QJsonObject &object) {
  qDebug() << Q_FUNC_INFO << "TODO" << object;
  return true;
}
