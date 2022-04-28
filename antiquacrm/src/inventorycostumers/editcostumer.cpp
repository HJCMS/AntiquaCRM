// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "editcostumer.h"
#include "costumerbillinginfo.h"
#include "costumercontact.h"
#include "costumeroverview.h"
#include "editoractionbar.h"
#include "genderbox.h"
#include "serialid.h"
#include "version.h"

#include <QtCore/QDebug>
#include <QtWidgets>

// Schalte SQL ausgaben ein
#ifndef SHOW_SQL_QUERIES
#define SHOW_SQL_QUERIES true
#endif

EditCostumer::EditCostumer(QWidget *parent) : EditorMain{parent} {
  setObjectName("EditCostumer");

  m_sql = new HJCMS::SqlCore(this);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("costumer_edit_layout");

  // BEGIN Info Header
  QHBoxLayout *headerLayout = new QHBoxLayout();
  headerLayout->setObjectName("header_layout");

  c_id = new SerialID(this);
  c_id->setObjectName("c_id");
  c_id->setInfo(tr("Costumers ID"));
  headerLayout->addWidget(c_id);

  infoLabel = new QLabel(this);
  headerLayout->addWidget(infoLabel);
  headerLayout->addStretch(1);
  mainLayout->addLayout(headerLayout);
  // END

  // BEGIN ToolBox
  m_dataBox = new QToolBox(this);
  m_dataBox->setObjectName("costumer_edit_box");

  m_overview = new CostumerOverview(m_dataBox);
  m_overview->setObjectName("overview");
  m_dataBox->addItem(m_overview, myIcon("edit_group"), tr("Overview"));

  m_contact = new CostumerContact(m_dataBox);
  m_contact->setObjectName("contact");
  m_dataBox->addItem(m_contact, myIcon("identity"), tr("Edit Contact"));

  m_billing = new CostumerBillingInfo(m_dataBox);
  m_billing->setObjectName("shipping");
  m_dataBox->addItem(m_billing, myIcon("list"), tr("Edit Billing"));
  mainLayout->addWidget(m_dataBox);
  // END

  // BEGIN Actions Bar
  m_actionBar = new EditorActionBar(this);
  mainLayout->addWidget(m_actionBar);
  // END

  setLayout(mainLayout);

  // SIGNALS
  connect(m_actionBar, SIGNAL(s_cancelClicked()), this,
          SLOT(finalLeaveEditor()));
  connect(m_actionBar, SIGNAL(s_restoreClicked()), this,
          SLOT(restoreDataset()));
  connect(m_actionBar, SIGNAL(s_saveClicked()), this, SLOT(saveData()));
  connect(m_actionBar, SIGNAL(s_finishClicked()), this,
          SLOT(checkLeaveEditor()));
}

void EditCostumer::setInputList() {
  inputList = m_sql->fields(tableName);
  if (inputList.isEmpty()) {
    qWarning("Costumers InputList is Empty!");
  }
}

void EditCostumer::importSqlResult() {
  if (sqlQueryResult.size() < 15)
    return;

  blockSignals(true);
  QHash<QString, QString> nodeset;
  for (int i = 0; i < sqlQueryResult.size(); ++i) {
    DataEntries f = sqlQueryResult.at(i);
    setSqlQueryData(f.field, f.data);
    nodeset.insert(f.field, f.data.toString());
  }
  blockSignals(false);

  m_overview->createDocument(nodeset);
  resetModified();
}

bool EditCostumer::sendSqlQuery(const QString &sqlStatement) {
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
    resetModified();
    return true;
  }
}

const QHash<QString, QVariant> EditCostumer::createSqlDataset() {
  QHash<QString, QVariant> data;
  MessageBox messanger;
  foreach (QString k, inputList) {
    if (k == "c_id")
      continue;

    if (k == "c_gender") { /**< Integer */
      // GenderBox
      data.insert(k, m_contact->c_gender->value());
      continue;
    }
    if (k == "c_title") { /**< String */
      // SalutationBox
      data.insert(k, m_contact->c_title->value());
      continue;
    }
    if (k == "c_postalcode") { /**< String */
      // PostalCode
      data.insert(k, m_contact->c_postalcode->value());
      continue;
    }
    if (k == "c_company") { /**< Boolean */
      // GroupBox
      data.insert(k, m_contact->c_company->value());
      continue;
    }
    if (k == "c_trusted") { /**< Integer */
      data.insert(k, m_billing->c_trusted->value());
      continue;
    }
    // Booleans
    QList<BoolBox *> bb = findChildren<BoolBox *>(k, findOption);
    QList<BoolBox *>::Iterator bb_it;
    for (bb_it = bb.begin(); bb_it != bb.end(); ++bb_it) {
      BoolBox *bEdit = *bb_it;
      if (bEdit->isRequired() && !bEdit->isValid()) {
        messanger.notice(bEdit->notes());
        bEdit->setFocus();
        data.clear();
        return data;
      }
      data.insert(bEdit->objectName(), bEdit->value());
    }
    // LineEdit
    QList<LineEdit *> le = findChildren<LineEdit *>(k, findOption);
    QList<LineEdit *>::Iterator le_it;
    for (le_it = le.begin(); le_it != le.end(); ++le_it) {
      LineEdit *lEdit = *le_it;
      if (lEdit->isRequired() && !lEdit->isValid()) {
        messanger.notice(lEdit->notes());
        lEdit->setFocus();
        data.clear();
        return data;
      }
      data.insert(lEdit->objectName(), lEdit->value());
    }
    // PhoneEdit
    QList<PhoneEdit *> pe = findChildren<PhoneEdit *>(k, findOption);
    QList<PhoneEdit *>::Iterator pe_it;
    for (pe_it = pe.begin(); pe_it != pe.end(); ++pe_it) {
      PhoneEdit *pEdit = *pe_it;
      if (pEdit->isRequired() && !pEdit->isValid()) {
        messanger.notice(pEdit->notes());
        pEdit->setFocus();
        data.clear();
        return data;
      }
      data.insert(pEdit->objectName(), pEdit->value());
    }
    // EMailEdit
    QList<EMailEdit *> ee = findChildren<EMailEdit *>(k, findOption);
    QList<EMailEdit *>::Iterator ee_it;
    for (ee_it = ee.begin(); ee_it != ee.end(); ++ee_it) {
      EMailEdit *eEdit = *ee_it;
      if (eEdit->isRequired() && !eEdit->isValid()) {
        messanger.notice(eEdit->notes());
        eEdit->setFocus();
        data.clear();
        return data;
      }
      data.insert(eEdit->objectName(), eEdit->value());
    }
    // StrLineEdit
    QList<StrLineEdit *> se = findChildren<StrLineEdit *>(k, findOption);
    QList<StrLineEdit *>::Iterator se_it;
    for (se_it = se.begin(); se_it != se.end(); ++se_it) {
      StrLineEdit *sEdit = *se_it;
      if (sEdit->isRequired() && !sEdit->isValid()) {
        messanger.notice(sEdit->notes());
        sEdit->setFocus();
        data.clear();
        return data;
      }
      data.insert(sEdit->objectName(), sEdit->value());
    }
    // TextField
    QList<TextField *> te = findChildren<TextField *>(k, findOption);
    QList<TextField *>::Iterator te_it;
    for (te_it = te.begin(); te_it != te.end(); ++te_it) {
      TextField *tEdit = *te_it;
      if (tEdit->isRequired() && !tEdit->isValid()) {
        messanger.notice(tEdit->notes());
        tEdit->setFocus();
        data.clear();
        return data;
      }
      data.insert(tEdit->objectName(), tEdit->value());
    }
  }

  return data;
}

void EditCostumer::createSqlUpdate() {
  QString cid = c_id->value().toString();
  if (cid.isEmpty())
    return;

  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 3)
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

  QString sql("UPDATE " + tableName);
  sql.append(" SET ");
  sql.append(set.join(","));
  sql.append(",c_changed=CURRENT_TIMESTAMP");
  sql.append(" WHERE c_id=");
  sql.append(cid);
  sql.append(";");

  sendSqlQuery(sql);
}

void EditCostumer::createSqlInsert() {
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

  QString sql("INSERT INTO " + tableName + " (");
  sql.append(column.join(","));
  sql.append(",c_since,c_changed) VALUES (");
  sql.append(values.join(","));
  sql.append(",CURRENT_TIMESTAMP,CURRENT_TIMESTAMP);");

  if (sendSqlQuery(sql))
    checkLeaveEditor();
}

void EditCostumer::setSqlQueryData(const QString &key, const QVariant &value) {
  /** Dataset */
  if (key.contains("c_id")) {
    c_id->setValue(value);
    return;
  }
  if (key == "fullname" || key == "c_company_name") {
    infoLabel->setText(value.toString());
  }
  if (value.type() == QVariant::Bool) {
    if (key == "c_company") {
      m_contact->c_company->setValue(value);
      return;
    }
    BoolBox *v = findChild<BoolBox *>(key, findOption);
    if (v != nullptr)
      v->setValue(value);

    return;
  }
  if (value.type() == QVariant::Int) {
    if (key == "c_gender") {
      m_contact->c_gender->setValue(value);
      return;
    }
    if (key == "c_trusted") {
      m_billing->c_trusted->setValue(value);
      return;
    }
  }
  if (value.type() == QVariant::String) {
    StrLineEdit *l1 = findChild<StrLineEdit *>(key, findOption);
    if (l1 != nullptr) {
      l1->setValue(value);
      return;
    }
    LineEdit *l2 = findChild<LineEdit *>(key, findOption);
    if (l2 != nullptr) {
      l2->setValue(value);
      return;
    }
    PhoneEdit *l3 = findChild<PhoneEdit *>(key, findOption);
    if (l3 != nullptr) {
      l3->setValue(value);
      return;
    }
    EMailEdit *l4 = findChild<EMailEdit *>(key, findOption);
    if (l4 != nullptr) {
      l4->setValue(value);
      return;
    }
    TextField *l5 = findChild<TextField *>(key, findOption);
    if (l5 != nullptr) {
      l5->setValue(value);
      return;
    }
    if (key == "c_postalcode") {
      m_contact->c_postalcode->setValue(value);
      return;
    }
    if (key == "c_title") {
      m_contact->c_title->setValue(value);
      return;
    }
  }
  // qDebug() << "MISSING" << key << value;
}

void EditCostumer::resetModified() {
  foreach (QString name, inputList) {
    QObject *child = findChild<QObject *>(name, Qt::FindChildrenRecursively);
    if (child != nullptr) {
      QMetaObject::invokeMethod(child, "setModified", Qt::DirectConnection,
                                Q_ARG(bool, false));
    }
  }
}

void EditCostumer::saveData() {
  if (c_id->value().toInt() < 1) {
    createSqlInsert();
  } else if (c_id->value().toInt() > 0) {
    createSqlUpdate();
  }
}

void EditCostumer::clearDataFields() {
  foreach (QString name, inputList) {
    QObject *child = findChild<QObject *>(name, Qt::FindChildrenRecursively);
    if (child != nullptr) {
      QMetaObject::invokeMethod(child, "reset", Qt::DirectConnection);
    }
  }
}

bool EditCostumer::checkIsModified() {
  foreach (QString name, inputList) {
    QObject *child = findChild<QObject *>(name, Qt::FindChildrenRecursively);
    if (child != nullptr) {
      bool b = false;
      if (QMetaObject::invokeMethod(child, "isModified", Qt::DirectConnection,
                                    Q_RETURN_ARG(bool, b))) {

        if (b) {
          return true;
        }
      }
    }
  }
  return false;
}

void EditCostumer::checkLeaveEditor() {
  if (checkIsModified()) {
    emit s_postMessage(
        tr("Unsaved Changes, don't leave this page before saved."));
    return;
  }
  finalLeaveEditor();
}

void EditCostumer::finalLeaveEditor() {
  sqlQueryResult.clear();             /**< SQL History leeren */
  clearDataFields();                  /**< Alle Datenfelder leeren */
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  emit s_leaveEditor();               /**< Zurück */
}

void EditCostumer::restoreDataset() {
  if (sqlQueryResult.isEmpty())
    return;

  importSqlResult();
}

void EditCostumer::updateCostumer(const QString &id) {
  setInputList();
  QString sql("SELECT *, concat_ws(' ',c_firstname,c_lastname) AS fullname ");
  sql.append(" FROM costumers");
  sql.append(" WHERE " + id);
  sql.append(" ORDER BY c_id LIMIT 1;");

  QSqlQuery q = m_sql->query(sql);
  if (q.size() != 0) {
    if (SHOW_SQL_QUERIES) {
      qDebug() << Q_FUNC_INFO << sql;
    }
    QSqlRecord r = m_sql->record("costumers");
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
      if (q.value("fullname").isValid()) {
        QVariant val = q.value("fullname");
        DataEntries d;
        d.field = "fullname";
        d.vtype = val.type();
        d.data = val;
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
}

void EditCostumer::createCostumer() {
  setInputList();
  sqlQueryResult.clear();             /**< SQL History leeren */
  clearDataFields();                  /**< Alle Datenfelder leeren */
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  if (inputList.size()) {
    m_dataBox->setCurrentWidget(m_contact);
  }
}
