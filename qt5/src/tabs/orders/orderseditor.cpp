// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderseditor.h"
#include "deliverservice.h"
#include "orderaddarticle.h"
#include "orderscostsettings.h"
#include "orderscustomerinfo.h"
#include "ordersitemlist.h"

#include <QLayout>
#include <QSizePolicy>

OrdersEditor::OrdersEditor(QWidget *parent)
    : InventoryEditor{"^[ocd]_[a-z_]+\\b$", parent} {
  setObjectName("orders_editor");
  setWindowTitle(tr("Edit Orders") + " [*]");
  // setStyleSheet("InputEdit {border:1px solid red;}");

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(0, 0, 0, 0);
  mainLayout->setObjectName("editor_main_layout");

  // BEGIN:Row0
  QHBoxLayout *row0 = new QHBoxLayout();
  row0->setContentsMargins(5, 2, 5, 2);
  // Auftrags Nummer
  o_id = new SerialID(this);
  o_id->setObjectName("o_id");
  o_id->setInfo(tr("Order ID"));
  o_id->setRequired(true);
  row0->addWidget(o_id);
  // Rechnungs Nummer
  o_invoice_id = new SerialID(this);
  o_invoice_id->setObjectName("o_invoice_id");
  o_invoice_id->setInfo(tr("Invoice ID"));
  o_invoice_id->setRequired(true);
  row0->addWidget(o_invoice_id);
  row0->addStretch(1);
  // Auftragsstatus
  o_order_status = new OrderStatusSelecter(this);
  o_order_status->setObjectName("o_order_status");
  o_order_status->setInfo(tr("Order status"));
  o_order_status->setValue(AntiquaCRM::OrderStatus::OPEN);
  row0->addWidget(o_order_status);
  // Zahlungsstatus
  o_payment_status = new OrderPaymentStatusSelecter(this);
  o_payment_status->setObjectName("o_payment_status");
  o_payment_status->setInfo(tr("Payment status"));
  o_payment_status->setValue(AntiquaCRM::OrderPayment::NOTPAID);
  row0->addWidget(o_payment_status);
  mainLayout->addLayout(row0);
  // END:Row0

  // BEGIN:Row1
  QHBoxLayout *row1 = new QHBoxLayout();
  row1->setContentsMargins(5, 2, 5, 2);
  m_customerInfo = new OrdersCustomerInfo(this);
  row1->addWidget(m_customerInfo);
  // TabWidget
  m_costSettings = new OrdersCostSettings(this);
  row1->addWidget(m_costSettings);
  row1->setStretch(1, 1);
  mainLayout->addLayout(row1);
  // END:Row1

  // BEGIN:Row2
  m_ordersList = new OrdersItemList(this);
  mainLayout->addWidget(m_ordersList);
  // END:Row2

  mainLayout->addStretch(1);

  m_actionBar = new EditorActionBar(this);
  PrinterButton::Buttons print_buttons(PrinterButton::Delivery |
                                       PrinterButton::Invoice |
                                       PrinterButton::Reminder);

  m_actionBar->setPrinterMenu(print_buttons);
  m_actionBar->setViewMailButton(true);
  m_actionBar->setViewActionAddButton(true);
  // ResetButton off
  m_actionBar->setRestoreable(false);
  m_actionBar->setViewRestoreButton(false);
  mainLayout->addWidget(m_actionBar);

  setLayout(mainLayout);

  // Signals:ActionsBar
  connect(m_actionBar, SIGNAL(sendRestoreClicked()), SLOT(setRestore()));
  connect(m_actionBar, SIGNAL(sendPrintDeliveryNote()),
          SLOT(createPrintDeliveryNote()));
  connect(m_actionBar, SIGNAL(sendPrintInvoiceNote()),
          SLOT(createPrintInvoiceNote()));
  connect(m_actionBar, SIGNAL(sendPrintPaymentReminder()),
          SLOT(createPrintPaymentReminder()));
  connect(m_actionBar, SIGNAL(sendCancelClicked()),
          SLOT(setFinalLeaveEditor()));
  connect(m_actionBar, SIGNAL(sendAddCustomAction()),
          SLOT(openSearchAddArticle()));
  connect(m_actionBar, SIGNAL(sendSaveClicked()), SLOT(setSaveData()));
  connect(m_actionBar, SIGNAL(sendFinishClicked()),
          SLOT(setCheckLeaveEditor()));
  connect(m_actionBar, SIGNAL(sendCreateMailMessage(const QString &)),
          SLOT(createMailMessage(const QString &)));
}

void OrdersEditor::setInputFields() {
  // Bei UPDATE/INSERT Ignorieren
  ignoreFields << "o_since";
  ignoreFields << "o_modified";
  // @note Darf nur Manuell gesetzt werden!
  ignoreFields << "o_delivered";
  // @deprecated o_provider_order
  ignoreFields << "o_provider_order";
  ignoreFields << "o_vat_included";
  // PayPal TaxId
  ignoreFields << "o_payment_confirmed";

  m_tableData = new AntiquaCRM::ASqlDataQuery("inventory_orders");
  inputFields = m_tableData->columnNames();

  /**
   * Diese Felder sind bei der Abfrage nicht in @ref m_tableData enthalten!
   */
  customInput = QStringList("c_postal_address");
  customInput << "c_shipping_address";

  if (inputFields.isEmpty()) {
    QStringList warn(tr("An error has occurred!"));
    warn << tr("Can't load input datafields!");
    warn << tr("When getting this Message, please check your Network and "
               "Database connection!");
    openNoticeMessage(warn.join("\n"));
  }
  // Preloaders
  m_costSettings->o_delivery_service->loadDataset();
  // Menübar SQL Abfrage starten
  m_actionBar->setMailMenu(MailButton::Orders);
}

bool OrdersEditor::setDataField(const QSqlField &field, const QVariant &value) {
  if (!field.isValid())
    return false;

  QString key = field.name();
  bool required = (field.requiredStatus() == QSqlField::Required);
  InputEdit *inp = findChild<InputEdit *>(key, Qt::FindChildrenRecursively);
  if (inp != nullptr) {
    inp->setProperties(field);
    // qDebug() << key << value << required;
    inp->setValue(value);
    return true;
  }

  if (ignoreFields.contains(key))
    return true;

  if (required) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Unknown:" << key << "|" << value;
#else
    qWarning("Unknown Key (%s) found.", qPrintable(key));
#endif
  }
  return false;
}

void OrdersEditor::generateDeliveryNumber(qint64 orderId) {
  QDate since;
  QString sql("SELECT o_since FROM ");
  sql.append("inventory_orders WHERE o_id=");
  sql.append(QString::number(orderId) + ";");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() == 1) {
    q.next();
    since = q.value("o_since").toDate();
  }
  QString dn;
  dn.append(QString::number(since.year()));
  dn.append(QString::number(since.dayOfYear()));
  dn.append(QString::number(orderId));
  m_tableData->setValue("o_delivery", QString());
  setDataField(m_tableData->getProperties("o_delivery"), dn);
}

bool OrdersEditor::checkDeliveryNumber() {
  QString id = getDataValue("o_delivery").toString();
  if (id.isEmpty() || id.length() < 8) {
    sendStatusMessage(tr("Missing Deliverynote Number"));
    return false;
  }
  return true;
}

void OrdersEditor::setOrderPaymentNumbers(qint64 orderId) {
  QString dn;
  QDate date = QDate::currentDate();
  dn.append(QString::number(date.year()));
  dn.append(QString::number(date.dayOfYear()));
  dn.append(QString::number(orderId));
  // Siehe PostgreSQL::new_invoice_id()
  QString sql("SELECT o_invoice_id FROM ");
  sql.append("inventory_orders WHERE o_id=");
  sql.append(QString::number(orderId) + ";");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() == 1) {
    q.next();
    qint64 id = q.value("o_invoice_id").toInt();
    m_tableData->setValue("o_invoice_id", id);
    setDataField(m_tableData->getProperties("o_invoice_id"), id);
    m_tableData->setValue("o_delivery", dn);
    setDataField(m_tableData->getProperties("o_delivery"), dn);
  } else {
    qWarning("SQL ERROR: %s", qPrintable(m_sql->lastError()));
  }
}

const OrdersEditor::IdsCheck OrdersEditor::getCheckEssentialsIds() {
  IdsCheck id_conf;
  qint64 oid = getSerialID("o_id");
  qint64 cid = getSerialID("o_customer_id");
  qint64 iid = getSerialID("o_invoice_id");
  if (oid > 1 && cid > 1 && iid > 1) {
    id_conf.isNotValid = false;
    id_conf.or_id = oid;
    id_conf.cu_id = cid;
    id_conf.in_id = iid;
    return id_conf;
  }

  QString info("<p>");
  info.append(tr("A Article can't inserted, if no Order-/Customer Id exists."));
  info.append("</p><p>");
  info.append(tr("Please save your your Order first."));
  info.append("</p>");
  openNoticeMessage(info);

  id_conf.isNotValid = true;
  return id_conf;
}

AntiquaCRM::SalesTax OrdersEditor::initSalesTax() {
  QString country = getDataValue("o_vat_country").toString();
  if (country.isEmpty() || country == "XX") {
    qInfo("No Eurpean country - set invoice tax to no!");
    getInputEdit("o_vat_levels")->setValue(AntiquaCRM::SalesTax::TAX_NOT);
    return AntiquaCRM::SalesTax::TAX_NOT;
  }
  int i = getDataValue("o_vat_levels").toInt();
  return static_cast<AntiquaCRM::SalesTax>(i);
}

int OrdersEditor::getSalesTaxValue(int index) {
  switch (static_cast<AntiquaCRM::ArticleType>(index)) {
  case (AntiquaCRM::ArticleType::BOOK):
    return m_cfg->value("payment/vat2").toInt();

  default:
    return m_cfg->value("payment/vat1").toInt();
  };
}

int OrdersEditor::getSalesTaxType(int index) {
  switch (static_cast<AntiquaCRM::ArticleType>(index)) {
  case (AntiquaCRM::ArticleType::BOOK):
    return 1; // VAT reduced

  default:
    return 0; // VAT normal
  };
}

const QPair<int, int> OrdersEditor::getDeliveryService(bool current) {
  DeliverService *m_ds = m_costSettings->o_delivery_service;
  if (current)
    m_ds->currentDeliveryService();

  return m_ds->defaultDeliveryService();
}

void OrdersEditor::setDeliveryService() {
  QPair<int, int> service(0, 0);
  service.first = m_tableData->getValue("o_delivery_service").toInt();
  service.second = m_tableData->getValue("o_delivery_package").toInt();
  m_costSettings->o_delivery_service->setDeliveryService(service);
}

void OrdersEditor::importSqlResult() {
  if (m_tableData == nullptr)
    return;

  QHashIterator<QString, QVariant> it(m_tableData->getDataset());
  blockSignals(true);
  while (it.hasNext()) {
    it.next();
    QSqlField field = m_tableData->getProperties(it.key());
    setDataField(field, it.value());
  }
  blockSignals(false);

  setResetModified(inputFields);
}

bool OrdersEditor::sendSqlQuery(const QString &query) {
  if (query.isEmpty())
    return false;

  QSqlQuery q = m_sql->query(query);
  if (q.lastError().type() != QSqlError::NoError) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << query << m_sql->lastError();
#else
    qWarning("OrdersEditor: SQL Error!");
#endif
    return false;
  }

  if (q.next()) {
    if (!q.isNull("o_id")) {
      qint64 oid = q.value("o_id").toInt();
      if (oid < 1) {
        qWarning("SQL-Insert: get empty OrderID!");
        return false;
      }
      m_tableData->setValue("o_id", oid);
      setDataField(m_tableData->getProperties("o_id"), oid);
      setResetModified(inputFields);
      setOrderPaymentNumbers(oid);
    }
  }

  return true;
}

const QHash<QString, QVariant> OrdersEditor::createSqlDataset() {
  QHash<QString, QVariant> data;
  QList<InputEdit *> list =
      findChildren<InputEdit *>(fieldPattern, Qt::FindChildrenRecursively);
  QList<InputEdit *>::Iterator it;
  for (it = list.begin(); it != list.end(); ++it) {
    InputEdit *cur = *it;
    QString objName = cur->objectName();
    if (ignoreFields.contains(objName) || customInput.contains(objName))
      continue;

    if (cur->isRequired() && !cur->isValid()) {
      openNoticeMessage(cur->notes());
      cur->setFocus();
      data.clear();
      return data;
    }
    data.insert(objName, cur->value());
  }
  list.clear();

  return data;
}

void OrdersEditor::createSqlUpdate() {
  qint64 oid = getSerialID("o_id");
  if (oid < 1)
    return;

  /*
   * Ein UPDATE ohne Artikel wird feige verweigert!
   */
  if (m_ordersList->isEmpty()) {
    openNoticeMessage(tr("No Article has been added to this order!"));
    m_actionBar->setFocusButton("article");
    return;
  }

  // Nach einem INSERT wieder aktivieren!
  QStringList requiredFields({"o_id", "o_invoice_id", "o_delivery"});
  foreach (QString objName, requiredFields) {
    getInputEdit(objName)->setRequired(true);
  }

  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 1)
    return;

  int changes = 0;
  QStringList set;
  QHashIterator<QString, QVariant> it(data);
  while (it.hasNext()) {
    it.next();
    if (it.key() == "o_id")
      continue;

    // Nur geänderte Felder in das Update aufnehmen!
    if (it.value() == m_tableData->getValue(it.key()))
      continue;

    if (it.value().type() == QVariant::String) {
      set.append(it.key() + "='" + it.value().toString() + "'");
    } else {
      set.append(it.key() + "=" + it.value().toString());
    }
    changes++;
  }

  /*
   * Wenn der Status der Datenbank nicht auf abschließen steht
   * und der Anwender den Auftrag abschließt.
   * Muss das Datum für geliefert hier gesetzt werden!
   */
  if (!databaseOrderStatus() && currentOrderStatus()) {
    set.append("o_delivered=CURRENT_TIMESTAMP");
    changes++;
  }

  // Artikel Bestelliste aktualisieren
  QString articles_sql = getSqlArticleOrders();
  if (articles_sql.isEmpty()) {
    sendStatusMessage(tr("No SQL Articles exist!"));
    return;
  }

  QString sql;
  // Update SQL-Update erstellen
  if (changes > 0) {
    sql.append("UPDATE inventory_orders SET ");
    sql.append(set.join(","));
    sql.append(",o_modified=CURRENT_TIMESTAMP WHERE o_id=");
    sql.append(QString::number(oid));
    sql.append(";");
  }

  // Füge Artikel zum SQL-Update hinzu
  if (articles_sql.size() > 1) {
    if (!sql.isEmpty())
      sql.append("\n");

    sql.append(articles_sql);
  }

  if (changes == 0 && sql.isEmpty()) {
    sendStatusMessage(tr("No Modifications found, nothing todo!"));
    setWindowModified(false);
    m_ordersList->setArticleChanged(false);
    return;
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << sql << Qt::endl;
#endif

  if (sendSqlQuery(sql)) {
    setResetModified(inputFields);
    m_ordersList->setArticleChanged(false);
    openSuccessMessage(tr("Order saved successfully!"));
  }
}

void OrdersEditor::createSqlInsert() {
  // Werden vom INSERT erstellt!
  QStringList insertIgnore({"o_id", "o_invoice_id", "o_delivery"});
  foreach (QString objName, insertIgnore) {
    getInputEdit(objName)->setRequired(false);
  }

  QHash<QString, QVariant> data = createSqlDataset();
  if (data.size() < 1)
    return;

  QStringList fields;
  QStringList values;
  QHashIterator<QString, QVariant> it(data);
  while (it.hasNext()) {
    it.next();
    QString value = it.value().toString();
    if (insertIgnore.contains(it.key()) || value.isEmpty())
      continue;

    if (it.value().type() == QVariant::String) {
      values << "'" + value.trimmed() + "'";
    } else {
      values << value;
    }
    fields << it.key();
  }

  QString sql("INSERT INTO inventory_orders (");
  sql.append(fields.join(","));
  sql.append(") VALUES (");
  sql.append(values.join(","));
  sql.append(") RETURNING o_id;");
  if (sendSqlQuery(sql)) {
    qint64 oid = getSerialID("o_id");
    if (oid < 1) {
      qWarning("FATAL: Missing o_id after INSERT!");
      openNoticeMessage("FATAL: Missing o_id after INSERT!");
      return;
    }
    // Artikel Id Setzen
    if (m_ordersList->setArticleOrderId(oid)) {
      // Artikel Bestelliste speichern
      QString articles_sql = getSqlArticleOrders();
      if (articles_sql.isEmpty()) {
        sendStatusMessage(tr("No SQL Articles exist!"));
        return;
      }
      if (sendSqlQuery(articles_sql)) {
        // Bestehende Artikelliste neu einlesen!
        m_ordersList->importArticles(queryOrderArticles(oid));
        sendStatusMessage(tr("Save Articles success!"));
      }
    }
  }
}

qint64 OrdersEditor::searchCustomer(const QJsonObject &obj, qint64 customerId) {
  QStringList f("c_firstname");
  f << "c_lastname";
  f << "c_postalcode";
  f << "c_email_0";

  QStringList clause;
  foreach (QString k, f) {
    QString v = obj.value(k).toString();
    if (!v.isEmpty())
      clause << k + "='" + v + "'";
  }

  // Search customer
  QString sql("SELECT c_id FROM customers WHERE (");
  if (customerId > 1) {
    sql.append("c_id=" + QString::number(customerId));
  } else {
    sql.append(clause.join(" AND "));
    sql.append(") OR (c_provider_import='");
    sql.append(obj.value("c_provider_import").toString());
    sql.append("'");
  }
  sql.append(") ORDER BY c_id;");
  // qDebug() << Q_FUNC_INFO << sql;
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    QList<qint64> cIds;
    while (q.next()) {
      cIds << q.value("c_id").toInt();
    }
    if (cIds.size() > 1)
      qWarning("Warning: Found more then one Customer!");

    return cIds.first();
  } else {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << sql << m_sql->lastError();
#endif
    return -1;
  }
  return -1;
}

const QList<AntiquaCRM::OrderArticleItems>
OrdersEditor::queryOrderArticles(qint64 orderId) {
  QList<AntiquaCRM::OrderArticleItems> articles;
  QString sql("SELECT * FROM article_orders WHERE a_order_id=");
  sql.append(QString::number(orderId) + ";");
  QSqlQuery q = m_sql->query(sql);
  if (q.size() > 0) {
    QSqlRecord r = q.record();
    while (q.next()) {
      AntiquaCRM::OrderArticleItems article;
      for (int c = 0; c < r.count(); c++) {
        AntiquaCRM::ArticleOrderItem item;
        item.key = r.field(c).name();
        item.value = q.value(item.key);
        article.append(item);
      }
      articles.append(article);
    }
  }
  return articles;
}

bool OrdersEditor::addArticleToOrderTable(qint64 articleId) {
  if (articleId < 1)
    return false;

  AntiquaCRM::ASqlFiles sqlFile("query_article_order_with_id");
  if (sqlFile.openTemplate()) {
    sqlFile.setWhereClause("i_id=" + QString::number(articleId));
    QSqlQuery q = m_sql->query(sqlFile.getQueryContent());
    if (q.size() > 0) {
      q.next();
      QSqlRecord r = q.record();
      AntiquaCRM::OrderArticleItems items;
      items.append(addArticleItem("a_order_id", getSerialID("o_id")));
      items.append(addArticleItem("a_customer_id", getSerialID("o_customer_id")));
      for (int i = 0; i < r.count(); i++) {
        QSqlField f = r.field(i);
        items.append(addArticleItem(f.name(), f.value()));
        // NOTE: a_sell_price is not in SQL Query
        if (f.name() == "a_price")
          items.append(addArticleItem("a_sell_price", f.value()));
      }
      if (items.size() > 0) {
        m_ordersList->insertArticle(items);
        return true;
      }
    }
#ifdef ANTIQUA_DEVELOPEMENT
    else {
      qDebug() << Q_FUNC_INFO << m_sql->lastError();
    }
#endif
  }
  sendStatusMessage(tr("Article: %1 not found or no stock!").arg(articleId));
  return false;
}

const QString OrdersEditor::getSqlArticleOrders() {
  IdsCheck test = getCheckEssentialsIds();
  if (test.isNotValid)
    return QString();

  QStringList queries = m_ordersList->getQueryData();
  if (queries.size() > 0) {
    // foreach (QString q, queries)
    // { qDebug() << "OrdersEditor:" << q << Qt::endl; }
    return queries.join("\n");
  }
  return QString();
}

const QList<BillingInfo> OrdersEditor::queryBillingInfo(qint64 oid,
                                                        qint64 cid) {
  QString o_id = QString::number(oid);
  QString c_id = QString::number(cid);
  QList<BillingInfo> list;
  AntiquaCRM::ASqlFiles sqlFile("query_printing_billing_info");
  if (sqlFile.openTemplate()) {
    QString wcl("a_order_id=" + o_id + " AND a_customer_id=" + c_id);
    sqlFile.setWhereClause(wcl);
    QSqlQuery q = m_sql->query(sqlFile.getQueryContent());
    if (q.size() > 0) {
      // Umsatzsteuer ermitteln.
      bool _vat_disabled =
          (getDataValue("o_vat_levels").toInt() == 0) ? true : false;
      AntiquaCRM::SalesTax _taxset = initSalesTax();
      QRegExp strip("\\-\\s+\\-");
      while (q.next()) {
        BillingInfo d;
        d.articleid = q.value("aid").toString();
        d.designation = q.value("title").toString().replace(strip, "-");
        d.quantity = q.value("quant").toInt();
        d.sellPrice = q.value("sellprice").toDouble();
        d.vatSet = _taxset;
        d.vatValue = getSalesTaxValue(q.value("a_type").toInt());
        d.vatDisabled = _vat_disabled;
        d.packagePrice = q.value("packageprice").toDouble();
        list.append(d);
      }
    }
#ifdef ANTIQUA_DEVELOPEMENT
    else {
      qDebug() << Q_FUNC_INFO << wcl << m_sql->lastError();
    }
#endif
  }
  return list;
}

AntiquaCRM::ArticleOrderItem
OrdersEditor::addArticleItem(const QString &key, const QVariant &value) const {
  return AntiquaCRM::AProviderOrder::createItem(key, value);
}

void OrdersEditor::setDefaultValues() {
  // Status
  m_tableData->setValue("o_order_status", AntiquaCRM::OrderStatus::STARTED);
  setDataField(m_tableData->getProperties("o_order_status"),
               m_tableData->getValue("o_order_status"));

  // Einstelleunegn für Umsatzsteuer
  m_tableData->setValue("o_vat_levels", AntiquaCRM::SalesTax::TAX_INCL);
  setDataField(m_tableData->getProperties("o_vat_levels"),
               m_tableData->getValue("o_vat_levels"));
  m_tableData->setValue("o_vat_country", QString("XX"));
  setDataField(m_tableData->getProperties("o_vat_country"),
               m_tableData->getValue("o_vat_country"));

  // Lieferdienst
  QPair<int, int> ds_t = getDeliveryService(true);
  m_tableData->setValue("o_delivery_service", ds_t.first);
  m_tableData->setValue("o_delivery_package", ds_t.second);
  m_tableData->setValue("o_delivery_send_id", "");
  m_tableData->setValue("o_delivery_add_price", false);
  setDataField(m_tableData->getProperties("o_delivery_add_price"),
               m_tableData->getValue("o_delivery_add_price"));
}

bool OrdersEditor::beforeCreate() {
  setInputFields();
  setResetModified(inputFields);
  setEnabled(true);
  return true;
}

bool OrdersEditor::databaseOrderStatus() {
  // AntiquaCRM::OrderStatus
  int i_os = m_tableData->getValue("o_order_status").toInt();
  AntiquaCRM::OrderStatus os_t = static_cast<AntiquaCRM::OrderStatus>(i_os);
  // AntiquaCRM::OrderPayment
  int i_ps = m_tableData->getValue("o_payment_status").toInt();
  AntiquaCRM::OrderPayment ps_t = static_cast<AntiquaCRM::OrderPayment>(i_ps);
  return (os_t == AntiquaCRM::OrderStatus::DELIVERED &&
          ps_t == AntiquaCRM::OrderPayment::PAYED);
}

bool OrdersEditor::currentOrderStatus() {
  return (paymentStatus() == AntiquaCRM::OrderPayment::PAYED &&
          orderStatus() == AntiquaCRM::OrderStatus::DELIVERED);
}

void OrdersEditor::setProtected(bool b) {
  o_payment_status->setEnabled(!b);
  o_order_status->setEnabled(!b);
}

void OrdersEditor::setSaveData() {
  if (getSerialID("o_id") < 1) {
    createSqlInsert();
    return;
  }
  createSqlUpdate();
}

void OrdersEditor::setCheckLeaveEditor() {
  if (checkIsModified() || m_ordersList->getArticleChanged()) {
    unsavedChangesPopup();
    return;
  }
  setFinalLeaveEditor(false);
}

void OrdersEditor::setFinalLeaveEditor(bool force) {
  if (force) // Wenn auf Abbrechen geklickt wurde!
    setWindowModified(false);

  setResetInputFields();
  m_ordersList->clearTable(); /**< Artikel Tabelle leeren! */
  m_ordersList->setArticleChanged(false);
  emit sendLeaveEditor(); /**< Zurück zur Hauptsansicht */
}

void OrdersEditor::createMailMessage(const QString &type) {
  IdsCheck ids = getCheckEssentialsIds();
  if (ids.isNotValid) {
    sendStatusMessage(tr("Missing essential Ids, save Order first!"));
    return;
  }

  if (!m_cfg->check(ANTIQUACRM_CONFIG_MAILLER_KEY)) {
    sendStatusMessage(tr("Incomplete eMail configuration!"));
  }

  MailForwardDialog *m_d = new MailForwardDialog(this);
  m_d->setOrderId(ids.or_id);
  if (m_d->exec(ids.cu_id, type) == QDialog::Accepted) {
    sendStatusMessage(tr("Send eMail Dialog finished!"));
  }
  m_d->deleteLater();
}

void OrdersEditor::createPrintDeliveryNote() {
  IdsCheck ids = getCheckEssentialsIds();
  if (ids.isNotValid) {
    sendStatusMessage(tr("Missing essential Ids, save Order first!"));
    return;
  }

  if (!checkDeliveryNumber())
    return;

  QString did = getDataValue("o_delivery").toString();
  QString c_add = getDataValue("c_shipping_address").toString();
  if (c_add.isEmpty()) {
    c_add = getDataValue("c_postal_address").toString();
  }

  if (c_add.isEmpty()) {
    sendStatusMessage(tr("Missing Delivery Address!"));
    return;
  }

  QList<BillingInfo> list = queryBillingInfo(ids.or_id, ids.cu_id);
  if (list.size() < 1) {
    sendStatusMessage(tr("No Data found! - Printing canceled."));
    return;
  }

  DeliveryNote *m_d = new DeliveryNote(this);
  m_d->setDelivery(ids.or_id, ids.cu_id, did);
  m_d->setCustomerAddress(c_add);

  if (m_d->exec(list) == QDialog::Accepted) {
    sendStatusMessage(tr("Printdialog closed."));
  }

  list.clear();
  m_d->deleteLater();
}

void OrdersEditor::createPrintInvoiceNote() {
  IdsCheck ids = getCheckEssentialsIds();
  if (ids.isNotValid) {
    sendStatusMessage(tr("Missing essential Ids, save Order first!"));
    return;
  }

  if (!checkDeliveryNumber())
    return;

  QString did = getDataValue("o_delivery").toString();
  QList<BillingInfo> list = queryBillingInfo(ids.or_id, ids.cu_id);
  if (list.size() < 1) {
    sendStatusMessage(tr("No Data found! - Printing canceled."));
    return;
  }

  qreal pkgPrice = 0.00;
  if (getDataValue("o_delivery_add_price").toBool()) {
    pkgPrice = m_costSettings->o_delivery_service->getPackagePrice();
  }

  Invoice *m_d = new Invoice(this);
  m_d->setInvoice(ids.or_id, ids.cu_id, ids.in_id, pkgPrice, did);
  QString c_add = getDataValue("c_postal_address").toString();
  m_d->setCustomerAddress(c_add);

  AntiquaCRM::OrderPayment paid = static_cast<AntiquaCRM::OrderPayment>(
      getDataValue("o_payment_status").toInt());
  if (m_d->exec(list, (paid == AntiquaCRM::OrderPayment::PAYED)) ==
      QDialog::Rejected) {
    sendStatusMessage(tr("Print document refused."));
  }
  list.clear();
  m_d->deleteLater();
}

void OrdersEditor::createPrintPaymentReminder() {
  IdsCheck ids = getCheckEssentialsIds();
  if (ids.isNotValid) {
    sendStatusMessage(tr("Missing essential Ids, save Order first!"));
    return;
  }

  if (!checkDeliveryNumber())
    return;

  QString did = getDataValue("o_delivery").toString();
  QString c_add = getDataValue("c_postal_address").toString();
  if (did.isEmpty() || c_add.isEmpty()) {
    sendStatusMessage(tr("Missing Deliverynote data"));
    return;
  }

  QList<BillingInfo> list = queryBillingInfo(ids.or_id, ids.cu_id);
  if (list.size() < 1) {
    sendStatusMessage(tr("No Data - Printing canceled."));
    return;
  }

  AntiquaTemplates body(this);
  QString sql("o_id=" + QString::number(ids.or_id));
  QString message_body = body.getTemplate("PDF_PAYMENT_REMINDER", sql);

  qreal pkgPrice = 0.00;
  if (getDataValue("o_delivery_add_price").toBool()) {
    pkgPrice = m_costSettings->o_delivery_service->getPackagePrice();
  }

  PaymentReminder *m_d = new PaymentReminder(this);
  m_d->setCustomerAddress(c_add);
  m_d->setPaymentInfo(ids.or_id, ids.cu_id, ids.in_id, pkgPrice, did);
  m_d->setMainText(message_body);
  if (m_d->exec(list) != QDialog::Accepted) {
    sendStatusMessage(tr("Print document refused."));
  }
  list.clear();
  m_d->deleteLater();
}

void OrdersEditor::openSearchAddArticle() {
  IdsCheck test = getCheckEssentialsIds();
  if (test.isNotValid) {
    sendStatusMessage(tr("Missing essential Ids, save Order first!"));
    return;
  }

  OrderAddArticle *d = new OrderAddArticle(this);
  if (d->exec() == QDialog::Rejected)
    return;

  qint64 aid = d->getArticle();
  if (aid < 1)
    return;

  addArticleToOrderTable(aid);
}

void OrdersEditor::setRestore() {
  importSqlResult();
  setEnabled(true);
}

bool OrdersEditor::openEditEntry(qint64 orderId) {
  bool status = false;
  if (orderId < 1)
    return status;

  QString o_id = QString::number(orderId);
  if (o_id.isEmpty())
    return status;

  setInputFields();
  setResetModified(inputFields);
  o_order_status->setValue(AntiquaCRM::OrderStatus::STARTED);

  AntiquaCRM::ASqlFiles file("query_order_by_oid");
  if (!file.openTemplate())
    return false;

  file.setWhereClause("o_id=" + o_id);

  QSqlQuery q = m_sql->query(file.getQueryContent());
  if (q.size() != 0) {
    QSqlRecord r = q.record();
    q.next();
    // Standard Felder
    foreach (QString key, inputFields) {
      m_tableData->setValue(key, q.value(r.indexOf(key)));
    }
    // Kunden Daten
    foreach (QString key, customInput) {
      QSqlField f = r.field(key);
      setDataField(f, q.value(f.name()));
    }

    // Bestehende Artikel Einkäufe mit orderId einlesen!
    m_ordersList->importArticles(queryOrderArticles(orderId));

    // Muss nachträglich gesetzt werden!
    setDeliveryService();

    setResetModified(customInput);
    status = true;
  } else {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
#endif
    status = false;
  }

  if (status) {
    importSqlResult();
    setResetModified(inputFields);
    setEnabled(true);
  }

  setProtected(databaseOrderStatus());
  return status;
}

bool OrdersEditor::addArticle(qint64 articleId) {
  if (getCheckEssentialsIds().isNotValid) {
    sendStatusMessage(tr("Missing essential Ids, save Order first!"));
    return false;
  }
  if (addArticleToOrderTable(articleId)) {
    // FIXME
    getInputEdit("o_delivery")->setModified(true);
    return true;
  }
  return false;
}

bool OrdersEditor::createNewOrder(qint64 customerId) {
  if (customerId < 1)
    return false;

  beforeCreate();
  setDefaultValues();

  // Nehme relevante Kundendaten
  AntiquaCRM::ASqlFiles customerQuery("query_customer_new_order");
  if (customerQuery.openTemplate()) {
    customerQuery.setWhereClause("c_id=" + QString::number(customerId));
    QSqlQuery q = m_sql->query(customerQuery.getQueryContent());
    if (q.size() > 0) {
      QSqlRecord rec = q.record();
      while (q.next()) {
        for (int c = 0; c < rec.count(); c++) {
          QSqlField f = rec.field(c);
          m_tableData->setValue(f.name(), q.value(f.name()));
          setDataField(f, q.value(f.name()));
        }
      }
      // Ignore this
      setResetModified(customInput);
      return true;
    } else {
#ifdef ANTIQUA_DEVELOPEMENT
      qDebug() << Q_FUNC_INFO << m_sql->lastError();
#endif
    }
  }
  return false;
}

bool OrdersEditor::createNewProviderOrder(const QJsonObject &prObject) {
  QString providerId = prObject.value("create_order").toString();
  if (providerId.isEmpty())
    return false;

  QString article_numbers = prObject.value("article_numbers").toString();
  QStringList articleNumberList = article_numbers.split(",");
  if (articleNumberList.size() < 1) {
    qWarning("NO Article Numbers exists!");
    return false;
  }

  AntiquaCRM::ASqlFiles sqlFile("query_provider_order_exists");
  if (!sqlFile.openTemplate())
    return false;

  sqlFile.setWhereClause("pr_order='" + providerId + "'");

  QSqlQuery q = m_sql->query(sqlFile.getQueryContent());
  if (q.size() != 1) {
    openNoticeMessage(tr("No Provider orders data found!"));
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
#endif
    return false;
  }

  q.next();
  QString o_provider_name = q.value("pr_name").toString();
  QString o_provider_order_id = q.value("pr_order").toString();
  qint64 customerId = q.value("pr_customer_id").toInt();
  QByteArray data = q.value("pr_order_data").toByteArray();
  QJsonObject obj = QJsonDocument::fromJson(data).object();
  if (obj.isEmpty())
    return false;

  setInputFields();

  AntiquaCRM::AProviderOrder prOrder(o_provider_name, o_provider_order_id);
  // Kunden Daten
  if (obj.contains("customer")) {
    QJsonObject customer = obj.value("customer").toObject();
    // NOTE: Wir benötigen bei einem Import eine gültige Kundennummer!
    customerId = searchCustomer(customer, customerId);
    if (customerId < 1) {
      openNoticeMessage(tr("Customer not found or set!"));
      return false;
    }
    prOrder.setValue("o_customer_id", customerId);
    // NOTE: Wir nehmen die korrigierten Kundendaten!
    AntiquaCRM::ASqlFiles customerQuery("query_order_import_customer");
    if (customerQuery.openTemplate()) {
      customerQuery.setWhereClause("c_id=" + QString::number(customerId));
      QSqlQuery q = m_sql->query(customerQuery.getQueryContent());
      if (q.size() > 0) {
        QSqlRecord rec = q.record();
        while (q.next()) {
          for (int c = 0; c < rec.count(); c++) {
            QSqlField f = rec.field(c);
            // qDebug() << f.name() << q.value(f.name());
            prOrder.setValue(f.name(), q.value(f.name()));
          }
        }
      } else {
#ifdef ANTIQUA_DEVELOPEMENT
        qDebug() << Q_FUNC_INFO << m_sql->lastError();
#endif
        qWarning("Using Provider Import!");
        foreach (QString key, customer.keys()) {
          QVariant val = customer.value(key).toVariant();
          if (!prOrder.setValue(key, val))
            qWarning("Customer value '%s' not set!", qPrintable(key));
        }
      }
    } else {
      openNoticeMessage("FATAL: Can't open Template file!");
      return false;
    }
  }

  // Standard Felder
  if (obj.contains("orderinfo")) {
    QJsonObject orderinfo = obj.value("orderinfo").toObject();
    if (orderinfo.contains("o_payment_confirmed")) {
      bool paypal_status = (!orderinfo.value("o_payment_confirmed").isNull());
      if (paypal_status)
        o_payment_status->setValue(AntiquaCRM::OrderPayment::PAYED);
    }

    foreach (QString key, orderinfo.keys()) {
      QVariant val = orderinfo.value(key).toVariant();
      if (ignoreFields.contains(key))
        continue;

      if (!prOrder.setValue(key, val)) {
        qWarning("Order value '%s' not set!", qPrintable(key));
      }
    }
  }

  // Artikel
  if (obj.contains("articles")) {
    QJsonArray orders = obj.value("articles").toArray();
    for (int i = 0; i < orders.size(); i++) {
      QList<AntiquaCRM::ArticleOrderItem> items;
      QJsonObject article = orders[i].toObject();
      // NOTE: Das Feld 'a_tax' ist noch nicht in „antiquacmd“ eingebunden!
      // o_media_type | a_type
      if (!article.contains("a_tax") && article.contains("a_type")) {
        article.insert("a_tax",
                       getSalesTaxType(article.value("a_type").toInt()));
      }

      qint64 aId = article.value("a_article_id").toInt();
      // Siehe QJsonValue::article_numbers
      if (!articleNumberList.contains(QString::number(aId)))
        continue;

      // Ist erst mal 0
      AntiquaCRM::ArticleOrderItem item;
      item.key = QString("a_order_id");
      item.value = 0;
      items.append(item);
      // Muss hier eingefügt werden!
      if (customerId > 0) {
        AntiquaCRM::ArticleOrderItem item;
        item.key = QString("a_customer_id");
        item.value = customerId;
        items.append(item);
      }
      // Auslesen
      foreach (QString key, article.keys()) {
        AntiquaCRM::ArticleOrderItem item;
        item.key = key;
        item.value = article.value(key).toVariant();
        items.append(item);
      }
      if (items.size() > 0)
        prOrder.insertOrderItems(items);
    }
  }

  // Fehlende felder in JsonObject setzen!
  // Setze auf Auftragsbegin
  prOrder.setValue("o_order_status", AntiquaCRM::OrderStatus::STARTED);

  // Begin:Umsatzsteuer
  prOrder.setValue("o_vat_levels", AntiquaCRM::SalesTax::TAX_INCL);
  // End:Umsatzsteuer

  // Begin:Lieferdienst
  QPair<int, int> pkgService = getDeliveryService(false);
  prOrder.setValue("o_delivery_service", pkgService.first);
  prOrder.setValue("o_delivery_package", pkgService.second);
  prOrder.setValue("o_delivery_send_id", "");
  prOrder.setValue("o_delivery_add_price", false);
  // Lieferschein Nr. ist hier noch leer!
  prOrder.setValue("o_delivery", QString());
  // End:Lieferdienst

  // Diese Felder bei neuen Einträgen ignorieren!
  QStringList ignored({"o_id", "o_invoice_id"});

  // 1) Standard Felder einfügen
  foreach (QString key, inputFields) {
    // @note Datenfelder ausschließen
    if (ignoreFields.contains(key))
      continue;

    if (ignored.contains(key)) {
      m_tableData->setValue(key, 0);
    } else {
      m_tableData->setValue(key, prOrder.getValue(key));
    }
  }

  // 2) Kunden Daten
  QSqlRecord r = AntiquaCRM::ASqlDataQuery("customers").record();
  foreach (QString key, customInput) {
    QSqlField f = r.field(key);
    setDataField(f, prOrder.getValue(key));
  }

  // 3) Artikel Importieren
  m_ordersList->importArticles(prOrder.orders());

  // 4) Datensätze Importieren
  importSqlResult();

  // 5) Erst nach einfügen reset
  setResetModified(inputFields);
  setResetModified(customInput);
  m_ordersList->setArticleChanged(false);

  return true;
}

AntiquaCRM::OrderPayment OrdersEditor::paymentStatus() {
  int id = o_payment_status->value().toInt();
  return static_cast<AntiquaCRM::OrderPayment>(id);
}

AntiquaCRM::OrderStatus OrdersEditor::orderStatus() {
  int id = o_order_status->value().toInt();
  return static_cast<AntiquaCRM::OrderStatus>(id);
}
