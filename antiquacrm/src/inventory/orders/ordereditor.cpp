// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordereditor.h"
#include "applsettings.h"
#include "orderspaymentbox.h"
#include "orderstatements.h"
#include "orderstatusbox.h"

#include <QDebug>
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

  QLabel *m_infoOrderStatus = new QLabel(this);
  m_infoOrderStatus->setText(tr("Status")+":");
  row0->addWidget(m_infoOrderStatus);

  o_order_status = new OrderStatusBox(this);
  row0->addWidget(o_order_status);

  QLabel *m_payment = new QLabel(this);
  m_payment->setText(tr("Payment")+":");
  row0->addWidget(m_payment);

  o_payment_status = new OrdersPaymentBox(this);
  row0->addWidget(o_payment_status);

  o_delivery_service = new DeliveryService(this);
  o_delivery_service->setInfo(tr("Delivery Service"));
  row0->addWidget(o_delivery_service);

  row0->addStretch(1);
  mainLayout->addLayout(row0);

  QHBoxLayout *row1 = new QHBoxLayout();
  o_locked = new BoolBox(this);
  o_locked->setInfo(tr("lock"));
  row1->addWidget(o_locked);

  o_closed = new BoolBox(this);
  o_closed->setInfo(tr("close"));
  row1->addWidget(o_closed);

  row1->addStretch(1);
  mainLayout->addLayout(row1);

  mainLayout->addStretch(1);
  setLayout(mainLayout);
}

void OrderEditor::setInputList() {
  inputList = m_sql->fields(tableName);
  if (inputList.isEmpty()) {
    qWarning("Costumers InputList is Empty!");
  }
  inputList.removeOne("o_since");
}

void OrderEditor::importSqlResult() {
  if (sqlQueryResult.size() < 5)
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
                          bool required) {
  qDebug() << "DATA:" << key << value << required;
}

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

void OrderEditor::initDefaults()
{
  setInputList();
  setEnabled(true);
  resetModified(inputList);
  o_delivery_service->loadSqlDataset();
}

void OrderEditor::updateOrder(int order) {
  initDefaults();
  if (order < 1) {
    qWarning("Empty o_id ...");
    return;
  }

  QString select("SELECT * FROM " + tableName + " WHERE o_id=");
  select.append(QString::number(order));
  select.append(";");

  QSqlQuery q = m_sql->query(select);
  if (q.size() > 0) {
    QSqlRecord r = m_sql->record(tableName);
    sqlQueryResult.clear();
    while (q.next()) {
      foreach (QString key, inputList) {
        QVariant val = q.value(r.indexOf(key));
        bool required = (r.field(key).requiredStatus() == QSqlField::Required);
        // qDebug() << Q_FUNC_INFO << key << val << required;
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

  if (!sqlQueryResult.isEmpty())
    importSqlResult();
}

void OrderEditor::createOrder(int costumerId) {
  initDefaults();
  if (costumerId > 0) {
    qDebug() << Q_FUNC_INFO << costumerId;
  }
}
