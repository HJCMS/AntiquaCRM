// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "bookseditor.h"
#include "booksconfig.h"

#include <AntiquaCRM>
#include <QDate>
#include <QLayout>

BooksEditor::BooksEditor(QWidget *parent)
    : AntiquaCRM::TabsEditor{BOOKS_SQL_EDITOR_PATTERN, parent} {
  setWindowTitle(tr("Edit Book"));
  setObjectName("tab_books_editor");

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("bookeditor_main_layout");
  mainLayout->setSizeConstraint(QLayout::SetMaximumSize);

  QString tempWhatsThis;
  AntiquaCRM::ALabel::Align _lbAlign = AntiquaCRM::ALabel::Align::MiddleRight;

  // Row 0
  QHBoxLayout *row0 = new QHBoxLayout();
  ib_id = new AntiquaCRM::SerialId(this);
  ib_id->setObjectName("ib_id");
  ib_id->setBuddyLabel(tr("Article ID"));
  tempWhatsThis = tr("Probably the most important parameter for the article "
                     "processing and is generated automatically when creating "
                     "a book. AntiquaCRM use a running numbering system.");
  ib_id->setWhatsThisText(tempWhatsThis);
  row0->addWidget(ib_id);

  ib_count = new AntiquaCRM::CrowdEdit(this);
  ib_count->setObjectName("ib_count");
  ib_count->setBuddyLabel(tr("Count"));
  tempWhatsThis =
      tr("The Article count is the main value for activating and deactivating "
         "books in your inventory. If you set it to 0, it will be disabled on "
         "your vendor pages and no longer visible for sale.\nNotes: These "
         "updates are not done directly, however daily performed from "
         "AntiquaCRM Server after working time.");
  ib_count->setWhatsThisText(tempWhatsThis);
  row0->addWidget(ib_count);

  double minPrice = m_cfg->value("payment/min_price", 5.00).toDouble();
  ib_price = new AntiquaCRM::PriceEdit(this);
  ib_price->setObjectName("ib_price");
  ib_price->setBuddyLabel(tr("Price"));
  ib_price->setMinimum(minPrice);
  tempWhatsThis =
      tr("The Book price you want to have for it.\nNotes: You can always set "
         "limits for Prices in the main Configuration dialog.");
  ib_price->setWhatsThisText(tempWhatsThis);
  row0->addWidget(ib_price);

  ib_signed = new AntiquaCRM::BoolBox(this);
  ib_signed->setObjectName("ib_signed");
  ib_signed->setBuddyLabel(tr("Signed Version"));
  tempWhatsThis = tr("Is this Book signed by Authors?");
  ib_signed->setWhatsThisText(tempWhatsThis);
  row0->addWidget(ib_signed);

  ib_restricted = new AntiquaCRM::BoolBox(this);
  ib_restricted->setObjectName("ib_restricted");
  ib_restricted->setBuddyLabel(tr("Local Usage only"));
  tempWhatsThis =
      tr("The Article will not exported to your Online Sellers when it is "
         "marked.\nNotes: You cannot do this afterwards, there are no remote "
         "deletion processes with your online retailer for this.");
  ib_restricted->setWhatsThisText(tempWhatsThis);
  row0->addWidget(ib_restricted);
  row0->addStretch(1);
  mainLayout->addLayout(row0, 0);

  // Row 1
  QHBoxLayout *row1 = new QHBoxLayout();
  ib_edition = new AntiquaCRM::NumEdit(this);
  ib_edition->setObjectName("ib_edition");
  ib_edition->setBuddyLabel(tr("Edition"));
  tempWhatsThis = tr("If known, set here the Book edition with "
                     "digits.\nExample: „first edition“ is equal to 1.");
  ib_edition->setWhatsThisText(tempWhatsThis);
  row1->addWidget(ib_edition);

  ib_pagecount = new AntiquaCRM::NumEdit(this);
  ib_pagecount->setObjectName("ib_pagecount");
  ib_pagecount->setContextMenuPolicy(Qt::DefaultContextMenu);
  ib_pagecount->setRange(10, 99999);
  ib_pagecount->setSingleStep(10);
  ib_pagecount->setBuddyLabel(tr("Page count"));
  tempWhatsThis = tr("Add here the Book page count.");
  ib_pagecount->setWhatsThisText(tempWhatsThis);
  row1->addWidget(ib_pagecount);

  ib_weight = new AntiquaCRM::NumEdit(this);
  ib_weight->setObjectName("ib_weight");
  ib_weight->setRange(100, 9999999);
  ib_weight->setSuffix(tr("g"));
  ib_weight->setBuddyLabel(tr("Weight"));
  tempWhatsThis = tr("Add here the Book weight in grams.");
  ib_weight->setWhatsThisText(tempWhatsThis);
  row1->addWidget(ib_weight);

  ib_year = new AntiquaCRM::YearEdit(this);
  ib_year->setObjectName("ib_year");
  ib_year->setBuddyLabel(tr("Year"));
  tempWhatsThis =
      tr("The Book Year input mask starts with 1400 and ending one Year in the "
         "future. If the Book edition Year is unknown, set an estimate and add "
         "a note in the description section.");
  ib_year->setWhatsThisText(tempWhatsThis);
  row1->addWidget(ib_year);

  ib_volume = new AntiquaCRM::NumEdit(this);
  ib_volume->setObjectName("ib_volume");
  ib_volume->setPrefix(tr("Bd."));
  ib_volume->setInputToolTip(tr("Book volume"));
  ib_volume->setBuddyLabel(tr("Volume"));
  tempWhatsThis = tr("Is this Book a part of a Book volume or the Article "
                     "contains one or more volumes? Then enter this here.");
  ib_volume->setWhatsThisText(tempWhatsThis);
  row1->addWidget(ib_volume);
  row1->addStretch(1);
  mainLayout->addLayout(row1);

  // Row 2
  m_splitter = new AntiquaCRM::Splitter(this);
  QWidget *row2Widget = new QWidget(m_splitter);
  row2Widget->setContentsMargins(0, 0, 0, 0);
  int row2c = 0;
  QGridLayout *row2 = new QGridLayout(row2Widget);
  row2->setContentsMargins(0, 0, 0, 0);

  AntiquaCRM::ALabel *infoText;
  infoText = new AntiquaCRM::ALabel(tr("Title"), _lbAlign, row2Widget);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ib_title = new AntiquaCRM::TextLine(this);
  ib_title->setObjectName("ib_title");
  ib_title->setInputToolTip(tr("Book title"));
  tempWhatsThis =
      tr("A Book title is restricted to 80 Characters. Some Online Providers "
         "only support this title length. Other Providers allowing more "
         "characters in Book titles. To get around this, the following "
         "Subtitle field will appended seperated by „-“ to the Book title when "
         "exporting the books data to your online shop providers.");
  ib_title->setWhatsThisText(tempWhatsThis);
  row2->addWidget(ib_title, row2c++, 1, 1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Subtitle"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ib_title_extended = new AntiquaCRM::TextLine(this);
  ib_title_extended->setObjectName("ib_title_extended");
  ib_title_extended->setInputToolTip(tr("Book subtitle"));
  // Warnung: Verwende „tempWhatsThis“ von ib_title!
  ib_title_extended->setWhatsThisText(tempWhatsThis);
  row2->addWidget(ib_title_extended, row2c++, 1, 1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Author"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ib_author = new AntiquaCRM::TextLine(this);
  ib_author->setObjectName("ib_author");
  ib_author->setInputToolTip(tr("Book Authors"));
  tempWhatsThis = tr("This field is reserved for Book Authors.\n"
                     "You can add more separated by comma.\n"
                     "e.g.: Karl Valentin, Franz Kafka\n"
                     "Some provider Platforms supporting spezial Keywords.\n"
                     "AntiquaCRM suggests the most common author group names, "
                     "when editing this field.");
  ib_author->setWhatsThisText(tempWhatsThis);
  row2->addWidget(ib_author, row2c++, 1, 1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Publisher"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ib_publisher = new AntiquaCRM::TextLine(this);
  ib_publisher->setObjectName("ib_publisher");
  ib_publisher->setInputToolTip(infoText->text());
  tempWhatsThis = tr("This field is reserved to insert Book Publishers.\n"
                     "Used Format is: „Publisher, Location“\n"
                     "Example: J.F. Lehmanns Verlag, Munich\n"
                     "AntiquaCRM suggests the most common publisher names, "
                     "when editing this field.");
  ib_publisher->setWhatsThisText(tempWhatsThis);
  row2->addWidget(ib_publisher, row2c++, 1, 1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Condition"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  QHBoxLayout *conditionlayout = new QHBoxLayout();
  ib_condition = new AntiquaCRM::ConditionEdit(this);
  ib_condition->setObjectName("ib_condition");
  ib_condition->setToolTip(tr("Book condition"));
  tempWhatsThis =
      tr("The Book condition is predefined by your Book Seller online shop.\n"
         "Mostly defined with 1-4, very good to heavily worn.");
  ib_condition->setWhatsThisText(tempWhatsThis);
  ib_condition->appendStretch();
  conditionlayout->addWidget(ib_condition, Qt::AlignLeft);

  infoText = new AntiquaCRM::ALabel(tr("Language"), _lbAlign, this);
  conditionlayout->addWidget(infoText, Qt::AlignRight);
  ib_language = new AntiquaCRM::SelectLanguage(this);
  ib_language->setObjectName("ib_language");
  ib_language->setInputToolTip(tr("Book content language"));
  tempWhatsThis = tr("This field is reserved to the content language of the "
                     "book. If Multilingual content select Europe.");
  ib_language->setWhatsThisText(tempWhatsThis);
  conditionlayout->addWidget(ib_language);
  row2->addLayout(conditionlayout, row2c++, 1, 1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Binding"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  // Bucheinband und Bindungsart
  QHBoxLayout *binding_layout = new QHBoxLayout();
  binding_layout->setContentsMargins(0, 0, 0, 0);
  ib_binding = new AntiquaCRM::BookBindingEdit(this);
  ib_binding->setObjectName("ib_binding");
  ib_binding->setInputToolTip(tr("Bookbinding"));
  tempWhatsThis =
      tr("The Bookbinding selecter  contains the most prefinied Book bindings. "
         "Please select the best Category for this Book.");
  ib_binding->setWhatsThisText(tempWhatsThis);
  binding_layout->addWidget(ib_binding);

  ib_designation = new AntiquaCRM::TextLine(this);
  ib_designation->setObjectName("ib_designation");
  ib_designation->setInputToolTip(tr("Book binding and cover condition."));
  tempWhatsThis =
      tr("This Field is reserved to add Book binding Designations. It is a "
         "addition for detailed Description of the Book cover. You can edit "
         "predefined Designations in your Database Configuration Menu.");
  ib_designation->setWhatsThisText(tempWhatsThis);
  binding_layout->addWidget(ib_designation);
  row2->addLayout(binding_layout, row2c++, 1, 1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Storage"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ib_storage = new AntiquaCRM::SelectStorage(this);
  ib_storage->setObjectName("ib_storage");
  ib_storage->setInputToolTip(tr("Storage location"));
  tempWhatsThis = tr("The Storage location where this Book has been stored. "
                     "You need to Configure Storage locations first in your "
                     "Database Configuration Menu before you can use it.");
  ib_storage->setWhatsThisText(tempWhatsThis);
  row2->addWidget(ib_storage, row2c++, 1, 1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Storage"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ib_storage_compartment = new AntiquaCRM::TextLine(this);
  ib_storage_compartment->setObjectName("ib_storage_compartment");
  ib_storage_compartment->setInputToolTip(tr("Storage compartment"));
  ib_storage_compartment->setBuddyLabel(tr("compartment"));
  tempWhatsThis =
      tr("Define Storage compartment in this Input field. It depends on the "
         "Storage location and will printed on top of the Book card. If you "
         "select Storage location first a completer will add for this input "
         "field.");
  ib_storage_compartment->setWhatsThisText(tempWhatsThis);
  ib_storage_compartment->appendStretch();
  row2->addWidget(ib_storage_compartment, row2c++, 1, 1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Keywords"), _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  ib_keyword = new AntiquaCRM::KeywordsEdit(this);
  ib_keyword->setObjectName("ib_keyword");
  tempWhatsThis = tr(
      "Keywords will help Buyers, to find your Article on Provider pages. This "
      "Articles keyword management can also be compared to „Search Engine "
      "Optimization“ (SEO).\n"
      "Restrictions:\n"
      "- A Keyword must have a minimum length from 3 characters.\n"
      "- A Keywords list is restricted to a Maximum length of 60 Characters.\n"
      "- A keyword cannot contain spaces or non unicode special characters.\n"
      "You can edit predefined Keywords in your Database Configuration Menu.");
  ib_keyword->setWhatsThisText(tempWhatsThis);
  row2->addWidget(ib_keyword, row2c++, 1, 1, 1);
  row2->setRowStretch(row2c++, 1);

  // @BEGIN_GROUP
  infoText = new AntiquaCRM::ALabel("ISBN/EAN", _lbAlign, this);
  row2->addWidget(infoText, row2c, 0, 1, 1);
  QHBoxLayout *tbLayout = new QHBoxLayout;
  ib_isbn = new AntiquaCRM::IsbnEdit(this);
  ib_isbn->setObjectName("ib_isbn");
  tbLayout->addWidget(ib_isbn);
  tbLayout->addStretch(1);
  m_imageToolBar = new AntiquaCRM::ImageToolBar(this);
  tbLayout->addWidget(m_imageToolBar);
  // @END_GROUP
  row2->addLayout(tbLayout, row2c++, 1, 1, 1);
  row2Widget->setLayout(row2);

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
  ib_description = new AntiquaCRM::TextField(m_tabWidget);
  ib_description->setObjectName("ib_description");
  m_tabWidget->insertTab(0, ib_description, tabIcons, tr("Public Description"));
  // Internal Description
  ib_internal_description = new AntiquaCRM::TextField(m_tabWidget);
  ib_internal_description->setObjectName("ib_internal_description");
  m_tabWidget->insertTab(1, ib_internal_description, tabIcons,
                         tr("Internal Description"));
  // Info Tab
  QWidget *m_infos = new QWidget(this);
  QGridLayout *infoLayout = new QGridLayout(m_infos);
  infoLayout->setColumnStretch(1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Created"), _lbAlign, m_infos);
  infoLayout->addWidget(infoText, 0, 0, 1, 1);
  ib_since = new AntiquaCRM::DateTimeInfo(m_infos);
  ib_since->setObjectName("ib_since");
  infoLayout->addWidget(ib_since, 0, 1, 1, 1);

  infoText = new AntiquaCRM::ALabel(tr("Changed"), _lbAlign, m_infos);
  infoLayout->addWidget(infoText, 1, 0, 1, 1);
  ib_changed = new AntiquaCRM::DateTimeInfo(m_infos);
  ib_changed->setObjectName("ib_changed");
  infoLayout->addWidget(ib_changed, 1, 1, 1, 1);

  infoLayout->setRowStretch(2, 1);
  m_infos->setLayout(infoLayout);
  m_tabWidget->insertTab(2, m_infos, tabIcons, tr("Information"));
  mainLayout->addWidget(m_tabWidget);

  m_actionBar = new AntiquaCRM::TabsEditActionBar(this);
  m_actionBar->setViewMailButton(false);
  m_actionBar->setPrinterMenu(AntiquaCRM::PRINT_BOOKCARD);
  mainLayout->addWidget(m_actionBar);

  setLayout(mainLayout);

  // Signals:ImageToolBar
  connect(m_imageToolBar, SIGNAL(sendDeleteImage(qint64)),
          SLOT(setRemoveThumbnail(qint64)));
  connect(m_imageToolBar, SIGNAL(sendOpenImage()), SLOT(setImportEditImage()));

  // Signals::Storage
  connect(ib_storage, SIGNAL(sendValueChanged()),
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

BooksEditor::~BooksEditor() {
  ignoreFields.clear();
  inputFields.clear();
  if (m_sql != nullptr)
    m_sql->deleteLater();

  if (m_cfg != nullptr)
    m_cfg->deleteLater();
}

void BooksEditor::setInputFields() {
  // Bei UPDATE/INSERT Ignorieren
  ignoreFields << "ib_since";
  ignoreFields << "ib_changed";
  ignoreFields << "ib_including_vat"; /**< @deprecated */

  m_tableData = new AntiquaCRM::ASqlDataQuery(BOOKS_SQL_TABLE_NAME);
  inputFields = m_tableData->columnNames();
  if (inputFields.isEmpty()) {
    QStringList warn(tr("An error has occurred!"));
    warn << tr("Can't load input datafields!");
    warn << tr("When getting this Message, please check your Network and "
               "Database connection!");
    openNoticeMessage(warn.join("\n"));
  }

  // Autoren
  QStringList authors(tr("Authors group"));
  authors << tr("Authors team");
  authors << tr("Various authors");
  ib_author->setCompleterList(authors);

  AntiquaCRM::ASharedDataFiles _dataFiles;
  QStringList _completer_data;

  // Herausgeber
  _completer_data = _dataFiles.getCompleterList("publishers", "name");
  ib_publisher->setCompleterList(_completer_data);

  // Buch Zustand
  ib_binding->initData();
  _completer_data = _dataFiles.getCompleterList("designations", "name");
  ib_designation->setCompleterList(_completer_data);

  // Lager
  ib_storage->initData();

  // Schlüsselwörter
  _completer_data = _dataFiles.getCompleterList("keywords", "name");
  ib_keyword->setCompleterList(_completer_data);

  _completer_data.clear();
}

bool BooksEditor::setDataField(const QSqlField &field, const QVariant &value) {
  if (!field.isValid())
    return false;

  QString key = field.name();
  // qDebug() << "setDataField:" << field.name() << value;
  bool required = (field.requiredStatus() == QSqlField::Required);
  AntiquaCRM::AbstractInput *inp =
      findChild<AntiquaCRM::AbstractInput *>(key, Qt::FindChildrenRecursively);
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

void BooksEditor::importSqlResult() {
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
  qint64 id = ib_id->getValue().toLongLong();
  if (id > 0)
    m_imageToolBar->setArticleId(id);

  m_actionBar->setRestoreable(m_tableData->isValid());
  setResetModified(inputFields);
}

bool BooksEditor::sendSqlQuery(const QString &query) {
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

const QHash<QString, QVariant> BooksEditor::createSqlDataset() {
  QHash<QString, QVariant> data;
  QList<AntiquaCRM::AbstractInput *> list =
      findChildren<AntiquaCRM::AbstractInput *>(fieldPattern,
                                                Qt::FindChildrenRecursively);
  QList<AntiquaCRM::AbstractInput *>::Iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    AntiquaCRM::AbstractInput *cur = *it;
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

void BooksEditor::createSqlUpdate() {
  int articleId = ib_id->getValue().toInt();
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
  int cur_count = ib_count->getValue().toInt();
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
  sql.append(ib_id->getValue().toString());
  sql.append(";");
  if (sendSqlQuery(sql)) {
    qInfo("SQL UPDATE Inventory Books success!");
    setWindowModified(false);
  }
}

void BooksEditor::createSqlInsert() {
  int articleId = ib_id->getValue().toInt();
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
  if (sendSqlQuery(sql) && ib_id->getValue().toInt() >= 1) {
    qInfo("SQL INSERT Inventory Books success!");
    // Zurücksetzen Knopf Aktivieren?
    m_actionBar->setRestoreable(m_tableData->isValid());
    // Bildaktionen erst bei vorhandener Artikel Nummer freischalten!
    m_imageToolBar->setArticleId(ib_id->getValue().toInt());
    ib_id->setRequired(true);
    setWindowModified(false);
  }
}

bool BooksEditor::realyDeactivateEntry() {
  qint8 _curCount = ib_count->getValue().toInt();
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

void BooksEditor::setSaveData() {
  if (ib_id->getValue().toString().isEmpty()) {
    createSqlInsert();
    return;
  }

  if (ib_count->getValue().toInt() == 0 && !realyDeactivateEntry())
    return;

  createSqlUpdate();
}

void BooksEditor::setCheckLeaveEditor() {
  if (checkIsModified()) {
    unsavedChangesPopup();
    return;
  }
  setFinalLeaveEditor();
}

void BooksEditor::setFinalLeaveEditor(bool force) {
  if (force) // Wenn auf Abbrechen geklickt wurde!
    setWindowModified(false);

  setResetInputFields();
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  m_thumbnail->clear();               /**< Bildvorschau leeren */
  emit sendLeaveEditor();             /**< Back to MainView */
}

void BooksEditor::setStorageCompartments() {
  QStringList _list = ib_storage->getCompartments();
  if (_list.size() > 0)
    ib_storage_compartment->setCompleterList(_list);

  _list.clear();
}

void BooksEditor::setPrintBookCard() {
  qDebug() << Q_FUNC_INFO << "__TODO__";
  // BookCard *m_d = new BookCard(this);
  // m_d->setObjectName("book_card_printing");
  // QHash<QString, QVariant> data;
  // data.insert("id", getDataValue("ib_id"));
  // data.insert("title", getDataValue("ib_title"));
  // data.insert("author", getDataValue("ib_author"));
  // data.insert("year", getDataValue("ib_year"));
  // data.insert("storage", getDataValue("ib_storage"));
  // data.insert("compartment", getDataValue("ib_storage_compartment"));
  // data.insert("since", getDataValue("ib_since"));
  // data.insert("keywords", getDataValue("ib_keyword"));
  // m_d->exec(data);
}

void BooksEditor::setLoadThumbnail(qint64 articleId) {
  if (articleId < 1)
    return;

  m_imageToolBar->setArticleId(articleId);

  AntiquaCRM::ImageFileSource image_preview;
  if (image_preview.findInDatabase(m_sql, articleId))
    m_thumbnail->setPixmap(image_preview.getThumbnail());
}

void BooksEditor::setRemoveThumbnail(qint64 articleId) {
  qint64 _id = ib_id->getValue().toLongLong();
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

void BooksEditor::setImportEditImage() {
  qint64 _id = ib_id->getValue().toLongLong();
  if (_id < 1) {
    pushStatusMessage(tr("No Article number for Image import!"));
    return;
  }

  AntiquaCRM::ImageImportDialog *d =
      new AntiquaCRM::ImageImportDialog(_id, "Books", this);
  connect(d, SIGNAL(sendThumbnail(const QPixmap &)), m_thumbnail,
          SLOT(setPixmap(const QPixmap &)));

  d->exec();
  d->deleteLater();
}

void BooksEditor::setRestore() {
  importSqlResult();
  ib_count->setFocus();
}

bool BooksEditor::openEditEntry(qint64 articleId) {
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

bool BooksEditor::createNewEntry() {
  setInputFields();
  m_thumbnail->clear();
  foreach (QString column, m_tableData->columnNames()) {
    QSqlField field = m_tableData->getProperties(column);
    if (column == "ib_id")
      field.setRequired(false);

    setDefaultInput(field);
  }
  setResetModified(inputFields);
  registerInputChanged();
  return isEnabled();
}
