// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "editcostumer.h"
#include "antiqua_global.h"
#include "costumerbillinginfo.h"
#include "costumercontact.h"
#include "costumeroverview.h"
#include "editoractionbar.h"
#include "genderbox.h"
#include "myicontheme.h"
#include "serialid.h"

#include <QDebug>
#include <QHBoxLayout>
#include <QVBoxLayout>

// Schalte SQL ausgaben ein
#ifndef SHOW_SQL_QUERIES
#define SHOW_SQL_QUERIES false
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
  inputList = m_sql->fields("costumers");
  if (inputList.isEmpty()) {
    qWarning("Costumers InputList is Empty!");
  }
  /** Werden Manuel gesetzt! */
  inputList.removeOne("c_since");
  inputList.removeOne("c_changed");
}

void EditCostumer::importSqlResult() {
  if (sqlQueryResult.size() < 15)
    return;

  blockSignals(true);
  for (int i = 0; i < sqlQueryResult.size(); ++i) {
    DataField f = sqlQueryResult.at(i);
    setData(f.field(), f.value(), f.isRequired());
  }
  blockSignals(false);

  m_overview->createDocument(sqlQueryResult);
  resetModified(inputList);
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
    resetModified(inputList);
    return true;
  }
}

const QHash<QString, QVariant> EditCostumer::createSqlDataset() {
  QHash<QString, QVariant> data;
  MessageBox messanger(this);
  QList<UtilsMain *> list =
      findChildren<UtilsMain *>(p_objPattern, Qt::FindChildrenRecursively);
  QList<UtilsMain *>::Iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    UtilsMain *cur = *it;
    if (cur->isRequired() && !cur->isValid()) {
      messanger.notice(cur->notes());
      cur->setFocus();
      data.clear();
      return data;
    }
    // qDebug() << "Costumer:" << cur->objectName() << cur->value();
    // 049 152 29331138
    data.insert(cur->objectName(), cur->value());
  }
  list.clear();
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
    if (it.key() == "c_id")
      continue;

    if (it.value().type() == QVariant::String) {
      set.append(it.key() + "='" + it.value().toString() + "'");
    } else {
      set.append(it.key() + "=" + it.value().toString());
    }
  }

  QString sql("UPDATE costumers SET ");
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

  QString sql("INSERT INTO costumers (");
  sql.append(column.join(","));
  sql.append(",c_since,c_changed) VALUES (");
  sql.append(values.join(","));
  sql.append(",CURRENT_TIMESTAMP,CURRENT_TIMESTAMP);");

  if (sendSqlQuery(sql))
    checkLeaveEditor();
}

void EditCostumer::setData(const QString &key, const QVariant &value,
                           bool required) {
  if (key.isEmpty())
    return;

  if (key == "fullname" || key == "c_company_name") {
    infoLabel->setText(value.toString());
  }

  UtilsMain *inp = findChild<UtilsMain *>(key, Qt::FindChildrenRecursively);
  if (inp != nullptr) {
    inp->setValue(value);
    if (required && !inp->isRequired())
      inp->setRequired(required);

    return;
  }
  qDebug() << "Missing Key:" << key << " Value" << value
           << " Required:" << required;
}

void EditCostumer::saveData() {
  if (c_id->value().toInt() < 1) {
    createSqlInsert();
  } else if (c_id->value().toInt() > 0) {
    createSqlUpdate();
  }
}

void EditCostumer::checkLeaveEditor() {
  if (checkIsModified(p_objPattern)) {
    emit s_postMessage(
        tr("Unsaved Changes, don't leave this page before saved."));
    return;
  }
  finalLeaveEditor();
}

void EditCostumer::finalLeaveEditor() {
  sqlQueryResult.clear();             /**< SQL History leeren */
  clearDataFields(p_objPattern);      /**< Alle Datenfelder leeren */
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
}

void EditCostumer::createCostumer() {
  setInputList();
  sqlQueryResult.clear();             /**< SQL History leeren */
  clearDataFields(p_objPattern);      /**< Alle Datenfelder leeren */
  m_actionBar->setRestoreable(false); /**< ResetButton off */
  m_dataBox->setCurrentWidget(m_contact);
  resetModified(inputList);
}
