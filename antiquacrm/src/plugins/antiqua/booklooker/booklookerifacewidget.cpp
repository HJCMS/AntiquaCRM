// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklookerifacewidget.h"
#include "booklookerconfig.h"
#include "booklookerrequester.h"

#include <QDebug>
#include <QHash>

BooklookerIfaceWidget::BooklookerIfaceWidget(const QString &widgetId,
                                             QWidget *parent)
    : Antiqua::InterfaceWidget{widgetId, parent} {
  m_requester = new BooklookerRequester(this);
  m_requester->setObjectName("requester_" + objectName());
  connect(m_requester, SIGNAL(errorMessage(int, const QString &)), this,
          SIGNAL(errorResponse(int, const QString &)));

  connect(m_requester, SIGNAL(response(const QJsonDocument &)), this,
          SLOT(setContent(const QJsonDocument &)));
}

const QJsonDocument
BooklookerIfaceWidget::customerRequest(const QJsonObject &object) {
  Q_UNUSED(object);
  QJsonObject customer;
  customer.insert("provider", CONFIG_PROVIDER);
  customer.insert("orderid", objectName().trimmed());
  customer.insert("type", "customer_request");
  foreach (QString f, m_order->customerSearchFields()) {
    customer.insert(f, p_customer.value(f));
  }
  return QJsonDocument(customer);
}

void BooklookerIfaceWidget::createOrderDataSet(const QJsonArray &array) {
  if (array.size() > 0) {
    // Bestellartikel einfügen
    m_order->setTableCount(0);
    QJsonArray items(array);
    QJsonArray::iterator at;
    for (at = items.begin(); at != items.end(); ++at) {
      QJsonObject article = (*at).toObject();
      int row = m_order->getTableCount();
      m_order->setTableCount((m_order->getTableCount() + 1));
      qint64 order_id = article.value("orderItemId").toInt();
      m_order->setTableData(row, 0, QString::number(order_id));
      m_order->setTableData(row, 1, article.value("orderNo"));
      qint64 mount = article.value("amount").toInt();
      m_order->setTableData(row, 2, QString::number(mount));
      m_order->setTableData(row, 3, article.value("singlePrice"));
      m_order->setTableData(row, 4, article.value("orderTitle"));
    }
  }
}

void BooklookerIfaceWidget::parseAddressBody(const QString &key,
                                             const QJsonObject &obj) {
  QStringList addressBody;
  // Käufer*in anzeigefeld
  QString person;
  QString c_gender = obj.value("title").toString().trimmed();
  if (!c_gender.isEmpty()) {
    person.append(c_gender);
    person.append(" ");
    m_order->setValue("c_gender", c_gender);
    p_customer.insert("c_gender", c_gender);
  }

  if (obj.contains("company")) {
    QString company = obj.value("company").toString().trimmed();
    p_customer.insert("c_company_name", company);
    m_order->setValue("c_company_name", company);
  }

  QString c_firstname = obj.value("firstName").toString().trimmed();
  m_order->setValue("c_firstname", c_firstname);
  p_customer.insert("c_firstname", c_firstname);

  person.append(c_firstname);
  person.append(" ");
  QString c_lastname = obj.value("name").toString().trimmed();
  m_order->setValue("c_lastname", c_lastname);
  p_customer.insert("c_lastname", c_lastname);
  person.append(c_lastname);
  addressBody << QString(c_firstname + " " + c_lastname);
  m_order->setValue("person", person);

  QString c_street = obj.value("street").toString().trimmed();
  m_order->setValue("c_street", c_street);
  p_customer.insert("c_street", c_street);
  addressBody << c_street;

  QString c_postalcode = obj.value("zip").toString().trimmed();
  m_order->setValue("c_postalcode", c_postalcode);
  p_customer.insert("c_postalcode", c_postalcode);

  QString c_location = obj.value("city").toString().trimmed();
  m_order->setValue("c_location", c_location);
  p_customer.insert("c_location", c_location);
  addressBody << QString(c_postalcode + " " + c_location);

  QString c_country = obj.value("country").toString().trimmed();
  if (c_country.isEmpty())
    c_country = tr("Germany");

  m_order->setValue("c_country", c_country);
  p_customer.insert("c_country", c_country);

  // Adresse einfügen
  m_order->setValue(key, addressBody.join("\n"));
  addressBody.clear();

  // Sende SQL Abfrage an Hauptfenster!
  if (key == "c_postal_address") {
    QJsonDocument qDoc = customerRequest(obj);
    if (!qDoc.isEmpty())
      emit checkCustomer(qDoc);
  }
}

void BooklookerIfaceWidget::checkCustomerClicked() {
  QJsonDocument qDoc = customerRequest(QJsonObject());
  if (!qDoc.isEmpty())
    emit checkCustomer(qDoc);
}

void BooklookerIfaceWidget::readCurrentArticleIds() {
  QList<int> ids = m_order->getArticleIDs();
  if (ids.count() > 0)
    emit checkArticleIds(ids);
}

void BooklookerIfaceWidget::createCustomerDocument() {
  if (p_currentDocument.isEmpty()) {
    qDebug() << Q_FUNC_INFO << "Current Json Document is empty!";
    return;
  }

  if (m_order->getCustomerId() > 0) {
    qInfo("CustomerId already exists!");
    return;
  }

  QJsonArray array = p_currentDocument["returnValue"].toArray();
  QJsonObject obj = array[0].toObject();
  QJsonObject customer;
  customer.insert("provider", QJsonValue(CONFIG_PROVIDER));
  customer.insert("type", "customer_create");
  QHashIterator<QString, QString> it(p_customer);
  while (it.hasNext()) {
    it.next();
    customer.insert(it.key(), it.value());
  }

  if (obj.contains("email")) {
    QString mail = obj.value("email").toString().trimmed();
    customer.insert("c_email_0", mail);
    m_order->setValue("c_email_0", mail);
  }

  if (obj.contains("phone")) {
    QString phone = obj.value("phone").toString().trimmed();
    customer.insert("c_phone_0", phone);
    m_order->setPhone("c_phone_0", phone);
  }

  QString pAddress = m_order->getValue("c_postal_address").toString();
  if (!pAddress.isEmpty())
    customer.insert("c_postal_address", pAddress);

  QString dAddress = m_order->getValue("c_shipping_address").toString();
  if (!dAddress.isEmpty())
    customer.insert("c_shipping_address", dAddress);

  emit createCustomer(QJsonDocument(customer));
}

void BooklookerIfaceWidget::setContent(const QJsonDocument &doc) {
  if (QJsonValue(doc["status"]).toString() == "NOK") {
    QString value = QJsonValue(doc["returnValue"]).toString();
    if (value == "INVALID_ORDERID") {
      qDebug() << Q_FUNC_INFO << "INVALID_ORDERID";
    } else if (value == "TOKEN_EXPIRED") {
      qDebug() << Q_FUNC_INFO << "TOKEN_EXPIRED";
      m_requester->authenticationRefresh();
    } else {
      qWarning("Booklooker setContent Error: %s", qPrintable(value));
    }
    return;
  }

#ifdef ANTIQUA_DEVELOPEMENT
  saveSources(doc, getOrderId());
#endif

  // Speichern
  p_currentDocument = doc;

  QJsonArray array = QJsonValue(doc["returnValue"]).toArray();
  for (int i = 0; i < array.size(); i++) {
    if (array[i].isObject()) {
      QJsonObject obj = array[i].toObject();
      QJsonObject::iterator it;
      for (it = obj.begin(); it != obj.end(); ++it) {
        QString f = it.key();
        QString sf = sqlParam(f);
        QJsonValue val = it.value();
        if (f == "invoiceAddress") {
          parseAddressBody("c_postal_address", it->toObject());
          continue;
        } else if (f == "deliveryAddress") {
          parseAddressBody("c_shipping_address", it->toObject());
          continue;
        } else if (f == "orderItems") {
          createOrderDataSet(val.toArray());
          continue;
        }
        // qDebug() << f << sqlParam(f) << val.toVariant();
        if (!sf.isEmpty() && sf.contains("c_")) {
          if ((val.toVariant().type() == QVariant::String) &&
              !p_customer.contains(sf))
            p_customer.insert(sf, val.toVariant().toString());
        }
        m_order->setValue(sf, val.toVariant());
      }
    }
  }
}

void BooklookerIfaceWidget::createOrderRequest(const QString &orderId) {
  m_requester->queryOrder(orderId);
}

void BooklookerIfaceWidget::setCustomerId(int customerId) {
  if (customerId > 0) {
    currentCustomerId = customerId;
    m_order->setCustomerId(customerId);
  }
}

const QMap<QString, QString> BooklookerIfaceWidget::fieldTranslate() const {
  QMap<QString, QString> map;
  // public.customers @{
  map.insert("title", "c_gender");
  map.insert("firstName", "c_firstname");
  map.insert("name", "c_lastname");
  map.insert("street", "c_street");
  map.insert("zip", "c_postalcode");
  map.insert("city", "c_location");
  map.insert("country", "c_country");
  map.insert("phone", "c_phone_0");
  map.insert("email", "c_email_0");
  map.insert("orderId", "o_provider_order_id");
  map.insert("orderDate", "o_since");
  map.insert("company", "c_company_name");
  // map.insert("", "c_company_employer");
  // Konto Inhaber
  map.insert("accountHolder", "c_comments");
  map.insert("accountIban", "c_iban");
  map.insert("accountBic", "c_swift_bic");
  map.insert("ustIdNr", "c_tax_id");
  // @}

  /**
   * Dieses Element wird nur übergeben, wenn die Versandkosten berechnet werden
   * konnten, bspw. bei vorhandener Gewichtsangabe und Nutzung der
   * Gewichts-Staffel-Tabelle zur Portoberechnung, oder bei Nutzung von
   * Pauschalpreisen
   */
  //  map.insert("calculatedShippingCost", "");
  map.insert("comments", "o_delivery_comment");
  //  map.insert("currentProvisionNet", "");
  //  map.insert("originalProvisionNet", "");

  /**
   * Aktuell nur für Zahlungsart PayPal: Falls uns der Zeitpunkt der
   * erfolgreichen Bezahlung vom Zahlungsdienstleister gemeldet wurde, steht
   * hier der Zeitpunkt im Format YYYY-MM-DD HH:MM:SS
   */
  map.insert("paymentConfirmed", "o_payment_confirmed");

  /**
   * Aktuell nur für Zahlungsart PayPal: Die von PayPal übermittelte txn_id der
   * zugehörigen Bezahlung
   */
  map.insert("transactionId", "o_payment_paypal_txn_id");

  /**
   * @brief Die vom Besteller gewählte Zahlungsart.
   * @class OrdersPaymentTypes
   * Folgende Werte sind möglich:
   * @li  1 : Banküberweisung (Vorkasse)
   * @li  2 : Offene Rechnung
   * @li  3 : Lastschrift (Vorkasse)
   * @li  4 : Kreditkarte (Vorkasse)
   * @li  5 : Nachnahme
   * @li  6 : PayPal (Vorkasse)
   * @li  8 : Skrill (Vorkasse)
   * @li  9 : Selbstabholung - Barzahlung
   * @li 10 : Sofortüberweisung
   * @li 11 : Offene Rechnung (Vorkasse vorbehalten)
   */
  map.insert("paymentId", "o_payment_method");

  // public.article_orders @{
  map.insert("mediaType", "i_inventory_type");
  map.insert("orderNo", "a_article_id");
  map.insert("author", "ib_author");
  map.insert("singlePrice", "ib_price");
  map.insert("totalPriceRebated", "a_sell_price");
  map.insert("orderItemId", "o_provider_purchase_id");
  map.insert("amount", "a_count");
  map.insert("orderTitle", "a_title");
  // @}
  return map;
}
