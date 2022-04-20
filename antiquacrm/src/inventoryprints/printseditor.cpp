// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "printseditor.h"
#include "applsettings.h"
#include "articleid.h"
#include "boolbox.h"
#include "editoractionbar.h"
#include "imagedialog.h"
#include "imagewidget.h"
#include "intspinbox.h"
#include "messagebox.h"
#include "priceedit.h"
#include "sqlcore.h"
#include "storageedit.h"
#include "strlineedit.h"
#include "textfield.h"
#include "version.h"
#include "yearedit.h"

#include <QtCore>
#include <QtGui/QDesktopServices>
#include <QtWidgets>

PrintsEditor::PrintsEditor(QWidget *parent) : QWidget{parent} {
  setObjectName("PrintEditor");
  setMinimumSize(800, 600);

  ApplSettings config;

  db = new HJCMS::SqlCore(this);

  Qt::Alignment defaultAlignment =
      (Qt::AlignRight | Qt::AlignTrailing | Qt::AlignVCenter);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("print_editor_layout");

  // Begin Horizontal:Row(0)
  QHBoxLayout *horizontalLayout = new QHBoxLayout();
  horizontalLayout->setObjectName("horizontalLayout");

  // Begin gridLayout
  //  Zeile 0
  QGridLayout *gridLayout = new QGridLayout();
  gridLayout->setObjectName("gridLayout");
  int glc = 0; /**< Gridlayout Row Counter */

  // Begin ErsteZeile
  QHBoxLayout *lay1 = new QHBoxLayout();
  lay1->setObjectName("lay1");

  ip_id = new ArticleID(this);
  ip_id->setObjectName("ip_id");
  ip_id->setRequired(true);

  lay1->addWidget(ip_id);

  QLabel *ibCountLabel = new QLabel(this);
  ibCountLabel->setObjectName("ip_countLabel");
  ibCountLabel->setAlignment(defaultAlignment);
  ibCountLabel->setText(tr("Count:"));

  lay1->addWidget(ibCountLabel);

  ip_count = new IntSpinBox(this);
  ip_count->setObjectName("ip_count");
  ip_count->setWindowTitle(tr("Count"));

  lay1->addWidget(ip_count);

  QLabel *ibPriceLabel = new QLabel(this);
  ibPriceLabel->setObjectName("ip_priceLabel");
  ibPriceLabel->setText(tr("Price:"));

  lay1->addWidget(ibPriceLabel);

  double minPrice = config.value("books/min_price", 8.00).toDouble();
  ip_price = new PriceEdit(this);
  ip_price->setObjectName("ip_price");
  ip_price->setRequired(true);
  ip_price->setMinimum(minPrice);

  lay1->addWidget(ip_price);

  ip_kolorit = new BoolBox(this);
  ip_kolorit->setObjectName("ip_kolorit");
  ip_kolorit->setText(tr("Signed Version"));

  lay1->addWidget(ip_kolorit);

  ip_restricted = new BoolBox(this);
  ip_restricted->setObjectName("ip_restricted");
  ip_restricted->setText(tr("Restricted Sale"));
  ip_restricted->setToolTip(
      tr("Is the title not for sale nationally or is it on a censorship list. "
         "This is relevant for the Shopsystem."));

  lay1->addWidget(ip_restricted);

  QLabel *ibYearLabel = new QLabel(this);
  ibYearLabel->setObjectName("ip_yearLabel");
  ibYearLabel->setAlignment(defaultAlignment);
  ibYearLabel->setText(tr("Year:"));

  lay1->addWidget(ibYearLabel);

  ip_year = new YearEdit(this);
  ip_year->setObjectName("ip_year");
  ip_year->setWindowTitle(tr("Year"));
  ip_year->setRequired(true);

  lay1->addWidget(ip_year);

  lay1->addStretch(1);

  gridLayout->addLayout(lay1, glc++, 0, 1, 2);
  // End ErsteZeile

  QHBoxLayout *storageLayout = new QHBoxLayout();
  ip_storage = new StorageEdit(this);
  ip_storage->setObjectName("ip_storage");
  ip_storage->setMinimumWidth(300);
  storageLayout->addWidget(ip_storage);
  storageLayout->addStretch(1);
  gridLayout->addLayout(storageLayout, glc++, 0, 1, 2);

  QLabel *ibTitleLabel = new QLabel(this);
  ibTitleLabel->setObjectName("ip_titleLabel");
  ibTitleLabel->setAlignment(defaultAlignment);
  ibTitleLabel->setText(tr("Book &Title:"));

  gridLayout->addWidget(ibTitleLabel, glc, 0, 1, 1);

  ip_title = new StrLineEdit(this);
  ip_title->setObjectName("ip_title");
  ip_title->setMaxAllowedLength(80);
  ip_title->setRequired(true);
  ip_title->setWindowTitle(tr("Booktitle"));
  ip_title->setToolTip(tr("Required input field. Limited to 80 characters, "
                          "Webshop Systems require this."));
  ibTitleLabel->setBuddy(ip_title);

  gridLayout->addWidget(ip_title, glc++, 1, 1, 1);

  // Zeile 1
  QLabel *ibExtendedLabel = new QLabel(this);
  ibExtendedLabel->setObjectName("ip_extendedLabel");
  ibExtendedLabel->setAlignment(defaultAlignment);
  ibExtendedLabel->setText(tr("Book Title Extended:"));

  gridLayout->addWidget(ibExtendedLabel, glc, 0, 1, 1);

  ip_title_extended = new StrLineEdit(this);
  ip_title_extended->setObjectName("ip_title_extended");
  ip_title_extended->setMaxAllowedLength(148);
  ip_title_extended->setToolTip(tr("Extended Title or Book Subtitle."));
  ip_title_extended->setWindowTitle(tr("Extended Title"));

  gridLayout->addWidget(ip_title_extended, glc++, 1, 1, 1);

  // Zeile 2
  QLabel *ibAuthorLabel = new QLabel(this);
  ibAuthorLabel->setObjectName("ip_authorLabel");
  ibAuthorLabel->setAlignment(defaultAlignment);
  ibAuthorLabel->setText(tr("&Author:"));

  gridLayout->addWidget(ibAuthorLabel, glc, 0, 1, 1);

  ip_author = new StrLineEdit(this);
  ip_author->setObjectName("ip_author");
  ip_author->setMaxAllowedLength(128);
  ip_author->setWindowTitle(tr("Author"));
  ip_author->setToolTip(
      tr("Format: Firstname lastname (Different Authors separated by comma)."));
  ibAuthorLabel->setBuddy(ip_author);

  gridLayout->addWidget(ip_author, glc++, 1, 1, 1);

  // Zeile 3
  QLabel *ibPublisherLabel = new QLabel(this);
  ibPublisherLabel->setObjectName("ip_techniqueLabel");
  ibPublisherLabel->setAlignment(defaultAlignment);
  ibPublisherLabel->setText(tr("Technique:"));
  ibPublisherLabel->setToolTip(tr("Enter the technique."));

  gridLayout->addWidget(ibPublisherLabel, glc, 0, 1, 1);

  ip_technique = new StrLineEdit(this);
  ip_technique->setObjectName("ip_technique");
  ip_technique->setMaxAllowedLength(128);
  ip_technique->setWindowTitle(tr("Publisher"));

  gridLayout->addWidget(ip_technique, glc++, 1, 1, 1);

  // Zeile 4
  QLabel *ibKeywordLabel = new QLabel(this);
  ibKeywordLabel->setObjectName("ip_formatLabel");
  ibKeywordLabel->setAlignment(defaultAlignment);
  ibKeywordLabel->setText(tr("Format:"));

  gridLayout->addWidget(ibKeywordLabel, glc, 0, 1, 1);

  ip_format = new StrLineEdit(this);
  ip_format->setObjectName("ip_format");
  ip_format->setMaxAllowedLength(60);
  ip_format->setToolTip(tr("Category Keywords for Shopsystems."));
  ip_format->setWindowTitle(tr("Chop Keyword"));

  gridLayout->addWidget(ip_format, glc++, 1, 1, 1);

  // Zeile 5
  QLabel *ibConditionLabel = new QLabel(this);
  ibConditionLabel->setObjectName("ip_conditionLabel");
  ibConditionLabel->setAlignment(defaultAlignment);
  ibConditionLabel->setText(tr("Condition:"));

  gridLayout->addWidget(ibConditionLabel, glc, 0, 1, 1);

  ip_condition = new StrLineEdit(this);
  ip_condition->setObjectName("ip_condition");
  ip_condition->setMaxAllowedLength(128);
  ip_condition->setWindowTitle(tr("Condition"));
  ip_condition->setToolTip(
      tr("Condition of this Book. See also Configuration conditions Table."));

  gridLayout->addWidget(ip_condition, glc++, 1, 1, 1);

  // Zeile 6
  QLabel *ibDesignationLabel = new QLabel(this);
  ibDesignationLabel->setObjectName("ip_designationLabel");
  ibDesignationLabel->setAlignment(defaultAlignment);
  ibDesignationLabel->setText(tr("Designation:"));

  gridLayout->addWidget(ibDesignationLabel, glc, 0, 1, 1);

  ip_designation = new StrLineEdit(this);
  ip_designation->setObjectName("ip_designation");
  ip_designation->setMaxAllowedLength(128);
  ip_designation->setWindowTitle(tr("Designation"));

  gridLayout->addWidget(ip_designation, glc++, 1, 1, 1);

  QHBoxLayout *lay5 = new QHBoxLayout();
  lay5->setObjectName("last_horizontal_layout");

  btn_createJob =
      new QPushButton(myIcon("autostart"), tr("Create order"), this);
  btn_createJob->setObjectName("CreateJobFromThis");
  btn_createJob->setToolTip(tr("Create a purchase order from this listing."));
  btn_createJob->setEnabled(false);
  lay5->addWidget(btn_createJob);

  lay5->addStretch(1);

  btn_imaging = new QPushButton(myIcon("image"), tr("Picture"), this);
  btn_imaging->setObjectName("OpenImagingButton");
  btn_imaging->setToolTip(
      tr("Open the Imaging Dialog for Import and Edit Pictures."));
  lay5->addWidget(btn_imaging);

  gridLayout->addLayout(lay5, glc++, 0, 1, 2);
  horizontalLayout->addLayout(gridLayout);
  // END gridLayout

  m_imageView = new ImageWidget(this);
  m_imageView->setObjectName("prints_image_view");
  m_imageView->setMinimumWidth(200);
  horizontalLayout->addWidget(m_imageView);
  // END Horizontal:Row(0)

  mainLayout->addLayout(horizontalLayout);

  m_tabWidget = new QTabWidget(this);
  m_tabWidget->setObjectName("TabWidget");

  ip_description = new TextField(this);
  ip_description->setObjectName("ip_description");
  ip_description->setWindowTitle(tr("Public Description"));
  m_tabWidget->insertTab(0, ip_description, tr("Public Description"));
  m_tabWidget->setTabIcon(0, myIcon("edit"));
  m_tabWidget->setTabToolTip(0, tr("This text is displayed on web pages"));

  ip_internal_description = new TextField(this);
  ip_internal_description->setObjectName("ip_internal_description");
  ip_description->setWindowTitle(tr("Internal Description"));
  m_tabWidget->insertTab(1, ip_internal_description,
                         tr("Internal Description"));
  m_tabWidget->setTabIcon(1, myIcon("edit"));
  m_tabWidget->setTabToolTip(1, tr("This text is for internal purposes"));

  mainLayout->addWidget(m_tabWidget);

  m_actionBar = new EditorActionBar(this);
  mainLayout->addWidget(m_actionBar);

  connect(btn_imaging, SIGNAL(clicked()), this, SLOT(openImageDialog()));
  connect(m_actionBar, SIGNAL(s_cancelClicked()), this,
          SLOT(finalLeaveEditor()));
  connect(m_actionBar, SIGNAL(s_restoreClicked()), this,
          SLOT(restoreDataset()));
  connect(m_actionBar, SIGNAL(s_saveClicked()), this, SLOT(saveData()));
  connect(m_actionBar, SIGNAL(s_finishClicked()), this,
          SLOT(checkLeaveEditor()));

  setInputList();
}

void PrintsEditor::setInputList() {
  QList<QObject *> list =
      findChildren<QObject *>(p_objPattern, Qt::FindChildrenRecursively);
  for (int i = 0; i < list.size(); ++i) {
    if (list.at(i) != nullptr) {
      if (!list.at(i)->objectName().isEmpty())
        inputList << list.at(i)->objectName();
    }
  }
}

void PrintsEditor::openImageDialog() {
  qulonglong id = ip_id->value().toLongLong();
  ApplSettings cfg;
  QString p = cfg.value("imaging/sourcepath").toString();
  ImageDialog *dialog = new ImageDialog(id, this);
  if (!dialog->setSourceTarget(p)) {
    qDebug() << Q_FUNC_INFO << id << "Invalid Source Target:" << p;
    return;
  }
  if (id >= 1)
    emit s_openImageEditor(id);

  if (dialog->exec()) {
    QImage img = dialog->getImage();
    if (!img.isNull())
      m_imageView->addNewImage(id, img);
  }
}

void PrintsEditor::resetModified() {
  foreach (QString name, inputList) {
    QObject *child = findChild<QObject *>(name, Qt::FindChildrenRecursively);
    if (child != nullptr) {
      QMetaObject::invokeMethod(child, "setModified", Qt::DirectConnection,
                                Q_ARG(bool, false));
    }
  }
}

bool PrintsEditor::sendSqlQuery(const QString &sqlStatement) {
  MessageBox msgBox(this);
  QSqlQuery q = db->query(sqlStatement);
  if (q.lastError().type() != QSqlError::NoError) {
    QString errorString = db->fetchErrors();
    qDebug() << errorString << Qt::endl;
    msgBox.queryFail(sqlStatement, errorString);
    return false;
  } else {
    msgBox.querySuccess(tr("Data saved successfully!"), 1);
    resetModified();
    return true;
  }
}

const QHash<QString, QVariant> PrintsEditor::createSqlDataset() {
  QHash<QString, QVariant> data;
  MessageBox messanger;
  QList<StrLineEdit *> listStr =
      findChildren<StrLineEdit *>(p_objPattern, Qt::FindDirectChildrenOnly);
  QList<StrLineEdit *>::Iterator i_str;
  for (i_str = listStr.begin(); i_str != listStr.end(); ++i_str) {
    StrLineEdit *cur = *i_str;
    if (cur->isRequired() && !cur->isValid()) {
      messanger.noticeMessage(cur->notes());
      cur->setFocus();
      data.clear();
      return data;
    }
    if (cur->value().toString().isEmpty())
      continue;

    data.insert(cur->objectName(), cur->value());
  }
  listStr.clear();
  QList<IntSpinBox *> listInt =
      findChildren<IntSpinBox *>(p_objPattern, Qt::FindDirectChildrenOnly);
  QList<IntSpinBox *>::Iterator i_int;
  for (i_int = listInt.begin(); i_int != listInt.end(); ++i_int) {
    IntSpinBox *cur = *i_int;
    if (cur->isRequired() && !cur->isValid()) {
      messanger.noticeMessage(cur->notes());
      cur->setFocus();
      data.clear();
      return data;
    }
    if (cur->value().toInt() == 0)
      continue;

    data.insert(cur->objectName(), cur->value());
  }
  listInt.clear();
  if (ip_kolorit->isChecked()) {
    data.insert("ip_kolorit", ip_kolorit->value());
  }
  if (ip_restricted->isChecked()) {
    data.insert("ip_restricted", ip_restricted->value());
  }
  if (ip_description->isValid()) {
    data.insert("ip_description", ip_description->value());
  }
  if (ip_internal_description->isValid()) {
    data.insert("ip_internal_description", ip_internal_description->value());
  }
  if (!ip_year->isValid()) {
    messanger.noticeMessage(ip_year->notes());
    ip_year->setFocus();
    data.clear();
    return data;
  } else {
    data.insert("ip_year", ip_year->value());
  }
  if (ip_storage->isValid()) {
    data.insert("ip_storage", ip_storage->value());
  } else {
    messanger.noticeMessage(ip_storage->notes());
    ip_storage->setFocus();
    data.clear();
    return data;
  }
  if (ip_price->isValid()) {
    data.insert("ip_price", ip_price->value());
  } else {
    messanger.noticeMessage(ip_price->notes());
    ip_price->setFocus();
    data.clear();
    return data;
  }
  return data;
}

void PrintsEditor::createSqlUpdate() {
  if (!ip_id->isValid()) {
    emit s_sendMessage(tr("Missing Article ID for Update."));
    return;
  }

  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 1)
    return;

  QStringList set;
  QHash<QString, QVariant>::iterator it;
  for (it = data.begin(); it != data.end(); ++it) {
    if (it.value().type() == QVariant::String) {
      set.append(it.key() + "='" + it.value().toString() + "'");
    } else {
      set.append(it.key() + "=" + it.value().toString());
    }
  }

  QString sql("UPDATE inventory_prints SET ");
  sql.append(set.join(","));
  sql.append(",ip_changed=CURRENT_TIMESTAMP WHERE ip_id=");
  sql.append(ip_id->value().toString());
  sql.append(";");
  sendSqlQuery(sql);
}

void PrintsEditor::createSqlInsert() {
  /** Bei neu Einträgen immer erforderlich */
  ip_count->setRequired(true);

  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 1)
    return;

  QStringList column; /**< SQL Columns */
  QStringList values; /**< SQL Values */
  QHash<QString, QVariant>::iterator it;
  for (it = data.begin(); it != data.end(); ++it) {
    if (it.value().toString().isEmpty())
      continue;

    column.append(it.key());
    if (it.value().type() == QVariant::String) {
      values.append("'" + it.value().toString() + "'");
    } else {
      values.append(it.value().toString());
    }
  }

  QString sql("INSERT INTO inventory_prints (");
  sql.append(column.join(","));
  sql.append(",ip_changed) VALUES (");
  sql.append(values.join(","));
  sql.append(",CURRENT_TIMESTAMP);");
  // qDebug() << Q_FUNC_INFO << sql << Qt::endl;
  if (sendSqlQuery(sql))
    checkLeaveEditor();
}

void PrintsEditor::importSqlResult() {
  if (sqlQueryResult.size() < 15)
    return;

  blockSignals(true);
  for (int i = 0; i < sqlQueryResult.size(); ++i) {
    DataEntries f = sqlQueryResult.at(i);
    setSqlQueryData(f.field, f.data);
  }
  blockSignals(false);

  // Nach Ersteintrag zurück setzen!
  resetModified();

  // Suche Bilddaten
  int id = ip_id->value().toInt();
  if (id > 0)
    m_imageView->searchImageById(id);
}

void PrintsEditor::clearDataFields() {
  QList<QObject *> list =
      findChildren<QObject *>(p_objPattern, Qt::FindChildrenRecursively);
  for (int i = 0; i < list.size(); ++i) {
    if (list.at(i) != nullptr) {
      QMetaObject::invokeMethod(list.at(i), "reset", Qt::QueuedConnection);
    }
  }
  m_imageView->clear();
}

bool PrintsEditor::checkIsModified() {
  QList<QObject *> list =
      findChildren<QObject *>(p_objPattern, Qt::FindChildrenRecursively);
  for (int i = 0; i < list.size(); ++i) {
    if (list.at(i) != nullptr) {
      bool b = false;
      if (QMetaObject::invokeMethod(list.at(i), "hasModified",
                                    Qt::DirectConnection,
                                    Q_RETURN_ARG(bool, b))) {

        if (b) {
          return true;
        }
      }
    }
  }
  return false;
}

void PrintsEditor::checkLeaveEditor() {
  if (checkIsModified()) {
    emit s_sendMessage(
        tr("Unsaved Changes, don't leave this page before saved."));
    return;
  }
  finalLeaveEditor();
}

void PrintsEditor::finalLeaveEditor() {
  sqlQueryResult.clear();             /**< SQL History leeren */
  clearDataFields();                  /**< Alle Datenfelder leeren */
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  emit s_leaveEditor();               /**< Zurück */
}

void PrintsEditor::restoreDataset() {
  if (sqlQueryResult.isEmpty())
    return;

  clearDataFields(); /**< Alle Datenfelder leeren */
  importSqlResult();
}

void PrintsEditor::setSqlQueryData(const QString &key, const QVariant &value) {
  if (key.contains("ip_id")) {
    ip_id->setValue(value);
    return;
  }
  if (key.contains("ip_year")) {
    ip_year->setValue(value);
    return;
  }
  if (key.contains("ip_storage")) {
    ip_storage->setValue(value);
    return;
  }
  if (key.contains("ip_price")) {
    ip_price->setValue(value);
    return;
  }
  if (value.type() == QVariant::Bool) {
    BoolBox *v = findChild<BoolBox *>(key, Qt::FindDirectChildrenOnly);
    if (v != nullptr)
      v->setValue(value);

    return;
  }
  if (value.type() == QVariant::Int) {
    IntSpinBox *v = findChild<IntSpinBox *>(key, Qt::FindDirectChildrenOnly);
    if (v != nullptr)
      v->setValue(value);

    return;
  }
  if (value.type() == QVariant::String) {
    StrLineEdit *v = findChild<StrLineEdit *>(key, Qt::FindDirectChildrenOnly);
    if (v != nullptr) {
      if (!value.toString().contains("NOT_SET")) {
        v->setValue(value);
      }
      return;
    }
  }
  if (key.contains("ip_description")) {
    ip_description->setValue(value);
    return;
  }
  if (key.contains("ip_internal_description")) {
    ip_internal_description->setValue(value);
    return;
  }
  qDebug() << "Missing" << key << value << value.type();
}

void PrintsEditor::saveData() {
  if (ip_id->text().isEmpty()) {
    createSqlInsert();
  } else {
    createSqlUpdate();
  }
}

void PrintsEditor::changeEvent(QEvent *event) {
  if (event->type() == QEvent::EnabledChange) {
    ip_condition->loadDataset("ip_condition");
    ip_designation->loadDataset("ip_designation");
    ip_storage->loadStorageData();
  }
}

void PrintsEditor::editPrintsEntry(const QString &condition) {
  if (condition.length() < 5)
    return;

  QString select("SELECT * FROM inventory_prints WHERE ");
  select.append(condition);
  select.append(" ORDER BY ip_id LIMIT 1;");

  QSqlQuery q = db->query(select);
  if (q.size() != 0) {
    QSqlRecord r = db->record("inventory_prints");
    sqlQueryResult.clear();
    while (q.next()) {
      foreach (QString key, inputList) {
        QVariant val = q.value(r.indexOf(key));
        DataEntries d;
        d.field = key;
        d.vtype = val.type();
        d.data = val;
        sqlQueryResult.append(d);
      }
    }
  } else {
    MessageBox messanger(this);
    messanger.queryFail(db->fetchErrors(), condition);
    return;
  }

  if (!sqlQueryResult.isEmpty() && !m_actionBar->isRestoreable())
    m_actionBar->setRestoreable(true);

  btn_imaging->setEnabled(true);
  importSqlResult();
}

void PrintsEditor::createPrintsEntry() {
  setEnabled(true);
  btn_imaging->setEnabled(false);
  resetModified();
}
