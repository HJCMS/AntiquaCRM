// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordereditor.h"
#include "applsettings.h"
#include "orderspaymentbox.h"
#include "orderstatements.h"
#include "orderstatusbox.h"

#include <QtCore/QDebug>
#include <QtWidgets>

OrderEditor::OrderEditor(QWidget *parent) : EditorMain{parent} {
  setObjectName("order_editor");
  setWindowTitle(tr("Edit Order"));

  ApplSettings config;

  Qt::Alignment defaultAlignment = (Qt::AlignRight | Qt::AlignVCenter);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setObjectName("mainLayout");

  QHBoxLayout *row0 = new QHBoxLayout();
  o_id = new SerialID(this);
  o_id->setObjectName("o_id");
  o_id->setInfo(tr("Order ID"));
  o_id->setRequired(true);
  row0->addWidget(o_id);

  mainLayout->addLayout(row0);

  setLayout(mainLayout);
}

void OrderEditor::setInputList() {
  inputList = m_sql->fields(tableName);
  if (inputList.isEmpty()) {
    qWarning("Costumers InputList is Empty!");
  }
}

void OrderEditor::importSqlResult() {
  if (sqlQueryResult.size() < 15)
    return;

  blockSignals(true);
  for (int i = 0; i < sqlQueryResult.size(); ++i) {
    DataField f = sqlQueryResult.at(i);
    setData(f.field(), f.value(), f.isRequired());
  }
  blockSignals(false);

  // Nach Ersteintrag zurück setzen!
  resetModified(inputList);
}

bool OrderEditor::sendSqlQuery(const QString &sqlStatement) {
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
    msgBox.success(tr("Order saved successfully!"), 1);
    resetModified(inputList);
    return true;
  }
}

void OrderEditor::createSqlUpdate() {
  qDebug() << Q_FUNC_INFO << "__TODO__" << Qt::endl;
}

void OrderEditor::createSqlInsert() {
  qDebug() << Q_FUNC_INFO << "__TODO__" << Qt::endl;
}

void OrderEditor::setData(const QString &key, const QVariant &value,
                          bool required) {}

void OrderEditor::saveData() {
  if (o_id->value().toString().isEmpty()) {
    createSqlInsert();
  } else {
    createSqlUpdate();
  }
}

void OrderEditor::checkLeaveEditor() {
  if (checkIsModified(p_objPattern)) {
    emit s_postMessage(
        tr("Unsaved Changes, don't leave this page before saved."));
    return;
  }
  finalLeaveEditor();
}

void OrderEditor::finalLeaveEditor() {
  sqlQueryResult.clear();        /**< SQL History leeren */
  clearDataFields(p_objPattern); /**< Alle Datenfelder leeren */
  emit s_leaveEditor();          /**< Zurück */
}

void OrderEditor::restoreDataset() {
  if (sqlQueryResult.isEmpty())
    return;

  importSqlResult();
}

void OrderEditor::updateOrder(int order) {
  if (order < 1)
    return;

  QString select = defaultQuery(order);
  QSqlQuery q = m_sql->query(select);
  if (q.size() != 0) {
    QSqlRecord r = m_sql->record(tableName);
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
    }
  } else {
    MessageBox messanger(this);
    messanger.failed(m_sql->fetchErrors(), select);
    return;
  }

  setEnabled(true);
  if (!sqlQueryResult.isEmpty())
    importSqlResult();
}

void OrderEditor::createOrder(int costumerId) {
  if(costumerId > 0)
  {
    qDebug() << Q_FUNC_INFO << costumerId;
  }
  setEnabled(true);
  resetModified(inputList);
}
