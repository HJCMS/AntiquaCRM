// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printsstitcheseditor.h"
#include "keywordlineedit.h"

#include <AntiquaCRM>
#include <AntiquaPrinting>
#include <QDebug>
#include <QMessageBox>

PrintsStitchesEditor::PrintsStitchesEditor(QWidget *parent)
    : InventoryEditor{"^ip_[a-z_]+\\b$", parent} {
  setObjectName("ps_editor");
  setWindowTitle(tr("Edit Prints"));
  setContentsMargins(0, 0, 0, 0);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("ps_editor_main_layout");
  mainLayout->setSizeConstraint(QLayout::SetMaximumSize);

  // Begin : Row0
  QHBoxLayout *row0 = new QHBoxLayout();
  ip_id = new SerialID(this);
  ip_id->setObjectName("ip_id");
  ip_id->setInfo(tr("Article ID"));
  ip_id->setRequired(true);
  row0->addWidget(ip_id);

  ip_count = new IntSpinBox(this);
  ip_count->setObjectName("ip_count");
  ip_count->setInfo(tr("Count"));
  row0->addWidget(ip_count);

  double minPrice = m_cfg->value("payment/min_price", 5.00).toDouble();
  ip_price = new PriceEdit(this);
  ip_price->setObjectName("ip_price");
  ip_price->setRequired(true);
  ip_price->setInfo(tr("Price"));
  ip_price->setMinimum(minPrice);
  row0->addWidget(ip_price);

  ip_including_vat = new BoolBox(this);
  ip_including_vat->setObjectName("ip_including_vat");
  ip_including_vat->setInfo(tr("incl. vat"));
  ip_including_vat->setRequired(false);
  row0->addWidget(ip_including_vat);

  ip_views = new BoolBox(this);
  ip_views->setObjectName("ip_views");
  ip_views->setInfo(tr("Views"));
  ip_views->setRequired(false);
  row0->addWidget(ip_views);

  ip_kolorit = new BoolBox(this);
  ip_kolorit->setObjectName("ip_kolorit");
  ip_kolorit->setInfo(tr("Kolorit"));
  ip_kolorit->setRequired(false);
  row0->addWidget(ip_kolorit);

  ip_landscape = new BoolBox(this);
  ip_landscape->setObjectName("ip_landscape");
  ip_landscape->setRequired(true);
  ip_landscape->setInfo(tr("Landscape"));
  row0->addWidget(ip_landscape);

  ip_year = new YearEdit(this);
  ip_year->setObjectName("ip_year");
  ip_year->setRequired(true);
  ip_year->setInfo(tr("Year"));
  ip_year->setValue(1900);
  row0->addWidget(ip_year);

  ip_restricted = new BoolBox(this);
  ip_restricted->setObjectName("ip_restricted");
  ip_restricted->setInfo(tr("Local Usage only"));
  ip_restricted->setToolTip(tr("When this Options is marked. Then this Article "
                               "will not exported to your Providers."));

  row0->addWidget(ip_restricted);
  row0->addStretch(1);
  mainLayout->addLayout(row0, 0);
  // END : Row0

  // Begin : Row1
  m_splitter = new QSplitter(Qt::Horizontal, this);
  m_splitter->setChildrenCollapsible(false);
  m_splitter->setContentsMargins(2, 2, 2, 2);
  // Eingabemasken
  QWidget *row1Widget = new QWidget(m_splitter);
  row1Widget->setContentsMargins(0, 0, 0, 0);
  int row1c = 0;
  QGridLayout *row1 = new QGridLayout(row1Widget);
  // ip_title
  m_infoLabel = new AntiquaILabel(tr("Title"), row1Widget);
  row1->addWidget(m_infoLabel, row1c, 0, 1, 1);
  ip_title = new LineEdit(this);
  ip_title->setObjectName("ip_title");
  ip_title->setToolTip(m_infoLabel->text());
  row1->addWidget(ip_title, row1c++, 1, 1, 1);
  // ip_title_extended
  m_infoLabel = new AntiquaILabel(tr("Title Extended"), row1Widget);
  row1->addWidget(m_infoLabel, row1c, 0, 1, 1);
  ip_title_extended = new LineEdit(this);
  ip_title_extended->setObjectName("ip_title_extended");
  ip_title_extended->setToolTip(m_infoLabel->text());
  row1->addWidget(ip_title_extended, row1c++, 1, 1, 1);
  // ip_author
  m_infoLabel = new AntiquaILabel(tr("Author"), row1Widget);
  row1->addWidget(m_infoLabel, row1c, 0, 1, 1);
  ip_author = new LineEdit(this);
  ip_author->setObjectName("ip_author");
  ip_author->setToolTip(m_infoLabel->text());
  row1->addWidget(ip_author, row1c++, 1, 1, 1);
  // ip_format
  m_infoLabel = new AntiquaILabel(tr("Format"), row1Widget);
  row1->addWidget(m_infoLabel, row1c, 0, 1, 1);
  ip_format = new LineEdit(this);
  ip_format->setObjectName("ip_format");
  ip_format->setToolTip(m_infoLabel->text());
  row1->addWidget(ip_format, row1c++, 1, 1, 1);
  // ip_condition
  m_infoLabel = new AntiquaILabel(tr("Condition"), row1Widget);
  row1->addWidget(m_infoLabel, row1c, 0, 1, 1);
  ip_condition = new ConditionEdit(this);
  ip_condition->setObjectName("ip_condition");
  ip_condition->setToolTip(m_infoLabel->text());
  row1->addWidget(ip_condition, row1c++, 1, 1, 1);
  // ib_designation
  m_infoLabel = new AntiquaILabel(tr("Designation"), row1Widget);
  row1->addWidget(m_infoLabel, row1c, 0, 1, 1);
  ib_designation = new LineEdit(this);
  ib_designation->setObjectName("ip_designation");
  ib_designation->setToolTip(m_infoLabel->text());
  row1->addWidget(ib_designation, row1c++, 1, 1, 1);
  // ip_technique
  m_infoLabel = new AntiquaILabel(tr("Technique"), row1Widget);
  row1->addWidget(m_infoLabel, row1c, 0, 1, 1);
  ip_technique = new TechniqueEdit(this);
  ip_technique->setObjectName("ip_technique");
  ip_technique->setToolTip(m_infoLabel->text());
  row1->addWidget(ip_technique, row1c++, 1, 1, 1);
  // ip_storage
  m_infoLabel = new AntiquaILabel(tr("Storage"), row1Widget);
  row1->addWidget(m_infoLabel, row1c, 0, 1, 1);
  ip_storage = new StorageLocation(this);
  ip_storage->setObjectName("ip_storage");
  ip_storage->setToolTip(m_infoLabel->text());
  row1->addWidget(ip_storage, row1c++, 1, 1, 1);
  // ip_keywords
  m_infoLabel = new AntiquaILabel(tr("Keywords"), row1Widget);
  row1->addWidget(m_infoLabel, row1c, 0, 1, 1);
  ip_keyword = new KeywordLineEdit(this);
  ip_keyword->setObjectName("ip_keyword");
  ip_keyword->setToolTip(m_infoLabel->text());
  row1->addWidget(ip_keyword, row1c++, 1, 1, 1);

  m_imageToolBar = new ImageToolBar(this);
  row1->addWidget(m_imageToolBar, row1c++, 0, 1, 2);
  row1Widget->setLayout(row1);
  m_splitter->insertWidget(0, row1Widget);
  // Image Viewer
  QSize maxSize = m_cfg->value("image/max_size", QSize(320, 320)).toSize();
  m_imageView = new ImageView(maxSize, m_splitter);
  m_imageView->setMaximumWidth(maxSize.width());
  m_splitter->insertWidget(1, m_imageView);
  mainLayout->addWidget(m_splitter, 1);
  // END : Row1

  // Begin : Row2
  m_tabWidget = new EditorTab(this);
  m_tabWidget->setObjectName("tab_widget");
  QIcon tabIcons = m_tabWidget->defaultIcon();
  // Description
  ip_description = new TextField(m_tabWidget);
  ip_description->setObjectName("ip_description");
  m_tabWidget->insertTab(0, ip_description, tabIcons, tr("Public Description"));
  // Internal Description
  ip_internal_description = new TextField(m_tabWidget);
  ip_internal_description->setObjectName("ip_internal_description");
  m_tabWidget->insertTab(1, ip_internal_description, tabIcons,
                         tr("Internal Description"));
  // Info Tab
  QWidget *m_infos = new QWidget(this);
  QVBoxLayout *m_infoLayout = new QVBoxLayout(m_infos);
  ip_since = new AntiquaDateInfo(this);
  ip_since->setObjectName("ip_since");
  ip_since->setInfo(tr("Created at"));
  m_infoLayout->addWidget(ip_since);
  ip_changed = new AntiquaDateInfo(this);
  ip_changed->setObjectName("ip_changed");
  ip_changed->setInfo(tr("Last changed"));
  m_infoLayout->addWidget(ip_changed);
  m_infoLayout->addStretch(1);
  m_infos->setLayout(m_infoLayout);
  m_tabWidget->insertTab(2, m_infos, tabIcons, tr("Information"));
  mainLayout->addWidget(m_tabWidget);
  // END : Row2

  // Begin : Row3
  m_actionBar = new EditorActionBar(this);
  mainLayout->addWidget(m_actionBar);
  // END : Row3

  mainLayout->addStretch(1);
  setLayout(mainLayout);

  // Signals:ImageToolBar
  /*
  connect(m_imageToolBar, SIGNAL(sendDeleteImage(qint64)),
          SLOT(actionRemoveImage(qint64)));
  connect(m_imageToolBar, SIGNAL(sendOpenImage()), SLOT(actionEditImages()));
  */
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

void PrintsStitchesEditor::setInputFields() {
  // Bei UPDATE/INSERT Ignorieren
  ignoreFields << "ip_since";
  ignoreFields << "ip_changed";
  ignoreFields << "ip_category_subject"; /** @warning OBSOLETE */
  m_tableData = new AntiquaCRM::ASqlDataQuery("inventory_prints");
  inputFields = m_tableData->columnNames();
  if (inputFields.isEmpty()) {
    QStringList warn(tr("An error has occurred!"));
    warn << tr("Can't load input datafields!");
    warn << tr("When getting this Message, please check your Network and "
               "Database connection!");
    openNoticeMessage(warn.join("\n"));
  }

  // Lager
  ip_storage->reset();
  ip_storage->loadDataset();

  // Technik
  ip_technique->reset();
  ip_technique->loadDataset();

  // Schlüsselwörter
  ip_keyword->loadDataset();
}

bool PrintsStitchesEditor::setDataField(const QSqlField &field,
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
#ifdef ANTIQUA_DEVELOPEMENT
  if (!ignoreFields.contains(key))
    qDebug() << "Unknown:" << key << "|" << value << "|" << required;
#endif
  return false;
}

void PrintsStitchesEditor::importSqlResult() {
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
  qint64 id = ip_id->value().toLongLong();
  if (id > 0) {
    m_imageToolBar->setArticleId(id);
    m_imageToolBar->setActive(true);
    m_imageView->readFromDatabase(id);
  }
  m_actionBar->setRestoreable(m_tableData->isValid());
  setResetModified(inputFields);
}

bool PrintsStitchesEditor::sendSqlQuery(const QString &query) {
  // qDebug() << Q_FUNC_INFO << query;
  // return true;
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

  openSuccessMessage(tr("Print & Stitches saved successfully!"));
  setResetModified(inputFields);
  return true;
}

const QHash<QString, QVariant> PrintsStitchesEditor::createSqlDataset() {
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

void PrintsStitchesEditor::createSqlUpdate() {
  int articleId = ip_id->value().toInt();
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
  int cur_count = ip_count->value().toInt();
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
  sql.append(ip_id->value().toString());
  sql.append(";");
  if (sendSqlQuery(sql)) {
    qInfo("SQL UPDATE Inventory Prints success!");
  }
}

void PrintsStitchesEditor::createSqlInsert() {
  int articleId = ip_id->value().toInt();
  if (articleId >= 1) {
    qWarning("Skip INSERT, switch to UPDATE with (ArticleID > 0)!");
    createSqlUpdate();
    return;
  }
  // Bei einem INSERT die Anforderungen anpassen.
  ip_count->setRequired(true);
  ip_id->setRequired(false);

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

  QString sql("INSERT INTO inventory_prints (");
  sql.append(column.join(","));
  sql.append(",ip_changed) VALUES (");
  sql.append(values.join(","));
  sql.append(",CURRENT_TIMESTAMP) RETURNING ip_id;");
  if (sendSqlQuery(sql) && ip_id->value().toInt() >= 1) {
    qInfo("SQL INSERT Inventory Prints success!");
    // Zurücksetzen Knopf Aktivieren?
    m_actionBar->setRestoreable(m_tableData->isValid());
    // Bildaktionen erst bei vorhandener Artikel Nummer freischalten!
    m_imageToolBar->setActive(true);
    ip_id->setRequired(true);
  }
}

void PrintsStitchesEditor::setRestore() {
  importSqlResult();
  ip_count->setFocus();
}

bool PrintsStitchesEditor::realyDeactivateEntry() {
  qint8 _curCount = ip_count->value().toInt();
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
      QMessageBox::question(this, tr("Prints deactivation"), body.join(""));
  if (ret == QMessageBox::No) {
    ip_count->setValue(m_tableData->getValue("ip_count"));
    ip_count->setRequired(true);
    return false;
  }

  ip_count->setRequired(false);
  return true;
}

void PrintsStitchesEditor::setSaveData() {
  if (ip_id->value().toString().isEmpty()) {
    createSqlInsert();
    return;
  }

  if (ip_count->value().toInt() == 0 && !realyDeactivateEntry())
    return;

  createSqlUpdate();
}

void PrintsStitchesEditor::setCheckLeaveEditor() {
  if (checkIsModified()) {
    QStringList info(tr("Unsaved Changes"));
    info << tr("Don't leave this page before save your changes!");
    openNoticeMessage(info.join("\n"));
    return;
  }
  setFinalLeaveEditor();
}

void PrintsStitchesEditor::setFinalLeaveEditor() {
  setResetInputFields();
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  m_imageView->clear();               /**< Bildvorschau leeren */
  emit sendLeaveEditor();             /**< Back to MainView */
}

bool PrintsStitchesEditor::openEditEntry(qint64 articleId) {
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

bool PrintsStitchesEditor::createNewEntry() {
  setInputFields();
  m_imageView->clear();
  foreach (QString column, m_tableData->columnNames()) {
    QSqlField field = m_tableData->getProperties(column);
    if (column == "ip_id")
      field.setRequired(false);

    setDefaultInput(field);
  }
  setResetModified(inputFields);
  return isEnabled();
}
