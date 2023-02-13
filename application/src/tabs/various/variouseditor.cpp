// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "variouseditor.h"

#include <QMessageBox>

VariousEditor::VariousEditor(QWidget *parent)
    : InventoryEditor{"^va_[a-z_]+\\b$", parent} {
  setObjectName("various_editor");
  setWindowTitle(tr("Edit Various"));
  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("various_editor_layout");
  mainLayout->setSizeConstraint(QLayout::SetMaximumSize);
  // Zellenbeschreibung
  AntiquaILabel *infolabel;

  // BEGIN : Row0
  QHBoxLayout *row0 = new QHBoxLayout();
  va_id = new SerialID(this);
  va_id->setObjectName("va_id");
  va_id->setInfo(tr("Article ID"));
  va_id->setRequired(true);
  row0->addWidget(va_id);
  // va_count
  va_count = new IntSpinBox(this);
  va_count->setObjectName("va_count");
  va_count->setInfo(tr("Count"));
  row0->addWidget(va_count);
  // va_price
  double minPrice = m_cfg->value("payment/min_price_media", 1.00).toDouble();
  va_price = new PriceEdit(this);
  va_price->setObjectName("va_price");
  va_price->setRequired(true);
  va_price->setInfo(tr("Price"));
  va_price->setMinimum(minPrice);
  row0->addWidget(va_price);
  // va_year
  va_year = new YearEdit(this);
  va_year->setObjectName("va_year");
  va_year->setRequired(true);
  va_year->setInfo(tr("Year"));
  va_year->setValue(1800);
  row0->addWidget(va_year);
  // va_including_vat
  va_including_vat = new BoolBox(this);
  va_including_vat->setObjectName("va_including_vat");
  va_including_vat->setInfo(tr("incl. vat"));
  va_including_vat->setRequired(false);
  row0->addWidget(va_including_vat);
  // va_restricted
  va_restricted = new BoolBox(this);
  va_restricted->setObjectName("va_restricted");
  va_restricted->setInfo(tr("Local Usage only"));
  va_restricted->setToolTip(tr("When this Options is marked. Then this Article "
                               "will not exported to your Providers."));
  row0->addWidget(va_restricted);
  row0->addStretch(1);
  mainLayout->addLayout(row0, 0);
  // END : Row0
  // BEGIN : Row1
  m_splitter = new AntiquaSplitter(this);
  // Eingabemasken
  QWidget *row1Widget = new QWidget(m_splitter);
  row1Widget->setContentsMargins(0, 0, 0, 0);
  int row1c = 0;
  QGridLayout *row1 = new QGridLayout(row1Widget);
  row1->setContentsMargins(0, 0, 0, 0);
  row1->setColumnStretch(1, 1);
  // va_title
  infolabel = new AntiquaILabel(tr("Title"), row1Widget);
  row1->addWidget(infolabel, row1c, 0, 1, 1);
  va_title = new LineEdit(this);
  va_title->setObjectName("va_title");
  va_title->setToolTip(infolabel->text());
  row1->addWidget(va_title, row1c++, 1, 1, 1);
  // va_title_extended
  infolabel = new AntiquaILabel(tr("Extended"), row1Widget);
  row1->addWidget(infolabel, row1c, 0, 1, 1);
  va_title_extended = new LineEdit(this);
  va_title_extended->setObjectName("va_title_extended");
  va_title_extended->setToolTip(infolabel->text());
  row1->addWidget(va_title_extended, row1c++, 1, 1, 1);
  // va_designation
  infolabel = new AntiquaILabel(tr("Designation"), row1Widget);
  row1->addWidget(infolabel, row1c, 0, 1, 1);
  va_designation = new LineEdit(this);
  va_designation->setObjectName("va_designation");
  va_designation->setToolTip(infolabel->text());
  row1->addWidget(va_designation, row1c++, 1, 1, 1);
  // va_condition
  infolabel = new AntiquaILabel(tr("Condition"), row1Widget);
  row1->addWidget(infolabel, row1c, 0, 1, 1);
  va_condition = new ConditionEdit(this);
  va_condition->setObjectName("va_condition");
  va_condition->setToolTip(infolabel->toolTip());
  row1->addWidget(va_condition, row1c++, 1, 1, 1);
  // va_storage
  infolabel = new AntiquaILabel(tr("Storage"), row1Widget);
  row1->addWidget(infolabel, row1c, 0, 1, 1);
  va_storage = new StorageLocation(this);
  va_storage->setObjectName("va_storage");
  va_storage->setToolTip(infolabel->text());
  row1->addWidget(va_storage, row1c++, 1, 1, 1);
  // image toolbar
  row1->setRowStretch(row1c++, 1);
  m_imageToolBar = new ImageToolBar(this);
  row1->addWidget(m_imageToolBar, row1c++, 1, 1, 1);
  row1Widget->setLayout(row1);
  m_splitter->addLeft(row1Widget);
  // Image Viewer
  m_imageView = new ImageView(QSize(400, 400), m_splitter);
  m_splitter->addRight(m_imageView);
  mainLayout->addWidget(m_splitter, 1);
  mainLayout->setStretch(1, 1);
  // END : Row1
  // BEGIN : Row2
  QIcon tabIcons = m_tabWidget->defaultIcon();
  m_tabWidget = new EditorTab(this);
  m_tabWidget->setObjectName("tab_widget");
  // va_description
  va_description = new TextField(this);
  va_description->setObjectName("va_description");
  m_tabWidget->insertTab(0, va_description, tabIcons, tr("Description"));
  // va_internal_description
  va_internal_description = new TextField(this);
  va_internal_description->setObjectName("va_internal_description");
  m_tabWidget->insertTab(1, va_internal_description, tabIcons,
                         tr("Internal Description"));
  // Info Tab
  QWidget *m_infos = new QWidget(this);
  QVBoxLayout *m_infoLayout = new QVBoxLayout(m_infos);
  // va_since
  va_since = new AntiquaDateInfo(this);
  va_since->setObjectName("va_since");
  va_since->setInfo(tr("Created at"));
  m_infoLayout->addWidget(va_since);
  // va_changed
  va_changed = new AntiquaDateInfo(this);
  va_changed->setObjectName("va_changed");
  va_changed->setInfo(tr("Last changed"));
  m_infoLayout->addWidget(va_changed);
  m_infoLayout->addStretch(1);
  m_infos->setLayout(m_infoLayout);
  m_tabWidget->insertTab(2, m_infos, tabIcons, tr("Information"));
  mainLayout->addWidget(m_tabWidget);
  // END : Row2
  // Begin : Row3
  m_actionBar = new EditorActionBar(this);
  mainLayout->addWidget(m_actionBar);
  // END : Row3

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
}

void VariousEditor::setInputFields() {
  // Bei UPDATE/INSERT Ignorieren
  ignoreFields << "va_since";
  ignoreFields << "va_changed";
  m_tableData = new AntiquaCRM::ASqlDataQuery("inventory_various");
  inputFields = m_tableData->columnNames();
  if (inputFields.isEmpty()) {
    QStringList warn(tr("An error has occurred!"));
    warn << tr("Can't load input datafields!");
    warn << tr("When getting this Message, please check your Network and "
               "Database connection!");
    openNoticeMessage(warn.join("\n"));
  }
  // Lager
  va_storage->reset();
  va_storage->loadDataset();
}

bool VariousEditor::setDataField(const QSqlField &field,
                                 const QVariant &value) {
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
  qint64 id = va_id->value().toLongLong();
  if (id > 0) {
    m_imageToolBar->setArticleId(id);
    m_imageToolBar->setActive(true);
    m_imageView->readFromDatabase(id);
  }
  m_actionBar->setRestoreable(m_tableData->isValid());
  setResetModified(inputFields);
}

bool VariousEditor::sendSqlQuery(const QString &query) {
  // qDebug() << Q_FUNC_INFO << query; return true;
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
  openSuccessMessage(tr("Saved successfully!"));
  setResetModified(inputFields);
  return true;
}

const QHash<QString, QVariant> VariousEditor::createSqlDataset() {
  QHash<QString, QVariant> data;
  QList<InputEdit *> list =
      findChildren<InputEdit *>(fieldPattern, Qt::FindChildrenRecursively);
  QList<InputEdit *>::Iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    InputEdit *cur = *it;
    QString objName = cur->objectName();
    if (ignoreFields.contains(objName))
      continue;

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

void VariousEditor::createSqlUpdate() {
  int articleId = va_id->value().toInt();
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
  int cur_count = va_count->value().toInt();
  int old_count = m_tableData->getValue("va_count").toInt();
  if (old_count != cur_count && cur_count == 0) {
    // Den Buchdaten Zwischenspeicher anpassen damit das Signal
    // an die Dienstleister nur einmal gesendet wird!
    m_tableData->setValue("va_count", cur_count);
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
  QString sql("UPDATE inventory_various SET ");
  sql.append(set.join(","));
  sql.append(",va_changed=CURRENT_TIMESTAMP WHERE va_id=");
  sql.append(va_id->value().toString());
  sql.append(";");
  if (sendSqlQuery(sql)) {
    qInfo("SQL Various UPDATE success!");
  }
}

void VariousEditor::createSqlInsert() {
  int articleId = va_id->value().toInt();
  if (articleId >= 1) {
    qWarning("Skip INSERT, switch to UPDATE with (ArticleID > 0)!");
    createSqlUpdate();
    return;
  }
  // Bei einem INSERT die Anforderungen anpassen.
  va_count->setRequired(true);
  va_id->setRequired(false);

  // Prüfung der Daten Klasse
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
    // Daten einfügen
    m_tableData->setValue(field, it.value());

    column.append(field);
    if (m_tableData->getType(field).id() == QMetaType::QString) {
      values.append("'" + it.value().toString() + "'");
    } else {
      values.append(it.value().toString());
    }
  }

  QString sql("INSERT INTO inventory_various (");
  sql.append(column.join(","));
  sql.append(",va_changed) VALUES (");
  sql.append(values.join(","));
  sql.append(",CURRENT_TIMESTAMP) RETURNING va_id;");
  if (sendSqlQuery(sql) && va_id->value().toInt() >= 1) {
    qInfo("Insert Inventory success!");
    // Zurücksetzen Knopf Aktivieren?
    m_actionBar->setRestoreable(m_tableData->isValid());
    // Bildaktionen erst bei vorhandener Artikel Nummer freischalten!
    m_imageToolBar->setActive(true);
    va_id->setRequired(true);
  }
}

bool VariousEditor::realyDeactivateEntry() {
  qint8 _curCount = va_count->value().toInt();
  qint8 _oldcount = m_tableData->getValue("va_count").toInt();
  if (_curCount == _oldcount)
    return true; // alles ok

  QStringList body("<b>");
  body << tr("When setting the Article Count to 0.");
  body << QString("</b><p>");
  body << tr("This marked the Article in all Shopsystem for deletion!");
  body << QString("</p><p>");
  body << tr("Are you sure to finish this operation?");
  body << QString("</p>");

  int ret = QMessageBox::question(this, tr("Deactivation"), body.join(""));
  if (ret == QMessageBox::No) {
    va_count->setValue(m_tableData->getValue("va_count"));
    va_count->setRequired(true);
    return false;
  }
  va_count->setRequired(false);
  return true;
}

void VariousEditor::setSaveData() {
  if (va_id->value().toString().isEmpty()) {
    createSqlInsert();
    return;
  }

  if (va_count->value().toInt() == 0 && !realyDeactivateEntry())
    return;

  createSqlUpdate();
}

void VariousEditor::setCheckLeaveEditor() {
  if (checkIsModified()) {
    unsavedChangesPopup();
    return;
  }
  setFinalLeaveEditor(false);
}

void VariousEditor::setFinalLeaveEditor(bool force) {
  if(force) // Wenn auf Abbrechen geklickt wurde!
    setWindowModified(false);

  setResetInputFields();
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  m_imageView->clear();               /**< Bildvorschau leeren */
  emit sendLeaveEditor();             /**< Back to MainView */
}

void VariousEditor::actionRemoveImage(qint64 articleId) {
  qint64 id = va_id->value().toLongLong();
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

void VariousEditor::actionEditImages() {
  qint64 id = va_id->value().toLongLong();
  if (id < 1)
    return;

  ImageDialog *d = new ImageDialog(id, this);
  d->setSubCategory("Various");
  if (d->exec())
    m_imageView->readFromDatabase(id);
}

void VariousEditor::setRestore() {
  importSqlResult();
  va_count->setFocus();
}

bool VariousEditor::openEditEntry(qint64 articleId) {
  bool status = false;
  if (articleId < 1)
    return status;

  QString va_id = QString::number(articleId);
  if (va_id.isEmpty())
    return status;

  setInputFields();
  QString table = m_tableData->tableName();
  QString query("SELECT * FROM " + table + " WHERE va_id=" + va_id + ";");
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
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
    status = false;
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
    if (column == "va_id")
      field.setRequired(false);

    setDefaultInput(field);
  }
  setResetModified(inputFields);
  return isEnabled();
}
