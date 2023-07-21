// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderseditor.h"
#include "ordersconfig.h"
#include "orderscostsettings.h"
#include "orderscustomerinfo.h"
#include "orderstableview.h"

#include <QLayout>
#include <QSizePolicy>

OrdersEditor::OrdersEditor(QWidget *parent)
    : AntiquaCRM::TabsEditor{ORDERS_SQL_EDITOR_PATTERN, parent} {
  setObjectName("orders_editor");
  setWindowTitle(tr("Edit Order") + " [*]");

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(0, 0, 0, 0);

  // BEGIN:Row0
  QHBoxLayout *row0 = new QHBoxLayout();
  row0->setContentsMargins(5, 2, 5, 2);
  // Auftrags Nummer
  o_id = new AntiquaCRM::SerialId(this);
  o_id->setObjectName("o_id");
  o_id->setBuddyLabel(tr("Order ID"));
  o_id->setRequired(true);
  row0->addWidget(o_id);
  // Rechnungs Nummer
  o_invoice_id = new AntiquaCRM::SerialId(this);
  o_invoice_id->setObjectName("o_invoice_id");
  o_invoice_id->setBuddyLabel(tr("Invoice ID"));
  o_invoice_id->setRequired(true);
  row0->addWidget(o_invoice_id);
  row0->addStretch(1);
  // Auftragsstatus
  o_order_status = new AntiquaCRM::SelectOrderStatus(this);
  o_order_status->setObjectName("o_order_status");
  o_order_status->setBuddyLabel(tr("Order status"));
  o_order_status->setValue(AntiquaCRM::OrderStatus::OPEN);
  row0->addWidget(o_order_status);
  // Zahlungsstatus
  o_payment_status = new AntiquaCRM::SelectOrderPayment(this);
  o_payment_status->setObjectName("o_payment_status");
  o_payment_status->setBuddyLabel(tr("Payment status"));
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
  m_ordersTable = new OrdersTableView(this, false);
/**
 * Für den Kunden ausblenden. Werden hier nicht benötigt!
 * @warning Die Spaltenzahl ist zu diesem Zeitpunkt noch nicht bekannt!
 *          Deshalb wird die Konstante „table_columns“ heran gezogen.
 */
#ifndef ANTIQUA_DEVELOPEMENT
  QStringList hideColumn("a_payment_id");
  hideColumn << "a_order_id";
  hideColumn << "a_customer_id";
  hideColumn << "a_modified";
  hideColumn << "a_provider_id";
  hideColumn << "a_refunds_cost";
  m_table->hideColumns(hideColumn);
#endif
  mainLayout->addWidget(m_ordersTable);
  mainLayout->setStretch(2, 0);
  // END:Row2

  /*
   * BEGIN:Row3
   * Wegen SQL-Abfrage „setMailMenu“ hier nicht setzen!
   * Siehe AntiquaCRM::MailButton::setSections ...
   */
  m_actionBar = new AntiquaCRM::TabsEditActionBar(this);
  m_actionBar->setViewMailButton(true);
  m_actionBar->setRestoreable(false); // ResetButton off
  m_actionBar->setViewRestoreButton(false);
  m_actionBar->setViewActionAddButton(true);
  m_actionBar->setPrinterMenu(AntiquaCRM::PRINT_DELIVERY |
                              AntiquaCRM::PRINT_INVOICE |
                              AntiquaCRM::PRINT_REMINDER);
  mainLayout->addWidget(m_actionBar);
  // END:Row3

  setLayout(mainLayout);

  // Signals:ActionsBar
  connect(m_actionBar, SIGNAL(sendRestoreClicked()), SLOT(setRestore()));
  connect(m_actionBar, SIGNAL(sendPrintDeliveryNote()),
          SLOT(createPrintDeliveryNote()));
  connect(m_actionBar, SIGNAL(sendPrintInvoiceNote()),
          SLOT(createPrintInvoiceNote()));
  connect(m_actionBar, SIGNAL(sendPrintPaymentReminder()),
          SLOT(createPrintPaymentReminder()));
  connect(m_actionBar, SIGNAL(sendCreateMailMessage(const QString &)),
          SLOT(createMailMessage(const QString &)));
  connect(m_actionBar, SIGNAL(sendAddCustomAction()),
          SLOT(openSearchInsertArticle()));
  connect(m_actionBar, SIGNAL(sendCancelClicked()),
          SLOT(setFinalLeaveEditor()));
  connect(m_actionBar, SIGNAL(sendSaveClicked()), SLOT(setSaveData()));
  connect(m_actionBar, SIGNAL(sendFinishClicked()),
          SLOT(setCheckLeaveEditor()));
}

OrdersEditor::~OrdersEditor() {}

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

  m_tableData = new AntiquaCRM::ASqlDataQuery(ORDERS_SQL_TABLE_NAME);
  inputFields = m_tableData->columnNames();

  // Diese Felder sind bei der Abfrage nicht in @ref m_tableData enthalten!
  customInput.clear();
  customInput << "c_postal_address";
  customInput << "c_shipping_address";

  if (inputFields.isEmpty()) {
    QStringList warn(tr("An error has occurred!"));
    warn << tr("Can't load input datafields!");
    warn << tr("When getting this Message, please check your Network and "
               "Database connection!");
    openNoticeMessage(warn.join("\n"));
  }

  // Delivery Service & Package
  m_costSettings->o_delivery_service->initData();
  connect(m_costSettings->o_delivery_service, SIGNAL(sendSelectedService(int)),
          m_costSettings->o_delivery_package, SLOT(loadPackages(int)));

  // Menübar SQL Abfrage starten
  m_actionBar->setMailMenu(AntiquaCRM::MAIL_ORDER_GROUP);
}

bool OrdersEditor::setDataField(const QSqlField &field, const QVariant &value) {
  if (!field.isValid())
    return false;

  QString key = field.name();
  bool required = (field.requiredStatus() == QSqlField::Required);
  AntiquaCRM::AInputWidget *inp =
      findChild<AntiquaCRM::AInputWidget *>(key, Qt::FindChildrenRecursively);
  if (inp != nullptr) {
    inp->setRestrictions(field);
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

void OrdersEditor::importSqlResult() {
  if (m_tableData == nullptr)
    return;

  QHashIterator<QString, QVariant> it(m_tableData->getDataset());
  blockSignals(true);
  while (it.hasNext()) {
    it.next();
    QSqlField _f = m_tableData->getProperties(it.key());
    setDataField(_f, it.value());
  }
  blockSignals(false);

  setResetModified(inputFields);
}

bool OrdersEditor::sendSqlQuery(const QString &query) {
  if (query.isEmpty())
    return false;

  QSqlQuery _q = m_sql->query(query);
  if (_q.lastError().type() != QSqlError::NoError) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << query << m_sql->lastError();
#else
    qWarning("OrdersEditor: SQL Error!");
#endif
    return false;
  }

  if (_q.next()) {
    if (!_q.isNull("o_id")) {
      qint64 _oid = _q.value("o_id").toInt();
      if (_oid < 1) {
        qWarning("SQL-Insert: get empty OrderID!");
        return false;
      }
      m_tableData->setValue("o_id", _oid);
      setDataField(m_tableData->getProperties("o_id"), _oid);
      setResetModified(inputFields);
      setOrderPaymentNumbers(_oid);
    }
  }

  return true;
}

const QHash<QString, QVariant> OrdersEditor::createSqlDataset() {
  QHash<QString, QVariant> _hash;
  QListIterator<AntiquaCRM::AInputWidget *> it(
      findChildren<AntiquaCRM::AInputWidget *>(fieldPattern,
                                               Qt::FindChildrenRecursively));
  while (it.hasNext()) {
    AntiquaCRM::AInputWidget *inp = it.next();
    QString _name = inp->objectName();
    if (ignoreFields.contains(_name) || customInput.contains(_name))
      continue;

    if (inp->isRequired() && !inp->isValid()) {
      openNoticeMessage(inp->statusHints()); // popUpHints
      inp->setFocus();
      _hash.clear();
      return _hash;
    }
    _hash.insert(_name, inp->getValue());
  }
  return _hash;
}

void OrdersEditor::createSqlUpdate() {
  qint64 _oid = getSerialID("o_id");
  if (_oid < 1)
    return;

  /*
   * Ein UPDATE ohne Artikel wird feige verweigert!
   */
  if (m_ordersTable->isEmpty()) {
    openNoticeMessage(tr("No Article has been added to this order!"));
    QPushButton *btn = m_actionBar->findChild<QPushButton *>("article");
    if (btn != nullptr && btn->isVisible()) {
      qDebug() << Q_FUNC_INFO << btn->objectName();
      btn->setFocus();
    }
    return;
  }

  // Nach einem INSERT wieder aktivieren!
  QStringList _required({"o_id", "o_invoice_id", "o_delivery"});
  foreach (QString n, _required) {
    getInputEdit(n)->setRequired(true);
  }

  QHash<QString, QVariant> _data = createSqlDataset();
  if (_data.size() < 1)
    return;

  int _changes = 0;
  QStringList _set;
  QHashIterator<QString, QVariant> it(_data);
  while (it.hasNext()) {
    it.next();
    if (it.key() == "o_id")
      continue;

    // Nur geänderte Felder in das Update aufnehmen!
    if (it.value() == m_tableData->getValue(it.key()))
      continue;

    if (it.value().metaType().id() == QMetaType::QString) {
      _set.append(it.key() + "='" + it.value().toString() + "'");
    } else {
      _set.append(it.key() + "=" + it.value().toString());
    }
    _changes++;
  }

  /*
   * Wenn der Status der Datenbank nicht auf abschließen steht
   * und der Anwender den Auftrag abschließt.
   * Muss das Datum für geliefert hier gesetzt werden!
   */
  if (!databaseOrderStatus() && currentOrderStatus()) {
    _set.append("o_delivered=CURRENT_TIMESTAMP");
    _changes++;
  }

  // Artikel Bestelliste aktualisieren
  const QString _articleSql = getSqlArticleOrders();
  if (_articleSql.isEmpty()) {
    pushStatusMessage(tr("No SQL Articles exist!"));
    return;
  }

  QString _sql;
  // Update SQL-Update erstellen
  if (_changes > 0) {
    _sql.append("UPDATE inventory_orders SET ");
    _sql.append(_set.join(","));
    _sql.append(",o_modified=CURRENT_TIMESTAMP WHERE o_id=");
    _sql.append(QString::number(_oid));
    _sql.append(";");
  }

  // Füge Artikel zum SQL-Update hinzu
  if (_articleSql.size() > 1) {
    if (!_sql.isEmpty())
      _sql.append("\n");

    _sql.append(_articleSql);
  }

  if (_changes == 0 && _sql.isEmpty()) {
    pushStatusMessage(tr("No Modifications found, nothing todo!"));
    setWindowModified(false); // ???
    return;
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << _sql << Qt::endl;
#endif

  if (sendSqlQuery(_sql)) {
    setResetModified(inputFields);
    openSuccessMessage(tr("Order saved successfully!"));
  }
}

void OrdersEditor::createSqlInsert() {
  //
  qDebug() << Q_FUNC_INFO << "TODO";
}

void OrdersEditor::generateDeliveryNumber(qint64 orderId) {
  QDate _date;
  QString _sql("SELECT o_since FROM ");
  _sql.append("inventory_orders WHERE o_id=");
  _sql.append(QString::number(orderId) + ";");
  QSqlQuery _q = m_sql->query(_sql);
  if (_q.size() == 1) {
    _q.next();
    _date = _q.value("o_since").toDate();
  }
  QString _dn; // DeliveryNumber
  _dn.append(QString::number(_date.year()));
  _dn.append(QString::number(_date.dayOfYear()));
  _dn.append(QString::number(orderId));
  m_tableData->setValue("o_delivery", QString());
  setDataField(m_tableData->getProperties("o_delivery"), _dn);
}

bool OrdersEditor::checkDeliveryNumber() {
  const QString _id = getDataValue("o_delivery").toString();
  if (_id.isEmpty() || _id.length() < 8) {
    pushStatusMessage(tr("Missing Deliverynote Number"));
    return false;
  }
  return true;
}

void OrdersEditor::setOrderPaymentNumbers(qint64 orderId) {
  const QDate _date = QDate::currentDate();
  QString _dn;
  _dn.append(QString::number(_date.year()));
  _dn.append(QString::number(_date.dayOfYear()));
  _dn.append(QString::number(orderId));
  // Siehe PostgreSQL::new_invoice_id()
  QString _sql("SELECT o_invoice_id FROM ");
  _sql.append("inventory_orders WHERE o_id=");
  _sql.append(QString::number(orderId) + ";");
  QSqlQuery _q = m_sql->query(_sql);
  if (_q.size() == 1) {
    _q.next();
    qint64 _iid = _q.value("o_invoice_id").toInt();
    m_tableData->setValue("o_invoice_id", _iid);
    setDataField(m_tableData->getProperties("o_invoice_id"), _iid);
    m_tableData->setValue("o_delivery", _dn);
    setDataField(m_tableData->getProperties("o_delivery"), _dn);
  } else {
    qWarning("SQL ERROR: %s", qPrintable(m_sql->lastError()));
  }
}

const OrdersEditor::IdsCheck OrdersEditor::getCheckEssentialsIds() {
  IdsCheck t_check;
  qint64 _oid = getSerialID("o_id");
  qint64 _cid = getSerialID("o_customer_id");
  qint64 _iid = getSerialID("o_invoice_id");
  if (_oid > 1 && _cid > 1 && _iid > 1) {
    t_check.isNotValid = false;
    t_check.or_id = _oid;
    t_check.cu_id = _cid;
    t_check.in_id = _iid;
    return t_check;
  }

  QString _m("<p>");
  _m.append(tr("A Article can't inserted, if no Order-/Customer Id exists."));
  _m.append("</p><p>");
  _m.append(tr("Please save your your Order first."));
  _m.append("</p>");
  openNoticeMessage(_m);

  t_check.isNotValid = true;
  return t_check;
}

AntiquaCRM::SalesTax OrdersEditor::initSalesTax() {
  QString _country = getDataValue("o_vat_country").toString();
  if (_country.isEmpty() || _country == "XX") {
    qInfo("No Eurpean country - set invoice tax to no!");
    getInputEdit("o_vat_levels")->setValue(AntiquaCRM::SalesTax::TAX_NOT);
    return AntiquaCRM::SalesTax::TAX_NOT;
  }
  int i = getDataValue("o_vat_levels").toInt();
  return static_cast<AntiquaCRM::SalesTax>(i);
}

int OrdersEditor::getSalesTaxValue(int index) {
  int _vat = -1;
  m_cfg->beginGroup("payment");
  switch (static_cast<AntiquaCRM::ArticleType>(index)) {
  case (AntiquaCRM::ArticleType::BOOK):
    _vat = m_cfg->value("vat2").toInt();
    break;

  default:
    _vat = m_cfg->value("vat1").toInt();
    break;
  };
  m_cfg->endGroup();
  return _vat;
}

int OrdersEditor::getSalesTaxType(int index) {
  switch (static_cast<AntiquaCRM::ArticleType>(index)) {
  case (AntiquaCRM::ArticleType::BOOK):
    return 1; // VAT reduced

  default:
    return 0; // VAT normal
  };
}

const QList<AntiquaCRM::OrderArticleItems>
OrdersEditor::queryOrderArticles(qint64 orderId) {
  QList<AntiquaCRM::OrderArticleItems> _list;
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
      _list.append(article);
    }
  }
  return _list;
}

AntiquaCRM::ArticleOrderItem
OrdersEditor::addArticleItem(const QString &key, const QVariant &value) const {
  return AntiquaCRM::AProviderOrder::createItem(key, value);
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
      items.append(
          addArticleItem("a_customer_id", getSerialID("o_customer_id")));
      for (int i = 0; i < r.count(); i++) {
        QSqlField f = r.field(i);
        items.append(addArticleItem(f.name(), f.value()));
        // NOTE: a_sell_price is not in SQL Query
        if (f.name() == "a_price")
          items.append(addArticleItem("a_sell_price", f.value()));
      }
      if (items.size() > 0) {
        m_ordersTable->addArticle(items);
        return true;
      }
    }
#ifdef ANTIQUA_DEVELOPEMENT
    else {
      qDebug() << Q_FUNC_INFO << m_sql->lastError();
    }
#endif
  }
  pushStatusMessage(tr("Article: %1 not found or no stock!").arg(articleId));
  return false;
}

const QString OrdersEditor::getSqlArticleOrders() {
  //
  qDebug() << Q_FUNC_INFO << "TODO";
  return QString();
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

qint64 OrdersEditor::findCustomer(const QJsonObject &obj, qint64 customerId) {
  QStringList _f("c_firstname");
  _f << "c_lastname";
  _f << "c_postalcode";
  _f << "c_email_0";

  QStringList _clause;
  foreach (QString k, _f) {
    QString v = obj.value(k).toString();
    if (!v.isEmpty())
      _clause << k + "='" + v + "'";
  }

  // Search customer
  QString _sql("SELECT c_id FROM customers WHERE (");
  if (customerId > 1) {
    _sql.append("c_id=" + QString::number(customerId));
  } else {
    _sql.append(_clause.join(" AND "));
    _sql.append(") OR (c_provider_import='");
    _sql.append(obj.value("c_provider_import").toString());
    _sql.append("'");
  }
  _sql.append(") ORDER BY c_id;");

  QSqlQuery _q = m_sql->query(_sql);
  if (_q.size() > 0) {
    QList<qint64> _list;
    while (_q.next()) {
      _list << _q.value("c_id").toInt();
    }
    if (_list.size() > 1)
      qWarning("Warning: Found more then one Customer!");

    return _list.first();
  }
  return -1;
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
  // QPair<int, int> _t_ds = getDeliveryService(true);
  // m_tableData->setValue("o_delivery_service", _t_ds.first);
  // m_tableData->setValue("o_delivery_package", _t_ds.second);
  m_tableData->setValue("o_delivery_send_id", "");
  m_tableData->setValue("o_delivery_add_price", false);
  setDataField(m_tableData->getProperties("o_delivery_add_price"),
               m_tableData->getValue("o_delivery_add_price"));
}

bool OrdersEditor::prepareCreateEntry() {
  setInputFields();
  setResetModified(inputFields);
  setEnabled(true);
  return true;
}

void OrdersEditor::setStatusProtection(bool b) {
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
  if (checkIsModified() || m_ordersTable->isWindowModified()) {
    unsavedChangesPopup();
    return;
  }
  setFinalLeaveEditor(false);
}

void OrdersEditor::setFinalLeaveEditor(bool force) {
  if (force) // Wenn auf Abbrechen geklickt wurde!
    setWindowModified(false);

  m_ordersTable->clearContents(); // Artikel Tabelle leeren!
  setResetInputFields();
  emit sendLeaveEditor(); // Zurück zur Hauptsansicht
}

void OrdersEditor::createMailMessage(const QString &type) {
  //
  qDebug() << Q_FUNC_INFO << "TODO" << type;
}

void OrdersEditor::createPrintDeliveryNote() {
  //
  qDebug() << Q_FUNC_INFO << "TODO";
}

void OrdersEditor::createPrintInvoiceNote() {
  //
  qDebug() << Q_FUNC_INFO << "TODO";
}

void OrdersEditor::createPrintPaymentReminder() {
  //
  qDebug() << Q_FUNC_INFO << "TODO";
}

void OrdersEditor::openSearchInsertArticle() {
  //
  qDebug() << Q_FUNC_INFO << "TODO";
}

void OrdersEditor::setRestore() {
  importSqlResult();
  setEnabled(true);
}

AntiquaCRM::OrderPayment OrdersEditor::paymentStatus() {
  int _id = o_payment_status->getValue().toInt();
  return static_cast<AntiquaCRM::OrderPayment>(_id);
}

AntiquaCRM::OrderStatus OrdersEditor::orderStatus() {
  int _id = o_order_status->getValue().toInt();
  return static_cast<AntiquaCRM::OrderStatus>(_id);
}

bool OrdersEditor::addArticle(qint64 articleId) {
  if (getCheckEssentialsIds().isNotValid) {
    pushStatusMessage(tr("Missing essential Ids, save Order first!"));
    return false;
  }
  if (addArticleToOrderTable(articleId)) {
    getInputEdit("o_delivery")->setWindowModified(true);
    return true;
  }
  return false;
}

bool OrdersEditor::openEditEntry(qint64 orderId) {
  bool _retval = false;
  if (orderId < 1)
    return _retval;

  const QString _oid_str = QString::number(orderId);
  if (_oid_str.isEmpty())
    return _retval;

  setInputFields();
  setResetModified(inputFields);
  o_order_status->setValue(AntiquaCRM::OrderStatus::STARTED);

  AntiquaCRM::ASqlFiles _tpl("query_order_by_oid");
  if (!_tpl.openTemplate())
    return false;

  _tpl.setWhereClause("o_id=" + _oid_str);

  QSqlQuery _q = m_sql->query(_tpl.getQueryContent());
  if (_q.size() != 0) {
    QSqlRecord _r = _q.record();
    _q.next();
    // Standard Felder
    foreach (QString key, inputFields) {
      m_tableData->setValue(key, _q.value(_r.indexOf(key)));
    }
    // Kunden Daten
    foreach (QString key, customInput) {
      QSqlField _f = _r.field(key);
      setDataField(_f, _q.value(_f.name()));
    }

    // Bestehende Artikel Einkäufe mit orderId einlesen!
    m_ordersTable->addArticles(queryOrderArticles(orderId));

    setResetModified(customInput);
    _retval = true;
  } else {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
#endif
    _retval = false;
  }

  if (_retval) {
    importSqlResult();
    setResetModified(inputFields);
    setEnabled(true);
  }

  setStatusProtection(databaseOrderStatus());
  return _retval;
}

bool OrdersEditor::createNewEntry() {
  qDebug() << Q_FUNC_INFO << "TODO new Order Assistant";
  return false;
}

bool OrdersEditor::createNewOrder(qint64 customerId) {
  if (customerId < 1)
    return false;

  prepareCreateEntry();
  setDefaultValues();

  // Nehme relevante Kundendaten
  AntiquaCRM::ASqlFiles _tpl("query_customer_new_order");
  if (_tpl.openTemplate()) {
    _tpl.setWhereClause("c_id=" + QString::number(customerId));
    QSqlQuery _q = m_sql->query(_tpl.getQueryContent());
    if (_q.size() > 0) {
      QSqlRecord _r = _q.record();
      while (_q.next()) {
        for (int c = 0; c < _r.count(); c++) {
          QSqlField _f = _r.field(c);
          m_tableData->setValue(_f.name(), _q.value(_f.name()));
          setDataField(_f, _q.value(_f.name()));
        }
      }
      // Ignore this
      setResetModified(customInput);
      return true;
    }
  }
  return false;
}

bool OrdersEditor::createCustomEntry(const QJsonObject &object) {
  qDebug() << Q_FUNC_INFO << "TODO" << object;

  const QString _action = object.value("ACTION").toString();
  if (_action.isEmpty())
    return false;

  const QString _provider_id = object.value("PRORDER").toString();
  if (_provider_id.isEmpty())
    return false;

  const QString _articles = object.value("ARTICLES").toString();
  QStringList _article_list = _articles.split(",");
  if (_article_list.size() < 1) {
    qWarning("NO ARTICLES exists!");
    return false;
  }

  AntiquaCRM::ASqlFiles _tpl("query_provider_order_exists");
  if (!_tpl.openTemplate())
    return false;

  _tpl.setWhereClause("pr_order='" + _provider_id + "'");
  QSqlQuery _q = m_sql->query(_tpl.getQueryContent());
  if (_q.size() != 1) {
    openNoticeMessage(tr("No Provider orders data found!"));
    return false;
  }

  _q.next();
  const QByteArray _data = _q.value("pr_order_data").toByteArray();
  QJsonObject _jobj = QJsonDocument::fromJson(_data).object();
  if (_jobj.isEmpty())
    return false;

  const QString o_provider_name = _q.value("pr_name").toString();
  const QString o_provider_order_id = _q.value("pr_order").toString();
  qint64 _customer_id = _q.value("pr_customer_id").toInt();
  if (_customer_id < 1) {
    qWarning("Empty Customer Id");
  }

  setInputFields();

  AntiquaCRM::AProviderOrder _prorder(o_provider_name, o_provider_order_id);
  // Kunden Daten
  if (_jobj.contains("customer")) {
    QJsonObject customer = _jobj.value("customer").toObject();
    // NOTE: Wir benötigen bei einem Import eine gültige Kundennummer!
    _customer_id = findCustomer(customer, _customer_id);
    if (_customer_id < 1) {
      openNoticeMessage(tr("Customer not found or set!"));
      return false;
    }
    _prorder.setValue("o_customer_id", _customer_id);
    // NOTE: Wir nehmen die korrigierten Kundendaten!
    AntiquaCRM::ASqlFiles customerQuery("query_order_import_customer");
    if (customerQuery.openTemplate()) {
      customerQuery.setWhereClause("c_id=" + QString::number(_customer_id));
      QSqlQuery q = m_sql->query(customerQuery.getQueryContent());
      if (q.size() > 0) {
        QSqlRecord rec = q.record();
        while (q.next()) {
          for (int c = 0; c < rec.count(); c++) {
            QSqlField f = rec.field(c);
            // qDebug() << f.name() << q.value(f.name());
            _prorder.setValue(f.name(), q.value(f.name()));
          }
        }
      } else {
#ifdef ANTIQUA_DEVELOPEMENT
        qDebug() << Q_FUNC_INFO << m_sql->lastError();
#endif
        qWarning("Using Provider Import!");
        foreach (QString key, customer.keys()) {
          QVariant val = customer.value(key).toVariant();
          if (!_prorder.setValue(key, val))
            qWarning("Customer value '%s' not set!", qPrintable(key));
        }
      }
    } else {
      openNoticeMessage("FATAL: Can't open Template file!");
      return false;
    }
  }

  // Standard Felder
  if (_jobj.contains("orderinfo")) {
    QJsonObject orderinfo = _jobj.value("orderinfo").toObject();
    if (orderinfo.contains("o_payment_confirmed")) {
      bool paypal_status = (!orderinfo.value("o_payment_confirmed").isNull());
      if (paypal_status)
        o_payment_status->setValue(AntiquaCRM::OrderPayment::PAYED);
    }

    foreach (QString key, orderinfo.keys()) {
      QVariant val = orderinfo.value(key).toVariant();
      if (ignoreFields.contains(key))
        continue;

      if (!_prorder.setValue(key, val)) {
        qWarning("Order value '%s' not set!", qPrintable(key));
      }
    }
  }

  // Artikel
  if (_jobj.contains("articles")) {
    QJsonArray _array = _jobj.value("articles").toArray();
    for (int i = 0; i < _array.size(); i++) {
      QList<AntiquaCRM::ArticleOrderItem> items;
      QJsonObject article = _array[i].toObject();
      // NOTE: Das Feld 'a_tax' ist noch nicht in „antiquacmd“ eingebunden!
      // o_media_type | a_type
      if (!article.contains("a_tax") && article.contains("a_type")) {
        article.insert("a_tax",
                       getSalesTaxType(article.value("a_type").toInt()));
      }

      qint64 _article_id = article.value("a_article_id").toInt();
      // Siehe QJsonValue::article_numbers
      if (!_article_list.contains(QString::number(_article_id)))
        continue;

      // Ist erst mal 0
      AntiquaCRM::ArticleOrderItem item;
      item.key = QString("a_order_id");
      item.value = 0;
      items.append(item);
      // Muss hier eingefügt werden!
      if (_customer_id > 0) {
        AntiquaCRM::ArticleOrderItem item;
        item.key = QString("a_customer_id");
        item.value = _customer_id;
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
        _prorder.insertOrderItems(items);
    }
  }

  // Fehlende felder in JsonObject setzen!
  // Setze auf Auftragsbegin
  _prorder.setValue("o_order_status", AntiquaCRM::OrderStatus::STARTED);

  // Begin:Umsatzsteuer
  _prorder.setValue("o_vat_levels", AntiquaCRM::SalesTax::TAX_INCL);
  // End:Umsatzsteuer

  // Begin:Lieferdienst
  _prorder.setValue("o_delivery_service",
                    getDataValue("o_delivery_service").toInt());
  _prorder.setValue("o_delivery_package",
                    getDataValue("o_delivery_package").toInt());
  _prorder.setValue("o_delivery_send_id", "");
  _prorder.setValue("o_delivery_add_price", false);
  // Lieferschein Nr. ist hier noch leer!
  _prorder.setValue("o_delivery", QString());
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
      m_tableData->setValue(key, _prorder.getValue(key));
    }
  }

  // 2) Kunden Daten
  QSqlRecord _record = AntiquaCRM::ASqlDataQuery("customers").record();
  foreach (QString key, customInput) {
    setDataField(_record.field(key), _prorder.getValue(key));
  }

  // 3) Artikel Importieren
  m_ordersTable->addArticles(_prorder.orders());

  // 4) Datensätze Importieren
  importSqlResult();

  // 5) Erst nach einfügen reset
  setResetModified(inputFields);
  setResetModified(customInput);

  return true;
}
