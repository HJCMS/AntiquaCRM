// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "customerseditor.h"
#include "customersbillings.h"
#include "customersdata.h"
#include "customersheaderframe.h"
#include "customersorders.h"
#include "customerstabwidget.h"
#include "mailforwarddialog.h"

#include <AntiquaCRM>
#include <QDebug>
#include <QLayout>
#include <QMessageBox>

CustomersEditor::CustomersEditor(QWidget *parent)
    : InventoryEditor{"^c_[\\w\\d_]+\\b$", parent} {
  setObjectName("customers_editor");
  setWindowTitle(tr("Customer Book"));

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("bookeditor_main_layout");
  mainLayout->setSizeConstraint(QLayout::SetMaximumSize);
  mainLayout->setContentsMargins(2, 0, 2, 0);

  m_headerFrame = new CustomersHeaderFrame(this);
  mainLayout->addWidget(m_headerFrame);

  m_tabWidget = new CustomersTabWidget(this);
  m_dataWidget = new CustomersData(m_tabWidget);
  m_tabWidget->placeTab(m_dataWidget, tr("Contact data"));

  m_billingWidget = new CustomersBillings(m_tabWidget);
  m_tabWidget->placeTab(m_billingWidget, tr("Payment details"));

  m_ordersWidget = new CustomersOrders(m_tabWidget);
  m_tabWidget->placeTab(m_ordersWidget, tr("Orders"));
  mainLayout->addWidget(m_tabWidget);

  m_actionBar = new EditorActionBar(this);
  m_actionBar->setViewPrintButton(false);
  m_actionBar->setViewMailButton(true);
  m_actionBar->setViewActionAddButton(true, tr("Create Order"));
  // ResetButton off
  m_actionBar->setRestoreable(false);
  m_actionBar->setViewRestoreButton(false);
  mainLayout->addWidget(m_actionBar);

  setLayout(mainLayout);

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

const QString CustomersEditor::generateProviderId() {
  QString str = getDataValue("c_firstname").toString();
  str.append(" ");
  str.append(getDataValue("c_lastname").toString());
  return str;
}

void CustomersEditor::setInputFields() {
  m_tableData = new AntiquaCRM::ASqlDataQuery("customers");
  inputFields = m_tableData->columnNames();
  // Dienstleister(Uniq Key) Ist hier nicht erfordelich!
  // Wird später bei einem INSERT Manuel gesetzt!
  ignoreFields << "c_provider_import";
  ignoreFields << "c_changed";
  // Diese Felder bei (INSERT|UPDATE) Ignorieren!
  ignoreFields << "c_since";
  ignoreFields << "c_transactions";
  ignoreFields << "c_purchases";

  if (inputFields.isEmpty()) {
    QStringList warn(tr("An error has occurred!"));
    warn << tr("Can't load input datafields!");
    warn << tr("When getting this Message, please check your Network and "
               "Database connection!");
    openNoticeMessage(warn.join("\n"));
  }

  // Completers
  m_dataWidget->c_postalcode->loadDataset();
  m_dataWidget->c_phone_0->loadDataset();
  // Menübar SQL Abfrage starten
  m_actionBar->setMailMenu(MailButton::Customers);
}

bool CustomersEditor::setDataField(const QSqlField &field,
                                   const QVariant &value) {
  if (!field.isValid())
    return false;

  QString key = field.name();
  bool required = (field.requiredStatus() == QSqlField::Required);
  InputEdit *inp = findChild<InputEdit *>(key, Qt::FindChildrenRecursively);
  if (inp != nullptr) {
    inp->setValue(value);
    inp->setProperties(field);
    if (key == "c_country") {
      m_dataWidget->c_postalcode->findCountry(value.toString());
    }
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

  m_actionBar->setRestoreable(m_tableData->isValid());
  setResetModified(inputFields);
}

bool CustomersEditor::sendSqlQuery(const QString &query) {
  QSqlQuery q = m_sql->query(query);
  if (q.lastError().type() != QSqlError::NoError) {
    qWarning("Customer SQL Query: %s\nError: %s\n)", qPrintable(query),
             qPrintable(m_sql->lastError()));
    return false;
  }

  if (q.next()) {
    if (!q.isNull("c_id")) {
      QSqlField field = m_tableData->getProperties("c_id");
      setDataField(field, q.value("c_id"));
    }
    qDebug() << "SqlQuery.last() = " << q.last();
  }
  openSuccessMessage(tr("Customer saved successfully!"));
  setResetModified(inputFields);
  return true;
}

const QHash<QString, QVariant> CustomersEditor::createSqlDataset() {
  QHash<QString, QVariant> data;
  QList<InputEdit *> list =
      findChildren<InputEdit *>(fieldPattern, Qt::FindChildrenRecursively);
  QList<InputEdit *>::Iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    InputEdit *cur = *it;
    QString objName = cur->objectName();
    if (ignoreFields.contains(objName))
      continue;

    // qDebug() << "Dataset:" << objName << cur->value();
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

void CustomersEditor::createSqlUpdate() {
  qint64 cId = getSerialID("c_id");
  if (cId < 1) {
    qWarning("Skip SQL_UPDATE: (cID < 1)!");
    return;
  }
  m_headerFrame->c_id->setRequired(true);

  if (m_tableData == nullptr || !m_tableData->isValid()) {
    qWarning("Invalid AntiquaCRM::ASqlDataQuery detected!");
    return;
  }

  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 1)
    return;

  QStringList set;
  QHash<QString, QVariant>::iterator it;
  int changes = 0;
  for (it = data.begin(); it != data.end(); ++it) {
    if (it.key() == "c_id")
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

  // Ist ein SQL-Update erforderlich?
  if (changes == 0) {
    sendStatusMessage(tr("No Modifications found, Update aborted!"));
    setWindowModified(false);
    return;
  }

  QString sql("UPDATE customers SET ");
  sql.append(set.join(","));
  sql.append(",c_changed=CURRENT_TIMESTAMP WHERE c_id=");
  sql.append(QString::number(getSerialID("c_id")));
  sql.append(";");
  if (sendSqlQuery(sql)) {
    qInfo("SQL UPDATE Customers success!");
    sendStatusMessage(tr("Save Customer data successed!"));
    m_actionBar->setRestoreable(m_tableData->isValid());
  }
}

void CustomersEditor::createSqlInsert() {
  qint64 cId = getSerialID("c_id");
  if (cId > 0) {
    qWarning("Skip SQL_INSERT: (cId > 0)!");
    return;
  }
  m_headerFrame->c_id->setRequired(false);

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

  // Erstelle »c_provider_import«
  column.append("c_provider_import");
  values.append("'" + generateProviderId() + "'");

  QString sql("INSERT INTO customers (");
  sql.append(column.join(","));
  sql.append(",c_changed) VALUES (");
  sql.append(values.join(","));
  sql.append(",CURRENT_TIMESTAMP) RETURNING c_id;");
  if (sendSqlQuery(sql) && getSerialID("c_id") >= 1) {
    qInfo("SQL INSERT Customer success!");
    sendStatusMessage(tr("Save Customer data successed!"));
    m_actionBar->setRestoreable(m_tableData->isValid());
    m_headerFrame->c_id->setRequired(true);
  }
}

void CustomersEditor::findPurchases() {
  qint64 c_id = getSerialID("c_id");
  if (c_id < 1)
    return;

  AntiquaCRM::ASqlFiles sqlFile("query_customer_orders_status");
  if (sqlFile.openTemplate()) {
    sqlFile.setWhereClause("c_id=" + QString::number(c_id));
    QSqlQuery q = m_sql->query(sqlFile.getQueryContent());
    if (q.size() > 0) {
      m_ordersWidget->clearContents();
      m_ordersWidget->setRowCount(q.size());
      int row = 0;
      while (q.next()) {
        int col = 0;
        m_ordersWidget->setItem(row, col++,
                                m_ordersWidget->paymentItem(q.value("payed")));
        m_ordersWidget->setItem(row, col++,
                                m_ordersWidget->numidItem(q.value("orderid")));
        m_ordersWidget->setItem(row, col++,
                                m_ordersWidget->numidItem(q.value("invoice")));
        m_ordersWidget->setItem(row, col++,
                                m_ordersWidget->createItem(q.value("article")));
        m_ordersWidget->setItem(row, col++,
                                m_ordersWidget->createItem(q.value("title")));
        m_ordersWidget->setItem(
            row, col++, m_ordersWidget->createItem(q.value("provider")));
        m_ordersWidget->setItem(row, col++,
                                m_ordersWidget->createItem(q.value("prorder")));
        m_ordersWidget->setItem(row, col++,
                                m_ordersWidget->createDate(q.value("since")));
        m_ordersWidget->setItem(row, col++,
                                m_ordersWidget->createDate(q.value("deliver")));

        row++;
      }
    }
  }
}

void CustomersEditor::setSaveData() {
  if (getSerialID("c_id") > 0) {
    createSqlUpdate();
  } else {
    createSqlInsert();
  }
}

void CustomersEditor::setCreateOrderSignal() {
  qint64 id = getSerialID("c_id");
  if (id > 0)
    emit sendEditorAction(id);
}

void CustomersEditor::setCheckLeaveEditor() {
  if (checkIsModified()) {
    unsavedChangesPopup();
    return;
  }
  setFinalLeaveEditor();
}

void CustomersEditor::setFinalLeaveEditor() {
  setResetInputFields();
  m_ordersWidget->clearContents();    /**< Einkäufe entfernen */
  m_ordersWidget->setRowCount(0);     /**< Einkäufe entfernen */
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  emit sendLeaveEditor();             /**< Zurück zur Hauptsansicht */
}

void CustomersEditor::setCreateMailMessage(const QString &action) {
  qint64 cid = getSerialID("c_id");
  if (cid < 1)
    return;

  MailForwardDialog *d = new MailForwardDialog(this);
  d->exec(cid, action);
  sendStatusMessage(tr("Send eMail finished!"));
}

void CustomersEditor::setRestore() { importSqlResult(); }

bool CustomersEditor::openEditEntry(qint64 cutomerId) {
  bool status = false;
  if (cutomerId < 1)
    return status;

  setInputFields();

  QString cId = QString::number(cutomerId);
  QString table = m_tableData->tableName();
  QString sql("SELECT *, concat_ws(' ',c_firstname,c_lastname) AS fullname");
  sql.append(" FROM " + table + " WHERE c_id=" + cId + " LIMIT 1;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() != 0) {
    QSqlRecord r = m_tableData->record();
    q.next();
    foreach (QString key, inputFields) {
      m_tableData->setValue(key, q.value(r.indexOf(key)));
    }
    m_headerFrame->setDisplayName(q.value("fullname").toString());
    status = true;
  } else {
    qWarning("Edit Customer SQL Error (%s)", qPrintable(m_sql->lastError()));
    status = false;
  }

  if (status) {
    setRestore();
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
  m_tabWidget->setCurrentIndex(0);
  return true;
}

qint64 CustomersEditor::queryCustomerByOrderId(qint64 orderId) {
  QString sql("SELECT o_customer_id FROM inventory_orders");
  sql.append(" WHERE o_id=" + QString::number(orderId) + ";");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    q.next();
    return q.value("o_customer_id").toInt();
  }
  return -1;
}
