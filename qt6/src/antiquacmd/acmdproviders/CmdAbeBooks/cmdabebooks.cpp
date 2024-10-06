// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "cmdabebooks.h"
#include "abebooksdocument.h"

#include <QtCore>

CmdAbeBooks::CmdAbeBooks(QObject* parent)
    : ACmdProviders{AntiquaCRM::NetworkQueryType::XML_QUERY, parent} {
  setObjectName("abebooks_cmd");
}

AbeBooksDocument CmdAbeBooks::initDocument() {
  Q_ASSERT(!(apiUser.isEmpty()) || (apiKey.isEmpty()));
  AbeBooksAccess ac;
  ac.user = apiUser;
  ac.key = apiKey;
  return AbeBooksDocument(ac);
}

const QRegularExpression CmdAbeBooks::charsetPattern() const {
  QRegularExpression _regexp("\\b(iso\\-8859)[\\-\\d]*", QRegularExpression::CaseInsensitiveOption);
  return _regexp;
}

const QString CmdAbeBooks::provider() const {
  return QString("AbeBooks");
}

void CmdAbeBooks::initConfiguration() {
  QString _sql("SELECT cfg_jsconfig::json FROM antiquacrm_configs");
  _sql.append(" WHERE cfg_group='CONFIG_ABEBOOKS';");
  QSqlQuery _q = pgsql->query(_sql);
  if (_q.size() == 0) {
    qWarning("AbeBooks: Configuration not found, abort!");
    emit sendDisjointed();
    return;
  }

  _q.next();
  QJsonObject _o = QJsonDocument::fromJson(_q.value(0).toByteArray()).object();
  _q.clear();

  baseUrl.setScheme("https");
  baseUrl.setHost(_o.value("api_host").toString());
  apiPort = _o.value("api_port").toInt();
  apiKey = _o.value("api_key").toString();
  apiPath = QString();
  apiUser = _o.value("api_user").toString();
  apiKey = _o.value("api_key").toString();
  // _o.value("seller_id");
}

const QUrl CmdAbeBooks::apiQuery(const QString& action) {
  QUrl url(baseUrl);
  url.setPort(apiPort);
  actionsCookie = QNetworkCookie("action", action.toLocal8Bit());
  actionsCookie.setDomain(url.host());
  actionsCookie.setSecure(true);
  return url;
}

void CmdAbeBooks::prepareContent(const QJsonDocument& document) {
  Q_UNUSED(document);
  qInfo("No Json support for this CmdAbeBooks plugin");
}

void CmdAbeBooks::prepareContent(const QDomDocument& document) {
  // Bei fehlern sofort aussteigen!
  if (document.documentElement().tagName() == "requestError") {
    AbeBooksDocument errXml(document);
    QPair<int, QString> err = errXml.errorResponseCode();
    qWarning("AbeBooks Request Error %d (%s)", err.first, qPrintable(err.second));
    emit sendDisjointed();
    return;
  }
  // Konvertiere LATIN1 zu UTF-8 und ändere den XML Header!
  const QFileInfo _tempfile(AntiquaCRM::ASettings::getUserTempDir(), "abebooks_orders_temp.xml");

  QString _buffer = document.toString(1);
  _buffer.replace(charsetPattern(), "utf8");

  QFile _fp(_tempfile.filePath());
  if (_fp.open(QIODevice::WriteOnly)) {
    QTextStream writer(&_fp);
    writer.setEncoding(QStringConverter::Latin1);
    writer << _buffer;
    _buffer.clear();
    _fp.close();
  } else {
    qWarning("CmdAbeBooks: Can not write tempfile, abort import!");
    emit sendDisjointed();
    return;
  }
  // Wieder öffnen und neu einlesen!
  QDomDocument _import;
  if (_fp.open(QIODevice::ReadOnly)) {
    QString _errno;
    if (!_import.setContent(&_fp, false, &_errno)) {
      qWarning("CmdAbeBooks: XML Errors '%s'.", qPrintable(_errno));
      emit sendDisjointed();
      return;
    }
    _fp.close();
  }
  // Starte Verarbeitung
  AbeBooksDocument xml(_import);
  // purchaseOrderList
  QList<QJsonObject> ordersList;
  QStringList imported = currProviderIds(provider());
  QDomNodeList purchase_orders = xml.getPurchaseOrderList();
  if (purchase_orders.size() > 0) {
    for (int i = 0; i < purchase_orders.count(); i++) {
      // purchaseOrder::Node @{
      QDomNode orderNode = purchase_orders.at(i);
      QDomElement orderElement = orderNode.toElement();
      QString order_str_id = orderNode.toElement().attribute("id", "0");
      if (imported.contains(order_str_id)) {
        continue;
      }

      qint64 orderId = order_str_id.toInt();
      // @}
      QDateTime dateTime = xml.getOrderDate(orderElement);
      // BEGIN::CONVERT

      // Bestellinfos
      QJsonObject antiqua_orderinfo;
      antiqua_orderinfo.insert("o_provider_name", provider());
      antiqua_orderinfo.insert("o_provider_order_id", order_str_id);
      antiqua_orderinfo.insert("o_provider_purchase_id", orderId);
      antiqua_orderinfo.insert("o_since", dateTime.toString(Qt::ISODate));
      // antiqua_orderinfo.insert("o_media_type", AntiquaCRM::ArticleType::OTHER);

      /*
        Konvertiere Provider PaymentStatus => OrderStatus!
        Wir verwenden im Auftragssystem nur den OrderStatus!
        Der PaymentStatus ist eine reine Dienstleistergeschichte!
       */
      QDomNode statusNode = xml.firstChildNode(orderElement, "status");
      QString orderStatus = xml.getNodeValue(statusNode).toString();
      if (orderStatus.contains("Ordered")) {
        // AntiquaCRM::ProviderPaymentStatus::SHIPMENT_CREATED => AntiquaCRM::OrderStatus::STARTED
        antiqua_orderinfo.insert("o_order_status", AntiquaCRM::OrderStatus::STARTED);
      } else if (orderStatus.contains("Cancelled")) {
        // AntiquaCRM::ProviderPaymentStatus::ORDER_CANCELED => AntiquaCRM::OrderStatus::CANCELED
        antiqua_orderinfo.insert("o_order_status", AntiquaCRM::OrderStatus::CANCELED);
      } else {
        // AntiquaCRM::ProviderPaymentStatus::STATUS_NOT_SET => AntiquaCRM::OPEN
        antiqua_orderinfo.insert("o_order_status", AntiquaCRM::OrderStatus::OPEN);
      }

      // AntiquaCRM::PaymentMethod
      AntiquaCRM::PaymentMethod payment_method;
      payment_method = AntiquaCRM::PaymentMethod::PAYMENT_NOT_SET;
      QDomNode pmNode = xml.firstChildNode(orderElement, "purchaseMethod");
      QString purchaseMethod = xml.getNodeValue(pmNode).toString();
      if (purchaseMethod.toUpper() == "CC") {
        // AbeBooks Creditcard
        payment_method = AntiquaCRM::PaymentMethod::CREDIT_CARD_PREPAYMENT;
      } else if (purchaseMethod.toUpper() == "SD") {
        // Seller Direct
        QString sdType = pmNode.toElement().attribute("type", "Invoice");
        if (sdType.contains("Check"))
          payment_method = AntiquaCRM::PaymentMethod::CREDIT_CARD_PREPAYMENT;
        else if (sdType.contains("PayPal"))
          payment_method = AntiquaCRM::PaymentMethod::PAYPAL_PREPAYMENT;
        else if (sdType.contains("Debit"))
          payment_method = AntiquaCRM::PaymentMethod::DIRECT_DEBIT_PREPAYMENT;
        else if (sdType.contains("Bank"))
          payment_method = AntiquaCRM::PaymentMethod::BANK_PREPAYMENT;
        else if (sdType.contains("Invoice"))
          payment_method = AntiquaCRM::PaymentMethod::DELIVER_WITH_INVOICE;
        else if (sdType.contains("Money Order"))
          payment_method = AntiquaCRM::PaymentMethod::UNKNOWN_PREPAYMENT;
      }
      antiqua_orderinfo.insert("o_payment_method", payment_method);
      antiqua_orderinfo.insert("o_payment_status", 0);
      antiqua_orderinfo.insert("o_delivery_add_price", false);

      // Kundendaten
      QJsonObject antiqua_customer;
      QString bcp47 = "DE"; // Siehe orderinfo
      QDomNodeList buyerNodeList = orderElement.elementsByTagName("buyer");
      if (buyerNodeList.size() > 0) {
        QDomElement buyerNode = buyerNodeList.at(0).toElement();
        // Postaldata
        QDomNode addressNode = buyerNode.namedItem("mailingAddress");
        QPair<QString, QString> person = xml.getPerson(addressNode);
        antiqua_customer.insert("c_provider_import", xml.getFullname(addressNode));
        antiqua_customer.insert("c_gender", AntiquaCRM::Gender::NO_GENDER);
        antiqua_customer.insert("c_firstname", ucFirst(person.first));
        antiqua_customer.insert("c_lastname", ucFirst(person.second));
        antiqua_customer.insert("c_street", xml.getStreet(addressNode));
        antiqua_customer.insert("c_postalcode", xml.getPostalCode(addressNode).toString());
        antiqua_customer.insert("c_location", xml.getLocation(addressNode));
        QString p_country = xml.getCountry(addressNode);
        bcp47 = (p_country.isEmpty()) ? "Germany" : findBCP47(p_country);
        antiqua_customer.insert("c_country_bcp47", bcp47);
        antiqua_customer.insert("c_country", getCountry(bcp47));
        antiqua_orderinfo.insert("o_vat_country", bcp47);
        antiqua_customer.insert("c_phone_0", xml.getPhone(addressNode));
        antiqua_customer.insert("c_email_0", xml.getEMail(buyerNode));
        // Address Bodies
        QStringList buffer;
        buffer << xml.getFullname(addressNode);
        buffer << xml.getStreet(addressNode);
        buffer << xml.getPostalCode(addressNode).toString() + " " + xml.getLocation(addressNode);
        antiqua_customer.insert("c_postal_address", buffer.join("\n"));
        antiqua_customer.insert("c_shipping_address", "");
      }

      // Artikelliste - QDomNode::purchaseOrderItemList
      QJsonArray antiqua_articles;
      QDomNodeList orderItems = xml.getOrderItemList(orderElement);
      if (orderItems.size() > 0) {
        for (int i = 0; i < orderItems.size(); i++) {
          // QDomNode::purchaseOrderItem
          QDomElement orderItemNode = orderItems.at(i).toElement();
          QJsonObject order_article;
          // QDomNode::book
          QDomNode book = xml.firstChildNode(orderItemNode, "book");
          if (!book.isNull()) {
            QJsonValue temp;
            QDomElement bookElement = book.toElement();
            // Article title
            temp = QJsonValue(xml.getTagText(bookElement, "title"));
            order_article.insert("a_title", temp);
            // Provider Order ID
            temp = QJsonValue(bookElement.attribute("id", order_str_id));
            order_article.insert("a_provider_id", convert("a_provider_id", temp));
            // Article ID
            QDomNode queryNode = xml.firstChildNode(bookElement, "vendorKey");
            temp = QJsonValue(xml.getNodeValue(queryNode).toInt());
            order_article.insert("a_article_id", temp);
            // Find valid Article Type
            AntiquaCRM::ArticleType _type = findArticlType(temp.toString());
            order_article.insert("a_type", QJsonValue(_type));
            if (_type == AntiquaCRM::ArticleType::OTHER) {
                order_article.insert("a_tax", QJsonValue(AntiquaCRM::SalesTax::TAX_WITH));
            } else {
                order_article.insert("a_tax", QJsonValue(AntiquaCRM::SalesTax::TAX_INCL));
            }
            // Article Count
            order_article.insert("a_count", QJsonValue(1));
            order_article.insert("a_payment_id", QJsonValue(0));
            // Article Price
            queryNode = xml.firstChildNode(bookElement, "price");
            temp = QJsonValue(xml.getNodeValue(queryNode).toDouble());
            order_article.insert("a_price", convert("a_price", temp));
            // Article Sell Price (VK)
            queryNode = xml.firstChildNode(bookElement, "price");
            temp = QJsonValue(xml.getNodeValue(queryNode).toDouble());
            order_article.insert("a_sell_price", convert("a_sell_price", temp));
          }
          antiqua_articles.append(order_article);
        }
      }

      QJsonObject antiqua_order; // Bestellkopfdaten
      antiqua_order.insert("provider", QJsonValue(provider()));
      antiqua_order.insert("orderid", QJsonValue(order_str_id));
      antiqua_order.insert("orderinfo", QJsonValue(antiqua_orderinfo));
      antiqua_order.insert("customer", QJsonValue(antiqua_customer));
      antiqua_order.insert("articles", QJsonValue(antiqua_articles));

      ordersList.append(antiqua_order);
      // END::CONVERT
    }
    // END::QUERY_ORDERS

    if (createOrders(ordersList)) {
      qInfo("%s: New orders arrived!", qPrintable(provider()));
      emit sendFinished();
      return;
    }
  }

  qInfo("%s: Nothing todo!", qPrintable(provider()));
  emit sendDisjointed();
}

void CmdAbeBooks::responsed(const QByteArray& bread) {
  QDomDocument xml("response");
  // WARNING: We need a Header with ABEBOOKS_CHARSET
  QDomProcessingInstruction pir = xml.createProcessingInstruction(
      "xml", "version=\"1.0\" encoding=\"" + ABEBOOKS_CHARSET + "\"");
  xml.appendChild(pir);

  QString errorMsg;
  int errorLine = 0;
  int errorColumn = 0;
  if (!xml.setContent(bread, false, &errorMsg, &errorLine, &errorColumn)) {
    qWarning("AbeBooks: Responsed XML is not well format!");
    emit sendDisjointed();
    return;
  }
  prepareContent(xml);
}

void CmdAbeBooks::start() {
  const QString _action("getAllNewOrders");
  AbeBooksDocument _doc = initDocument();
  _doc.createAction(_action);
  netw->xmlPostRequest(apiQuery(_action), _doc, ABEBOOKS_CHARSET);
}

bool CmdAbeBooks::init() {
  if (pgsql == nullptr)
    return false;

  initConfiguration();

  if (baseUrl.isEmpty() || apiKey.isEmpty() || apiUser.isEmpty())
    return false;

  return true;
}
