// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderseditor.h"
#include "orderarticledialog.h"
#include "ordersconfig.h"
#include "orderscostsettings.h"
#include "orderscustomerinfo.h"
#include "orderstableview.h"
#include "orderstatusactionframe.h"
#include "refunddialog.h"

#include <AntiquaMail>
#include <AntiquaPrinting>
#include <QLayout>

OrdersEditor::OrdersEditor(QWidget* parent)
    : AntiquaCRM::TabsEditor{ORDERS_SQL_EDITOR_PATTERN, parent} {
  setObjectName("orders_editor");
  setWindowTitle(tr("Edit Order") + " [*]");

  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  mainLayout->setContentsMargins(0, 0, 0, 0);

  // BEGIN:Row0
  QHBoxLayout* row0 = new QHBoxLayout();
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

  // Delivery and Payment status
  m_orderStatus = new OrderStatusActionFrame(this);
  row0->addWidget(m_orderStatus);

  mainLayout->addLayout(row0);
  // END:Row0

  // BEGIN:Row1
  QHBoxLayout* row1 = new QHBoxLayout();
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
#ifndef ANTIQUA_DEVELOPMENT
  /*
   * Für den Kunden ausblenden. Werden hier nicht benötigt!
   * NOTE - Die Spaltenzahl ist zu diesem Zeitpunkt noch nicht bekannt!
   */
  QStringList hideColumn("a_payment_id");
  hideColumn << "a_order_id";
  hideColumn << "a_customer_id";
  hideColumn << "a_modified";
  hideColumn << "a_provider_id";
  hideColumn << "a_refunds_cost";
  m_ordersTable->hideColumns(hideColumn);
#endif
  mainLayout->addWidget(m_ordersTable);
  mainLayout->setStretch(2, 0);
  // END:Row2

  // BEGIN:Row3
  // WARNING - Wegen SQL-Abfrage setMailMenu hier nicht setzen!
  //           Siehe AntiquaCRM::MailButton::setSections ...
  m_actionBar = new AntiquaCRM::TabsEditActionBar(this);
  m_actionBar->setViewMailButton(true);
  m_actionBar->setRestoreable(false); // ResetButton off
  m_actionBar->setViewRestoreButton(false);
  m_actionBar->setViewActionAddButton(true);
  m_actionBar->setPrinterMenu(AntiquaCRM::PRINT_DELIVERY   // Delivery
                              | AntiquaCRM::PRINT_INVOICE  // Invoice
                              | AntiquaCRM::PRINT_REMINDER // Reminder
                              | AntiquaCRM::PRINT_REFUND   // Refunding
                              | AntiquaCRM::PRINT_ADMONITION);
  mainLayout->addWidget(m_actionBar);
  // END:Row3

  setLayout(mainLayout);

  // Register modified changes
  registerInputChanged();

  // Signals::OrderStatusActionFrame
  connect(m_orderStatus, SIGNAL(sendNoticeMessage(QString)), SLOT(openNoticeMessage(QString)));
  connect(m_orderStatus, SIGNAL(sendNotifyStatus(QString)), SLOT(pushStatusMessage(QString)));
  connect(m_orderStatus, SIGNAL(sendOrderPayment(AntiquaCRM::OrderPayment)),
          SLOT(hintsAboutRefund(AntiquaCRM::OrderPayment)));

  // Signals:ActionsBar
  connect(m_actionBar, SIGNAL(sendRestoreClicked()), SLOT(setRestore()));
  connect(m_actionBar, SIGNAL(sendPrintDeliveryNote()), SLOT(createPrintDeliveryNote()));
  connect(m_actionBar, SIGNAL(sendPrintInvoiceNote()), SLOT(createPrintInvoice()));
  connect(m_actionBar, SIGNAL(sendPrintPaymentReminder()), SLOT(createPrintPaymentReminder()));
  connect(m_actionBar, SIGNAL(sendPrintRefunding()), SLOT(createPrintRefundInvoice()));
  connect(m_actionBar, SIGNAL(sendPrintAdmonition()), SLOT(createPrintAdmonition()));
  connect(m_actionBar, SIGNAL(sendCreateMailMessage(QString)), SLOT(createMailMessage(QString)));
  connect(m_actionBar, SIGNAL(sendAddCustomAction()), SLOT(openSearchInsertArticle()));
  connect(m_actionBar, SIGNAL(sendCancelClicked()), SLOT(setFinalLeaveEditor()));
  connect(m_actionBar, SIGNAL(sendSaveClicked()), SLOT(setSaveData()));
  connect(m_actionBar, SIGNAL(sendFinishClicked()), SLOT(setCheckLeaveEditor()));
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
  // @deprecated Werden seit 1.1.* nicht mehr verwendet
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
    warn << tr("Can not load and prepare datafields!");
    warn << tr("Please check your Network or Database connection!");
    openNoticeMessage(warn.join("\n"));
  }

  // Delivery Service & Package
  m_costSettings->o_delivery_service->initData();

  // Menübar SQL Abfrage starten
  m_actionBar->setMailMenu(AntiquaCRM::MAIL_ORDER_GROUP);
}

bool OrdersEditor::setDataField(const QSqlField& field, const QVariant& value) {
  if (!field.isValid())
    return false;

  const QString _key = field.name();
  bool _required = (field.requiredStatus() == QSqlField::Required);
  AntiquaCRM::AInputWidget* inp = getInputEdit(_key);
  if (inp != nullptr) {
    inp->setRestrictions(field);
    // qDebug() << key << value << required;
    inp->setValue(value);
    return true;
  }

  if (ignoreFields.contains(_key))
    return true;

  if (_required) {
#ifdef ANTIQUA_DEVELOPMENT
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

bool OrdersEditor::sendSqlQuery(const QString& query) {
  if (query.isEmpty())
    return false;

  QSqlQuery _q = m_sql->query(query);
  if (_q.lastError().type() != QSqlError::NoError) {
    qWarning("OrdersEditor: SQL Error!");
    return false;
  }

  if (_q.next()) {
    if (!_q.isNull("o_id")) {
      qint64 _oid = _q.value("o_id").toLongLong();
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
  QListIterator<AntiquaCRM::AInputWidget*> it(getInputEditList(fieldPattern));
  while (it.hasNext()) {
    AntiquaCRM::AInputWidget* inp = it.next();
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

  // Ein UPDATE ohne Artikel wird feige verweigert!
  if (m_ordersTable->isEmpty()) {
    openNoticeMessage(tr("No Article has been added to this order!"));
    QPushButton* btn = m_actionBar->findChild<QPushButton*>("article");
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

  // Wenn der Status der Datenbank nicht auf abschließen steht
  // und der Anwender den Auftrag abschließt.
  // Muss das Datum für geliefert hier gesetzt werden!
  if (!isOrderStatusFinished() && m_orderStatus->currentOrderStatus()) {
    _set.append("o_delivered=CURRENT_TIMESTAMP");
    _changes++;
  }

  // Artikel Bestelliste aktualisieren
  const QString _articleSql = getOrderSqlArticleQuery();
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
#ifdef ANTIQUA_DEVELOPMENT
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
      QString articles_sql = getOrderSqlArticleQuery();
      if (articles_sql.isEmpty()) {
        pushStatusMessage(tr("No SQL Articles exist!"));
        return;
      }
      if (sendSqlQuery(articles_sql)) {
        // Bestehende Artikelliste neu einlesen!
        m_ordersTable->addArticles(queryArticles(oid));
        pushStatusMessage(tr("Save Articles success!"));
      }
    }
  }
}

/**
 * Generiert eine neue Lieferscheinnummer.
 * Mit dem Datum und der Auftragsnummer "o_invoice_id" wird die
 * Lieferscheinnummer erstellt.
 * Das Format ist "{Year}{DayOfYear}{InvoiceNr}"
 */
const QString OrdersEditor::createDeliveryNumber(const QDate d, qint64 id) {
  // DeliveryNumber
  QString _dn(QString::number(d.year()));
  _dn.append(AntiquaCRM::AUtil::zerofill(d.dayOfYear(), 3));
  _dn.append(AntiquaCRM::AUtil::zerofill(id, 3));
  return _dn;
}

/**
 * Weil die entgegenname von o_delivery_service und o_delivery_package asynchron
 * verläuft, muss o_delivery_package explizit noch mal aufgerufen werden!
 * Ansonsten würde wegen Signal "sendSelectedService" das Lieferpaket auf null
 * springen.
 */
void OrdersEditor::setDeliveryPackage() {
  // force o_delivery_service
  QSqlField _field = m_tableData->getProperties("o_delivery_service");
  setDataField(_field, m_tableData->getValue("o_delivery_service"));
  // force o_delivery_package
  _field = m_tableData->getProperties("o_delivery_package");
  if (_field.name().isEmpty()) {
    qWarning("Can't fetch o_delivery_package properties!");
    return;
  }
  setDataField(_field, m_tableData->getValue("o_delivery_package"));
}

void OrdersEditor::setOrderPaymentNumbers(qint64 oid) {
  if (oid < 1) {
    qWarning("Missing Id for Order Payment.");
    return;
  }

  // Siehe PgSQL::new_invoice_id()
  QString _sql("SELECT o_invoice_id,o_since FROM inventory_orders WHERE o_id=");
  _sql.append(QString::number(oid) + ";");
  QSqlQuery _query = m_sql->query(_sql);
  if (_query.size() == 1) {
    _query.next();
    qint64 _iid = _query.value("o_invoice_id").toLongLong();
    if (_iid < 1) {
      pushStatusMessage(tr("Missing invoice id, aborted."));
      return;
    }
    const QDate _date = _query.value("o_since").toDate();
    // Rechnungs Nummer
    m_tableData->setValue("o_invoice_id", _iid);
    setDataField(m_tableData->getProperties("o_invoice_id"), _iid);
    // Lieferschein Nummer
    const QString _did = createDeliveryNumber(_date, _iid);
    m_tableData->setValue("o_delivery", _did);
    setDataField(m_tableData->getProperties("o_delivery"), _did);
  }
}

const OrdersEditor::Idset OrdersEditor::identities() {
  Idset t_ids;
  qint64 _oid = getSerialID("o_id");
  qint64 _cid = getSerialID("o_customer_id");
  qint64 _iid = getSerialID("o_invoice_id");
  QString _did = getDataValue("o_delivery").toString();
  if (_oid > 1 && _cid > 1 && _iid > 1) {
    t_ids.isValid = true;
    t_ids.or_id = _oid;
    t_ids.cu_id = _cid;
    t_ids.in_id = _iid;
    t_ids.de_id = _did;
    return t_ids;
  }

  const QString _msg =
      tr("<b>Operation execution rejected!</b><p>Missing required Order-, "
         "Invoice- or Customer Number.</p><p>Please save your current Order "
         "first and make sure that all required fields available.</p>");
  openNoticeMessage(_msg);

  t_ids.isValid = false;
  return t_ids;
}

const QJsonObject OrdersEditor::createDialogData(qint64 oid) const {
  QJsonObject _jso;
  if (oid < 1)
    return _jso;

  QString _sql("SELECT * FROM inventory_orders WHERE ");
  _sql.append("o_id=" + QString::number(oid) + " LIMIT 1;");

  QSqlQuery _query = m_sql->query(_sql);
  if (_query.size() == 1) {
    _query.next();
    const QSqlRecord _record(_query.record());
    for (int i = 0; i < _record.count(); i++) {
      const QSqlField _field = _record.field(i);
      QVariant _buffer = _query.value(_field.name());
      QVariant _value(_field.metaType(), &_buffer);
      _jso.insert(_field.name(), _value.toJsonValue());
      _buffer.clear();
    }
    _query.clear();
  }
#ifdef ANTIQUA_DEVELOPMENT
  qDebug() << Q_FUNC_INFO << _jso;
#endif
  return _jso;
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

qint32 OrdersEditor::getSalesTaxType(int type) const {
  switch (static_cast<AntiquaCRM::ArticleType>(type)) {
    case (AntiquaCRM::ArticleType::BOOK):
      return 1; // VAT reduced

    default:
      return 0; // VAT normal
  };
}

const QList<AntiquaCRM::OrderArticleItems> OrdersEditor::queryArticles(qint64 oid) {
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

AntiquaCRM::ArticleOrderItem OrdersEditor::addArticleItem(const QString& key,
                                                          const QVariant& value) const {
  return AntiquaCRM::AProviderOrder::createItem(key, value);
}

AntiquaCRM::ArticleOrderItem OrdersEditor::addArticleItem(const QSqlField& field) const {
  return AntiquaCRM::AProviderOrder::createItem(field);
}

bool OrdersEditor::addOrderTableArticle(qint64 aid) {
  if (aid < 1)
    return false;

  // Doppelte Einträge verhindern.
  if (m_ordersTable->articleExists(aid)) {
    pushStatusMessage(tr("Article: %1 already Exists!").arg(aid));
    return false;
  }

  // Auftragsnummer und Kundennummer sind erforderlich!
  qint64 _oid = getSerialID("o_id");
  qint64 _cid = getSerialID("o_customer_id");
  if (_oid < 1 || _cid < 1) {
    pushStatusMessage(tr("Missing required Identities!"));
    return false;
  }

  /*
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
        QSqlField _field = _record.field(i);
        // ??? _field.setValue(_q.value(_field.name()));
        _items.append(addArticleItem(_field));
      }
      if (_items.size() > 0) {
        m_ordersTable->addArticle(_items);
        m_ordersTable->setWindowModified(true);
        return true;
      }
    }
  }
  pushStatusMessage(tr("Article: %1 not found or no stock!").arg(aid));
  return false;
}

const QString OrdersEditor::getOrderSqlArticleQuery() {
  if (!identities().isValid)
    return QString();

  QStringList queries = m_ordersTable->getSqlQuery();
  if (queries.size() > 0)
    return queries.join("\n");

  return QString();
}

/**
 * Ermittelt mit „o_order_status“ und „o_payment_status“ den
 * aktuellen Auftragsstatus. Wurde der Auftrag geliefert und Bezahlt,
 * gilt er als abgeschlossen.
 */
bool OrdersEditor::isOrderStatusFinished() {
  int i_os = m_tableData->getValue("o_order_status").toInt();
  AntiquaCRM::OrderStatus os_t = static_cast<AntiquaCRM::OrderStatus>(i_os);
  // AntiquaCRM::OrderPayment
  int i_ps = m_tableData->getValue("o_payment_status").toInt();
  AntiquaCRM::OrderPayment ps_t = static_cast<AntiquaCRM::OrderPayment>(i_ps);
  return (os_t == AntiquaCRM::OrderStatus::DELIVERED && ps_t == AntiquaCRM::OrderPayment::PAYED);
}

qint64 OrdersEditor::findCustomer(const QJsonObject& obj, qint64 cid) {
  // Zeiger auf Datenfelder der json Struktur
  const QStringList _qfl({"c_firstname", "c_lastname", "c_postalcode", "c_email_0"});

  // Abfrageklausel erstellen („AND“ Query Clause)
  QStringList _qcl;
  foreach (QString _f, _qfl) {
    const QString _v = obj.value(_f).toString();
    if (_v.isEmpty())
      continue;

    _qcl << _f + "='" + _v + "'";
  }

  // Erstelle SQL anfrage
  QString _sql("SELECT c_id FROM customers WHERE (");
  if (cid > 1) {
    _sql.append("c_id=" + QString::number(cid));
  } else {
    _sql.append(_qcl.join(" AND "));
    _sql.append(") OR (c_provider_import='");
    _sql.append(obj.value("c_provider_import").toString());
    _sql.append("'");
  }
  _sql.append(") ORDER BY c_id;");

  // aufräumen
  _qcl.clear();

  // Starte abfrage
  QSqlQuery _q = m_sql->query(_sql);
  if (_q.size() > 0) {
    QList<qint64> _list;
    while (_q.next()) {
      _list << _q.value("c_id").toInt();
    }
    if (_list.size() > 1)
      qWarning("Warning: Found more then one Customer, using first entry!");

    return _list.first();
  }
  return -1;
}

bool OrdersEditor::prepareCreateEntry() {
  setInputFields();
  setResetModified(inputFields);
  setEnabled(true);
  return true;
}

void OrdersEditor::setDefaultValues() {
  // Order Status
  m_tableData->setValue("o_order_status", AntiquaCRM::OrderStatus::STARTED);
  setDataField(m_tableData->getProperties("o_order_status"), AntiquaCRM::OrderStatus::STARTED);

  // Payment Status
  m_tableData->setValue("o_payment_status", AntiquaCRM::OrderPayment::NOTPAID);
  setDataField(m_tableData->getProperties("o_payment_status"), AntiquaCRM::OrderPayment::NOTPAID);

  // VAT Settings
  m_tableData->setValue("o_vat_levels", AntiquaCRM::SalesTax::TAX_INCL);
  setDataField(m_tableData->getProperties("o_vat_levels"), AntiquaCRM::SalesTax::TAX_INCL);

  // Delivery Country
  const QString _co = QLocale::system().bcp47Name().toUpper();
  m_tableData->setValue("o_vat_country", _co);
  setDataField(m_tableData->getProperties("o_vat_country"), _co);

  // Delivery Service add price
  m_tableData->setValue("o_delivery_add_price", false);
  setDataField(m_tableData->getProperties("o_delivery_add_price"), false);

  // Payment Method
  setDataField(m_tableData->getProperties("o_payment_method"),
               AntiquaCRM::INVOICE_PREPAYMENT_RESERVED);
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
  // qDebug() << Q_FUNC_INFO << "FORCE" << force;
  if (force) // Wenn auf Abbrechen geklickt wurde!
    setWindowModified(false);

  m_ordersTable->clearContents(); // Artikel Tabelle leeren!
  m_orderStatus->stepOut();       // Signale wieder sperren
  setResetInputFields();
  emit sendLeaveEditor(); // Zurück zur Hauptsansicht
}

void OrdersEditor::createMailMessage(const QString& caller) {
  OrdersEditor::Idset _ids = identities();
  if (!_ids.isValid)
    return;

  QJsonObject _obj;
  _obj.insert("tb_tab", "ORDERS");
  _obj.insert("tb_caller", caller);
  _obj.insert("CRM_CUSTOMER_ID", _ids.cu_id);
  _obj.insert("CRM_INVOICE_ID", _ids.in_id);
  _obj.insert("CRM_ORDER_ID", _ids.or_id);

  AntiquaCRM::MailDialog* d = new AntiquaCRM::MailDialog(this);
  if (d->exec(_obj) == QDialog::Accepted) {
    pushStatusMessage(tr("Mail Message done!"));
  }
  d->deleteLater();
}

void OrdersEditor::createPrintDeliveryNote() {
  OrdersEditor::Idset _ids = identities();
  if (!_ids.isValid)
    return;

  AntiquaCRM::PrintDeliveryNote* d = new AntiquaCRM::PrintDeliveryNote(this);
  if (d->exec(createDialogData(_ids.or_id)) == QDialog::Accepted) {
    pushStatusMessage(tr("Delivery note printed."));
  }
  d->deleteLater();
}

void OrdersEditor::createPrintInvoice() {
  OrdersEditor::Idset _ids = identities();
  if (!_ids.isValid)
    return;

  QJsonObject _obj = createDialogData(_ids.or_id);
  if (_obj.isEmpty())
    return;

  if (_obj.value("o_delivery_add_price").toBool()) {
    _obj.insert("package_price", m_costSettings->o_delivery_package->getPackagePrice());
  } else {
    _obj.insert("package_price", 0.00);
  }

  AntiquaCRM::PrintInvoice* d = new AntiquaCRM::PrintInvoice(this);
  if (d->exec(_obj) == QDialog::Accepted) {
    pushStatusMessage(tr("Invoice printed."));
  }
  d->deleteLater();
}

void OrdersEditor::createPrintPaymentReminder() {
  OrdersEditor::Idset _ids = identities();
  if (!_ids.isValid)
    return;

  QJsonObject _obj = createDialogData(_ids.or_id);
  if (_obj.isEmpty())
    return;

  if (_obj.value("o_delivery_add_price").toBool()) {
    _obj.insert("package_price", m_costSettings->o_delivery_package->getPackagePrice());
  } else {
    _obj.insert("package_price", 0.00);
  }

  AntiquaCRM::PrintReminder* d = new AntiquaCRM::PrintReminder(this);
  if (d->exec(_obj) == QDialog::Accepted) {
    pushStatusMessage(tr("Reminder printed."));
  }
  d->deleteLater();
}

void OrdersEditor::createPrintRefundInvoice() {
  if (m_orderStatus->getOrderPayment() != AntiquaCRM::OrderPayment::RETURN) {
    pushStatusMessage(tr("This order is not a refund!"));
    return;
  }

  OrdersEditor::Idset _ids = identities();
  if (!_ids.isValid)
    return;

  QJsonObject _obj = createDialogData(_ids.or_id);
  if (_obj.isEmpty())
    return;

  if (_obj.value("o_delivery_add_price").toBool()) {
    _obj.insert("package_price", m_costSettings->o_delivery_package->getPackagePrice());
  } else {
    _obj.insert("package_price", 0.00);
  }

  AntiquaCRM::PrintRefund* d = new AntiquaCRM::PrintRefund(this);
  if (d->exec(_obj) == QDialog::Accepted) {
    pushStatusMessage(tr("Refund printed."));
  }
  d->deleteLater();
}

void OrdersEditor::createPrintAdmonition() {
  if (m_orderStatus->getOrderPayment() == AntiquaCRM::OrderPayment::RETURN) {
    pushStatusMessage(tr("This order is a refunding!"));
    return;
  }

  OrdersEditor::Idset _ids = identities();
  if (!_ids.isValid)
    return;

  QJsonObject _obj = createDialogData(_ids.or_id);
  if (_obj.isEmpty())
    return;

  if (_obj.value("o_delivery_add_price").toBool()) {
    _obj.insert("package_price", m_costSettings->o_delivery_package->getPackagePrice());
  } else {
    _obj.insert("package_price", 0.00);
  }

  AntiquaCRM::PrintAdmonition* d = new AntiquaCRM::PrintAdmonition(this);
  if (d->exec(_obj) == QDialog::Accepted) {
    pushStatusMessage(tr("Admonition printed."));
  }
  d->deleteLater();
}

/**
 * Es kann hier keine Rückerstattung erfolgen!
 */
void OrdersEditor::hintsAboutRefund(AntiquaCRM::OrderPayment stat) {
  if (stat != AntiquaCRM::OrderPayment::RETURN)
    return; // Nur bei Rückerstattung ausführen!

  if (m_tableData->getValue("o_payment_status").toInt() == stat)
    return; // Der Auftrag wurde bereits rückerstattet!

  openNoticeMessage(
      tr("<b>You cannot issue a refund in this Interface!</b>"
         "<ul><li>The Order status must set to Delivered.</li>"
         "<li>The Payment status must have paid.</li></ul>"
         "<p>If this Order hasn't paid, use Order status Canceled.</p>"
         "<p>To create a refund, return to the main view, right-click on the "
         "completed order you want to refund, and select 'Create refund' "
         "option.</p>"));

  m_orderStatus->o_payment_status->setReject();
  if (m_orderStatus->isWindowModified())
    m_orderStatus->setWindowModified(false);
}

void OrdersEditor::openSearchInsertArticle() {
  if (!identities().isValid)
    return;

  if (m_orderStatus->isProtectable()) {
    QString _info(
        tr("<b>You cannot add a new item to a refund!</b>"
           "<p>A better choice is to create a new order.</p>"));
    openNoticeMessage(_info);
    return;
  }

  OrderArticleDialog* d = new OrderArticleDialog(this);
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

bool OrdersEditor::addArticle(qint64 aid) {
  if (aid < 1)
    return false;

  // Ist eine gespeicherte Bestellung geöffnet?
  if (!identities().isValid)
    return false;

  // Artikel in Bestellungen hinzufügen
  return addOrderTableArticle(aid);
}

bool OrdersEditor::openEditEntry(qint64 oid) {
  bool _retval = false;
  if (oid < 1)
    return _retval;

  // Eingabefelder zurücksetzen
  setInputFields();
  setResetModified(inputFields);

  // BEGIN:DatensatzImport
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
      if (_field.name().contains("o_delivery_package"))
        continue;

      setDataField(_field, _query.value(_field.name()));
    }
    // Reihenfolge einhalten "o_delivery_service" dann "o_delivery_package"!
    setDeliveryPackage();

    // Bestehende Artikel Einkäufe mit orderId einlesen!
    m_ordersTable->addArticles(queryArticles(oid));

    setResetModified(customInput);
    _retval = true;
  }
  // END:DatensatzImport

  // Eingabefelder befüllen
  if (_retval) {
    importSqlResult();
    m_ordersTable->setWindowModified(false);
    setResetModified(inputFields);
    setEnabled(true);
  }
  //
  m_orderStatus->stepIn();
  if (m_orderStatus->isProtectable()) {
    qInfo("Protected order refunding.");
    m_ordersTable->setProtected(true);
  }
  return _retval;
}

/*
 * Weil der SLOT mit einem Signal von OrderStatusActionFrame ausgelöst wird.
 * Müssen einige Abfragen durchgeführt werden damit dder RefundingDialog nicht
 * beim öffnen des Editors ausgelöst wird.
 *
 * Es kann an diesem Punkt nur der Status abgefragt werden.
 * Die Methode OrderStatusActionFrame::initProtection() kann an dieser
 * stelle nicht aufgerufen werden weil, das setzen von OrderStatus und
 * PaymentStatus beim laden des Auftrages asyncron erfolgen!
 */
bool OrdersEditor::createOrderRefund(qint64 oid) {
  if (!openEditEntry(oid))
    return false;

  if (!m_orderStatus->currentOrderStatus()) {
    openNoticeMessage(
        tr("<b>You cannot issue a refund for this Order!</b>"
           "<ul><li>The Order status must have Delivered.</li>"
           "<li>The Payment status must have Paid.</li></ul>"
           "<p>If this Order hasn't paid, open it and change "
           "status to Canceled.</p>"));

    return false;
  }

  // Restore for a new Entry!
  foreach (QString _n, QStringList({"o_id", "o_invoice_id"})) {
    setDataField(m_tableData->getProperties(_n), 0);
  }
  // Reihenfolge einhalten "o_delivery_service" dann "o_delivery_package"!
  setDeliveryPackage();

  m_tableData->setValue("o_payment_status", AntiquaCRM::OrderPayment::RETURN);
  setDataField(m_tableData->getProperties("o_payment_status"), AntiquaCRM::OrderPayment::RETURN);

  RefundingDialog* d = new RefundingDialog(oid, this);
  if (d->exec() == QDialog::Rejected) {
    pushStatusMessage(tr("Refunding dialog aborted."));
    return false;
  } else {
    // An dieser Stelle muss die Bestelltabelle neu geladen werden!
    // Damit der Klient durch ein Speichern nicht die Daten verfälscht.
    // Note: Das Refunding erfolgt asyncron zum aktuellen Datensatz!
    QList<AntiquaCRM::OrderArticleItems> _articles = d->refundArticles();
    if (_articles.size() > 0)
      m_ordersTable->addArticles(_articles);

    m_ordersTable->setWindowModified(true);
    setWindowModified(true);
  }
  d->deleteLater();
  pushStatusMessage(tr("Refund created."));
  return true;
}

bool OrdersEditor::createNewEntry() {
  qInfo("Create new Order without Customer Id will rejected.");
  return false;
}

bool OrdersEditor::createNewOrder(qint64 cid) {
  if (cid < 1)
    return false;

  prepareCreateEntry();
  setDefaultValues();

  // Nehme relevante Kundendaten
  AntiquaCRM::ASqlFiles _tpl("query_customer_new_order");
  if (_tpl.openTemplate()) {
    _tpl.setWhereClause("c_id=" + QString::number(cid));
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
  m_orderStatus->stepIn();
  return false;
}

bool OrdersEditor::changeOrderToCustomer(const QJsonObject& object) {
  qint64 _cid = object.value("VALUE").toInt();
  if (_cid == getSerialID("o_customer_id"))
    return false; // nothing todo

  // 1) query customer data
  AntiquaCRM::ASqlFiles _tpl("query_customer_new_order");
  if (_tpl.openTemplate()) {
    _tpl.setWhereClause("c_id=" + QString::number(_cid));
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
    }
  }

  // 2) check Identities
  setDataField(m_tableData->getProperties("o_customer_id"), _cid);
  if (getSerialID("o_customer_id") != _cid)
    return false;

  qint64 _oid = getSerialID("o_id");
  if (_oid < 1 || _cid < 1) {
    pushStatusMessage(tr("Missing required Identities!"));
    return false;
  }

  // 3) rewrite customer Id in articles
  QList<AntiquaCRM::OrderArticleItems> _list;
  QListIterator<AntiquaCRM::OrderArticleItems> it(queryArticles(_oid));
  while (it.hasNext()) {
    AntiquaCRM::OrderArticleItems _items;
    foreach (AntiquaCRM::ArticleOrderItem i, it.next()) {
      if (i.key == "a_customer_id") {
        _items.append(addArticleItem(i.key, _cid));
      } else {
        _items.append(addArticleItem(i.key, i.value));
      }
    }
    _list.append(_items);
  }

  // 4) update dataset
  if (_list.size() > 0) {
    QString _sql("UPDATE inventory_orders SET ");
    _sql.append("o_customer_id=" + QString::number(_cid));
    _sql.append(",o_modified=CURRENT_TIMESTAMP WHERE o_id=");
    _sql.append(QString::number(_oid));
    _sql.append(";");
    if (sendSqlQuery(_sql)) {
      m_ordersTable->clearContents();
      m_ordersTable->addArticles(_list);
      m_ordersTable->setWindowModified(true);
      setSaveData();
      return true;
    }
  }
  return false;
}

bool OrdersEditor::createCustomEntry(const QJsonObject& object) {
  // Zuerst die Aktion prüfen
  const QString _action = object.value("ACTION").toString();
  if (_action.isEmpty())
    return false;

  // Ohne Dienstleister Bestellnummer keine weitere Ausführung.
  const QString _provider_id = object.value("PRORDER").toString();
  if (_provider_id.isEmpty())
    return false;

  // Lese Artikelliste ein ...
  const QString _articles = object.value("ARTICLES").toString();
  QStringList _article_list = _articles.split(",");
  if (_article_list.size() < 1) {
    qWarning("ARTICLES list not exists or empty!");
    return false;
  }

  // Existierenden Import von „autiquacmd“ suchen ...
  AntiquaCRM::ASqlFiles _tpl("query_provider_order_exists");
  if (!_tpl.openTemplate())
    return false;

  _tpl.setWhereClause("pr_order='" + _provider_id + "'");
  QSqlQuery _q = m_sql->query(_tpl.getQueryContent());
  if (_q.size() != 1) {
    openNoticeMessage(tr("No Provider orders data found!"));
    return false;
  }

  // Auftrag gefunden und einlesen
  _q.next();
  const QByteArray _data = _q.value("pr_order_data").toByteArray();
  QJsonObject _jobj = QJsonDocument::fromJson(_data).object();
  if (_jobj.isEmpty())
    return false;

  // Dienstleister name und id in Variablen speichern.
  const QString o_provider_name = _q.value("pr_name").toString();
  const QString o_provider_order_id = _q.value("pr_order").toString();
  // Ermittelte Kundennummer auslesen ...
  qint64 _customer_id = _q.value("pr_customer_id").toInt();
  if (_customer_id < 1) {
    qWarning("Empty Customer Id");
  }

  // Eingabefelder setzen
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
        m_orderStatus->setOrderPayment(AntiquaCRM::OrderPayment::PAYED);
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

      // Add Missing Fields
      // NOTE: Das Feld 'a_tax' ist noch nicht in „antiquacmd“ eingebunden!
      // o_media_type | a_type
      if (!article.contains("a_tax") && article.contains("a_type")) {
        article.insert("a_tax", getSalesTaxType(article.value("a_type").toInt()));
      }
      if (!article.contains("a_refunds_cost")) {
        article.insert("a_refunds_cost", 0.00);
      }
      if (!article.contains("a_modified")) {
        article.insert("a_modified", m_sql->getDateTime());
      }

      qint64 _article_id = article.value("a_article_id").toInt();
      // Siehe QJsonValue::article_numbers
      if (!_article_list.contains(QString::number(_article_id)))
        continue;

      // Ist erst mal 0
      items.append(_prorder.createItem("a_order_id", 0));
      // Muss hier eingefügt werden!
      if (_customer_id > 0)
        items.append(_prorder.createItem("a_customer_id", _customer_id));

      foreach (QString key, article.keys()) {
        items.append(_prorder.createItem(key, article.value(key).toVariant()));
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
  _prorder.setValue("o_delivery_service", getDataValue("o_delivery_service").toInt());
  _prorder.setValue("o_delivery_package", getDataValue("o_delivery_package").toInt());
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

  // 5) Signale initialisieren
  m_orderStatus->stepIn();

  // 6) Erst nach einfügen reset
  setResetModified(inputFields);
  setResetModified(customInput);

  return true;
}
