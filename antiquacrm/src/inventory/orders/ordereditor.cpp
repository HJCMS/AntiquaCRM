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
  o_order_status->setObjectName("o_order_status");
  o_order_status->setInfo(tr("Status"));
  row0->addWidget(o_order_status);

  o_payment_status = new OrdersPaymentBox(this);
  o_payment_status->setObjectName("o_payment_status");
  o_payment_status->setInfo(tr("Payment"));
  row0->addWidget(o_payment_status);

  o_delivery_service = new DeliveryService(this);
  o_delivery_service->setObjectName("o_delivery_service");
  o_delivery_service->setInfo(tr("Delivery Service"));
  row0->addWidget(o_delivery_service);

  row0->addStretch(1);
  mainLayout->addLayout(row0);

  QGridLayout *row1 = new QGridLayout();
  o_costumer_id = new SerialID(this);
  o_costumer_id->setObjectName("o_costumer_id");
  o_costumer_id->setInfo(tr("Address for Costumer ID"));
  row1->addWidget(o_costumer_id, 0, 0, 1, 1, Qt::AlignLeft);
  o_provider_name = new LineEdit(this);
  o_provider_name->setObjectName("o_provider_name");
  o_provider_name->setInfo(tr("Provider Order"));
  row1->addWidget(o_provider_name, 0, 1, 1, 1);
  m_costumer_address = new TextField(this);
  m_costumer_address->setObjectName("m_costumer_address");
  row1->addWidget(m_costumer_address, 1, 0, 1, 1);
  o_provider_order = new TextField(this);
  o_provider_order->setObjectName("o_provider_order");
  row1->addWidget(o_provider_order, 1, 1, 1, 1);
  mainLayout->addLayout(row1);

  QHBoxLayout *row2 = new QHBoxLayout();
  row2->addStretch(1);
  o_notify = new BoolBox(this);
  o_notify->setObjectName("o_notify");
  o_notify->setInfo(tr("Notification"));
  row2->addWidget(o_notify);
  o_locked = new BoolBox(this);
  o_locked->setObjectName("o_locked");
  o_locked->setInfo(tr("lock"));
  row2->addWidget(o_locked);
  o_closed = new BoolBox(this);
  o_closed->setObjectName("o_closed");
  o_closed->setInfo(tr("close"));
  row2->addWidget(o_closed);
  mainLayout->addLayout(row2);

  m_paymentList = new OrdersItemList(this);
  m_paymentList->setObjectName("m_paymentList");
  mainLayout->addWidget(m_paymentList);

  mainLayout->addStretch(1);

  m_actionBar = new EditorActionBar(this);
  m_actionBar->setObjectName("m_actionBar");
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
  connect(o_costumer_id, SIGNAL(s_serialChanged(int)), this,
          SLOT(findCostumer(int)));
}

void OrderEditor::setInputList() {
  inputList = m_sql->fields("inventory_orders");
  if (inputList.isEmpty()) {
    qWarning("Costumers InputList is Empty!");
  }
  // Werden manuell gesetzt!
  inputList.removeOne("o_since");
  inputList.removeOne("o_modified");
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

const QHash<QString, QVariant> OrderEditor::createSqlDataset() {
  QHash<QString, QVariant> data;
  MessageBox messanger;
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
    // qDebug() << "Orders:" << cur->objectName() << cur->value();
    data.insert(cur->objectName(), cur->value());
  }
  list.clear();

  return data;
}

bool OrderEditor::sendSqlQuery(const QString &sqlStatement) {
  if (SHOW_SQL_QUERIES) {
    qDebug() << Q_FUNC_INFO << sqlStatement;
  }
  MessageBox messanger(this);
  QSqlQuery q = m_sql->query(sqlStatement);
  if (q.lastError().type() != QSqlError::NoError) {
    QString errorString = m_sql->fetchErrors();
    qDebug() << errorString << Qt::endl;
    messanger.failed(sqlStatement, errorString);
    return false;
  } else {
    messanger.success(tr("Order saved successfully!"), 1);
    resetModified(inputList);
    return true;
  }
}

void OrderEditor::createSqlUpdate() {
  QString oid = o_id->value().toString();
  if (oid.isEmpty())
    return;

  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 1)
    return;

  QStringList set;
  QHash<QString, QVariant>::iterator it;
  for (it = data.begin(); it != data.end(); ++it) {
    if (it.key() == "o_id")
      continue;

    if (it.value().type() == QVariant::String) {
      set.append(it.key() + "='" + it.value().toString() + "'");
    } else {
      set.append(it.key() + "=" + it.value().toString());
    }
  }

  QString sql("UPDATE inventory_orders SET ");
  sql.append(set.join(","));
  sql.append(",o_modified=CURRENT_TIMESTAMP");
  sql.append(" WHERE o_id=");
  sql.append(oid);
  sql.append(";");

  qDebug() << Q_FUNC_INFO << "WARNING TODO UPDATE INSERT article_orders ";

  m_paymentList->getArticleOrder();

  // sendSqlQuery(sql);
}

void OrderEditor::createSqlInsert() {
  qDebug() << Q_FUNC_INFO
           << "WARNING TODO INSERT inventory_orders,article_orders";
}

void OrderEditor::setData(const QString &key, const QVariant &value,
                          bool required) {
  if (key.isEmpty()) {
    qWarning("No setData Key:%s", qPrintable(key));
    return;
  }

  UtilsMain *inp = findChild<UtilsMain *>(key, Qt::FindChildrenRecursively);
  if (inp != nullptr) {
    inp->setValue(value);
    if (required && !inp->isRequired())
      inp->setRequired(required);

    return;
  }
  qDebug() << "Missing:" << key << " Value" << value
           << " Required:" << required;
}

void OrderEditor::findCostumer(int cid) { setCostumerAddress(cid); }

void OrderEditor::findArticle(int aid) {
  QString select = inventoryArticle(aid);
  if (select.isEmpty())
    return;

  if (SHOW_SQL_QUERIES) {
    qDebug() << Q_FUNC_INFO << select << Qt::endl;
  }

  QSqlQuery q = m_sql->query(select);
  if (q.size() > 0) {
    q.next();
    OrderArticle data;
    data.setArticle(q.value("aid").toInt());
    data.setCount(q.value("counts").toInt());
    data.setPrice(q.value("price").toDouble());
    data.setSellPrice(q.value("price").toDouble());
    data.setTitle(q.value("title").toString());
    data.setSummary(
        tr("Article %1, Price %2, Count: %3, Title: %4")
            .arg(q.value("aid").toString(), q.value("price").toString(),
                 q.value("counts").toString(), q.value("title").toString()));

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
    if (sendSqlQuery(closeOrder(order_id))) {
      emit postMessage(tr("Order deactivated!"));
      emit s_isModified(true);
    }
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

  QString select("SELECT c_postal_address,c_shipping_address");
  select.append(" FROM costumers WHERE c_id=");
  select.append(QString::number(id));
  select.append(";");

  if (SHOW_SQL_QUERIES) {
    qDebug() << Q_FUNC_INFO << select << Qt::endl;
  }

  QSqlQuery q = m_sql->query(select);
  QString buffer;
  if (q.size() > 0) {
    while (q.next()) {
      buffer.append(q.value("c_postal_address").toString());
      QVariant val = q.value("c_shipping_address");
      if (!val.toString().isEmpty()) {
        buffer.append("\n\n" + tr("Shipping Address") + ":\n");
        buffer.append(val.toString());
        break;
      }
    }
    m_costumer_address->setValue(buffer);
  } else {
    qWarning("SQL ERROR: %s", qPrintable(m_sql->lastError()));
  }
}

void OrderEditor::updateOrder(int oid) {
  initDefaults();
  if (oid < 1) {
    qWarning("Empty o_id ...");
    return;
  }

  QString select("SELECT * FROM inventory_orders WHERE o_id=");
  select.append(QString::number(oid));
  select.append(";");

  if (SHOW_SQL_QUERIES) {
    qDebug() << Q_FUNC_INFO << select << Qt::endl;
  }

  QSqlQuery q = m_sql->query(select);
  if (q.size() > 0) {
    QSqlRecord r = m_sql->record("inventory_orders");
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

void OrderEditor::createOrder(int cid) {
  initDefaults();
  if (cid > 0) {
    o_costumer_id->setValue(cid);
    setCostumerAddress(cid);
  }
}
