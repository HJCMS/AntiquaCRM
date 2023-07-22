// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderseditor.h"
#include "orderarticledialog.h"
#include "ordersconfig.h"
#include "orderscostsettings.h"
#include "orderscustomerinfo.h"
#include "orderstableview.h"

#include <QLayout>

OrdersEditor::OrdersEditor(QWidget *parent)
    : AntiquaCRM::TabsEditor{ORDERS_SQL_EDITOR_PATTERN, parent} {
  setObjectName("orders_editor");
  setWindowTitle(tr("Edit Order") + " [*]");

  QVBoxLayout *mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(0, 0, 0, 0);

  QString _infotxt;
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
  _infotxt = tr("Set the order status for the current order.");
  o_order_status->setWhatsThisText(_infotxt);
  row0->addWidget(o_order_status);
  // Zahlungsstatus
  o_payment_status = new AntiquaCRM::SelectOrderPayment(this);
  o_payment_status->setObjectName("o_payment_status");
  o_payment_status->setBuddyLabel(tr("Payment status"));
  o_payment_status->setValue(AntiquaCRM::OrderPayment::NOTPAID);
  _infotxt = tr("Set the payment status for the current order.");
  o_payment_status->setWhatsThisText(_infotxt);
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
  mainLayout->addLayout(row1);
  // END:Row1

  // BEGIN:Row2
  m_ordersTable = new OrdersTableView(this, false);
/**
 * Für den Kunden ausblenden. Werden hier nicht benötigt!
 * @note Die Spaltenzahl ist zu diesem Zeitpunkt noch nicht bekannt!
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

  /**
   * BEGIN:Row3
   * Wegen SQL-Abfrage setMailMenu hier nicht setzen!
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

  // Register modified changes
  registerInputChanged();

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

OrdersEditor::~OrdersEditor() {
  ignoreFields.clear();
  inputFields.clear();
  customInput.clear();
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

  // Menübar SQL Abfrage starten
  m_actionBar->setMailMenu(AntiquaCRM::MAIL_ORDER_GROUP);
}

bool OrdersEditor::setDataField(const QSqlField &field, const QVariant &value) {
  if (!field.isValid())
    return false;

  const QString _key = field.name();
  bool _required = (field.requiredStatus() == QSqlField::Required);
  AntiquaCRM::AInputWidget *inp = getInputEdit(_key);
  if (inp != nullptr) {
    inp->setRestrictions(field);
    // qDebug() << key << value << required;
    inp->setValue(value);
    return true;
  }

  if (ignoreFields.contains(_key))
    return true;

  if (_required) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << "Unknown:" << _key << "|" << value;
#else
    qWarning("Unknown Key (%s) found.", qPrintable(_key));
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
    setDataField(m_tableData->getProperties(it.key()), it.value());
  }
  blockSignals(false);

  setResetModified(inputFields);
}

bool OrdersEditor::sendSqlQuery(const QString &query) {
  if (query.isEmpty())
    return false;

  QSqlQuery _q = m_sql->query(query);
  if (_q.lastError().type() != QSqlError::NoError) {
    qWarning("OrdersEditor: SQL Error!");
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
      setOrderPaymentNumbers(_oid);
      setResetModified(inputFields);
    }
  }

  return true;
}

const QHash<QString, QVariant> OrdersEditor::createSqlDataset() {
  QHash<QString, QVariant> _hash;
  QListIterator<AntiquaCRM::AInputWidget *> it(getInputEditList(fieldPattern));
  while (it.hasNext()) {
    AntiquaCRM::AInputWidget *inp = it.next();
    QString _name = inp->objectName();
    if (ignoreFields.contains(_name) || customInput.contains(_name))
      continue;

    if (inp->isRequired() && !inp->isValid()) {
      openNoticeMessage(inp->popUpHints());
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
  const QString _articleSql = getOrderTableSqlQuery();
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
    return;
  }

  if (sendSqlQuery(_sql)) {
    m_ordersTable->setWindowModified(false);
    setResetModified(inputFields);
    openSuccessMessage(tr("Order saved successfully!"));
  } else {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << _sql << Qt::endl;
#endif
    openSuccessMessage(tr("Save operation exited with errors!"));
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

    if (it.value().metaType().id() == QMetaType::QString) {
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
    if (m_ordersTable->setOrderId(oid)) {
      // Artikel Bestelliste speichern
      QString articles_sql = getOrderTableSqlQuery();
      if (articles_sql.isEmpty()) {
        pushStatusMessage(tr("No SQL Articles exist!"));
        return;
      }
      if (sendSqlQuery(articles_sql)) {
        // Bestehende Artikelliste neu einlesen!
        m_ordersTable->addArticles(queryArticles(oid));
        m_ordersTable->setWindowModified(false);
        pushStatusMessage(tr("Save Articles success!"));
      }
    }
  }
}

void OrdersEditor::generateDeliveryNumber(qint64 orderId) {
  QDate _date;
  QString _sql("SELECT o_since FROM inventory_orders WHERE o_id=");
  _sql.append(QString::number(orderId) + ";");
  QSqlQuery _query = m_sql->query(_sql);
  if (_query.size() == 1) {
    _query.next();
    _date = _query.value("o_since").toDate();
  }
  QString _dn; // DeliveryNumber
  _dn.append(QString::number(_date.year()));
  _dn.append(QString::number(_date.dayOfYear()));
  _dn.append(QString::number(orderId));
  // Muss leer sein und wird nach dem Speichern eingefügt.
  m_tableData->setValue("o_delivery", QString());
  // o_delivery_note_id
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

void OrdersEditor::setOrderPaymentNumbers(qint64 oid) {
  if (oid < 1) {
    qWarning("Missing Order Id for Order Payment.");
    return;
  }

  const QDate _date = QDate::currentDate();
  QString _dn; // DeliveryNumber
  _dn.append(QString::number(_date.year()));
  _dn.append(QString::number(_date.dayOfYear()));
  _dn.append(QString::number(oid));
  // Siehe PostgreSQL::new_invoice_id()
  QString _sql("SELECT o_invoice_id FROM inventory_orders WHERE o_id=");
  _sql.append(QString::number(oid) + ";");
  QSqlQuery _query = m_sql->query(_sql);
  if (_query.size() == 1) {
    _query.next();
    qint64 _iid = _query.value("o_invoice_id").toInt();
    m_tableData->setValue("o_invoice_id", _iid);
    setDataField(m_tableData->getProperties("o_invoice_id"), _iid);
    m_tableData->setValue("o_delivery", _dn);
    setDataField(m_tableData->getProperties("o_delivery"), _dn);
  } else {
    qWarning("SQL ERROR: %s", qPrintable(m_sql->lastError()));
  }
}

const OrdersEditor::Idset OrdersEditor::identities() {
  Idset t_ids;
  qint64 _oid = getSerialID("o_id");
  qint64 _cid = getSerialID("o_customer_id");
  qint64 _iid = getSerialID("o_invoice_id");
  if (_oid > 1 && _cid > 1 && _iid > 1) {
    t_ids.isValid = true;
    t_ids.or_id = _oid;
    t_ids.cu_id = _cid;
    t_ids.in_id = _iid;
    return t_ids;
  }

  QString _m("<p>");
  _m.append(tr("New Article can not inserted.") + "\n");
  _m.append(tr("When no Order, Invoice or Customer Id exists."));
  _m.append("</p><p>");
  _m.append(tr("Please save your your Order first."));
  _m.append("</p>");
  openNoticeMessage(_m);

  t_ids.isValid = false;
  return t_ids;
}

AntiquaCRM::SalesTax OrdersEditor::initSalesTax() {
  QString _country = getDataValue("o_vat_country").toString();
  if (_country.isEmpty() || _country == "XX") {
    qInfo("No Eurpean country - set invoice tax to no!");
    getInputEdit("o_vat_levels")->setValue(AntiquaCRM::SalesTax::TAX_NOT);
    return AntiquaCRM::SalesTax::TAX_NOT;
  }
  int _vat = getDataValue("o_vat_levels").toInt();
  return static_cast<AntiquaCRM::SalesTax>(_vat);
}

int OrdersEditor::getSalesTaxType(int type) {
  switch (static_cast<AntiquaCRM::ArticleType>(type)) {
  case (AntiquaCRM::ArticleType::BOOK):
    return 1; // VAT reduced

  default:
    return 0; // VAT normal
  };
}

const QList<AntiquaCRM::OrderArticleItems>
OrdersEditor::queryArticles(qint64 oid) {
  QList<AntiquaCRM::OrderArticleItems> _list;
  QString sql("SELECT * FROM article_orders WHERE a_order_id=");
  sql.append(QString::number(oid) + ";");
  QSqlQuery _query = m_sql->query(sql);
  if (_query.size() > 0) {
    const QSqlRecord _record = _query.record();
    while (_query.next()) {
      AntiquaCRM::OrderArticleItems _items;
      for (int i = 0; i < _record.count(); i++) {
        const QString _fn = _record.field(i).name();
        _items.append(addArticleItem(_fn, _query.value(_fn)));
      }
      _list.append(_items);
    }
  }
  return _list;
}

AntiquaCRM::ArticleOrderItem
OrdersEditor::addArticleItem(const QString &key, const QVariant &value) const {
  return AntiquaCRM::AProviderOrder::createItem(key, value);
}

bool OrdersEditor::addOrderTableArticle(qint64 aid) {
  if (aid < 1)
    return false;

  /**
   * Doppelte Einträge verhindern.
   */
  if (m_ordersTable->articleExists(aid)) {
    pushStatusMessage(tr("Article: %1 already Exists!").arg(aid));
    return false;
  }

  /**
   * Auftragsnummer und Kundennummer sind erforderlich!
   */
  qint64 _oid = getSerialID("o_id");
  qint64 _cid = getSerialID("o_customer_id");
  if (_oid < 1 || _cid < 1) {
    pushStatusMessage(tr("Missing required Identities!"));
    return false;
  }

  /**
   * @note Beim Hinzufügen muss das Feld a_payment_id=0 sein!
   * @sa OrdersTableView::getSqlQuery
   */
  AntiquaCRM::ASqlFiles _tpl("query_article_order_with_id");
  if (_tpl.openTemplate()) {
    _tpl.setWhereClause("i_id=" + QString::number(aid));
    QSqlQuery _q = m_sql->query(_tpl.getQueryContent());
    if (_q.size() > 0) {
      _q.next();
      QSqlRecord _record = _q.record();
      AntiquaCRM::OrderArticleItems _items;
      _items.append(addArticleItem("a_order_id", _oid));
      _items.append(addArticleItem("a_customer_id", _cid));
      for (int i = 0; i < _record.count(); i++) {
        const QSqlField _field = _record.field(i);
        if (_field.metaType().id() == QMetaType::UnknownType) {
          qWarning("Unknown Metatype for '%s'!", qPrintable(_field.name()));
        }
        _items.append(addArticleItem(_field.name(), _field.value()));
      }
      if (_items.size() > 0) {
        m_ordersTable->addArticle(_items);
        return true;
      }
    }
  }
  pushStatusMessage(tr("Article: %1 not found or no stock!").arg(aid));
  return false;
}

const QString OrdersEditor::getOrderTableSqlQuery() {
  if (!identities().isValid)
    return QString();

  QStringList queries = m_ordersTable->getSqlQuery();
  if (queries.size() > 0)
    return queries.join("\n");

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

qint64 OrdersEditor::findCustomer(const QJsonObject &obj, qint64 cid) {
  QStringList _clause;
  QStringList _fields("c_firstname");
  _fields << "c_lastname";
  _fields << "c_postalcode";
  _fields << "c_email_0";
  foreach (QString _f, _fields) {
    const QString _v = obj.value(_f).toString();
    if (_v.isEmpty())
      continue;

    _clause << _f + "='" + _v + "'";
  }
  _fields.clear();

  // Search customer
  QString _sql("SELECT c_id FROM customers WHERE (");
  if (cid > 1) {
    _sql.append("c_id=" + QString::number(cid));
  } else {
    _sql.append(_clause.join(" AND "));
    _sql.append(") OR (c_provider_import='");
    _sql.append(obj.value("c_provider_import").toString());
    _sql.append("'");
  }
  _sql.append(") ORDER BY c_id;");

  QSqlQuery _query = m_sql->query(_sql);
  if (_query.size() > 0) {
    QList<qint64> _list;
    while (_query.next()) {
      _list << _query.value("c_id").toInt();
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

  // Einstellungen für Umsatzsteuer
  m_tableData->setValue("o_vat_levels", AntiquaCRM::SalesTax::TAX_INCL);
  setDataField(m_tableData->getProperties("o_vat_levels"),
               m_tableData->getValue("o_vat_levels"));
  m_tableData->setValue("o_vat_country", QString("XX"));
  setDataField(m_tableData->getProperties("o_vat_country"),
               m_tableData->getValue("o_vat_country"));

  // Lieferdienst
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
  o_payment_status->setReadOnly(b);
  o_order_status->setReadOnly(b);
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
  if (!identities().isValid)
    return;

  OrderArticleDialog *d = new OrderArticleDialog(this);
  if (d->exec() == QDialog::Rejected)
    return;

  qint64 _aid = d->getArticle();
  if (_aid < 1)
    return;

  if (addOrderTableArticle(_aid))
    setWindowModified(true);

  d->deleteLater();
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

bool OrdersEditor::addArticle(qint64 aid) {
  if (aid < 1)
    return false;

  if (!identities().isValid)
    return false;

  if (addOrderTableArticle(aid))
    return true;

  return false;
}

bool OrdersEditor::openEditEntry(qint64 oid) {
  bool _retval = false;
  if (oid < 1)
    return _retval;

  setInputFields();
  setResetModified(inputFields);
  o_order_status->setValue(AntiquaCRM::OrderStatus::STARTED);

  AntiquaCRM::ASqlFiles _tpl("query_order_by_oid");
  if (!_tpl.openTemplate())
    return false;

  _tpl.setWhereClause("o_id=" + QString::number(oid));

  QSqlQuery _query = m_sql->query(_tpl.getQueryContent());
  if (_query.size() != 0) {
    const QSqlRecord _record = _query.record();
    _query.next();
    // Standard Felder
    foreach (QString k, inputFields) {
      m_tableData->setValue(k, _query.value(_record.indexOf(k)));
    }
    // Kunden Daten
    foreach (QString k, customInput) {
      const QSqlField _field = _record.field(k);
      setDataField(_field, _query.value(_field.name()));
    }
    // Bestehende Artikel Einkäufe mit orderId einlesen!
    m_ordersTable->addArticles(queryArticles(oid));

    setResetModified(customInput);
    _retval = true;
  }

  if (_retval) {
    importSqlResult();
    m_ordersTable->setWindowModified(false);
    setResetModified(inputFields);
    setEnabled(true);
  }

  setStatusProtection(databaseOrderStatus());
  return _retval;
}

bool OrdersEditor::createNewEntry() {
  qInfo("Create new Order without Customer Id will rejected.");
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
  qDebug() << Q_FUNC_INFO << "Experimental:" << object;

  const QString _action = object.value("ACTION").toString();
  if (_action.isEmpty())
    return false;

  const QString _provider_id = object.value("PRORDER").toString();
  if (_provider_id.isEmpty())
    return false;

  const QString _articles = object.value("ARTICLES").toString();
  QStringList _article_list = _articles.split(",");
  if (_article_list.size() < 1) {
    qWarning("ARTICLES list not exists or empty!");
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
  m_ordersTable->setWindowModified(false);

  // 4) Datensätze Importieren
  importSqlResult();

  // 5) Erst nach einfügen reset
  setResetModified(inputFields);
  setResetModified(customInput);

  return true;
}
