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
  c_id->setValue(0);
  headerLayout->addWidget(c_id);

  infoLabel = new QLabel(this);
  infoLabel->setText("Buch- und Kunsthandlung Antiquariat Wilms");
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
  for (int i = 0; i < sqlQueryResult.size(); ++i) {
    DataEntries f = sqlQueryResult.at(i);
    setSqlQueryData(f.field, f.data);
  }
  blockSignals(false);

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

void EditCostumer::createSqlUpdate() { qDebug() << Q_FUNC_INFO << "__TODO__"; }

void EditCostumer::createSqlInsert() { qDebug() << Q_FUNC_INFO << "__TODO__"; }

void EditCostumer::setSqlQueryData(const QString &key, const QVariant &value) {
  Qt::FindChildOptions option(Qt::FindChildrenRecursively);
  /** Overview */
  m_overview->setTextBlock(key, value);
  /** Dataset */
  if (key.contains("c_id")) {
    c_id->setValue(value);
    return;
  }
  if (key == "fullname" || key == "c_company_name") {
    infoLabel->setText(value.toString());
  }
  if (value.type() == QVariant::Bool) {
    BoolBox *v = findChild<BoolBox *>(key, option);
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
    StrLineEdit *l1 = findChild<StrLineEdit *>(key, option);
    if (l1 != nullptr) {
      l1->setValue(value);
      return;
    }
    LineEdit *l2 = findChild<LineEdit *>(key, option);
    if (l2 != nullptr) {
      l2->setValue(value);
      return;
    }
    PhoneEdit *l3 = findChild<PhoneEdit *>(key, option);
    if (l3 != nullptr) {
      l3->setValue(value);
      return;
    }
    EMailEdit *l4 = findChild<EMailEdit *>(key, option);
    if (l4 != nullptr) {
      l4->setValue(value);
      return;
    }
    TextField *l5 = findChild<TextField *>(key, option);
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
  if (c_id->value().toString().isEmpty()) {
    createSqlInsert();
  } else {
    createSqlUpdate();
  }
}

void EditCostumer::clearDataFields() {
  foreach (QString name, inputList) {
    QObject *child = findChild<QObject *>(name, Qt::FindChildrenRecursively);
    if (child != nullptr) {
      QMetaObject::invokeMethod(child, "reset", Qt::DirectConnection);
    } else {
      qDebug() << Q_FUNC_INFO << "MISSING" << name;
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
      if(q.value("fullname").isValid())
      {
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

void EditCostumer::createCostumer() { setInputList(); }
