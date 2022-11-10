// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "orderseditor.h"
#include "deliverservice.h"
#include "orderscostsettings.h"
#include "orderscustomerinfo.h"
#include "ordersitemlist.h"

#include <QLayout>
#include <QSizePolicy>

OrdersEditor::OrdersEditor(QWidget *parent)
    : InventoryEditor{"^[ocd]_[a-z_]+\\b$", parent} {
  setObjectName("orders_editor");
  setWindowTitle(tr("Edit Orders"));

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
  // Zahlungsstatus
  o_payment_status = new BoolBox(this);
  o_payment_status->setObjectName("o_payment_status");
  o_payment_status->setInfo(tr("Payment status"));
  o_payment_status->setToolTip(tr("Current Payment status"));
  o_payment_status->setValue(0);
  row0->addWidget(o_payment_status);
  // Auftragsstatus
  o_order_status = new OrderStatusSelecter(this);
  o_order_status->setObjectName("o_order_status");
  o_order_status->setInfo(tr("Current order status"));
  o_order_status->setValue(1);
  row0->addWidget(o_order_status);
  mainLayout->addLayout(row0);
  // END:Row0

  // BEGIN:Row1
  QHBoxLayout *row1 = new QHBoxLayout();
  row1->setContentsMargins(5, 2, 5, 2);
  m_customerInfo = new OrdersCustomerInfo(this);
  row1->addWidget(m_customerInfo);
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
  setLayout(mainLayout);
}

void OrdersEditor::setInputFields() {
  // Bei UPDATE/INSERT Ignorieren
  ignoreFields << "o_since";
  ignoreFields << "o_modified";
  ignoreFields << "o_delivered";
  // @deprecated o_provider_order
  ignoreFields << "o_provider_order";

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

  if (!ignoreFields.contains(key))
    qDebug() << "Unknown:" << key << "|" << value << "|" << required;

  return false;
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

  // m_actionBar->setRestoreable(m_tableData->isValid());
  setResetModified(inputFields);
}

bool OrdersEditor::sendSqlQuery(const QString &query) {
  qDebug() << Q_FUNC_INFO << query;
  return true;

  QSqlQuery q = m_sql->query(query);
  if (q.lastError().type() != QSqlError::NoError) {
    qDebug() << Q_FUNC_INFO << query << m_sql->lastError();
    return false;
  }

  if (q.next()) {
    if (!q.isNull("o_id")) {
      QSqlField field = m_tableData->getProperties("o_id");
      setDataField(field, q.value("o_id"));
    }
  }

  openSuccessMessage(tr("Order saved successfully!"));
  setResetModified(inputFields);
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
    if (ignoreFields.contains(objName))
      continue;

    // qDebug() << objName << cur->isRequired() << cur->isValid() <<
    // cur->value();
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
  // TODO
}

void OrdersEditor::createSqlInsert() {
  // TODO
}

qint64 OrdersEditor::searchCustomer(const QJsonObject &obj) {
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
  sql.append(clause.join(" AND "));
  sql.append(") OR (c_provider_import='");
  sql.append(obj.value("c_provider_import").toString());
  sql.append("') ORDER BY c_id;");
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
    qDebug() << Q_FUNC_INFO << sql << m_sql->lastError();
    return -1;
  }
  return -1;
}

void OrdersEditor::setSaveData() {
  if (o_id->value().toInt() < 1) {
    createSqlInsert();
    return;
  }
  createSqlUpdate();
}

void OrdersEditor::setCheckLeaveEditor() {
  if (checkIsModified()) {
    QStringList info(tr("Unsaved Changes"));
    info << tr("Don't leave this page before save your changes!");
    openNoticeMessage(info.join("\n"));
    return;
  }
  setFinalLeaveEditor();
}

void OrdersEditor::setFinalLeaveEditor() {
  setResetInputFields();
  // m_actionBar->setRestoreable(false); /**< ResetButton off */
  emit sendLeaveEditor(); /**< Zurück zur Hauptsansicht */
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

  AntiquaCRM::ASqlFiles file("query_order_by_oid");
  if (!file.openTemplate())
    return false;

  file.setWhereClause("o_id=" + o_id);

  QSqlQuery q = m_sql->query(file.getQueryContent());
  if (q.size() != 0) {
    QSqlRecord r = q.record();
    while (q.next()) {
      // Standard Felder
      foreach (QString key, inputFields) {
        m_tableData->setValue(key, q.value(r.indexOf(key)));
      }
      // Kunden Daten
      foreach (QString key, customInput) {
        QSqlField f = r.field(key);
        setDataField(f, q.value(f.name()));
      }
      // Artikel
    }
    status = true;
  } else {
    qDebug() << Q_FUNC_INFO << m_sql->lastError();
    status = false;
  }

  if (status) {
    importSqlResult();
    setEnabled(true);
  }

  return status;
}

bool OrdersEditor::createNewEntry() {
  setInputFields();
  setResetModified(inputFields);
  setEnabled(true);
  return true;
}

bool OrdersEditor::createNewProviderOrder(const QString &providerId) {
  if (providerId.isEmpty())
    return false;

  QString sql("SELECT pr_name,pr_order,pr_order_data");
  sql.append(" FROM provider_order_history WHERE");
  sql.append(" pr_order='" + providerId + "' ORDER BY pr_order;");
  QSqlQuery q = m_sql->query(sql);
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
  QByteArray data = q.value("pr_order_data").toByteArray();
  QJsonObject obj = QJsonDocument::fromJson(data).object();
  if (obj.isEmpty())
    return false;

  setInputFields();
  setResetModified(inputFields);
  setEnabled(true);

  AntiquaCRM::AProviderOrder prOrder(o_provider_name, o_provider_order_id);
  // Kunden Daten
  if (obj.contains("customer")) {
    QJsonObject customer = obj.value("customer").toObject();
    // NOTE: Wir benötigen bei einem Import eine gültige Kundennummer!
    qint64 c_id = searchCustomer(customer);
    if (!prOrder.setValue("o_customer_id", c_id)) {
      qWarning("OrderEditor: Customer not found or set!");
      return false;
    }

    foreach (QString key, customer.keys()) {
      QVariant val = customer.value(key).toVariant();
      if (!prOrder.setValue(key, val))
        qWarning("Customer value '%s' not set!", qPrintable(key));
    }
  }

  // Standard Felder
  if (obj.contains("orderinfo")) {
    QJsonObject orderinfo = obj.value("orderinfo").toObject();
    foreach (QString key, orderinfo.keys()) {
      QVariant val = orderinfo.value(key).toVariant();
      if (ignoreFields.contains(key))
        continue;

      if (!prOrder.setValue(key, val)) {
        qWarning("Order value '%s' not set!", qPrintable(key));
        // qDebug() << key << val;
      }
    }
  }

  // Artikel
  if (obj.contains("articles")) {
    QJsonArray orders = obj.value("articles").toArray();
    for (int i = 0; i < orders.size(); i++) {
      QList<AntiquaCRM::ArticleOrderItem> items;
      QJsonObject article = orders[i].toObject();
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
  // TODO
  prOrder.setValue("o_delivery_service", 0);
  prOrder.setValue("o_delivery_send_id", "");
  prOrder.setValue("o_delivery", "");
  prOrder.setValue("o_delivery_package", 22);
  prOrder.setValue("o_vat_included", true);
  prOrder.setValue("o_vat_levels", 7);
  prOrder.setValue("o_delivery_add_price", false);

  // Ignore it in "New Entries"
  QStringList ignored({"o_id", "o_invoice_id"});

  // 1) Standard Felder einfügen
  foreach (QString key, inputFields) {
    if (ignoreFields.contains(key))
      continue;

    if (ignored.contains(key))
      m_tableData->setValue(key, 0);
    else
      m_tableData->setValue(key, prOrder.getValue(key));
  }

  // 2) Kunden Daten
  QSqlRecord r = AntiquaCRM::ASqlDataQuery("customers").record();
  foreach (QString key, customInput) {
    QSqlField f = r.field(key);
    setDataField(f, prOrder.getValue(key));
  }

  // 3) Artikel Importieren
  m_ordersList->importPayments(prOrder.orders());

  importSqlResult();
  o_payment_status->setFocus();
  return true;
}
