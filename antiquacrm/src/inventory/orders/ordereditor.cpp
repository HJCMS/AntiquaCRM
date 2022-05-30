// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "ordereditor.h"
#include "deliverynote.h"
#include "editoractionbar.h"
#include "invoice.h"
#include "myicontheme.h"
#include "ordersitemlist.h"
#include "orderstatements.h"
#include <AntiquaCRM>

#include <QDebug>
#include <QMessageBox>
#include <QtWidgets>

OrderEditor::OrderEditor(QWidget *parent) : EditorMain{parent} {
  setObjectName("order_editor");
  setWindowTitle(tr("Edit Order"));

  /**
   * Tabellenfelder welche NICHT bei INSERT/UPDATE
   * benötigt werden aber im Overview enthalten sind.
   */
  ignoreList.clear();
  ignoreList.append("o_since");
  ignoreList.append("o_modified");

  ignoreOnInsert.clear();
  ignoreOnInsert.append("o_invoice_id");
  ignoreOnInsert.append("o_delivery");

  Qt::Alignment defaultAlignment = (Qt::AlignRight | Qt::AlignVCenter);

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(2, 2, 2, 2);
  mainLayout->setObjectName("mainLayout");

  QScrollArea *m_scroolView = new QScrollArea(this);
  m_scroolView->setObjectName("editor_scroll_view");
  m_scroolView->setWidgetResizable(true);

  QWidget *mainWidget = new QWidget(m_scroolView);
  mainWidget->setObjectName("editor_main_widget");
  mainWidget->setMinimumHeight(680);
  m_scroolView->setWidget(mainWidget);

  QVBoxLayout *mainWidgetLayout = new QVBoxLayout(mainWidget);
  mainWidgetLayout->setObjectName("mainLayout");

  QHBoxLayout *row0 = new QHBoxLayout();
  o_id = new SerialID(this);
  o_id->setObjectName("o_id");
  o_id->setInfo(tr("Order ID"));
  o_id->setRequired(true);
  row0->addWidget(o_id);

  o_invoice_id = new SerialID(this);
  o_invoice_id->setObjectName("o_invoice_id");
  o_invoice_id->setInfo(tr("Invoice ID"));
  row0->addWidget(o_invoice_id);

  o_order_status = new OrderStatusBox(mainWidget);
  o_order_status->setObjectName("o_order_status");
  o_order_status->setInfo(tr("Status"));
  o_order_status->setValue(1);
  row0->addWidget(o_order_status);

  o_payment_status = new OrdersPaymentBox(mainWidget);
  o_payment_status->setObjectName("o_payment_status");
  o_payment_status->setInfo(tr("Payment"));
  o_payment_status->setValue(0);
  row0->addWidget(o_payment_status);

  row0->addStretch(1);
  mainWidgetLayout->addLayout(row0);

  // Zweite Reihe
  QGridLayout *row1 = new QGridLayout();
  o_customer_id = new SerialID(mainWidget);
  o_customer_id->setObjectName("o_customer_id");
  o_customer_id->setInfo(tr("Address for Customer ID"));
  row1->addWidget(o_customer_id, 0, 0, 1, 2, Qt::AlignLeft);

  /*
   * GridLayout links
   * Dient nur zur Info die Daten werden mit Tabelle "customers" Verknüpft!
   */
  m_customer_address = new TextField(mainWidget);
  m_customer_address->setObjectName("m_customer_address");
  m_customer_address->setToolTip(tr("Customer Info"));
  row1->addWidget(m_customer_address, 1, 0, 1, 1);

  // GridLayout rechts
  QVBoxLayout *rFrameLayout = new QVBoxLayout();
  // Lieferdienst
  m_deliveryBox = new QGroupBox(this);
  m_deliveryBox->setTitle(tr("Delivery Service"));
  QGridLayout *dsLayout = new QGridLayout(m_deliveryBox);
  o_delivery_service = new DeliveryService(m_deliveryBox);
  o_delivery_service->setObjectName("o_delivery_service");
  o_delivery_service->setInfo(tr("Service"));
  dsLayout->addWidget(o_delivery_service, 0, 0, 1, 1, Qt::AlignLeft);
  o_notify = new BoolBox(this);
  o_notify->setObjectName("o_notify");
  o_notify->setInfo(tr("Notification"));
  dsLayout->addWidget(o_notify, 0, 1, 1, 2, Qt::AlignRight);

  o_delivery_send_id = new LineEdit(m_deliveryBox);
  o_delivery_send_id->setObjectName("o_delivery_send_id");
  o_delivery_send_id->setInfo(tr("Package send id"));
  dsLayout->addWidget(o_delivery_send_id, 1, 0, 1, 2);
  o_delivery = new LineEdit(m_deliveryBox);
  o_delivery->setObjectName("o_delivery");
  o_delivery->setInfo(tr("Delivery note number"));
  dsLayout->addWidget(o_delivery, 2, 1, 1, 1);
  QToolButton *btn_gen_deliver = new QToolButton(m_deliveryBox);
  btn_gen_deliver->setObjectName("btn_gen_delivernote_id");
  btn_gen_deliver->setToolTip(tr("Generate delivery note Number"));
  btn_gen_deliver->setIcon(myIcon("folder_public"));
  dsLayout->addWidget(btn_gen_deliver, 2, 2, 1, 1);
  rFrameLayout->addWidget(m_deliveryBox);

  // Dienstleister
  m_providerBox = new QGroupBox(this);
  m_providerBox->setTitle(tr("Provider"));
  QVBoxLayout *providerLayout = new QVBoxLayout(m_providerBox);
  o_provider_name = new LineEdit(mainWidget);
  o_provider_name->setObjectName("o_provider_name");
  o_provider_name->setInfo(tr("Provider"));
  providerLayout->addWidget(o_provider_name);
  o_provider_order_id = new LineEdit(mainWidget);
  o_provider_order_id->setObjectName("o_provider_order_id");
  o_provider_order_id->setInfo("Id");
  providerLayout->addWidget(o_provider_order_id);
  m_providerBox->setLayout(providerLayout);
  rFrameLayout->addWidget(m_providerBox);

  rFrameLayout->addStretch(1);
  row1->addLayout(rFrameLayout, 1, 1, 1, 1);
  mainWidgetLayout->addLayout(row1);

  // Artikel Einkaufsliste
  m_paymentList = new OrdersItemList(mainWidget);
  m_paymentList->setObjectName("m_paymentList");
  mainWidgetLayout->addWidget(m_paymentList);

  m_actionsBox = new QGroupBox(this);
  QHBoxLayout *actionsLayout = new QHBoxLayout(m_actionsBox);
  o_closed = new BoolBox(this);
  o_closed->setObjectName("o_closed");
  o_closed->setInfo(tr("close"));
  actionsLayout->addWidget(o_closed);
  o_locked = new BoolBox(this);
  o_locked->setObjectName("o_locked");
  o_locked->setInfo(tr("lock"));
  actionsLayout->addWidget(o_locked);
  actionsLayout->addStretch(1);
  m_actionsBox->setLayout(actionsLayout);

  mainWidgetLayout->addWidget(m_actionsBox, Qt::AlignLeft);

  mainWidgetLayout->addStretch(1);
  mainWidget->setLayout(mainWidgetLayout);
  mainLayout->addWidget(m_scroolView);

  m_actionBar = new EditorActionBar(this);
  m_actionBar->setObjectName("m_actionBar");
  m_actionBar->viewPrintButton(true);
  mainLayout->addWidget(m_actionBar);

  setLayout(mainLayout);

  // SIGNALS
  QList<UtilsMain *> list =
      findChildren<UtilsMain *>(p_objPattern, Qt::FindChildrenRecursively);
  QList<UtilsMain *>::Iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    UtilsMain *e = *it;
    connect(e, SIGNAL(hasModified(bool)), this, SLOT(setWindowModified(bool)));
  }
  connect(m_paymentList, SIGNAL(hasModified(bool)), this,
          SLOT(setWindowModified(bool)));

  connect(btn_gen_deliver, SIGNAL(clicked()), this,
          SLOT(generateDeliveryNumber()));
  connect(m_actionBar, SIGNAL(s_cancelClicked()), this,
          SLOT(finalLeaveEditor()));
  connect(m_actionBar, SIGNAL(s_restoreClicked()), this,
          SLOT(restoreDataset()));
  connect(m_actionBar, SIGNAL(s_saveClicked()), this, SLOT(saveData()));
  connect(m_actionBar, SIGNAL(s_finishClicked()), this,
          SLOT(checkLeaveEditor()));
  connect(m_actionBar, SIGNAL(s_printDeliveryNote()), this,
          SLOT(openPrinterDeliveryDialog()));
  connect(m_actionBar, SIGNAL(s_printInvoiceNote()), this,
          SLOT(openPrinterInvoiceDialog()));
  connect(m_paymentList, SIGNAL(searchArticle(int)), this,
          SLOT(findArticle(int)));
  connect(m_paymentList, SIGNAL(statusMessage(const QString &)), this,
          SLOT(showMessagePoUp(const QString &)));
  connect(o_closed, SIGNAL(checked(bool)), this, SLOT(createCloseOrder(bool)));
  connect(o_customer_id, SIGNAL(s_serialChanged(int)), this,
          SLOT(findCustomer(int)));
  connect(m_paymentList, SIGNAL(askToRemoveRow(int)), this,
          SLOT(findRemoveTableRow(int)));
}

void OrderEditor::setInputList() {
  inputList = m_sql->fields("inventory_orders");
  if (inputList.isEmpty()) {
    qWarning("Customers InputList is Empty!");
  }
  /**
   * Werden manuell gesetzt oder sind hier nicht notwendig!
   */
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
  m_paymentList->setModified(false);
}

const QHash<QString, QVariant> OrderEditor::createSqlDataset() {
  QHash<QString, QVariant> data;
  QList<UtilsMain *> list =
      findChildren<UtilsMain *>(p_objPattern, Qt::FindChildrenRecursively);
  QList<UtilsMain *>::Iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    UtilsMain *cur = *it;
    if (ignoreList.contains(cur->objectName(), Qt::CaseSensitive))
      continue;

    if (cur->isRequired() && !cur->isValid()) {
      sqlNoticeMessage(cur->notes());
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
  QSqlQuery q = m_sql->query(sqlStatement);
  if (q.lastError().type() != QSqlError::NoError) {
    sqlErrnoMessage(sqlStatement, m_sql->fetchErrors());
    return false;
  } else {
    if (q.next()) {
      if (!q.record().isNull("o_id")) {
        o_id->setValue(q.value("o_id"));
      }
      if (!q.record().isNull("a_payment_id")) {
        last_payment_id = q.value("a_payment_id").toInt();
      }
    }
    if (showSuccessFully) {
      sqlSuccessMessage(tr("Order saved successfully!"));
    }
    resetModified(inputList);
    m_paymentList->setModified(false);
    emit s_isModified(false);
    return true;
  }
}

bool OrderEditor::createSqlArticleOrder() {
  int payments = m_paymentList->payments();
  if (payments < 1) {
    qWarning("Create SQL Article Order - Missing Payments");
    return false;
  }

  /**
   * @warning Die Id's "oid" und "cid" sind hier zwingend Notwendig!
   */
  int oid = o_id->value().toInt();
  if (oid < 1) {
    qWarning("Missing Order ID");
    return false;
  }

  int cid = o_customer_id->value().toInt();
  if (o_customer_id->value().toString().isEmpty() || cid < 1) {
    qWarning("Missing Customer ID");
    return false;
  }

  QStringList queries;
  for (int row = 0; row < payments; row++) {
    QStringList fields;
    QStringList values;
    QHash<QString, QString> updateSet;
    QHashIterator<QString, QVariant> it(m_paymentList->getTableRow(row));
    int primaryIndex = 0;
    while (it.hasNext()) {
      it.next();
      QString key = it.key();
      if (key == "a_payment_id") {
        int index = it.value().toInt();
        if (index >= 1) {
          primaryIndex = index;
        }
        continue;
      }
      fields.append(key);
      if (key == "a_order_id") {
        updateSet.insert(key, o_id->value().toString());
        values.append(o_id->value().toString());
        continue;
      } else if (key == "a_customer_id") {
        updateSet.insert(key, o_customer_id->value().toString());
        values.append(o_customer_id->value().toString());
        continue;
      }

      QString data = it.value().toString();
      if (it.value().type() == QVariant::String) {
        data = "'" + it.value().toString() + "'";
        updateSet.insert(key, data);
        values.append(data);
      } else {
        updateSet.insert(key, data);
        values.append(data);
      }
    }
    if (primaryIndex > 0) {
      QString sql("UPDATE article_orders SET ");
      QHashIterator<QString, QString> fields(updateSet);
      while (fields.hasNext()) {
        fields.next();
        sql.append(fields.key() + "=" + fields.value() + ",");
      }
      // qDebug() << Q_FUNC_INFO << it.key() << it.value() << sql;
      sql.append("a_modified=CURRENT_TIMESTAMP WHERE a_payment_id=");
      sql.append(QString::number(primaryIndex));
      sql.append(";");
      queries.append(sql);
    } else {
      QString sql("INSERT INTO article_orders (");
      sql.append(fields.join(","));
      sql.append(") VALUES (");
      sql.append(values.join(","));
      sql.append(") RETURNING a_payment_id;");
      queries.append(sql);
    }
  }
  if (sendSqlQuery(queries.join(" "))) {
    /**
     * @note Wir müssen hier alles neu einlesen!
     * Weil "a_payment_id" sonst in der ArtikelTabelle
     * fehlt und es zu Doppelten Einträgen kommt!
     */
    const QList<OrderArticle> list = getOrderArticles(oid, cid);
    if (list.size() >= 1) {
      m_paymentList->clearTable();
      m_paymentList->importPayments(list);
      m_paymentList->setModified(false);
    }
    return true;
  }
  return false;
}

void OrderEditor::createSqlUpdate() {
  QString oid = o_id->value().toString();
  if (oid.isEmpty())
    return;

  QString did = o_delivery->value().toString();
  if (did.isEmpty()) {
    generateDeliveryNumber();
  }

  o_id->setRequired(true);
  o_invoice_id->setRequired(true);
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

  // Artikel Bestelliste aktualisieren
  showSuccessFully = false;
  if (!createSqlArticleOrder()) {
    qWarning("UPDATE canceld");
    return;
  }
  showSuccessFully = true;

  QString sql("UPDATE inventory_orders SET ");
  sql.append(set.join(","));
  sql.append(",o_modified=CURRENT_TIMESTAMP");
  sql.append(" WHERE o_id=");
  sql.append(oid);
  sql.append(";");
  sendSqlQuery(sql);
}

void OrderEditor::createSqlInsert() {
  if (!o_id->value().toString().isEmpty())
    return;

  o_id->setRequired(false);
  o_invoice_id->setRequired(false);
  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 1)
    return;

  QStringList fields;
  QStringList values;
  QHash<QString, QVariant>::iterator it;
  for (it = data.begin(); it != data.end(); ++it) {
    if (it.key() == "o_id")
      continue;

    if (ignoreOnInsert.contains(it.key(), Qt::CaseSensitive))
      continue;

    fields.append(it.key());
    if (it.value().type() == QVariant::String) {
      values.append("'" + it.value().toString() + "'");
    } else {
      values.append(it.value().toString());
    }
  }

  QString sql("INSERT INTO inventory_orders (");
  sql.append(fields.join(","));
  sql.append(") VALUES (");
  sql.append(values.join(","));
  sql.append(") RETURNING o_id;");

  bool result = sendSqlQuery(sql);
  if (result && !o_id->value().toString().isEmpty()) {
    /*
     *  Erstelle Lieferschein Nummer
     */
    generateDeliveryNumber();

    initInvoiceNumber(o_id->value().toInt());
    /*
     * Wenn die Artikel-Bestellisten Tabelle nicht leer ist.
     * Danach createSqlArticleOrder() aufrufen!
     */
    m_paymentList->setEnabled(true);
    if (m_paymentList->payments() > 0) {
      createSqlArticleOrder();
    }
  }
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
  qDebug() << "Missing k:" << key << " v:" << value << " r:" << required;
}

void OrderEditor::findCustomer(int cid) {
  if (cid <= 1)
    return;

  if (!getCustomerAddress(cid)) {
    qWarning("No Customer Data");
  }
}

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
    data.setPayment(-1); // Darf keine positive Zahl sein!
    data.setArticle(q.value("aid").toInt());
    int count = q.value("counts").toInt();
    if (count > 0) {
      // Wir fügen immer nur ein Artikel ein!
      count = 1;
    }
    data.setCount(count);
    data.setPrice(q.value("price").toDouble());
    data.setSellPrice(q.value("price").toDouble());
    data.setTitle(q.value("title").toString());
    data.setSummary(
        tr("Article %1, Price %2, Count: %3, Title: %4")
            .arg(q.value("aid").toString(), q.value("price").toString(),
                 q.value("counts").toString(), q.value("title").toString()));

    m_paymentList->addArticleRow(data);
  }
}

void OrderEditor::findRemoveTableRow(int row) {
  QHash<QString, QVariant> items(m_paymentList->getTableRow(row));
  QString pId = items.value("a_payment_id").toString();
  QString aId = items.value("a_article_id").toString();
  QString body("<p>");
  body.append(tr("Do you really want to remove this Article from the list?"));
  body.append("</p><p>" + items.value("a_title").toString() + "</p><p>");
  body.append("</p><p>");
  body.append(tr("If so, the entry will no longer be visible here!"));
  body.append("</p>");
  int ret = QMessageBox::question(this, tr("Delete"), body);
  if (ret == QMessageBox::Yes) {
    m_paymentList->removeTableRow(row);
    showSuccessFully = false;
    if (sendSqlQuery(paymentRemove(pId, aId))) {
      emit postMessage(tr("Item removed!"));
      m_paymentList->setModified(true);
    }
    showSuccessFully = true;
  }
  pId.clear();
  aId.clear();
  items.clear();
}

void OrderEditor::saveData() {
  if (o_id->value().toString().isEmpty()) {
    createSqlInsert();
  } else {
    createSqlUpdate();
  }
}

void OrderEditor::checkLeaveEditor() {
  QString notify(tr("Unsaved Changes, don't leave this page before saved."));
  if (checkIsModified(p_objPattern)) {
    emit s_postMessage(notify);
    return;
  }
  /** @note Muss Manuel gesetzt werden */
  if (m_paymentList->isModified()) {
    emit s_postMessage(notify);
    return;
  }
  finalLeaveEditor();
}

void OrderEditor::finalLeaveEditor() {
  sqlQueryResult.clear();        /**< SQL History leeren */
  clearDataFields(p_objPattern); /**< Alle Datenfelder leeren */
  m_paymentList->clearTable();   /**< Tabelle leeren */
  emit s_leaveEditor();          /**< Zurück */
}

void OrderEditor::openPrinterDeliveryDialog() {
  int oid = o_id->value().toInt();
  if (oid < 1) {
    emit s_postMessage(tr("Missing Order-Id"));
    return;
  }

  int cid = o_customer_id->value().toInt();
  if (cid < 1) {
    emit s_postMessage(tr("Missing Customer-Id"));
    return;
  }

  QString did = o_delivery->value().toString();
  if (did.isEmpty()) {
    did = deliveryNumber();
    o_delivery->setValue(did);
  }

  DeliveryNote *dialog = new DeliveryNote(this);
  dialog->setObjectName("print_delivery_note_dialog");
  dialog->setDelivery(oid, cid, did);
  // Address
  QString c_add;
  QString sql = queryCustomerShippingAddress(cid);
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    q.next();
    c_add.append(q.value(0).toString());
  } else {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
    emit s_postMessage(tr("No Customer Address found"));
    return;
  }
  dialog->setCustomerAddress(c_add);
  // Articles
  QList<DeliveryNote::Delivery> deliveries;
  q = m_sql->query(queryDeliveryNotes(oid, cid));
  QRegExp strip("\\-\\s+\\-");
  while (q.next()) {
    DeliveryNote::Delivery d;
    d.articleid = q.value("aid").toString();
    d.designation = q.value("title").toString().replace(strip, "-");
    d.quantity = q.value("quant").toString();
    deliveries.append(d);
  }
  if (deliveries.count() < 1) {
    qDebug() << m_sql->lastError();
    emit s_postMessage(tr("no deliveries found"));
    return;
  }
  // Start Dialog
  if (dialog->exec(deliveries)) {
    qDebug() << Q_FUNC_INFO << "Finished";
  }
}

void OrderEditor::openPrinterInvoiceDialog() {
  int oid = o_id->value().toInt();
  if (oid < 1) {
    emit s_postMessage(tr("Missing Order-Id"));
    return;
  }

  int cid = o_customer_id->value().toInt();
  if (cid < 1) {
    emit s_postMessage(tr("Missing Customer-Id"));
    return;
  }

  int in_id = o_invoice_id->value().toInt();
  if (in_id < 1) {
    emit s_postMessage(tr("Missing Invoice-Id"));
    return;
  }

  QString did = o_delivery->value().toString();
  if (did.isEmpty()) {
    did = deliveryNumber();
    o_delivery->setValue(did);
  }

  Invoice *dialog = new Invoice(this);
  dialog->setObjectName("print_invoice_dialog");
  // Address
  QString c_add;
  QString sql = queryCustomerShippingAddress(cid);
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    q.next();
    c_add.append(q.value(0).toString());
  } else {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
    emit s_postMessage(tr("No Customer Address found"));
    return;
  }
  dialog->setCustomerAddress(c_add);
  dialog->setInvoice(oid, cid, in_id, did);

  QList<Invoice::BillingInfo> list;
  q = m_sql->query(queryBillingInfo(oid, cid));
  if (q.size() > 0) {
    QRegExp strip("\\-\\s+\\-");
    while (q.next()) {
      Invoice::BillingInfo d;
      d.articleid = q.value("aid").toString();
      d.designation = q.value("title").toString().replace(strip, "-");
      d.quantity = q.value("quant").toInt();
      d.sellPrice = q.value("sellPrice").toDouble();
      list.append(d);
    }
  } else {
    list.clear();
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
    emit s_postMessage(tr("No Billing Info found"));
    return;
  }

  if (dialog->exec(list)) {
    qDebug() << Q_FUNC_INFO << "TODO";
  }
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
      m_paymentList->setModified(true);
    }
  } else {
    o_closed->setChecked(false);
  }
}

void OrderEditor::generateDeliveryNumber() {
  QString did = o_delivery->value().toString();
  if (did.isEmpty()) {
    o_delivery->setValue(deliveryNumber());
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
  if (o_provider_name->value().toString().isEmpty())
    o_provider_name->setValue(tr("Internal"));

  if (o_provider_order_id->value().toString().isEmpty())
    o_provider_order_id->setValue(tr("Internal"));
}

void OrderEditor::initInvoiceNumber(int orderId) {
  QString sql("SELECT o_invoice_id FROM ");
  sql.append("inventory_orders WHERE o_id=");
  sql.append(QString::number(orderId));
  sql.append(";");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() >= 1) {
    while (q.next()) {
      o_invoice_id->setValue(q.value("o_invoice_id"));
    }
  } else {
    qWarning("SQL ERROR: %s", qPrintable(m_sql->lastError()));
  }
}

const QList<OrderArticle> OrderEditor::getOrderArticles(int oid, int cid) {
  QList<OrderArticle> list;
  QString sql = paymentArticleOrders(oid, cid);
  QSqlQuery q = m_sql->query(sql);
  if (q.size() >= 1) {
    QSqlRecord r = q.record();
    while (q.next()) {
      OrderArticle order;
      for (int f = 0; f < r.count(); f++) {
        QString fn(r.fieldName(f));
        if (fn == "a_payment_id") {
          order.setPayment(q.value(fn).toInt());
        } else if (fn == "a_order_id") {
          order.setOrder(q.value(fn).toInt());
        } else if (fn == "a_article_id") {
          order.setArticle(q.value(fn).toInt());
        } else if (fn == "a_customer_id") {
          order.setCustomer(q.value(fn).toInt());
        } else if (fn == "a_price") {
          order.setPrice(q.value(fn).toDouble());
        } else if (fn == "a_sell_price") {
          order.setSellPrice(q.value(fn).toDouble());
        } else if (fn == "a_count") {
          order.setCount(q.value(fn).toInt());
        } else if (fn == "a_title") {
          order.setTitle(q.value(fn).toString());
        }
      }
      list.append(order);
    }
  } else {
    qWarning("SQL ERROR: %s", qPrintable(m_sql->lastError()));
  }
  return list;
}

bool OrderEditor::getCustomerAddress(int cid) {
  if (cid < 1)
    return false;

  QString select = queryCustomerAddress(cid);
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
    m_customer_address->setValue(buffer);
  } else {
    qWarning("SQL ERROR: %s", qPrintable(m_sql->lastError()));
  }
  return true;
}

const QString OrderEditor::deliveryNumber() {
  int order_id = o_id->value().toInt();
  QString f;
  QDate date = QDate::currentDate();
  f.append(QString::number(date.year()));
  f.append(QString::number(date.dayOfYear()));
  f.append(QString::number(order_id));
  return f;
}

void OrderEditor::addArticleId(int articleId) {
  m_paymentList->insertSearchId(articleId);
}

void OrderEditor::openUpdateOrder(int oid) {
  initDefaults();
  if (oid < 1) {
    qWarning("Empty o_id ...");
    return;
  }
  m_paymentList->setEnabled(true);

  QString select("SELECT * FROM inventory_orders WHERE o_id=");
  select.append(QString::number(oid));
  select.append(";");

  if (SHOW_SQL_QUERIES) {
    qDebug() << Q_FUNC_INFO << select << Qt::endl;
  }

  int cid = -1;
  QSqlQuery q = m_sql->query(select);
  if (q.size() > 0) {
    QSqlRecord r = m_sql->record("inventory_orders");
    sqlQueryResult.clear();
    while (q.next()) {
      foreach (QString key, inputList) {
        QVariant val = q.value(r.indexOf(key));
        bool required = (r.field(key).requiredStatus() == QSqlField::Required);
        // qDebug() << Q_FUNC_INFO << key << val << required;
        if (key == "o_customer_id") {
          cid = val.toInt();
        }
        DataField d;
        d.setField(key);
        d.setType(val.type());
        d.setRequired(required);
        d.setValue(val);
        sqlQueryResult.append(d);
      }
    }
  } else {
    sqlErrnoMessage(m_sql->fetchErrors(), select);
    return;
  }

  if (cid >= 1) {
    const QList<OrderArticle> list = getOrderArticles(oid, cid);
    if (list.size() >= 1)
      m_paymentList->importPayments(list);
  }

  if (!sqlQueryResult.isEmpty())
    importSqlResult();
}

void OrderEditor::openCreateOrder(int cid) {
  initDefaults();
  if (cid > 0) {
    m_paymentList->setEnabled(false);
    o_customer_id->setValue(cid);
    if (getCustomerAddress(cid)) {
      emit isModified(true);
    }
  }
}

void OrderEditor::openCreateOrder(const ProviderOrder &order) {
  initDefaults();
  ProviderOrder copy(order);
  int cid = copy.customerId();
  if (cid > 0) {
    o_customer_id->setValue(cid);
    o_provider_order_id->setValue(copy.providerId());
    o_provider_name->setValue(copy.provider());
    if (getCustomerAddress(cid)) {
      QList<OrderArticle> list;
      foreach (QString said, copy.articleIds()) {
        int aid = said.toInt();
        QString sql = inventoryArticle(aid);
        QSqlQuery q = m_sql->query(sql);
        if (q.size() > 0) {
          // QSqlRecord r = q.record();
          while (q.next()) {
            OrderArticle d;
            d.setPayment(-1);
            d.setArticle(q.value("aid").toInt());
            d.setOrder(-1);
            d.setCustomer(cid);
            int count = q.value("counts").toInt();
            if (count > 0) {
              // Wir fügen immer nur ein Artikel ein!
              count = 1;
            }
            d.setCount(count);
            d.setPrice(q.value("price").toDouble());
            d.setSellPrice(q.value("price").toDouble());
            d.setTitle(q.value("title").toString());
            d.setSummary(tr("Article %1, Price %2, Count: %3, Title: %4")
                             .arg(q.value("aid").toString(),
                                  q.value("price").toString(),
                                  q.value("counts").toString(),
                                  q.value("title").toString()));

            list.append(d);
          }
        } else {
          QString sqlError = m_sql->lastError();
          if (sqlError.isEmpty()) {
            QString info("<p>");
            info.append(tr("One or more items on the list are not available!"));
            info.append("</p><p>");
            info.append(tr("Therefore, they cannot be added"));
            info.append("</p>");
            QMessageBox::warning(this, tr("Order"), info, QMessageBox::Ok);
          } else {
            sqlErrnoMessage(sqlError, sql);
          }
          m_paymentList->setEnabled(false);
          return;
        }
      }
      m_paymentList->importPayments(list);
      m_paymentList->setEnabled(false);
      emit isModified(true);
    }
  }
}
