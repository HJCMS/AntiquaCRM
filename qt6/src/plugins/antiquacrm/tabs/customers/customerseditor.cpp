// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customerseditor.h"
#include "customersconfig.h"
#include "customersdata.h"
#include "customersfinancial.h"
#include "customersorders.h"

#include <AntiquaCRM>
#include <AntiquaMail>
#include <QDate>
#include <QFrame>
#include <QLayout>

CustomersEditor::CustomersEditor(QWidget *parent)
    : AntiquaCRM::TabsEditor{CUSTOMERS_SQL_EDITOR_PATTERN, parent} {
  setWindowTitle(tr("Edit Book"));
  setObjectName("tab_customers_editor");

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("customersedit_main_layout");
  mainLayout->setSizeConstraint(QLayout::SetMaximumSize);
  setLayout(mainLayout);

  // Begin::row1 {
  // Header Frame
  QFrame *firstFrame = new QFrame(this);
  firstFrame->setContentsMargins(2, 0, 2, 0);
  QHBoxLayout *layout1 = new QHBoxLayout(firstFrame);
  layout1->setContentsMargins(0, 0, 0, 0);
  layout1->addWidget(new QLabel(tr("Customer Id:"), firstFrame));
  c_id = new AntiquaCRM::SerialId(firstFrame);
  c_id->setObjectName("c_id");
  layout1->addWidget(c_id);
  displayName = new QLabel(firstFrame);
  layout1->addWidget(displayName);
  layout1->addStretch(1);
  c_since = new AntiquaCRM::DateTimeInfo(firstFrame);
  c_since->setObjectName("c_since");
  layout1->addWidget(c_since);
  c_changed = new AntiquaCRM::DateTimeInfo(firstFrame);
  c_changed->setObjectName("c_changed");
  layout1->addWidget(c_changed);
  firstFrame->setLayout(layout1);
  mainLayout->addWidget(firstFrame);
  // } End::row1;

  // Begin::row2 {
  m_tabWidget = new AntiquaCRM::TabsWidget(this);
  m_tabWidget->setObjectName("customers_data_tab");
  m_dataWidget = new CustomersData(m_tabWidget);
  // Adressdaten
  m_tabWidget->insertTab(0, m_dataWidget, m_dataWidget->windowIcon(),
                         m_dataWidget->windowTitle());
  // Finanzdaten
  m_financialData = new CustomersFinancial(m_tabWidget);
  m_tabWidget->insertTab(1, m_financialData, m_financialData->windowIcon(),
                         m_financialData->windowTitle());
  // Bestellungen
  m_ordersTable = new CustomersOrders(m_tabWidget);
  m_tabWidget->insertTab(2, m_ordersTable, m_ordersTable->windowIcon(),
                         m_ordersTable->windowTitle());
  mainLayout->addWidget(m_tabWidget);
  // } End::row2;

  // Begin::row3 {
  m_actionBar = new AntiquaCRM::TabsEditActionBar(this);
  m_actionBar->setViewPrintButton(false);
  m_actionBar->setPrinterMenu(AntiquaCRM::PrinterGroup::PRINT_NOBUTTON);
  m_actionBar->setViewMailButton(true);
  m_actionBar->setMailMenu(AntiquaCRM::MailGroup::MAIL_CUSTOMER_GROUP);
  m_actionBar->setViewActionAddButton(true, tr("Create Order"));
  // ResetButton off
  m_actionBar->setViewRestoreButton(true);
  m_actionBar->setRestoreable(true);
  mainLayout->addWidget(m_actionBar);
  // } End::row3;

  mainLayout->setStretch(1, 1); // row2

  // Signals:ActionBar
  connect(m_actionBar, SIGNAL(sendCancelClicked()),
          SLOT(setFinalLeaveEditor()));
  connect(m_actionBar, SIGNAL(sendRestoreClicked()), SLOT(setRestore()));
  connect(m_actionBar, SIGNAL(sendSaveClicked()), SLOT(setSaveData()));
  connect(m_actionBar, SIGNAL(sendFinishClicked()),
          SLOT(setCheckLeaveEditor()));
  connect(m_actionBar, SIGNAL(sendCreateMailMessage(const QString &)),
          SLOT(setCreateMailMessage(const QString &)));
  connect(m_actionBar, SIGNAL(sendAddCustomAction()),
          SLOT(setCreateOrderSignal()));
}

CustomersEditor::~CustomersEditor() {
  ignoreFields.clear();
  inputFields.clear();
  if (m_sql != nullptr)
    m_sql->deleteLater();

  if (m_cfg != nullptr)
    m_cfg->deleteLater();
}

void CustomersEditor::setInputFields() {
  // Bei UPDATE/INSERT Ignorieren
  ignoreFields << "c_since";
  ignoreFields << "c_changed";
  ignoreFields << "c_provider_import";

  // Load default table data
  m_tableData = initTableData(CUSTOMERS_SQL_TABLE_NAME);
  if (m_tableData == nullptr)
    return;

  // Completers
  m_dataWidget->c_postalcode->initData();
}

bool CustomersEditor::setDataField(const QSqlField &field,
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

void CustomersEditor::importSqlResult() {
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

  setResetModified(inputFields);
}

bool CustomersEditor::sendSqlQuery(const QString &query) {
  QSqlQuery q = m_sql->query(query);
  if (q.lastError().type() != QSqlError::NoError) {
    qDebug() << Q_FUNC_INFO << query << m_sql->lastError();
    return false;
  }

  if (q.next()) {
    if (!q.isNull("c_id")) {
      QSqlField field = m_tableData->getProperties("c_id");
      setDataField(field, q.value("c_id"));
    }
  }

  openSuccessMessage(tr("Bookdata saved successfully!"));
  setResetModified(inputFields);
  return true;
}

const QHash<QString, QVariant> CustomersEditor::createSqlDataset() {
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

    // qDebug() << Q_FUNC_INFO << fieldPattern << objName << Qt::endl
    //     << "- " << cur->isRequired() << cur->isValid() << Qt::endl
    //     << "- " << cur->getValue();

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

void CustomersEditor::createSqlUpdate() {
  int articleId = c_id->getValue().toInt();
  if (articleId < 0) {
    openNoticeMessage(tr("Missing Article ID for Update."));
    return;
  }
  // UPDATE Anforderungen
  c_id->setRequired(true);

  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 1)
    return;

  QStringList set;
  QHash<QString, QVariant>::iterator it;
  int changes = 0;
  for (it = data.begin(); it != data.end(); ++it) {
    if (it.key() == "c_id")
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

  if (changes < 1) {
    pushStatusMessage(tr("No Modifications found, Update aborted!"));
    setWindowModified(false);
    return;
  }

  QString sql("UPDATE customers SET ");
  sql.append(set.join(","));
  sql.append(",c_changed=CURRENT_TIMESTAMP WHERE c_id=");
  sql.append(c_id->getValue().toString());
  sql.append(";");
  if (sendSqlQuery(sql)) {
    qInfo("SQL UPDATE Inventory Customers success!");
    setWindowModified(false);
  }
}

void CustomersEditor::createSqlInsert() {
  int articleId = c_id->getValue().toInt();
  if (articleId >= 1) {
    qWarning("Skip INSERT, switch to UPDATE with (ArticleID > 0)!");
    createSqlUpdate();
    return;
  }
  // Bei einem INSERT die Anforderungen anpassen.
  c_id->setRequired(false);

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

  QString sql("INSERT INTO customers (");
  sql.append(column.join(","));
  sql.append(",c_changed) VALUES (");
  sql.append(values.join(","));
  sql.append(",CURRENT_TIMESTAMP) RETURNING c_id;");
  if (sendSqlQuery(sql) && c_id->getValue().toInt() >= 1) {
    c_id->setRequired(true);
    setWindowModified(false);
  }
}

void CustomersEditor::findPurchases() {
  qint64 c_id = getSerialID("c_id");
  if (c_id < 1)
    return;

  AntiquaCRM::ASqlFiles sfile("query_customer_orders_status");
  if (sfile.openTemplate()) {
    sfile.setWhereClause("c_id=" + QString::number(c_id));
    QSqlQuery q = m_sql->query(sfile.getQueryContent());
    if (q.size() > 0) {
      m_ordersTable->clearContents();
      m_ordersTable->setRowCount(q.size());
      int row = 0;
      while (q.next()) {
        int col = 0;
        m_ordersTable->setItem(row, col++,
                               m_ordersTable->paymentItem(q.value("payed")));
        m_ordersTable->setItem(row, col++,
                               m_ordersTable->numidItem(q.value("orderid")));
        m_ordersTable->setItem(row, col++,
                               m_ordersTable->numidItem(q.value("invoice")));
        m_ordersTable->setItem(row, col++,
                               m_ordersTable->createItem(q.value("article")));
        m_ordersTable->setItem(row, col++,
                               m_ordersTable->createItem(q.value("title")));
        m_ordersTable->setItem(row, col++,
                               m_ordersTable->createItem(q.value("provider")));
        m_ordersTable->setItem(row, col++,
                               m_ordersTable->createItem(q.value("prorder")));
        m_ordersTable->setItem(row, col++,
                               m_ordersTable->createDate(q.value("since")));
        m_ordersTable->setItem(row, col++,
                               m_ordersTable->createDate(q.value("deliver")));

        row++;
      }
    }
  }
}

void CustomersEditor::setSaveData() {
  if (c_id->getValue().toString().isEmpty()) {
    createSqlInsert();
    return;
  }

  createSqlUpdate();
}

void CustomersEditor::setCheckLeaveEditor() {
  if (checkIsModified()) {
    unsavedChangesPopup();
    return;
  }
  setFinalLeaveEditor();
}

void CustomersEditor::setFinalLeaveEditor(bool force) {
  if (force) // Wenn auf Abbrechen geklickt wurde!
    setWindowModified(false);

  setResetInputFields();
  m_ordersTable->clearContents(); /**< Einkäufe entfernen */
  m_ordersTable->setRowCount(0);  /**< Einkäufe entfernen */
  emit sendLeaveEditor();         /**< Back to MainView */
}

void CustomersEditor::setCreateOrderSignal() {
  qint64 id = getSerialID("c_id");
  if (id > 0)
    emit sendEditorAction(id);
}

void CustomersEditor::setCreateMailMessage(const QString &action) {
  qint64 cid = getSerialID("c_id");
  if (cid < 1)
    return;

  QJsonObject _obj;
  _obj.insert("tb_tab", "CUSTOMER");
  _obj.insert("tb_caller", action.toUpper());
  _obj.insert("CRM_CUSTOMER_ID", cid);

  AntiquaCRM::MailDialog *d = new AntiquaCRM::MailDialog(this);
  if (d->exec(_obj))
    pushStatusMessage(tr("Send eMail finished!"));
  else
    pushStatusMessage(tr("Send eMail canceled!"));
}

void CustomersEditor::setRestore() { importSqlResult(); }

bool CustomersEditor::openEditEntry(qint64 articleId) {
  bool status = false;
  if (articleId < 1)
    return status;

  QString c_id = QString::number(articleId);
  if (c_id.isEmpty())
    return status;

  AntiquaCRM::ASqlFiles _sf("query_customer_data");
  if (!_sf.openTemplate()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "No Template found";
#endif
    return false;
  }

  setInputFields();
  _sf.setWhereClause("c_id=" + c_id);
  QSqlQuery q = m_sql->query(_sf.getQueryContent());
  if (q.size() != 0) {
    QSqlRecord r = m_tableData->record();
    q.next();
    displayName->setText(q.value("c_fullname").toString());
    foreach (QString key, inputFields) {
      m_tableData->setValue(key, q.value(r.indexOf(key)));
    }
    status = true;
  } else {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
    status = false;
  }

  if (status) {
    // Die aktuelle Abfolge ist Identisch mit setRestore!
    setRestore();
    registerInputChanged();

    QString _country = m_tableData->getValue("c_country").toString();
    if (_country.length() > 3)
      m_dataWidget->setCountry(_country);

    findPurchases();
  }

  m_tabWidget->setCurrentIndex(0);
  return status;
}

bool CustomersEditor::createNewEntry() {
  setInputFields();
  foreach (QString column, m_tableData->columnNames()) {
    QSqlField field = m_tableData->getProperties(column);
    if (column == "c_id")
      field.setRequired(false);

    setDefaultInput(field);
  }
  setResetModified(inputFields);
  registerInputChanged();
  m_tabWidget->setCurrentIndex(0);
  return isEnabled();
}
