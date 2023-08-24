// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cdsvinyleditor.h"
#include "cdsvinylconfig.h"
#include "dialog/cdreaddialog.h"
#include "dialog/selectgenre.h"

#include <AntiquaCRM>
#include <AntiquaImageDialog>
#include <AntiquaPrinting>
#include <QDate>
#include <QLayout>
#include <QPixmap>

CDsVinylEditor::CDsVinylEditor(QWidget *parent)
    : AntiquaCRM::TabsEditor{CDSVINYL_SQL_EDITOR_PATTERN, parent} {
  setWindowTitle(tr("Edit entry"));
  setObjectName("tab_cdsvinyl_editor");

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("media_editor_main_layout");
  mainLayout->setSizeConstraint(QLayout::SetMaximumSize);

  QString tempWhatsThis;
  // Row 0
  QHBoxLayout *row0 = new QHBoxLayout();
  cv_id = new AntiquaCRM::SerialId(this);
  cv_id->setObjectName("cv_id");
  cv_id->setBuddyLabel(tr("Article ID"));
  row0->addWidget(cv_id);

  // cv_count
  cv_count = new AntiquaCRM::CrowdEdit(this);
  cv_count->setObjectName("cv_count");
  cv_count->setBuddyLabel(tr("Count"));
  tempWhatsThis = tr("Current article stock.");
  cv_count->setWhatsThisText(tempWhatsThis);
  row0->addWidget(cv_count);

  // cv_price
  double minPrice = m_cfg->value("payment/min_price_media", 1.00).toDouble();
  cv_price = new AntiquaCRM::PriceEdit(this);
  cv_price->setObjectName("cv_price");
  cv_price->setRequired(true);
  cv_price->setBuddyLabel(tr("Price"));
  cv_price->setMinimum(minPrice);
  tempWhatsThis = tr("Current price you are requesting for this item.");
  cv_price->setWhatsThisText(tempWhatsThis);
  row0->addWidget(cv_price);

  // cv_year
  cv_year = new AntiquaCRM::YearEdit(this);
  cv_year->setObjectName("cv_year");
  cv_year->setRequired(true);
  cv_year->setBuddyLabel(tr("Year"));
  cv_year->setValue(1800);
  tempWhatsThis = tr("The year of publication for this item.");
  cv_year->setWhatsThisText(tempWhatsThis);
  row0->addWidget(cv_year);

  // cv_restricted
  cv_restricted = new AntiquaCRM::BoolBox(this);
  cv_restricted->setObjectName("cv_restricted");
  cv_restricted->setBuddyLabel(tr("Local Usage only"));
  tempWhatsThis = tr("If the option is selected, the article will be listed on "
                     "your online portals. Please keep in mind that articles "
                     "are not automatically removed from your Providers, if "
                     "this option is subsequently deactivated!");
  cv_restricted->setWhatsThisText(tempWhatsThis);
  row0->addWidget(cv_restricted);

  row0->addStretch(1);
  mainLayout->addLayout(row0, 0);

  // Row 1
  AntiquaCRM::ALabel *infolabel;
  m_splitter = new AntiquaCRM::Splitter(this);
  QWidget *row2Widget = new QWidget(m_splitter);
  row2Widget->setContentsMargins(0, 0, 0, 0);
  int row2c = 0;
  QGridLayout *row2 = new QGridLayout(row2Widget);
  row2->setContentsMargins(0, 0, 0, 0);
  row2->setColumnStretch(1, 1);

  // cv_title
  infolabel = new AntiquaCRM::ALabel(tr("Title"), row2Widget);
  row2->addWidget(infolabel, row2c, 0, 1, 1);
  cv_title = new AntiquaCRM::TextLine(this);
  cv_title->setObjectName("cv_title");
  tempWhatsThis = tr("Insert the title for the media album here.");
  cv_title->setWhatsThisText(tempWhatsThis);
  row2->addWidget(cv_title, row2c++, 1, 1, 4);

  // cv_title_extended
  infolabel = new AntiquaCRM::ALabel(tr("Extended"), row2Widget);
  row2->addWidget(infolabel, row2c, 0, 1, 1);
  cv_title_extended = new AntiquaCRM::TextLine(this);
  cv_title_extended->setObjectName("cv_title_extended");
  tempWhatsThis =
      tr("The extended title gives you the option of including subtitles or "
         "additional information in the displayed service provider title.");
  cv_title_extended->setWhatsThisText(tempWhatsThis);
  row2->addWidget(cv_title_extended, row2c++, 1, 1, 4);

  // cv_author
  infolabel = new AntiquaCRM::ALabel(tr("Artists"), row2Widget);
  row2->addWidget(infolabel, row2c, 0, 1, 1);
  cv_author = new AntiquaCRM::TextLine(this);
  cv_author->setObjectName("cv_author");
  cv_author->setToolTip(infolabel->text());
  tempWhatsThis = tr("Typically the name of the artist or group.");
  cv_author->setWhatsThisText(tempWhatsThis);
  row2->addWidget(cv_author, row2c++, 1, 1, 4);

  // cv_publisher
  infolabel = new AntiquaCRM::ALabel(tr("Publisher"), row2Widget);
  row2->addWidget(infolabel, row2c, 0, 1, 1);
  cv_publisher = new AntiquaCRM::TextLine(this);
  cv_publisher->setObjectName("cv_publisher");
  cv_publisher->setToolTip(infolabel->text());
  tempWhatsThis =
      tr("In this field please indicate the publisher or the record label.");
  cv_publisher->setWhatsThisText(tempWhatsThis);
  row2->addWidget(cv_publisher, row2c++, 1, 1, 4);

  // @BEGIN_GROUP {
  // cv_mtype
  infolabel = new AntiquaCRM::ALabel(tr("Mediatype"), row2Widget);
  row2->addWidget(infolabel, row2c, 0, 1, 1);
  cv_mtype = new AntiquaCRM::SelectMediaType(this);
  cv_mtype->setObjectName("cv_mtype");
  cv_mtype->setToolTip(infolabel->text());
  row2->addWidget(cv_mtype, row2c, 1, 1, 1);
  // cv_condition
  infolabel = new AntiquaCRM::ALabel(tr("Condition"), row2Widget);
  infolabel->setToolTip(tr("Booklet or Cover condition"));
  row2->addWidget(infolabel, row2c, 2, 1, 1);
  cv_condition = new AntiquaCRM::ConditionEdit(this);
  cv_condition->setObjectName("cv_condition");
  cv_condition->setToolTip(infolabel->toolTip());
  tempWhatsThis = tr("The current condition for the medium such as CD or vinyl."
                     "\nNote: In the case of a CD, the plastic sleeve is not "
                     "included in the description.");
  cv_condition->setWhatsThisText(tempWhatsThis);
  row2->addWidget(cv_condition, row2c++, 3, 1, 1);
  // } @END_GROUP

  // cv_designation
  infolabel = new AntiquaCRM::ALabel(tr("Designation"), row2Widget);
  row2->addWidget(infolabel, row2c, 0, 1, 1);
  cv_designation = new AntiquaCRM::TextLine(this);
  cv_designation->setObjectName("cv_designation");
  cv_designation->setToolTip(infolabel->text());
  tempWhatsThis = tr("Description for this multimedia article.");
  cv_designation->setWhatsThisText(tempWhatsThis);
  row2->addWidget(cv_designation, row2c++, 1, 1, 4);

  // cv_storage
  infolabel = new AntiquaCRM::ALabel(tr("Storage"), row2Widget);
  row2->addWidget(infolabel, row2c, 0, 1, 1);
  cv_storage = new AntiquaCRM::SelectStorage(this);
  cv_storage->setObjectName("cv_storage");
  cv_storage->setToolTip(infolabel->text());
  row2->addWidget(cv_storage, row2c++, 1, 1, 4);

  // cv_keywords
  infolabel = new AntiquaCRM::ALabel(tr("Keywords"), row2Widget);
  row2->addWidget(infolabel, row2c, 0, 1, 1);
  cv_keyword = new AntiquaCRM::KeywordsEdit(this);
  cv_keyword->setObjectName("cv_keyword");
  cv_keyword->setToolTip(infolabel->text());
  row2->addWidget(cv_keyword, row2c++, 1, 1, 4);

  // cv_eangtin
  infolabel = new AntiquaCRM::ALabel(tr("Barcode"), row2Widget);
  row2->addWidget(infolabel, row2c, 0, 1, 1);
  cv_eangtin = new AntiquaCRM::IsbnEdit(this, // GTIN13
                                        AntiquaCRM::IsbnEdit::CodeType::GTIN);
  cv_eangtin->setObjectName("cv_eangtin");
  cv_eangtin->setToolTip(infolabel->text());
  cv_eangtin->appendStretch(0);
  row2->addWidget(cv_eangtin, row2c++, 1, 1, 4);
  row2->setRowStretch(row2c++, 1);

  // @BEGIN_GROUP {
  QFrame *toolBarFrame = new QFrame(this);
  toolBarFrame->setContentsMargins(0, 2, 0, 2);
  QHBoxLayout *tbfLayout = new QHBoxLayout(toolBarFrame);
  tbfLayout->setContentsMargins(0, 0, 0, 0);
  // Erfordert: http://musicbrainz.org/doc/libdiscid 64Bit
  btn_cdread = new QPushButton(tr("Read CD"), toolBarFrame);
  btn_cdread->setIcon(AntiquaCRM::antiquaIcon("view-search"));
  btn_cdread->setToolTip(tr("Opens a Metadata readout Dialog for Music CD."));
  tbfLayout->addWidget(btn_cdread);
  tbfLayout->addStretch(1);
  m_imageToolBar = new AntiquaCRM::ImageToolBar(toolBarFrame);
  tbfLayout->addWidget(m_imageToolBar);
  toolBarFrame->setLayout(tbfLayout);
  row2->addWidget(toolBarFrame, row2c++, 0, 1, 4);
  // } @END_GROUP

  // Image Viewer
  QSize _max_size = m_cfg->value("image/max_size", QSize(320, 320)).toSize();
  m_thumbnail = new AntiquaCRM::ImageThumbnail(this);
  m_thumbnail->setMinimumWidth(100);
  m_thumbnail->setMaximumWidth(_max_size.width());

  m_splitter->addLeft(row2Widget);
  m_splitter->addRight(m_thumbnail);
  mainLayout->addWidget(m_splitter, 1);

  // Row3
  m_tabWidget = new AntiquaCRM::TabsWidget(this);
  QIcon tabIcons = m_tabWidget->defaultIcon();

  // cv_description
  cv_description = new AntiquaCRM::TextField(m_tabWidget);
  cv_description->setObjectName("cv_description");
  m_tabWidget->insertTab(0, cv_description, tabIcons, tr("Description"));
  // cv_internal_description
  cv_internal_description = new AntiquaCRM::TextField(m_tabWidget);
  cv_internal_description->setObjectName("cv_internal_description");
  m_tabWidget->insertTab(1, cv_internal_description, tabIcons,
                         tr("Internal Description"));
  // Info Tab
  QWidget *m_infos = new QWidget(m_tabWidget);
  QVBoxLayout *m_infoLayout = new QVBoxLayout(m_infos);
  // cv_since
  cv_since = new AntiquaCRM::DateTimeInfo(this);
  cv_since->setObjectName("cv_since");
  cv_since->setBuddyLabel(tr("Created at"));
  cv_since->appendStretch(0);
  m_infoLayout->addWidget(cv_since);
  // cv_changed
  cv_changed = new AntiquaCRM::DateTimeInfo(this);
  cv_changed->setObjectName("cv_changed");
  cv_changed->setBuddyLabel(tr("Last changed"));
  cv_changed->appendStretch(0);
  m_infoLayout->addWidget(cv_changed);
  m_infoLayout->addStretch(1);
  m_infos->setLayout(m_infoLayout);
  m_tabWidget->insertTab(2, m_infos, tabIcons, tr("Information"));
  mainLayout->addWidget(m_tabWidget);

  m_actionBar = new AntiquaCRM::TabsEditActionBar(this);
  m_actionBar->setViewMailButton(false);
  m_actionBar->setPrinterMenu(AntiquaCRM::PRINT_BOOKCARD);
  mainLayout->addWidget(m_actionBar);

  setLayout(mainLayout);

  // Register modified changes
  registerInputChanged();

  // Signals:ToolBar
  connect(btn_cdread, SIGNAL(clicked()), SLOT(setReadMediaDialog()));
  connect(m_imageToolBar, SIGNAL(sendDeleteImage(qint64)),
          SLOT(setRemoveThumbnail(qint64)));
  connect(m_imageToolBar, SIGNAL(sendOpenImage()), SLOT(setImportEditImage()));

  // Signals:ActionBar
  connect(m_actionBar, SIGNAL(sendCancelClicked()),
          SLOT(setFinalLeaveEditor()));
  connect(m_actionBar, SIGNAL(sendRestoreClicked()), SLOT(setRestore()));
  connect(m_actionBar, SIGNAL(sendSaveClicked()), SLOT(setSaveData()));
  connect(m_actionBar, SIGNAL(sendFinishClicked()),
          SLOT(setCheckLeaveEditor()));
  connect(m_actionBar, SIGNAL(sendPrintBookCard()), SLOT(setPrintQRCode()));
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
  ignoreFields << "cv_type";          // only for orders!
  ignoreFields << "cv_including_vat"; /* DEPRECATED */

  m_tableData = initTableData(CDSVINYL_TABLE_NAME);
  if (m_tableData == nullptr)
    return;

  // Settings input defaults
  const QJsonObject _jobj = loadSqlConfig(CDSVINYL_CONFIG_POINTER);
  double _price_lowest = _jobj.value("media_price_lowest").toDouble();
  if (_price_lowest > 1.0)
    cv_price->setMinimum(_price_lowest);

  double _price_default = _jobj.value("media_price_normal").toDouble();
  if (_price_default > 1.5)
    cv_price->setValue(_price_default);

  // Standards für CD Import einstellen.
  cv_mtype->setValue(AntiquaCRM::MediaType::MEDIA_DISC_COMPACT);
  // Artikelanzahl
  if (cv_count->getValue().toInt() == 0)
    cv_count->setValue(QVariant(1));

  // Schlüsselwörter
  QStringList genres = SelectGenre::completer();
  if (genres.size() > 5) {
    cv_keyword->setCompleterList(genres);
  }

  // Lager
  cv_storage->reset();
  cv_storage->initData();
  int _storage_id = _jobj.value("media_storage_location").toInt();
  if (_storage_id > 0)
    cv_storage->setValue(_storage_id);
}

bool CDsVinylEditor::setDataField(const QSqlField &field,
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

  QString sql("UPDATE " + CDSVINYL_TABLE_NAME + " SET ");
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

  QString sql("INSERT INTO " + CDSVINYL_TABLE_NAME + " (");
  sql.append(column.join(","));
  sql.append(",cv_changed) VALUES (");
  sql.append(values.join(","));
  sql.append(",CURRENT_TIMESTAMP) RETURNING cv_id;");
  if (sendSqlQuery(sql) && cv_id->getValue().toInt() >= 1) {
    qInfo("SQL INSERT Inventory CD's & Vinyl success!");
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
      new AntiquaCRM::ImageImportDialog(_id, "Media", this);
  connect(d, SIGNAL(sendThumbnail(const QPixmap &)), m_thumbnail,
          SLOT(setPixmap(const QPixmap &)));

  d->exec();
  d->deleteLater();
}

void CDsVinylEditor::setReadMediaDialog() {
  CDReadDialog *d = new CDReadDialog(this);
  if (d->exec() == QDialog::Accepted) {
    QJsonObject obj = d->data();
    foreach (QString key, obj.keys()) {
      if (key == "tracks" || obj.value(key).isNull())
        continue;

      AntiquaCRM::TextLine *e = findChild<AntiquaCRM::TextLine *>(key);
      if (e != nullptr) {
        e->setValue(obj.value(key).toVariant());
      }
    }
    qint64 year = obj.value("cv_year").toDouble();
    if (year > 0)
      cv_year->setValue(year);

    QString genres = obj.value("cv_keyword").toString();
    if (!genres.isEmpty())
      cv_keyword->setValue(genres);

    QString barcode = obj.value("cv_eangtin").toString();
    if (!barcode.isEmpty())
      cv_eangtin->setValue(barcode);

    QStringList desc;
    desc << cv_description->getValue().toString();
    QJsonObject tracks = obj.value("tracks").toObject();
    QJsonObject::Iterator it;
    for (it = tracks.begin(); it != tracks.end(); ++it) {
      desc << it.key() + ") " + it.value().toString();
    }
    cv_description->setValue(desc.join("\n"));
  }
  d->deleteLater();
}

void CDsVinylEditor::setPrintQRCode() {
  qint64 _aid = getDataValue("cv_id").toLongLong();
  if (_aid < 1) {
    pushStatusMessage(tr("Missing valid Article Id!"));
    return;
  }

  QJsonObject _config = cv_storage->getBookcardData();
  _config.insert("aid", _aid);
  _config.insert("basename", AntiquaCRM::AUtil::zerofill(_aid));
  _config.insert("title", getDataValue("cv_title").toString());
  _config.insert("year", getDataValue("cv_year").toString());
  _config.insert("author", getDataValue("cv_author").toString());

  QUrl _qr_url;
  m_cfg->beginGroup("printer");
  _qr_url.setUrl(m_cfg->value("qrcode_url").toString());

  QString _query(m_cfg->value("qrcode_query").toString());
  _query.append("=");
  _query.append(_config.value("aid").toString());
  _qr_url.setQuery(_query);

  _config.insert("qrquery", _qr_url.toString());
  m_cfg->endGroup();

  QString _changed = getDataValue("cv_changed")
                         .toDate()
                         .toString(ANTIQUACRM_SHORT_DATE_DISPLAY);
  _config.insert("changed", _changed.trimmed());

  AntiquaCRM::PrintBookCard *m_d = new AntiquaCRM::PrintBookCard(this);
  if (m_d->exec(_config) == QDialog::Accepted) {
    pushStatusMessage(tr("Card print successfully."));
  } else {
    pushStatusMessage(tr("Card print canceled."));
  }
}

void CDsVinylEditor::setRestore() {
  importSqlResult();
  cv_count->setFocus();
}

bool CDsVinylEditor::openEditEntry(qint64 articleId) {
  if (articleId < 1)
    return false;

  QString _id = QString::number(articleId);
  if (_id.isEmpty())
    return false;

  setInputFields();
  bool _status = false;
  QString _table = m_tableData->tableName();
  QString _query("SELECT * FROM " + _table + " WHERE cv_id=" + _id + ";");
  QSqlQuery _q = m_sql->query(_query);
  if (_q.size() != 0) {
    QSqlRecord _r = m_tableData->record();
    while (_q.next()) {
      foreach (QString key, inputFields) {
        m_tableData->setValue(key, _q.value(_r.indexOf(key)));
      }
    }
    setLoadThumbnail(articleId);
    _status = true;
  } else {
    _status = false;
  }

  if (_status) {
    // Die aktuelle Abfolge ist Identisch mit setRestore!
    setRestore();
  }

  return _status;
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
  return isEnabled();
}

bool CDsVinylEditor::createCustomEntry(const QJsonObject &object) {
  qDebug() << Q_FUNC_INFO << "TODO" << object;
  return true;
}
