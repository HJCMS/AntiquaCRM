// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordereditor.h"
#include "applsettings.h"
#include "editoractionbar.h"
#include "myicontheme.h"
#include "ordersitemlist.h"
#include "orderstatements.h"

#include <QDebug>
#include <QMessageBox>
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

  o_order_status = new OrderStatusBox(this);
  o_order_status->setInfo(tr("Status"));
  row0->addWidget(o_order_status);

  o_payment_status = new OrdersPaymentBox(this);
  o_payment_status->setInfo(tr("Payment"));
  row0->addWidget(o_payment_status);

  o_delivery_service = new DeliveryService(this);
  o_delivery_service->setInfo(tr("Delivery Service"));
  row0->addWidget(o_delivery_service);

  row0->addStretch(1);
  mainLayout->addLayout(row0);

  QGridLayout *row1 = new QGridLayout();
  o_costumer_id = new SerialID(this);
  o_costumer_id->setInfo(tr("Costumer ID"));
  row1->addWidget(o_costumer_id, 0, 0, 1, 1, Qt::AlignLeft);
  o_provider = new LineEdit(this);
  o_provider->setInfo(tr("Provider"));
  row1->addWidget(o_provider, 0, 1, 1, 1);
  m_costumer_address = new TextField(this);
  row1->addWidget(m_costumer_address, 1, 0, 1, 1);
  m_provider_info = new TextField(this);
  row1->addWidget(m_provider_info, 1, 1, 1, 1);
  mainLayout->addLayout(row1);

  QHBoxLayout *row2 = new QHBoxLayout();
  row2->addStretch(1);
  o_locked = new BoolBox(this);
  o_locked->setInfo(tr("lock"));
  row2->addWidget(o_locked);
  o_closed = new BoolBox(this);
  o_closed->setInfo(tr("close"));
  row2->addWidget(o_closed);
  mainLayout->addLayout(row2);

  m_paymentList = new OrdersItemList(this);
  mainLayout->addWidget(m_paymentList);

  mainLayout->addStretch(1);

  m_actionBar = new EditorActionBar(this);
  mainLayout->addWidget(m_actionBar);

  setLayout(mainLayout);

  connect(m_actionBar, SIGNAL(s_cancelClicked()), this,
          SLOT(finalLeaveEditor()));
  connect(m_actionBar, SIGNAL(s_restoreClicked()), this,
          SLOT(restoreDataset()));
  connect(m_actionBar, SIGNAL(s_saveClicked()), this, SLOT(saveData()));
  connect(m_actionBar, SIGNAL(s_finishClicked()), this,
          SLOT(checkLeaveEditor()));

  connect(m_paymentList, SIGNAL(searchArticle(int)), this,
          SLOT(findArticle(int)));

  connect(m_paymentList, SIGNAL(statusMessage(const QString &)), this,
          SLOT(showMessagePoUp(const QString &)));

  connect(o_closed, SIGNAL(checked(bool)), this, SLOT(createCloseOrder(bool)));
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

  if (key == "o_id") {
    o_id->setValue(value);
    return;
  }

  if (key == "o_order_status") {
    o_order_status->setValue(value);
    return;
  }

  if (key == "o_payment_status") {
    o_payment_status->setValue(value);
    return;
  }

  if (key == "o_costumer_id") {
    o_costumer_id->setValue(value);
    setCostumerAddress(value.toInt());
    return;
  }

  if (key == "o_delivery_service") {
    o_delivery_service->setValue(value);
    return;
  }

  if (key == "o_provider") {
    o_provider->setValue(value);
    return;
  }

  if (key == "o_locked") {
    o_locked->setValue(value);
    return;
  }

  if (key == "o_closed") {
    o_closed->setValue(value);
    return;
  }

  qDebug() << "TODO:" << key << "setValue" << value << required;
}

void OrderEditor::findArticle(int aid) {
  QString select = findBookArticle(aid);
  if (select.isEmpty())
    return;

  QSqlQuery q = m_sql->query(select);
  if (q.size() > 0) {
    q.next();
    OrdersItemList::Article data;
    data.articleId = q.value("ib_id").toInt();
    data.count = q.value("ib_count").toInt();
    data.title = q.value("ib_title").toString();
    data.price = q.value("ib_price").toDouble();
    data.summary =
        tr("Article %1, Price %2, Count: %3, Title: %4")
            .arg(q.value("ib_id").toString(), q.value("ib_price").toString(),
                 q.value("ib_count").toString(),
                 q.value("ib_title").toString());
    m_paymentList->foundArticle(data);
  }
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

void OrderEditor::createCloseOrder(bool b) {
  if (!b)
    return;

  int order_id = o_id->value().toInt();
  QString body("<p>");
  body.append(tr("Do you really want to close this order and pass it on "
                 "to accounting?"));
  body.append("</p><p>");
  body.append(tr("If so, the entry will no longer be visible here!"));
  body.append("</p>");
  int ret = QMessageBox::question(this, tr("Finish order"), body);
  if (ret == QMessageBox::Yes) {
    qDebug() << Q_FUNC_INFO << "FUNCTION DISABLED TODO" << order_id;
    /*
    if (sendSqlQuery(closeOrder(order_id)))
      finalLeaveEditor();
    */
  } else {
    o_closed->setChecked(false);
  }
}

void OrderEditor::restoreDataset() {
  if (sqlQueryResult.isEmpty())
    return;

  importSqlResult();
}

void OrderEditor::showMessagePoUp(const QString &str) {
  QMessageBox::information(this, tr("Notice"), str, QMessageBox::Ok);
}

void OrderEditor::initDefaults() {
  setInputList();
  setEnabled(true);
  resetModified(inputList);
  o_delivery_service->loadSqlDataset();
}

void OrderEditor::setCostumerAddress(int id) {
  if (id < 1)
    return;

  QString select("SELECT c_postal_address FROM costumers WHERE c_id=");
  select.append(QString::number(id));
  select.append(";");
  QSqlQuery q = m_sql->query(select);
  if (q.size() > 0) {
    while (q.next()) {
      QVariant val = q.value("c_postal_address");
      if (!val.isNull()) {
        m_costumer_address->setValue(val);
        break;
      }
    }
  }
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
