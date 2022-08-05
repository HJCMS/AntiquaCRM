// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklookerifacewidget.h"
#include "booklookerconfig.h"
#include "booklookerremoteactions.h"
#include "booklookerrequester.h"

#include <QDebug>
#include <QHash>
#include <QMessageBox>

BooklookerIfaceWidget::BooklookerIfaceWidget(const QString &orderId,
                                             QWidget *parent)
    : Antiqua::InterfaceWidget{orderId, parent} {
  // Wegen langen Abfragewartezeiten den Zugriff unterdrücken!
  setEnabled(false);

  m_requester = new BooklookerRequester(this);
  m_requester->setObjectName("requester_" + orderId);
  connect(m_requester,
          SIGNAL(errorMessage(Antiqua::ErrorStatus, const QString &)), this,
          SIGNAL(sendErrorResponse(Antiqua::ErrorStatus, const QString &)));

  connect(m_requester, SIGNAL(response(const QJsonDocument &)), this,
          SLOT(setContent(const QJsonDocument &)));
}

void BooklookerIfaceWidget::setCurrentDocument(const QJsonObject &obj) {
  QJsonArray arr;
  arr.append(QJsonValue(obj));
  QJsonObject out;
  out.insert("returnValue", arr);
  out.insert("status", "ok");
  p_currentDocument = QJsonDocument(out);
#ifdef ANTIQUA_DEVELOPEMENT
  saveSources(p_currentDocument, getOrderId());
#endif
}

const QJsonDocument
BooklookerIfaceWidget::customerRequest(const QJsonObject &object) {
  Q_UNUSED(object);
  QJsonObject customer;
  customer.insert("provider", CONFIG_PROVIDER);
  customer.insert("orderid", objectName().trimmed());
  customer.insert("type", "customer_request");
  foreach (QString f, customerSearchFields()) {
    customer.insert(f, p_customer.value(f));
  }
  return QJsonDocument(customer);
}

void BooklookerIfaceWidget::createOrderDataSet(const QJsonArray &array) {
  if (array.size() > 0) {
    // Bestellartikel einfügen
    setTableCount(0);
    QJsonArray items(array);
    QJsonArray::iterator at;
    for (at = items.begin(); at != items.end(); ++at) {
      QJsonObject article = (*at).toObject();
      int row = getTableCount();
      setTableCount((getTableCount() + 1));
      qint64 order_id = article.value("orderItemId").toInt();
      setTableData(row, 0, QString::number(order_id));
      setTableData(row, 1, article.value("orderNo"));
      qint64 mount = article.value("amount").toInt();
      setTableData(row, 2, QString::number(mount));
      setTableData(row, 3, article.value("singlePrice"));
      setTableData(row, 4, article.value("orderTitle"));
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
    setValue("c_gender", c_gender);
    p_customer.insert("c_gender", c_gender);
  }

  if (obj.contains("company")) {
    QString company = obj.value("company").toString().trimmed();
    p_customer.insert("c_company_name", company);
    setValue("c_company_name", company);
  }

  QString c_firstname = obj.value("firstName").toString().trimmed();
  setValue("c_firstname", c_firstname);
  p_customer.insert("c_firstname", c_firstname);

  person.append(c_firstname);
  person.append(" ");
  QString c_lastname = obj.value("name").toString().trimmed();
  setValue("c_lastname", c_lastname);
  p_customer.insert("c_lastname", c_lastname);
  person.append(c_lastname);
  addressBody << QString(c_firstname + " " + c_lastname);
  setValue("person", person);

  QString c_street = obj.value("street").toString().trimmed();
  setValue("c_street", c_street);
  p_customer.insert("c_street", c_street);
  addressBody << c_street;

  QString c_postalcode = obj.value("zip").toString().trimmed();
  setValue("c_postalcode", c_postalcode);
  p_customer.insert("c_postalcode", c_postalcode);

  QString c_location = obj.value("city").toString().trimmed();
  setValue("c_location", c_location);
  p_customer.insert("c_location", c_location);
  addressBody << QString(c_postalcode + " " + c_location);

  QString c_country = obj.value("country").toString().trimmed();
  if (c_country.isEmpty())
    c_country = tr("Germany");

  setValue("c_country", c_country);
  p_customer.insert("c_country", c_country);

  // Adresse einfügen
  setValue(key, addressBody.join("\n"));
  addressBody.clear();

  // Sende SQL Abfrage an Hauptfenster!
  if (key == "c_postal_address") {
    QJsonDocument qDoc = customerRequest(obj);
    if (!qDoc.isEmpty())
      emit sendCheckCustomer(qDoc);
  }
}

void BooklookerIfaceWidget::checkCustomerExists() {
  QJsonDocument qDoc = customerRequest(QJsonObject());
  if (!qDoc.isEmpty())
    emit sendCheckCustomer(qDoc);
}

void BooklookerIfaceWidget::readCurrentArticleIds() {
  QList<int> ids = getArticleIDs();
  if (ids.count() > 0)
    emit sendCheckArticleIds(ids);
}

void BooklookerIfaceWidget::createCustomerDocument() {
  if (p_currentDocument.isEmpty()) {
    qDebug() << Q_FUNC_INFO << "Current Json Document is empty!";
    return;
  }

  if (getCustomerId() > 0) {
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
    setValue("c_email_0", mail);
  }

  if (obj.contains("phone")) {
    QString phone = obj.value("phone").toString().trimmed();
    customer.insert("c_phone_0", phone);
    setPhone("c_phone_0", phone);
  }

  QString pAddress = getValue("c_postal_address").toString();
  if (!pAddress.isEmpty())
    customer.insert("c_postal_address", pAddress);

  QString dAddress = getValue("c_shipping_address").toString();
  if (!dAddress.isEmpty())
    customer.insert("c_shipping_address", dAddress);

  emit sendCreateCustomer(QJsonDocument(customer));
}

void BooklookerIfaceWidget::setContent(const QJsonDocument &doc) {
  // Muss unbedingt an dieser Stelle geleert werden!
  p_currentDocument = QJsonDocument();

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

  qint64 searchId = getOrderId().toLongLong();
  QJsonArray array = QJsonValue(doc["returnValue"]).toArray();
  for (int i = 0; i < array.size(); i++) {
    if (array[i].isObject()) {
      QJsonObject obj = array[i].toObject();
      qint64 index = obj.value("orderId").toInt();
      if (index != searchId)
        continue;

      // Puffer erstellen
      setCurrentDocument(obj);

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
        setValue(sf, val.toVariant());
      }
    }
  }
  // jetzt aktivieren
  setEnabled(true);
}

void BooklookerIfaceWidget::createOrderRequest() {
  if (getOrderId().isEmpty()) {
    qWarning("Invalid Booklooker OrderId");
    return;
  }
  m_requester->queryOrder(getOrderId());
}

void BooklookerIfaceWidget::createProviderOrderUpdate() {
  m_dialog = new BooklookerRemoteActions(this);
  // Käufername
  QStringList person;
  person << getValue("c_gender").toString();
  person << getValue("c_firstname").toString();
  person << getValue("c_lastname").toString();
  m_dialog->setPurchaser(person.join(" "));
  // E-Mail
  m_dialog->setEMail(getValue("c_email_0").toString());

  if (m_dialog->exec(getOrderId()) == QDialog::Accepted) {
    qDebug() << Q_FUNC_INFO << "TODO";
  }
  m_dialog->deleteLater();
}

void BooklookerIfaceWidget::setOrderUpdateTypes() {
  QMap<Antiqua::PaymentStatus, QString> map;
  map.insert(Antiqua::PaymentStatus::ORDER_WAIT_FOR_PAYMENT,
             QString("WAITING_FOR_PAYMENT"));
  // fertig zum Versand
  map.insert(Antiqua::PaymentStatus::ORDER_READY_FOR_SHIPMENT,
             QString("READY_FOR_SHIPMENT"));
  // versendet, warte auf Zahlung
  map.insert(Antiqua::PaymentStatus::ORDER_SHIPPED_WAIT_FOR_PAYMENT,
             QString("SHIPPED_WAITING_FOR_PAYMENT"));
  // versendet & bezahlt
  map.insert(Antiqua::PaymentStatus::ORDER_SHIPPED_AND_PAID,
             QString("SHIPPED_AND_PAID"));
  // Kunde reagiert nicht
  map.insert(Antiqua::PaymentStatus::ORDER_BUYER_NO_REACTION,
             QString("BUYER_NO_REACTION"));

  // setOrderUpdateActions(map);
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
