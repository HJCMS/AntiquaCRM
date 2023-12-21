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

  // Register modified changes
  registerInputChanged();

  connect(m_ordersTable, SIGNAL(pushMessage(const QString &)),
          SLOT(pushStatusMessage(const QString &)));

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

  // PLZ Completers
  m_dataWidget->c_postalcode->initData();

  // Salutations
  m_dataWidget->c_title->initData();
}

bool CustomersEditor::setDataField(const QSqlField &field,
                                   const QVariant &value) {
  if (!field.isValid())
    return false;

  QString _key = field.name();
  // qDebug() << "setDataField:" << field.name() << value;
  AntiquaCRM::AInputWidget *m_w = findChild<AntiquaCRM::AInputWidget *>(_key);
  if (m_w != nullptr) {
    m_w->setRestrictions(field);
    // Muss nach setRestrictions kommen!
    m_w->setValue(value);
    return true;
  }

  if (ignoreFields.contains(_key))
    return true;

  if (field.requiredStatus() == QSqlField::Required) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Unknown:" << _key << "|" << value;
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
    setDataField(m_tableData->getProperties(it.key()), it.value());
  }
  blockSignals(false);
  setResetModified(inputFields);
}

bool CustomersEditor::sendSqlQuery(const QString &query) {
  QSqlQuery _q = m_sql->query(query);
  if (_q.lastError().type() != QSqlError::NoError) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << query << m_sql->lastError();
#endif
    return false;
  }

  if (_q.next()) {
    if (!_q.isNull("c_id")) {
      QSqlField _field = m_tableData->getProperties("c_id");
      setDataField(_field, _q.value("c_id"));
    }
  }

  openSuccessMessage(tr("Customer saved successfully!"));
  setResetModified(inputFields);
  return true;
}

const QHash<QString, QVariant> CustomersEditor::createSqlDataset() {
  QHash<QString, QVariant> _data;
  QList<AntiquaCRM::AInputWidget *> _list =
      findChildren<AntiquaCRM::AInputWidget *>(fieldPattern,
                                               Qt::FindChildrenRecursively);
  QList<AntiquaCRM::AInputWidget *>::Iterator it;
  for (it = _list.begin(); it != _list.end(); ++it) {
    AntiquaCRM::AInputWidget *m_w = *it;
    QString _name = m_w->objectName();
    if (ignoreFields.contains(_name))
      continue;

    // qDebug() << Q_FUNC_INFO << fieldPattern << objName << Qt::endl
    //     << "- " << cur->isRequired() << cur->isValid() << Qt::endl
    //     << "- " << cur->getValue();

    if (m_w->isRequired() && !m_w->isValid()) {
      openNoticeMessage(m_w->popUpHints());
      m_w->setFocus();
      _data.clear();
      return _data;
    }
    _data.insert(_name, m_w->getValue());
  }
  _list.clear();

  return _data;
}

void CustomersEditor::createSqlUpdate() {
  qint64 _cid = c_id->getValue().toInt();
  const QString str_cid = QString::number(_cid);
  if (_cid < 1 || str_cid.isEmpty()) {
    openNoticeMessage(tr("Missing Customer-Id for Update."));
    return;
  }
  // UPDATE Anforderungen
  c_id->setRequired(true);

  QHash<QString, QVariant> _data = createSqlDataset();
  if (_data.size() < 1)
    return;

  QStringList _list;
  qint32 _changes = 0;
  QHash<QString, QVariant>::iterator it;
  for (it = _data.begin(); it != _data.end(); ++it) {
    if (it.key() == "c_id")
      continue;

    const QString _key = it.key();
    // Nur geänderte Felder in das Update aufnehmen!
    if (!isModifiedCompare(_key, m_tableData->getValue(_key)))
      continue;

    if (m_tableData->getType(_key).id() == QMetaType::QString) {
      _list.append(_key + "='" + it.value().toString() + "'");
      _changes++;
    } else {
      _list.append(_key + "=" + it.value().toString());
      _changes++;
    }
  }

  if (_changes < 1) {
    pushStatusMessage(tr("No Modifications found, Update aborted!"));
    setWindowModified(false);
    return;
  }

  QString _sql("UPDATE customers SET ");
  _sql.append(_list.join(","));
  _sql.append(",c_changed=CURRENT_TIMESTAMP WHERE c_id=" + str_cid + ";");
  if (sendSqlQuery(_sql)) {
    setWindowModified(false);
  }
}

void CustomersEditor::createSqlInsert() {
  qint64 _cid = c_id->getValue().toInt();
  if (_cid > 0) {
    qWarning("Skip INSERT, switch to UPDATE (c_id=%lli)!", _cid);
    createSqlUpdate();
    return;
  }
  // Bei einem INSERT die Anforderungen anpassen.
  c_id->setRequired(false);

  // Die Initialisierung erfolgt in setInputFields!
  // Bei einem INSERT wird es hier befüllt!
  if (m_tableData == nullptr || !m_tableData->isValid()) {
    qWarning("Invalid AntiquaCRM::ASqlDataQuery detected!");
    return;
  }

  QHash<QString, QVariant> _data = createSqlDataset();
  if (_data.size() < 1)
    return;

  // »c_provider_import«
  // Wird in der Datenbank durch ein »NOT NULL« eingeschränkt!
  // Bei INSERT muss deshalb »c_provider_import« manuell befüllt werden!
  QString _c_pr_import = _data["c_firstname"].toString().toLower();
  _c_pr_import.append(" ");
  _c_pr_import.append(_data["c_lastname"].toString().toLower());
  if (_data.contains("c_provider_import")) {
    _data["c_provider_import"] = _c_pr_import;
  } else {
    _data.insert("c_provider_import", _c_pr_import);
  }

  QStringList _columns; // SQL Columns
  QStringList _values;  // SQL Values
  QHash<QString, QVariant>::iterator it;
  for (it = _data.begin(); it != _data.end(); ++it) {
    if (it.value().isNull())
      continue;

    QString _field = it.key();
    m_tableData->setValue(_field, it.value());
    _columns.append(_field);
    if (m_tableData->getType(_field).id() == QMetaType::QString) {
      _values.append("'" + it.value().toString() + "'");
    } else {
      _values.append(it.value().toString());
    }
  }

  QString _sql("INSERT INTO customers (");
  _sql.append(_columns.join(","));
  _sql.append(",c_changed) VALUES (");
  _sql.append(_values.join(","));
  _sql.append(",CURRENT_TIMESTAMP) RETURNING c_id;");
  if (sendSqlQuery(_sql) && getSerialID("c_id") >= 1) {
    c_id->setRequired(true);
    setWindowModified(false);
  }
}

void CustomersEditor::findPurchases() {
  qint64 _cid = getSerialID("c_id");
  if (_cid < 1)
    return;

  AntiquaCRM::ASqlFiles _tpl("query_customer_orders_status");
  if (!_tpl.openTemplate()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qWarning("No query_customer_orders_status file found");
#endif
    return;
  }

  _tpl.setWhereClause("c_id=" + QString::number(_cid));
  QSqlQuery q = m_sql->query(_tpl.getQueryContent());
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
  if (force) // cancel action triggered
    setWindowModified(false);

  setResetInputFields();
  m_ordersTable->clearContents(); /**< Einkäufe entfernen */
  m_ordersTable->setRowCount(0);  /**< Einkäufe entfernen */
  emit sendLeaveEditor();         /**< Back to MainView */
}

void CustomersEditor::setCreateOrderSignal() {
  qint64 _cid = getSerialID("c_id");
  if (_cid > 0)
    emit sendEditorAction(_cid);
}

void CustomersEditor::setCreateMailMessage(const QString &action) {
  qint64 _cid = getSerialID("c_id");
  if (_cid < 1)
    return;

  QJsonObject _obj;
  _obj.insert("tb_tab", "CUSTOMER");
  _obj.insert("tb_caller", action.toUpper());
  _obj.insert("CRM_CUSTOMER_ID", _cid);

  AntiquaCRM::MailDialog *d = new AntiquaCRM::MailDialog(this);
  if (d->exec(_obj))
    pushStatusMessage(tr("Send eMail finished!"));
  else
    pushStatusMessage(tr("Send eMail canceled!"));
}

void CustomersEditor::setRestore() { importSqlResult(); }

bool CustomersEditor::openEditEntry(qint64 articleId) {
  if (articleId < 1)
    return false;

  QString _cid = QString::number(articleId);
  if (_cid.isEmpty())
    return false;

  AntiquaCRM::ASqlFiles _tpl("query_customer_data");
  if (!_tpl.openTemplate()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qWarning("No query_customer_data file found");
#endif
    return false;
  }

  setInputFields();

  _tpl.setWhereClause("c_id=" + _cid);

  bool _status = false;
  QSqlQuery _q = m_sql->query(_tpl.getQueryContent());
  if (_q.size() != 0) {
    QSqlRecord _r = m_tableData->record();
    _q.next();
    displayName->setText(_q.value("c_fullname").toString());
    foreach (QString key, inputFields) {
      m_tableData->setValue(key, _q.value(_r.indexOf(key)));
    }
    _status = true;
  } else {
    _status = false;
  }

  if (_status) {
    // Die aktuelle Abfolge ist Identisch mit setRestore!
    setRestore();

    QString _co = m_tableData->getValue("c_country").toString();
    if (_co.length() > 3)
      m_dataWidget->setCountry(_co);

    findPurchases();
  }

  m_tabWidget->setCurrentIndex(0);
  setResetModified(inputFields);
  return _status;
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
  m_tableData->setValue("c_provider_import", "");
  return isEnabled();
}

bool CustomersEditor::createCustomEntry(const QJsonObject &object) {
  qDebug() << Q_FUNC_INFO << "TODO" << object;
  // "ACTION", "open_customer");
  // "TARGET", "customers_tab");
  // "CUSTOMER", _cid);
  return true;
}
