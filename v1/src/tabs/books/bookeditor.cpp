// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookeditor.h"
#include "editoractionbar.h"
#include "keywordlineedit.h"

#include <AntiquaCRM>
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
  ib_including_vat->setToolTip(tr("VAT Settings must be set."));
  row0->addWidget(ib_including_vat);

  ib_signed = new BoolBox(this);
  ib_signed->setObjectName("ib_signed");
  ib_signed->setInfo(tr("Signed Version"));
  row0->addWidget(ib_signed);

  ib_restricted = new BoolBox(this);
  ib_restricted->setObjectName("ib_restricted");
  ib_restricted->setInfo(tr("Restricted Sale"));
  ib_restricted->setToolTip(
      tr("Is the title not for sale nationally or is it on a censorship list. "
         "This is relevant for the Shopsystem."));

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
  QWidget *row2Widget = new QWidget(this);
  row2Widget->setMinimumHeight(380);
  int row2c = 0;
  QGridLayout *row2 = new QGridLayout(row2Widget);

  AntiquaILabel *lb_title = new AntiquaILabel(tr("Booktitle"), row2Widget);
  row2->addWidget(lb_title, row2c, 0, 1, 1);
  ib_title = new LineEdit(this);
  ib_title->setObjectName("ib_title");
  ib_title->setToolTip(lb_title->text());
  row2->addWidget(ib_title, row2c++, 1, 1, 1);

  AntiquaILabel *lb_ext_title = new AntiquaILabel(tr("Book Subtitle"), this);
  row2->addWidget(lb_ext_title, row2c, 0, 1, 1);
  ib_title_extended = new LineEdit(this);
  ib_title_extended->setObjectName("ib_title_extended");
  ib_title_extended->setToolTip(lb_ext_title->text());
  row2->addWidget(ib_title_extended, row2c++, 1, 1, 1);

  AntiquaILabel *lb_author = new AntiquaILabel(tr("Author"), this);
  row2->addWidget(lb_author, row2c, 0, 1, 1);
  ib_author = new LineEdit(this);
  ib_author->setObjectName("ib_author");
  ib_author->setToolTip(lb_author->text());
  row2->addWidget(ib_author, row2c++, 1, 1, 1);

  AntiquaILabel *lb_publisher = new AntiquaILabel(tr("Publisher"), this);
  row2->addWidget(lb_publisher, row2c, 0, 1, 1);
  ib_publisher = new LineEdit(this);
  ib_publisher->setObjectName("ib_publisher");
  ib_publisher->setToolTip(lb_publisher->text());
  row2->addWidget(ib_publisher, row2c++, 1, 1, 1);

  AntiquaILabel *lb_condition = new AntiquaILabel(tr("Condition"), this);
  row2->addWidget(lb_condition, row2c, 0, 1, 1);
  QHBoxLayout *conditionlayout = new QHBoxLayout();
  ib_condition = new ConditionEdit(this);
  ib_condition->setObjectName("ib_condition");
  ib_condition->setToolTip(tr("Condition of this Book."));
  conditionlayout->addWidget(ib_condition, Qt::AlignLeft);
  AntiquaILabel *lb_language = new AntiquaILabel(tr("Language"), this);
  conditionlayout->addWidget(lb_language, Qt::AlignRight);
  ib_language = new Language(this);
  ib_language->setObjectName("ib_language");
  conditionlayout->addWidget(ib_language);
  row2->addLayout(conditionlayout, row2c++, 1, 1, 1);

  AntiquaILabel *lb_designation = new AntiquaILabel(tr("Designation"), this);
  row2->addWidget(lb_designation, row2c, 0, 1, 1);
  // Bucheinband und Bindungsart
  QHBoxLayout *binding_layout = new QHBoxLayout();
  binding_layout->setContentsMargins(0, 0, 0, 0);
  ib_binding = new BookBinding(this);
  ib_binding->setObjectName("ib_binding");
  ib_binding->setToolTip(tr("Cover"));
  binding_layout->addWidget(ib_binding);
  ib_designation = new LineEdit(this);
  ib_designation->setObjectName("ib_designation");
  ib_designation->setToolTip(lb_designation->text());
  binding_layout->addWidget(ib_designation);
  row2->addLayout(binding_layout, row2c++, 1, 1, 1);

  AntiquaILabel *lb_storage = new AntiquaILabel(tr("Storage"), this);
  row2->addWidget(lb_storage, row2c, 0, 1, 1);
  ib_storage = new StorageLocation(this);
  ib_storage->setObjectName("ib_storage");
  ib_storage->setToolTip(lb_storage->text());
  row2->addWidget(ib_storage, row2c++, 1, 1, 1);

  AntiquaILabel *lb_keywords = new AntiquaILabel(tr("Keywords"), this);
  row2->addWidget(lb_keywords, row2c, 0, 1, 1);
  ib_keyword = new KeywordLineEdit(this);
  ib_keyword->setObjectName("ib_keyword");
  ib_keyword->setToolTip(lb_keywords->text());
  row2->addWidget(ib_keyword, row2c++, 1, 1, 1);

  AntiquaILabel *lb_isbn = new AntiquaILabel(tr("ISBN"), this);
  row2->addWidget(lb_isbn, row2c, 0, 1, 1);
  ib_isbn = new IsbnEdit(this);
  ib_isbn->setObjectName("ib_isbn");
  ib_isbn->setToolTip(lb_isbn->text());
  row2->addWidget(ib_isbn, row2c++, 1, 1, 1);

  // TODO Image Toolbar
  row2->addWidget(new QLabel(tr("TODO Image Toolbar"), this), row2c++, 0, 1, 2);
  /*
   * TEST
  PostalCodeEdit* plz = new PostalCodeEdit(this);
  plz->loadDataset();
  row2->addWidget(plz, row2c++, 0, 1, 2);
  */

  // TODO Image Viewer
  QSize maxSize = m_cfg->value("image/max_size", QSize(320, 320)).toSize();
  QWidget *m_imageView = new QWidget(this);
  m_imageView->setFixedSize(maxSize);
  row2->addWidget(m_imageView, 0, 2, (row2c + 1), 1);

  row2Widget->setLayout(row2);
  mainLayout->addWidget(row2Widget);

  m_tabWidget = new EditorTab(this);
  m_tabWidget->setObjectName("tab_widget");
  QIcon tabIcons = m_tabWidget->defaultIcon();
  // Description
  ib_description = new TextField(m_tabWidget);
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
  setEnabled(false);

  // SIGNALS
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
  ignoreFields << "ib_json_category";
  ignoreFields << "ib_since";
  ignoreFields << "ib_changed";

  m_bookData = new AntiquaCRM::ASqlDataQuery("inventory_books");
  inputFields = m_bookData->columnNames();
  if (inputFields.isEmpty()) {
    qWarning("Books InputList is Empty!");
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

  // Buch Zustand
  ib_binding->loadDataset();
  AntiquaCRM::ACompleterData designations("designations");
  ib_designation->setCompleter(designations.completition("name"));

  // Schlüsselwörter
  ib_keyword->loadDataset();
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

  qDebug() << "Unknown:" << key << "|" << value << "|" << required;
  return false;
}

void BookEditor::importSqlResult() {
  if (m_bookData == nullptr)
    return;

  QHashIterator<QString, QVariant> it(m_bookData->getDataset());
  while (it.hasNext()) {
    it.next();
    QSqlField field = m_bookData->getProperties(it.key());
    setDataField(field, it.value());
  }
  m_actionBar->setRestoreable(m_bookData->isValid());
  setResetModified(inputFields);
}

bool BookEditor::sendSqlQuery(const QString &query) {
  //  qDebug() << Q_FUNC_INFO << query;
  //  return true;
  QSqlQuery q = m_sql->query(query);
  if (q.lastError().type() != QSqlError::NoError) {
    qDebug() << Q_FUNC_INFO << query << m_sql->lastError();
    return false;
  } else {
    if (q.next()) {
      if (!q.isNull("ib_id")) {
        ib_id->setValue(q.value("ib_id"));
      }
    }
    openSuccessMessage(tr("Bookdata saved successfully!"));
    setResetModified(inputFields);
    return true;
  }
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
    if (it.value() == m_bookData->getValue(it.key()))
      continue;

    if (m_bookData->getType(it.key()).id() == QMetaType::QString) {
      set.append(it.key() + "='" + it.value().toString() + "'");
      changes++;
    } else {
      set.append(it.key() + "=" + it.value().toString());
      changes++;
    }
  }

  if(changes == 0) {
    openNoticeMessage(tr("No Modifications found, Update aborted!"));
    return;
  }

  /*
   * Artikel auf Deaktivierung prüfen!
   * Wenn sich die Anzahl geändert hat, ein Update senden!
   */
  int _curCount = ib_count->value().toInt();
  int _oldcount = m_bookData->getValue("ib_count").toInt();
  if (_oldcount != _curCount && _curCount == 0) {
    /*
     * Den Buchdaten Zwischenspeicher anpassen damit das Signal
     * an die Dienstleister nur einmal Aufgerufen wird!
     */
    m_bookData->setValue("ib_count", _curCount);

    // Ab diesen Zeitpunkt ist das Zurücksetzen erst mal nicht mehr gültig!
    m_actionBar->setRestoreable(false);

    // Senden Bestands Mitteilung an das Elternfenster
    emit sendArticleChanged(articleId, _curCount);
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
  /*
   * Bei einem INSERT die Anforderungen anpassen.
   */
  ib_count->setRequired(true);
  ib_id->setRequired(false);

  /*
   * Prüfung der Buchdaten Klasse
   * Die Initialisierung erfolgt in setInputFields!
   * Bei einem INSERT wir diese hier befüllt!
   */
  if (m_bookData == nullptr || !m_bookData->isValid()) {
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
    m_bookData->setValue(field, it.value());

    column.append(field);
    if (m_bookData->getType(field).id() == QMetaType::QString) {
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
    m_actionBar->setRestoreable(m_bookData->isValid());
    // Bildaktionen erst bei vorhandener Artikel Nummer freischalten!
    // m_imageToolBar->setActive(true);
    ib_id->setRequired(true);
  }
}

bool BookEditor::realyDeactivateEntry() {
  qint8 _curCount = ib_count->value().toInt();
  qint8 _oldcount = m_bookData->getValue("ib_count").toInt();
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
    ib_count->setValue(m_bookData->getValue("ib_count"));
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
    QStringList info(tr("Unsaved Changes"));
    info << tr("Don't leave this page before save your changes!");
    openNoticeMessage(info.join("\n"));
    return;
  }
  setFinalLeaveEditor();
}

void BookEditor::setFinalLeaveEditor() {
  setResetInputFields();
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  emit sendLeaveEditor();             /**< Zurück zur Hauptsansicht */
}

void BookEditor::setPrintBookCard() {
  // TODO
  qDebug() << Q_FUNC_INFO << "TODO";
}

void BookEditor::setRestore() {
  importSqlResult();
  setEnabled(true);
}

bool BookEditor::openEditEntry(qint64 articleId) {
  bool status = false;
  if (articleId < 1)
    return status;

  QString ib_id = QString::number(articleId);
  if (ib_id.isEmpty())
    return status;

  setInputFields();
  QString table = m_bookData->tableName();
  QString query("SELECT * FROM " + table + " WHERE ib_id=" + ib_id + ";");
  QSqlQuery q = m_sql->query(query);
  if (q.size() != 0) {
    QSqlRecord r = m_bookData->record();
    while (q.next()) {
      foreach (QString key, inputFields) {
        m_bookData->setValue(key, q.value(r.indexOf(key)));
      }
    }
    status = true;
  } else {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
    status = false;
  }

  if (status) {
    importSqlResult();
    setEnabled(true);
  }

  return status;
}

bool BookEditor::createNewEntry() {
  setInputFields();
  qDebug() << Q_FUNC_INFO << "TODO";
  setEnabled(true);
  return true;
}
