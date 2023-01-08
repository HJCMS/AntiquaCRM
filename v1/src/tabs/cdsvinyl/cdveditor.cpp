// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cdveditor.h"
#include "keywordlineedit.h"

#include <AntiquaCRM>
#include <AntiquaPrinting>
#include <QDebug>
#include <QMessageBox>

CDVEditor::CDVEditor(QWidget *parent)
    : InventoryEditor{"^cv_[a-z_]+\\b$", parent} {
  setObjectName("cdv_editor");
  setWindowTitle(tr("Edit CD or Vinyl"));

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("cdv_editor_layout");
  mainLayout->setSizeConstraint(QLayout::SetMaximumSize);

  // BEGIN : Row0
  QHBoxLayout *row0 = new QHBoxLayout();
  cv_id = new SerialID(this);
  cv_id->setObjectName("cv_id");
  cv_id->setInfo(tr("Article ID"));
  cv_id->setRequired(true);
  row0->addWidget(cv_id);

  cv_count = new IntSpinBox(this);
  cv_count->setObjectName("cv_count");
  cv_count->setInfo(tr("Count"));
  row0->addWidget(cv_count);

  double minPrice = m_cfg->value("payment/min_price", 2.50).toDouble();
  cv_price = new PriceEdit(this);
  cv_price->setObjectName("cv_price");
  cv_price->setRequired(true);
  cv_price->setInfo(tr("Price"));
  cv_price->setMinimum(minPrice);
  row0->addWidget(cv_price);

  cv_year = new YearEdit(this);
  cv_year->setObjectName("cv_year");
  cv_year->setRequired(true);
  cv_year->setInfo(tr("Year"));
  cv_year->setValue(1800);
  row0->addWidget(cv_year);

  cv_including_vat = new BoolBox(this);
  cv_including_vat->setObjectName("cv_including_vat");
  cv_including_vat->setInfo(tr("incl. vat"));
  cv_including_vat->setRequired(false);
  row0->addWidget(cv_including_vat);

  cv_restricted = new BoolBox(this);
  cv_restricted->setObjectName("cv_restricted");
  cv_restricted->setInfo(tr("Local Usage only"));
  cv_restricted->setToolTip(tr("When this Options is marked. Then this Article "
                               "will not exported to your Providers."));
  row0->addWidget(cv_restricted);

  row0->addStretch(1);
  mainLayout->addLayout(row0, 0);
  // END : Row0

  // BEGIN : Row1
  m_splitter = new QSplitter(Qt::Horizontal, this);
  m_splitter->setChildrenCollapsible(false);
  m_splitter->setContentsMargins(2, 2, 2, 2);
  // Eingabemasken
  QWidget *row1Widget = new QWidget(m_splitter);
  row1Widget->setContentsMargins(0, 0, 0, 0);
  int row1c = 0;
  QGridLayout *row1 = new QGridLayout(row1Widget);
  row1->setColumnStretch(1, 1);
  // cv_title
  m_infoLabel = new AntiquaILabel(tr("Title"), row1Widget);
  row1->addWidget(m_infoLabel, row1c, 0, 1, 1);
  cv_title = new LineEdit(this);
  cv_title->setObjectName("cv_title");
  cv_title->setToolTip(m_infoLabel->text());
  row1->addWidget(cv_title, row1c++, 1, 1, 4);
  // cv_title_extended
  m_infoLabel = new AntiquaILabel(tr("Extended"), row1Widget);
  row1->addWidget(m_infoLabel, row1c, 0, 1, 1);
  cv_title_extended = new LineEdit(this);
  cv_title_extended->setObjectName("cv_title_extended");
  cv_title_extended->setToolTip(m_infoLabel->text());
  row1->addWidget(cv_title_extended, row1c++, 1, 1, 4);
  // cv_author
  m_infoLabel = new AntiquaILabel(tr("Artists"), row1Widget);
  row1->addWidget(m_infoLabel, row1c, 0, 1, 1);
  cv_author = new LineEdit(this);
  cv_author->setObjectName("cv_author");
  cv_author->setToolTip(m_infoLabel->text());
  row1->addWidget(cv_author, row1c++, 1, 1, 4);
  // @GROUP {
  // cv_type
  m_infoLabel = new AntiquaILabel(tr("Mediatype"), row1Widget);
  row1->addWidget(m_infoLabel, row1c, 0, 1, 1);
  cv_type = new SelectMediaType(this);
  cv_type->setObjectName("cv_type");
  cv_type->setToolTip(m_infoLabel->text());
  row1->addWidget(cv_type, row1c, 1, 1, 1);
  // cv_condition
  m_infoLabel = new AntiquaILabel(tr("Condition"), row1Widget);
  row1->addWidget(m_infoLabel, row1c, 2, 1, 1);
  cv_condition = new ConditionEdit(this);
  cv_condition->setObjectName("cv_condition");
  cv_condition->setToolTip(m_infoLabel->text());
  row1->addWidget(cv_condition, row1c++, 3, 1, 1);
  // }
  // cv_storage
  m_infoLabel = new AntiquaILabel(tr("Storage"), row1Widget);
  row1->addWidget(m_infoLabel, row1c, 0, 1, 1);
  cv_storage = new StorageLocation(this);
  cv_storage->setObjectName("cv_storage");
  cv_storage->setToolTip(m_infoLabel->text());
  row1->addWidget(cv_storage, row1c++, 1, 1, 4);
  // cv_keywords
  m_infoLabel = new AntiquaILabel(tr("Keywords"), row1Widget);
  row1->addWidget(m_infoLabel, row1c, 0, 1, 1);
  cv_keyword = new KeywordLineEdit(this);
  cv_keyword->setObjectName("cv_keyword");
  cv_keyword->setToolTip(m_infoLabel->text());
  row1->addWidget(cv_keyword, row1c++, 1, 1, 4);
  // cv_isbn
  AntiquaILabel *lb_isbn = new AntiquaILabel("ISBN/EAN", this);
  row1->addWidget(lb_isbn, row1c, 0, 1, 1);
  cv_isbn = new IsbnEdit(this);
  cv_isbn->setObjectName("cv_isbn");
  cv_isbn->setToolTip(lb_isbn->text());
  row1->addWidget(cv_isbn, row1c++, 1, 1, 4);
  // image toolbar
  m_imageToolBar = new ImageToolBar(this);
  row1->addWidget(m_imageToolBar, row1c++, 1, 1, 4);
  row1Widget->setLayout(row1);

  // Image Viewer
  QSize maxSize = m_cfg->value("image/max_size", QSize(320, 320)).toSize();
  m_imageView = new ImageView(maxSize, m_splitter);
  m_imageView->setMaximumWidth(maxSize.width());
  m_splitter->insertWidget(1, m_imageView);
  mainLayout->addWidget(m_splitter, 1);
  // END : Row1

  // BEGIN : Row2
  m_tabWidget = new EditorTab(this);
  m_tabWidget->setObjectName("tab_widget");

  mainLayout->addWidget(m_tabWidget);
  // END : Row2

  // Begin : Row3
  m_actionBar = new EditorActionBar(this);
  mainLayout->addWidget(m_actionBar);
  // END : Row3

  mainLayout->addStretch(1);
  setLayout(mainLayout);

  // Signals:ActionBar
  connect(m_actionBar, SIGNAL(sendCancelClicked()),
          SLOT(setFinalLeaveEditor()));
  connect(m_actionBar, SIGNAL(sendRestoreClicked()), SLOT(setRestore()));
  connect(m_actionBar, SIGNAL(sendSaveClicked()), SLOT(setSaveData()));
  connect(m_actionBar, SIGNAL(sendFinishClicked()),
          SLOT(setCheckLeaveEditor()));
}

void CDVEditor::setInputFields() {
  // Bei UPDATE/INSERT Ignorieren
  ignoreFields << "cv_since";
  ignoreFields << "cv_changed";
  m_tableData = new AntiquaCRM::ASqlDataQuery("inventory_cdvinyl");
  inputFields = m_tableData->columnNames();
  if (inputFields.isEmpty()) {
    QStringList warn(tr("An error has occurred!"));
    warn << tr("Can't load input datafields!");
    warn << tr("When getting this Message, please check your Network and "
               "Database connection!");
    openNoticeMessage(warn.join("\n"));
  }
  // Lager
  cv_storage->reset();
  cv_storage->loadDataset();

  // Schlüsselwörter
  cv_keyword->loadDataset();
}

bool CDVEditor::setDataField(const QSqlField &field, const QVariant &value) {
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

void CDVEditor::importSqlResult() {
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
  qint64 id = cv_id->value().toLongLong();
  if (id > 0) {
    m_imageToolBar->setArticleId(id);
    m_imageToolBar->setActive(true);
    m_imageView->readFromDatabase(id);
  }
  m_actionBar->setRestoreable(m_tableData->isValid());
  setResetModified(inputFields);
}

bool CDVEditor::sendSqlQuery(const QString &query) {
  qDebug() << Q_FUNC_INFO << query;
  return true;

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

  openSuccessMessage(tr("Saved successfully!"));
  setResetModified(inputFields);
  return true;
}

const QHash<QString, QVariant> CDVEditor::createSqlDataset() {
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

void CDVEditor::createSqlUpdate() {
  int articleId = cv_id->value().toInt();
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
  int cur_count = cv_count->value().toInt();
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

  QString sql("UPDATE inventory_cdvinyl SET ");
  sql.append(set.join(","));
  sql.append(",cv_changed=CURRENT_TIMESTAMP WHERE cv_id=");
  sql.append(cv_id->value().toString());
  sql.append(";");
  if (sendSqlQuery(sql)) {
    qInfo("SQL CD/Vinyl UPDATE success!");
  }
}

void CDVEditor::createSqlInsert() {
  int articleId = cv_id->value().toInt();
  if (articleId >= 1) {
    qWarning("Skip INSERT, switch to UPDATE with (ArticleID > 0)!");
    createSqlUpdate();
    return;
  }
  // Bei einem INSERT die Anforderungen anpassen.
  cv_count->setRequired(true);
  cv_id->setRequired(false);

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

  QString sql("INSERT INTO inventory_cdvinyl (");
  sql.append(column.join(","));
  sql.append(",cv_changed) VALUES (");
  sql.append(values.join(","));
  sql.append(",CURRENT_TIMESTAMP) RETURNING cv_id;");
  if (sendSqlQuery(sql) && cv_id->value().toInt() >= 1) {
    qInfo("SQL INSERT Inventory Prints success!");
    // Zurücksetzen Knopf Aktivieren?
    m_actionBar->setRestoreable(m_tableData->isValid());
    // Bildaktionen erst bei vorhandener Artikel Nummer freischalten!
    m_imageToolBar->setActive(true);
    cv_id->setRequired(true);
  }
}

bool CDVEditor::realyDeactivateEntry() {
  qint8 _curCount = cv_count->value().toInt();
  qint8 _oldcount = m_tableData->getValue("cv_count").toInt();
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

  int ret = QMessageBox::question(this, tr("Deactivation"), body.join(""));
  if (ret == QMessageBox::No) {
    cv_count->setValue(m_tableData->getValue("cv_count"));
    cv_count->setRequired(true);
    return false;
  }

  cv_count->setRequired(false);
  return true;
}

bool CDVEditor::checkYear() {
  int _mv = 1889; /**< min. Vinyl Year */
  int _mc = 1980; /**< min. Compact Disc Year */
  int _y = cv_year->value().toInt();
  int _t = cv_type->value().toInt();
  if (_t == 0) {
    QString txt = tr("Missing Mediatype for Year, aborted!");
    openNoticeMessage(txt);
    return false;
  } else if ((_y < _mv) && (_t > 3)) {
    QString txt = tr("Invalid Vinyl record Year, aborted!");
    openNoticeMessage(txt);
    return false;
  } else if ((_y < _mc) && (_t < 4)) {
    QString txt = tr("Invalid Compact Disc Year, aborted!");
    openNoticeMessage(txt);
    return false;
  }
  return true;
}

void CDVEditor::setSaveData() {
  if (!checkYear())
    return;

  if (cv_id->value().toString().isEmpty()) {
    createSqlInsert();
    return;
  }

  if (cv_count->value().toInt() == 0 && !realyDeactivateEntry())
    return;

  createSqlUpdate();
}

void CDVEditor::setCheckLeaveEditor() {
  if (checkIsModified()) {
    unsavedChangesPopup();
    return;
  }
  setFinalLeaveEditor();
}

void CDVEditor::setFinalLeaveEditor() {
  setResetInputFields();
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  m_imageView->clear();               /**< Bildvorschau leeren */
  emit sendLeaveEditor();             /**< Back to MainView */
}

void CDVEditor::actionRemoveImage(qint64 articleId) {
  qint64 id = cv_id->value().toLongLong();
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

void CDVEditor::actionEditImages() {
  qint64 id = cv_id->value().toLongLong();
  if (id < 1)
    return;

  ImageDialog *d = new ImageDialog(id, this);
  if (d->exec()) {
    m_imageView->readFromDatabase(id);
  }
}

void CDVEditor::setRestore() {
  importSqlResult();
  cv_count->setFocus();
}

bool CDVEditor::openEditEntry(qint64 articleId) {
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

bool CDVEditor::createNewEntry() {
  setInputFields();
  m_imageView->clear();
  foreach (QString column, m_tableData->columnNames()) {
    QSqlField field = m_tableData->getProperties(column);
    if (column == "cv_id")
      field.setRequired(false);

    setDefaultInput(field);
  }
  setResetModified(inputFields);
  return isEnabled();
}
