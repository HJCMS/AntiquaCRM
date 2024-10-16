// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "stitcheseditor.h"

#include <AntiquaCRM>
#include <AntiquaImageDialog>
#include <AntiquaPrinting>
#include <QDate>
#include <QLayout>

StitchesEditor::StitchesEditor(QWidget* parent)
    : AntiquaCRM::TabsEditor{STITCHES_SQL_EDITOR_PATTERN, parent} {
  setWindowTitle(tr("Edit entry"));
  setObjectName("tab_stitches_editor");

  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("printseditor_main_layout");
  mainLayout->setSizeConstraint(QLayout::SetMaximumSize);

  QString tempWhatsThis;
  AntiquaCRM::ALabel::Align _lbAlign = AntiquaCRM::ALabel::Align::MiddleRight;

  // Row 0
  QHBoxLayout* row0 = new QHBoxLayout();
  ip_id = new AntiquaCRM::SerialId(this);
  ip_id->setObjectName("ip_id");
  ip_id->setBuddyLabel(tr("Article ID"));
  tempWhatsThis =
      tr("Probably the most important parameter for the article "
         "processing and is generated automatically when creating "
         "a entry. AntiquaCRM use a running numbering system.");
  ip_id->setWhatsThisText(tempWhatsThis);
  row0->addWidget(ip_id);

  ip_count = new AntiquaCRM::CrowdEdit(this);
  ip_count->setObjectName("ip_count");
  ip_count->setBuddyLabel(tr("Count"));
  tempWhatsThis =
      tr("The Article count is the main value for activating and deactivating "
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
  tempWhatsThis =
      tr("In painting, color means the selection, harmony, shading "
         "and composition of the different colors in a painting.");
  ip_kolorit->setWhatsThisText(tempWhatsThis);
  row0->addWidget(ip_kolorit);

  ip_landscape = new AntiquaCRM::BoolBox(this);
  ip_landscape->setObjectName("ip_landscape");
  ip_landscape->setBuddyLabel(tr("Landscape"));
  tempWhatsThis = tr("Is it a portrait or landscape format?");
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
  QWidget* row2Widget = new QWidget(m_splitter);
  row2Widget->setContentsMargins(0, 0, 0, 0);
  int row2c = 0;
  QGridLayout* row2 = new QGridLayout(row2Widget);
  row2->setContentsMargins(0, 0, 0, 0);

  AntiquaCRM::ALabel* infoText;
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

  infoText = new AntiquaCRM::ALabel(tr("Authors"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ip_author = new AntiquaCRM::TextLine(this);
  ip_author->setObjectName("ip_author");
  ip_author->setInputToolTip(tr("Authors"));
  tempWhatsThis =
      tr("This field is reserved for Authors.\nYou can add more "
         "separated by comma.\nSome provider Platforms supporting "
         "spezial Keywords.\nAntiquaCRM suggests the most common "
         "author group names, when editing this field.");
  ip_author->setWhatsThisText(tempWhatsThis);
  row2->addWidget(ip_author, row2c++, 1, 1, 1);

  // Begin:Format+Condition
  infoText = new AntiquaCRM::ALabel(tr("Format"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  QHBoxLayout* formatLayout = new QHBoxLayout;
  ip_format = new AntiquaCRM::TextLine(this);
  ip_format->setObjectName("ip_format");
  ip_format->setInputToolTip(infoText->text());
  tempWhatsThis =
      tr("The format information depends on the metric unit system of the "
         "local user. Images have standard formats which are defined "
         "elsewhere. Mainly dimensions are given here.");
  ip_format->setWhatsThisText(tempWhatsThis);
  ip_format->appendStretch();
  formatLayout->addWidget(ip_format);
  QString ip_condition_label(tr("Condition"));
  ip_condition = new AntiquaCRM::ConditionEdit(this);
  ip_condition->setObjectName("ip_condition");
  ip_condition->setInputToolTip(ip_condition_label);
  ip_condition->setWhatsThisText(ip_condition->defaultWhatsThis());
  formatLayout->addWidget(new AntiquaCRM::ALabel(ip_condition_label, _lbAlign, this));
  formatLayout->addWidget(ip_condition);
  row2->addLayout(formatLayout, row2c++, 1, 1, 1);
  // End:Format+Condition

  // Begin:Storage
  infoText = new AntiquaCRM::ALabel(tr("Storage"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);

  QHBoxLayout* storageLayout = new QHBoxLayout;

  ip_storage = new AntiquaCRM::SelectStorage(this);
  ip_storage->setObjectName("ip_storage");
  ip_storage->setInputToolTip(tr("Storage location"));
  tempWhatsThis = tr("The Storage location where this %1 has been stored. "
                     "You need to Configure Storage locations first in your "
                     "Database Configuration Menu before you can use it.")
                      .arg(tr("Article"));
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
  tempWhatsThis =
      tr("Keywords will help Buyers, to find your Article on Provider pages. This "
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
  m_thumbnail = new AntiquaCRM::ImageThumbnail(this);
  m_thumbnail->setMinimumWidth(100);

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
  m_tabWidget->insertTab(1, ip_internal_description, tabIcons, tr("Internal Description"));
  // Info Tab
  QWidget* m_infos = new QWidget(m_tabWidget);
  QGridLayout* infoLayout = new QGridLayout(m_infos);
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
  connect(m_imageToolBar, SIGNAL(sendDeleteImage(qint64)), SLOT(setRemoveThumbnail(qint64)));
  connect(m_imageToolBar, SIGNAL(sendOpenImage()), SLOT(setImportEditImage()));

  // Signals::Storage
  connect(ip_storage, SIGNAL(sendValueChanged()), SLOT(setStorageCompartments()));

  // Signals:ActionBar
  connect(m_actionBar, SIGNAL(sendCancelClicked()), SLOT(setFinalLeaveEditor()));
  connect(m_actionBar, SIGNAL(sendRestoreClicked()), SLOT(setRestore()));
  connect(m_actionBar, SIGNAL(sendSaveClicked()), SLOT(setSaveData()));
  connect(m_actionBar, SIGNAL(sendFinishClicked()), SLOT(setCheckLeaveEditor()));
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
  ignoreFields << "ip_type";          // only for orders!
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

  // ip_condition

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

bool StitchesEditor::setDataField(const QSqlField& field, const QVariant& value) {
  if (!field.isValid())
    return false;

  QString key = field.name();
  // qDebug() << "setDataField:" << field.name() << value;
  bool required = (field.requiredStatus() == QSqlField::Required);
  AntiquaCRM::AInputWidget* inp =
      findChild<AntiquaCRM::AInputWidget*>(key, Qt::FindChildrenRecursively);
  if (inp != nullptr) {
    inp->setRestrictions(field);
    // Muss nach setRestrictions kommen!
    inp->setValue(value);
    return true;
  }

  if (ignoreFields.contains(key))
    return true;

  if (required) {
#ifdef ANTIQUA_DEVELOPMENT
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
  // qDebug() << Q_FUNC_INFO << inputFields;
  setResetModified(inputFields);
}

bool StitchesEditor::sendSqlQuery(const QString& query) {
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
  QHash<QString, QVariant> _data;
  QList<AntiquaCRM::AInputWidget*> _list =
      findChildren<AntiquaCRM::AInputWidget*>(fieldPattern, Qt::FindChildrenRecursively);
  QList<AntiquaCRM::AInputWidget*>::Iterator _it;
  for (_it = _list.begin(); _it != _list.end(); ++_it) {
    AntiquaCRM::AInputWidget* cur = *_it;
    const QString _objectName = cur->objectName();
    if (ignoreFields.contains(_objectName))
      continue;

    // qDebug() << _objectName << cur->isRequired() << cur->isValid() <<
    // cur->getValue();

    if (cur->isRequired() && !cur->isValid()) {
      openNoticeMessage(cur->popUpHints());
      cur->setFocus();
      _data.clear();
      return _data;
    }
    _data.insert(_objectName, cur->getValue());
  }
  _list.clear();

  return _data;
}

void StitchesEditor::createSqlUpdate() {
  int _aid = ip_id->getValue().toInt();
  if (_aid < 0) {
    openNoticeMessage(tr("Missing Article ID for Update."));
    return;
  }
  // UPDATE Anforderungen
  ip_id->setRequired(true);

  QHash<QString, QVariant> _data = createSqlDataset();
  if (_data.size() < 1)
    return;

  QStringList _set; // SQL Field SET
  QHash<QString, QVariant>::iterator _it;
  int _changes = 0; // count changes
  for (_it = _data.begin(); _it != _data.end(); ++_it) {
    if (_it.key() == "ip_id")
      continue; // ignore

    const QString _key = _it.key();
    // Compare with m_tableData and only include changed fields.
    if (!isModifiedCompare(_key, m_tableData->getValue(_key)))
      continue;

    if (m_tableData->getType(_key).id() == QMetaType::QString) {
      _set.append(_key + "='" + _it.value().toString() + "'");
      _changes++;
    } else {
      _set.append(_key + "=" + _it.value().toString());
      _changes++;
    }
  }

  // check change count
  if (_changes == 0) {
    pushStatusMessage(tr("No Modifications found, Update aborted!"));
    setWindowModified(false);
    return;
  }

  // Checking article for no activation!
  // If the number has changed, send an update!
  // NOTE The user query is done beforehand in setSaveData()!
  int cur_count = ip_count->getValue().toInt();
  int old_count = m_tableData->getValue("ip_count").toInt();
  if (old_count != cur_count && cur_count == 0) {
    // Adjust the data cache so that the signal is only sent to the service provider once!
    m_tableData->setValue("ip_count", cur_count);
    // From this point on, the reset is no longer valid!
    m_actionBar->setRestoreable(false);
    // Send inventory message to socket
    QJsonObject _jso_parent, _jso_child;
    _jso_child.insert("type", QJsonValue("article_update"));
    _jso_child.insert("articleId", QJsonValue(_aid));
    _jso_child.insert("count", QJsonValue(cur_count));
    _jso_parent.insert("plugin_operation", QJsonValue(_jso_child));
    pushPluginOperation(_jso_parent);
  }

  QString _sql("UPDATE inventory_prints SET ");
  _sql.append(_set.join(","));
  _sql.append(",ip_changed=CURRENT_TIMESTAMP WHERE ip_id=");
  _sql.append(ip_id->getValue().toString());
  _sql.append(";");
  if (sendSqlQuery(_sql)) {
    qInfo("SQL UPDATE Inventory Stitches success!");
    setWindowModified(false);
  }
}

void StitchesEditor::createSqlInsert() {
  if (ip_id->getValue().toInt() >= 1) {
    qWarning("Skip INSERT, switch to UPDATE with (ArticleID > 0)!");
    createSqlUpdate();
    return;
  }
  // Adjust the requirements for an INSERT.
  ip_count->setRequired(true);
  ip_id->setRequired(false);

  // The initialization of m_tableData takes place in function setInputFields!
  // We need to checking for Base data here.
  if (m_tableData == nullptr || !m_tableData->isValid()) {
    // If no Data in m_tableData, canceling on this point.
    qWarning("Invalid AntiquaCRM::ASqlDataQuery detected!");
    return;
  }

  QHash<QString, QVariant> _data = createSqlDataset();
  // preventing corrupted SQL data sets
  if (_data.size() < 1) {
    qWarning("Sql Dataset is empty, canceling SQL INSERT!");
    return;
  }

  QStringList _columns; // SQL Columns
  QStringList _values;  // SQL Values
  QHash<QString, QVariant>::iterator _it;
  for (_it = _data.begin(); _it != _data.end(); ++_it) {
    if (_it.value().isNull())
      continue; // nothing todo

    QString _field = _it.key();
    m_tableData->setValue(_field, _it.value());

    _columns.append(_field);

    // Checking MetaType for SQL string declarations.
    if (m_tableData->getType(_field).id() == QMetaType::QString) {
      _values.append("'" + _it.value().toString() + "'");
    } else {
      _values.append(_it.value().toString());
    }
  }

  QString _sql("INSERT INTO inventory_prints (");
  _sql.append(_columns.join(","));
  _sql.append(",ip_changed) VALUES (");
  _sql.append(_values.join(","));
  _sql.append(",CURRENT_TIMESTAMP) RETURNING ip_id;");
  if (sendSqlQuery(_sql) && ip_id->getValue().toInt() >= 1) {
    qInfo("SQL INSERT Inventory Prints & Stitches success!");
    // Reset button Activation?
    m_actionBar->setRestoreable(m_tableData->isValid());
    // Only activate image actions if the article number is available!
    m_imageToolBar->setArticleId(ip_id->getValue().toInt());
    ip_id->setRequired(true);
    setWindowModified(false);
  }
  // cleanup
  _columns.clear();
  _values.clear();
}

bool StitchesEditor::realyDeactivateEntry() {
  qint8 _curCount = ip_count->getValue().toInt();
  qint8 _oldcount = m_tableData->getValue("ip_count").toInt();
  if (_curCount == _oldcount)
    return true; // alles ok

  QStringList _l;
  _l << QString("<b>");
  _l << tr("When setting the Article Count to 0.");
  _l << QString("</b><p>");
  _l << tr("This marked the Article in all Shopsystem for deletion!");
  _l << QString("</p><p>");
  _l << tr("Are you sure to finish this operation?");
  _l << QString("</p>");

  int _ret = QMessageBox::question(this, tr("Entry deactivation"), _l.join(""));
  if (_ret == QMessageBox::No) {
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
  m_actionBar->setRestoreable(false); // ResetButton off
  m_thumbnail->clear();               // clear thumbnail
  emit sendLeaveEditor();             // Back to MainView
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

  _buffer = getDataValue("ip_changed").toDate().toString(ANTIQUACRM_SHORT_DATE_DISPLAY);
  _config.insert("changed", _buffer.trimmed());
  _buffer.clear();

  AntiquaCRM::PrintBookCard* m_card = new AntiquaCRM::PrintBookCard(this);
  if (m_card->exec(_config) == QDialog::Accepted) {
    pushStatusMessage(tr("Card print successfully."));
  } else {
    pushStatusMessage(tr("Card print canceled."));
  }
  m_card->deleteLater();
}

void StitchesEditor::setLoadThumbnail(qint64 articleId) {
  if (articleId < 1)
    return;

  m_imageToolBar->setArticleId(articleId);

  AntiquaCRM::ImageFileSource m_imgPreview;
  if (m_imgPreview.findInDatabase(m_sql, articleId))
    m_thumbnail->setPixmap(m_imgPreview.getThumbnail());
}

void StitchesEditor::setRemoveThumbnail(qint64 articleId) {
  qint64 _id = ip_id->getValue().toLongLong();
  if (articleId != _id) {
    qWarning("Remove Thumbnail ids not equal!");
    return;
  }

  QMessageBox::StandardButton _retval = QMessageBox::question(
      this, tr("Remove Image from Database"),
      tr("%1\n\nImage - Article Id: %2")
          .arg(tr("Do you really want to delete the Image?"), QString::number(_id)));
  if (_retval == QMessageBox::Yes) {
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

  QString _folder = AntiquaCRM::AUtil::ucFirst(STITCHES_INTERFACE_NAME);
  AntiquaCRM::ImageImportDialog* m_d = new AntiquaCRM::ImageImportDialog(_id, _folder, this);
  connect(m_d, SIGNAL(sendThumbnail(QPixmap)), m_thumbnail, SLOT(setPixmap(QPixmap)));
  m_d->exec();
  m_d->deleteLater();
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

bool StitchesEditor::createCustomEntry(const QJsonObject& object) {
  Q_UNUSED(object);
  qInfo("Unused function for this plugin, skipped!");
  return true;
}
