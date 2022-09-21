// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "editcustomer.h"
#include "customerbillinginfo.h"
#include "customercontact.h"
#include "customerpayments.h"
#include "editoractionbar.h"
#include "mailforwarddialog.h"
#include "myicontheme.h"
#include "serialid.h"

#include <QDebug>
#include <QLayout>

// Schalte SQL ausgaben ein
#ifndef SHOW_SQL_QUERIES
#define SHOW_SQL_QUERIES false
#endif

EditCustomer::EditCustomer(QWidget *parent) : EditorMain{parent} {
  setObjectName("EditCustomer");

  m_sql = new HJCMS::SqlCore(this);

  /**
   * Tabellenfelder welche NICHT bei INSERT/UPDATE
   * benötigt werden aber im Overview enthalten sind.
   */
  ignoreList.clear();
  ignoreList.append("c_since");
  ignoreList.append("c_changed");
  ignoreList.append("c_transactions");
  ignoreList.append("c_purchases");

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("customer_edit_layout");

  // BEGIN Info Header
  QHBoxLayout *headerLayout = new QHBoxLayout();
  headerLayout->setObjectName("header_layout");

  c_id = new SerialID(this);
  c_id->setObjectName("c_id");
  c_id->setInfo(tr("Customers ID"));
  headerLayout->addWidget(c_id);

  infoLabel = new QLabel(this);
  headerLayout->addWidget(infoLabel);
  headerLayout->addStretch(1);
  mainLayout->addLayout(headerLayout);
  // END

  m_dataBox = new QToolBox(this);
  m_dataBox->setObjectName("customer_boxes");
  mainLayout->addWidget(m_dataBox);

  // BEGIN Edit Customer
  m_contact = new CustomerContact(m_dataBox);
  m_contact->setObjectName("contact");
  m_dataBox->addItem(m_contact, myIcon("identity"), tr("Edit Contact"));
  // END

  // BEGIN Billing Info & Payments Table
  QFrame *m_frame = new QFrame(m_dataBox);
  QVBoxLayout *frameLayout = new QVBoxLayout(m_frame);
  m_billing = new CustomerBillingInfo(m_frame);
  m_billing->setObjectName("billing");
  frameLayout->addWidget(m_billing);

  QString pinfo = tr("Payments");
  QLabel *m_lb = new QLabel(pinfo, m_frame);
  frameLayout->addWidget(m_lb);

  m_payments = new CustomerPayments(m_frame);
  m_payments->setObjectName("payments");
  frameLayout->addWidget(m_payments);

  m_frame->setLayout(frameLayout);
  m_dataBox->addItem(m_frame, myIcon("list"), tr("Edit Billing"));
  // END

  // BEGIN Actions Bar
  m_actionBar = new EditorActionBar(this);
  m_actionBar->setViewPrintButton(false);
  m_actionBar->setMailMenu(MailButton::Customers);
  mainLayout->addWidget(m_actionBar);
  // END

  setLayout(mainLayout);

  // SIGNALS
  QList<UtilsMain *> list =
      findChildren<UtilsMain *>(p_objPattern, Qt::FindChildrenRecursively);
  QList<UtilsMain *>::Iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    UtilsMain *e = *it;
    connect(e, SIGNAL(hasModified(bool)), this, SLOT(setWindowModified(bool)));
  }

  connect(m_actionBar, SIGNAL(s_cancelClicked()), this,
          SLOT(finalLeaveEditor()));
  connect(m_actionBar, SIGNAL(s_restoreClicked()), this,
          SLOT(restoreDataset()));
  connect(m_actionBar, SIGNAL(s_saveClicked()), this, SLOT(saveData()));
  connect(m_actionBar, SIGNAL(s_finishClicked()), this,
          SLOT(checkLeaveEditor()));
  connect(m_actionBar, SIGNAL(s_createMailMessage(const QString &)), this,
          SLOT(openEMailDialog(const QString &)));
}

void EditCustomer::setInputList() {
  inputList = m_sql->fields("customers");
  if (inputList.isEmpty()) {
    qWarning("Customers InputList is Empty!");
  }
}

const QString EditCustomer::dateString(const QDate &date) const {
  return date.toString(Qt::DefaultLocaleLongDate);
}

void EditCustomer::createPaymentsTable() {
  int customerID = c_id->value().toInt();
  if (customerID < 1)
    return;

  QString sql("SELECT o_id, o_since, o_delivered");
  sql.append(" FROM inventory_orders WHERE o_customer_id=");
  sql.append(QString::number(customerID) + " AND ");
  sql.append(" o_payment_status=true;");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    m_payments->restore();
    m_payments->setRowCount(q.size());
    int row = 0;
    while (q.next()) {
      QTableWidgetItem *id =
          new QTableWidgetItem(q.value("o_id").toString(), Qt::DisplayRole);
      m_payments->setItem(row, 0, id);
      QTableWidgetItem *since = new QTableWidgetItem(
          dateString(q.value("o_since").toDate()), Qt::DisplayRole);
      m_payments->setItem(row, 1, since);
      QTableWidgetItem *delivered = new QTableWidgetItem(
          dateString(q.value("o_delivered").toDate()), Qt::DisplayRole);
      m_payments->setItem(row, 2, delivered);
      row++;
    }
  }
#ifdef ANTIQUA_DEVELOPEMENT
  else if(!m_sql->lastError().isNull()) {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
  }
#endif
}

void EditCustomer::importSqlResult() {
  if (sqlQueryResult.size() < 15)
    return;

  blockSignals(true);
  for (int i = 0; i < sqlQueryResult.size(); ++i) {
    DataField f = sqlQueryResult.at(i);
    setData(f.field(), f.value(), f.isRequired());
  }
  blockSignals(false);

  createPaymentsTable();
  resetModified(inputList);
}

bool EditCustomer::sendSqlQuery(const QString &sqlStatement) {
  if (SHOW_SQL_QUERIES) {
    qDebug() << Q_FUNC_INFO << sqlStatement;
  }
  MessageBox msgBox(this);
  QSqlQuery q = m_sql->query(sqlStatement);
  if (q.lastError().type() != QSqlError::NoError) {
    QString errorString = m_sql->fetchErrors();
    qDebug() << errorString << Qt::endl;
    msgBox.failed(sqlStatement, errorString);
    return false;
  } else {
    msgBox.success(tr("Saved successfully!"), 1);
    resetModified(inputList);
    return true;
  }
}

const QHash<QString, QVariant> EditCustomer::createSqlDataset() {
  QHash<QString, QVariant> data;
  MessageBox messanger(this);
  QList<UtilsMain *> list =
      findChildren<UtilsMain *>(p_objPattern, Qt::FindChildrenRecursively);
  QList<UtilsMain *>::Iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    UtilsMain *cur = *it;
    if (ignoreList.contains(cur->objectName(), Qt::CaseSensitive))
      continue;

    if (cur->isRequired() && !cur->isValid()) {
      messanger.notice(cur->notes());
      cur->setFocus();
      data.clear();
      return data;
    }
    data.insert(cur->objectName(), cur->value());
  }
  list.clear();
  return data;
}

void EditCustomer::createSqlUpdate() {
  QString cid = c_id->value().toString();
  if (cid.isEmpty())
    return;

  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 3)
    return;

  QStringList set;
  QHash<QString, QVariant>::iterator it;
  for (it = data.begin(); it != data.end(); ++it) {
    if (it.key() == "c_id")
      continue;

    if (it.value().type() == QVariant::String) {
      set.append(it.key() + "='" + it.value().toString() + "'");
    } else {
      set.append(it.key() + "=" + it.value().toString());
    }
  }

  QString sql("UPDATE customers SET ");
  sql.append(set.join(","));
  sql.append(",c_changed=CURRENT_TIMESTAMP");
  sql.append(" WHERE c_id=");
  sql.append(cid);
  sql.append(";");

  if (sendSqlQuery(sql)) {
    updateCustomer("c_id=" + cid);
  }
#ifdef ANTIQUA_DEVELOPEMENT
  else {
    qInfo("SQLQuery:NOT OK - Update Customer Interfaces");
  }
#endif
}

void EditCustomer::createSqlInsert() {
  QString cid = c_id->value().toString();
  if (!cid.isEmpty())
    return;

  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 3)
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

  QString sql("INSERT INTO customers (");
  sql.append(column.join(","));
  sql.append(",c_since,c_changed) VALUES (");
  sql.append(values.join(","));
  sql.append(",CURRENT_TIMESTAMP,CURRENT_TIMESTAMP);");

  if (sendSqlQuery(sql))
    checkLeaveEditor();
}

void EditCustomer::setData(const QString &key, const QVariant &value,
                           bool required) {
  if (key.isEmpty())
    return;

  if (key == "fullname") {
    infoLabel->setText(value.toString());
    return; // aussteigen
  } else if (key == "c_company_name") {
    infoLabel->setText(value.toString());
  }

  if (ignoreList.contains(key, Qt::CaseSensitive))
    return;

  UtilsMain *inp = findChild<UtilsMain *>(key, Qt::FindChildrenRecursively);
  if (inp != nullptr) {
    inp->setValue(value);
    if (required && !inp->isRequired())
      inp->setRequired(required);

    if (key == "c_email_0" && !value.toString().isEmpty()) {
      m_actionBar->setViewMailButton(true);
    }

    return;
  }
  qWarning("Missing Key: %s", qPrintable(key));
}

void EditCustomer::saveData() {
  if (c_id->value().toInt() < 1) {
    createSqlInsert();
  } else if (c_id->value().toInt() > 0) {
    createSqlUpdate();
  }
}

void EditCustomer::checkLeaveEditor() {
  if (checkIsModified(p_objPattern)) {
    QString msg = tr("Unsaved Changes, don't leave this page before saved.");
    emit sendStatusMessage(msg);
    return;
  }
  finalLeaveEditor();
}

void EditCustomer::finalLeaveEditor() {
  sqlQueryResult.clear();             /**< SQL History leeren */
  clearDataFields(p_objPattern);      /**< Alle Datenfelder leeren */
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  m_dataBox->setCurrentIndex(0);      /**< Auf Vorschau stellen */
  emit s_leaveEditor();               /**< Zurück */
}

void EditCustomer::openEMailDialog(const QString &tpl) {
  int cid = c_id->value().toInt();
  if (cid < 1)
    return;

  MailForwardDialog *d = new MailForwardDialog(this);
  if (d->exec(cid, tpl) == QDialog::Rejected) {
    qInfo("Customer Simple eMail canceled.");
  }
}

void EditCustomer::restoreDataset() {
  if (sqlQueryResult.isEmpty())
    return;

  importSqlResult();
}

void EditCustomer::updateCustomer(const QString &statement) {
  setInputList();
  QString sql("SELECT *, concat_ws(' ',c_firstname,c_lastname) AS fullname ");
  sql.append(" FROM customers");
  sql.append(" WHERE " + statement);
  sql.append(" ORDER BY c_id LIMIT 1;");

  QSqlQuery q = m_sql->query(sql);
  if (q.size() != 0) {
    if (SHOW_SQL_QUERIES) {
      qDebug() << Q_FUNC_INFO << sql;
    }
    QSqlRecord r = m_sql->record("customers");
    sqlQueryResult.clear();
    while (q.next()) {
      foreach (QString key, inputList) {
        QVariant val = q.value(r.indexOf(key));
        bool required = (r.field(key).requiredStatus() == QSqlField::Required);
        DataField d;
        d.setField(key);
        d.setType(val.type());
        d.setRequired(required);
        d.setValue(val);
        sqlQueryResult.append(d);
      }
      if (q.value("fullname").isValid()) {
        QVariant val = q.value("fullname");
        DataField d("fullname", val.type(), false, val);
        sqlQueryResult.append(d);
      }
    }
  } else {
    MessageBox messanger(this);
    messanger.failed(m_sql->fetchErrors(), sql);
    return;
  }

  if (!sqlQueryResult.isEmpty() && !m_actionBar->isRestoreable())
    m_actionBar->setRestoreable(true);

  importSqlResult();
  m_dataBox->setCurrentWidget(m_contact);
}

void EditCustomer::createCustomer() {
  setInputList();
  infoLabel->clear();                 /**< Muss Leer sein */
  sqlQueryResult.clear();             /**< SQL History leeren */
  clearDataFields(p_objPattern);      /**< Alle Datenfelder leeren */
  c_id->reset();                      /**< CustomerId Leeren */
  c_id->setRequired(false);           /**< CustomerId Nicht erforderlich */
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  m_dataBox->setCurrentWidget(m_contact);
  resetModified(inputList);
}
