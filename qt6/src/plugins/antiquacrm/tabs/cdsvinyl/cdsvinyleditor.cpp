// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cdsvinylconfig.h"
#include "cdsvinyleditor.h"

#include <AntiquaCRM>
#include <AntiquaPrinting>
#include <QDate>
#include <QLayout>

CDsVinylEditor::CDsVinylEditor(QWidget *parent)
    : AntiquaCRM::TabsEditor{CDSVINYL_SQL_EDITOR_PATTERN, parent} {
  setWindowTitle(tr("Edit entry"));
  setObjectName("tab_cdsvinyl_editor");

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("printseditor_main_layout");
  mainLayout->setSizeConstraint(QLayout::SetMaximumSize);

  QString tempWhatsThis;
  // AntiquaCRM::ALabel::Align _lbAlign = AntiquaCRM::ALabel::Align::MiddleRight;

  // Row 0
  QHBoxLayout *row0 = new QHBoxLayout();
  cv_id = new AntiquaCRM::SerialId(this);
  cv_id->setObjectName("cv_id");
  cv_id->setBuddyLabel(tr("Article ID"));
  tempWhatsThis = tr("__TODO__");
  cv_id->setWhatsThisText(tempWhatsThis);
  row0->addWidget(cv_id);

  cv_count = new AntiquaCRM::CrowdEdit(this);
  cv_count->setObjectName("cv_count");
  cv_count->setBuddyLabel(tr("Count"));
  tempWhatsThis = tr("__TODO__");
  cv_count->setWhatsThisText(tempWhatsThis);
  row0->addWidget(cv_count);

  row0->addStretch(1);
  mainLayout->addLayout(row0, 0);

  // Row 1
  m_splitter = new AntiquaCRM::Splitter(this);
  QWidget *row2Widget = new QWidget(m_splitter);
  row2Widget->setContentsMargins(0, 0, 0, 0);
  int row2c = 0;
  QGridLayout *row2 = new QGridLayout(row2Widget);
  row2->setContentsMargins(0, 0, 0, 0);

  row2->setRowStretch(row2c++, 1);

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

  mainLayout->addWidget(m_tabWidget);

  m_actionBar = new AntiquaCRM::TabsEditActionBar(this);
  m_actionBar->setViewMailButton(false);
  mainLayout->addWidget(m_actionBar);

  setLayout(mainLayout);

  // Signals:ActionBar
  connect(m_actionBar, SIGNAL(sendCancelClicked()),
          SLOT(setFinalLeaveEditor()));
  connect(m_actionBar, SIGNAL(sendRestoreClicked()), SLOT(setRestore()));
  connect(m_actionBar, SIGNAL(sendSaveClicked()), SLOT(setSaveData()));
  connect(m_actionBar, SIGNAL(sendFinishClicked()),
          SLOT(setCheckLeaveEditor()));
}

CDsVinylEditor::~CDsVinylEditor() {
  ignoreFields.clear();
  inputFields.clear();
  if (m_sql != nullptr)
    m_sql->deleteLater();

  if (m_cfg != nullptr)
    m_cfg->deleteLater();
}

void CDsVinylEditor::setInputFields() {
  // Bei UPDATE/INSERT Ignorieren
  ignoreFields << "cv_since";
  ignoreFields << "cv_changed";

  // Settings input defaults
  // const QJsonObject _jobj = loadSqlConfig(CDSVINYL_CONFIG_POINTER);
  // double _price_lowest = _jobj.value("media_price_lowest").toDouble();
  // double _price_default = _jobj.value("media_price_lowest").toDouble();
  //if (_price_lowest > 1.0)
  //  cv_price->setMinimum(_price_lowest);

  //if (_price_default > 2.0)
  //  cv_price->setValue(_price_default);

  m_tableData = new AntiquaCRM::ASqlDataQuery("inventory_prints");
  inputFields = m_tableData->columnNames();
  if (inputFields.isEmpty()) {
    QStringList warn(tr("An error has occurred!"));
    warn << tr("Can't load input datafields!");
    warn << tr("When getting this Message, please check your Network and "
               "Database connection!");
    openNoticeMessage(warn.join("\n"));
  }

  AntiquaCRM::ASharedDataFiles _dataFiles;
  QStringList _completer_data;
  // TODO
  _completer_data.clear();
}

bool CDsVinylEditor::setDataField(const QSqlField &field,
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

void CDsVinylEditor::importSqlResult() {
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
  qint64 id = cv_id->getValue().toLongLong();
  if (id > 0)
    m_imageToolBar->setArticleId(id);

  m_actionBar->setRestoreable(m_tableData->isValid());
  setResetModified(inputFields);
}

bool CDsVinylEditor::sendSqlQuery(const QString &query) {
  //  qDebug() << Q_FUNC_INFO << query;
  //  return true;
  QSqlQuery q = m_sql->query(query);
  if (q.lastError().type() != QSqlError::NoError) {
    qDebug() << Q_FUNC_INFO << query << m_sql->lastError();
    return false;
  }

  if (q.next()) {
    if (!q.isNull("cv_id")) {
      QSqlField field = m_tableData->getProperties("cv_id");
      setDataField(field, q.value("cv_id"));
    }
  }

  openSuccessMessage(tr("Data saved successfully!"));
  setResetModified(inputFields);
  return true;
}

const QHash<QString, QVariant> CDsVinylEditor::createSqlDataset() {
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

void CDsVinylEditor::createSqlUpdate() {
  int articleId = cv_id->getValue().toInt();
  if (articleId < 0) {
    openNoticeMessage(tr("Missing Article ID for Update."));
    return;
  }
  // UPDATE Anforderungen
  cv_id->setRequired(true);

  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 1)
    return;

  QStringList set;
  QHash<QString, QVariant>::iterator it;
  int changes = 0;
  for (it = data.begin(); it != data.end(); ++it) {
    if (it.key() == "cv_id")
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
  int cur_count = cv_count->getValue().toInt();
  int old_count = m_tableData->getValue("cv_count").toInt();
  if (old_count != cur_count && cur_count == 0) {
    // Den Buchdaten Zwischenspeicher anpassen damit das Signal
    // an die Dienstleister nur einmal gesendet wird!
    m_tableData->setValue("cv_count", cur_count);
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
  sql.append(",cv_changed=CURRENT_TIMESTAMP WHERE cv_id=");
  sql.append(cv_id->getValue().toString());
  sql.append(";");
  if (sendSqlQuery(sql)) {
    qInfo("SQL UPDATE Inventory Stitches success!");
    setWindowModified(false);
  }
}

void CDsVinylEditor::createSqlInsert() {
  int articleId = cv_id->getValue().toInt();
  if (articleId >= 1) {
    qWarning("Skip INSERT, switch to UPDATE with (ArticleID > 0)!");
    createSqlUpdate();
    return;
  }
  // Bei einem INSERT die Anforderungen anpassen.
  cv_count->setRequired(true);
  cv_id->setRequired(false);

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
  sql.append(",cv_changed) VALUES (");
  sql.append(values.join(","));
  sql.append(",CURRENT_TIMESTAMP) RETURNING cv_id;");
  if (sendSqlQuery(sql) && cv_id->getValue().toInt() >= 1) {
    qInfo("SQL INSERT Inventory Prints & Stitches success!");
    // Zurücksetzen Knopf Aktivieren?
    m_actionBar->setRestoreable(m_tableData->isValid());
    // Bildaktionen erst bei vorhandener Artikel Nummer freischalten!
    m_imageToolBar->setArticleId(cv_id->getValue().toInt());
    cv_id->setRequired(true);
    setWindowModified(false);
  }
}

void CDsVinylEditor::setSaveData() {
  if (cv_id->getValue().toString().isEmpty()) {
    createSqlInsert();
    return;
  }

  // if (cv_count->getValue().toInt() == 0 && !realyDeactivateEntry())

  createSqlUpdate();
}

void CDsVinylEditor::setCheckLeaveEditor() {
  if (checkIsModified()) {
    unsavedChangesPopup();
    return;
  }
  setFinalLeaveEditor();
}

void CDsVinylEditor::setFinalLeaveEditor(bool force) {
  if (force) // Wenn auf Abbrechen geklickt wurde!
    setWindowModified(false);

  setResetInputFields();
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  m_thumbnail->clear();               /**< Bildvorschau leeren */
  emit sendLeaveEditor();             /**< Back to MainView */
}

void CDsVinylEditor::setStorageCompartments() {
  // TODO
}

void CDsVinylEditor::setLoadThumbnail(qint64 articleId) {
  if (articleId < 1)
    return;

  m_imageToolBar->setArticleId(articleId);

  AntiquaCRM::ImageFileSource image_preview;
  if (image_preview.findInDatabase(m_sql, articleId))
    m_thumbnail->setPixmap(image_preview.getThumbnail());
}

void CDsVinylEditor::setRemoveThumbnail(qint64 articleId) {
  qint64 _id = cv_id->getValue().toLongLong();
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

void CDsVinylEditor::setImportEditImage() {
  qint64 _id = cv_id->getValue().toLongLong();
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

void CDsVinylEditor::setRestore() {
  importSqlResult();
  cv_count->setFocus();
}

bool CDsVinylEditor::openEditEntry(qint64 articleId) {
  bool status = false;
  if (articleId < 1)
    return status;

  QString cv_id = QString::number(articleId);
  if (cv_id.isEmpty())
    return status;

  setInputFields();
  QString table = m_tableData->tableName();
  QString query("SELECT * FROM " + table + " WHERE cv_id=" + cv_id + ";");
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
    registerInputChanged();
  }

  return status;
}

bool CDsVinylEditor::createNewEntry() {
  setInputFields();
  m_thumbnail->clear();
  foreach (QString column, m_tableData->columnNames()) {
    QSqlField field = m_tableData->getProperties(column);
    if (column == "cv_id")
      field.setRequired(false);

    setDefaultInput(field);
  }
  setResetModified(inputFields);
  registerInputChanged();
  return isEnabled();
}
