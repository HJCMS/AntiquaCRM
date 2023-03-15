// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "providersorderpage.h"
#include "providerbuyerinfo.h"
#include "providerorderinfo.h"
#include "providerpurchasebar.h"
#include "providerpurchaseheader.h"
#include "providerpurchasetable.h"
#include "providerspageview.h"

#include <ASettings>
#include <QDebug>
#include <QDialog>
#include <QLayout>
#include <QMessageBox>
#include <QMutex>
#include <QRadioButton>

ProvidersOrderPage::ProvidersOrderPage(const QJsonObject &order,
                                       QWidget *parent)
    : QWidget{parent}, p_order{order} {
  setWindowTitle(p_order.value("orderid").toString());
  setContentsMargins(0, 0, 0, 0);
  setEnabled(false);

  m_sql = new AntiquaCRM::ASqlCore(this);

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 2, 0, 2);

  m_header = new ProviderPurchaseHeader(this);
  layout->addWidget(m_header);

  m_tab = new ProvidersPageView(this);
  m_tab->setTabPosition(QTabWidget::South);
  m_buyerInfo = new ProviderBuyerInfo(m_tab);
  m_tab->addFixedTab(m_buyerInfo, tr("Buyer"));
  m_orderInfo = new ProviderOrderInfo(m_tab);
  m_tab->addFixedTab(m_orderInfo, tr("Paymentinfo"));
  layout->addWidget(m_tab);

  m_table = new ProviderPurchaseTable(this);
  layout->addWidget(m_table);

  m_actionBar = new ProviderPurchaseBar(this);
  layout->addWidget(m_actionBar);

  setLayout(layout);

  connect(m_table, SIGNAL(sendCheckArticles()), SLOT(findArticleIds()));
  connect(m_table, SIGNAL(sendOpenArticle(qint64)), SLOT(openArticle(qint64)));
  connect(m_actionBar, SIGNAL(sendViewCustomer()), SLOT(createOpenCustomer()));
  connect(m_actionBar, SIGNAL(sendCheckArticles()), SLOT(findArticleIds()));
  connect(m_actionBar, SIGNAL(sendCreateOrder()), SLOT(prepareCreateOrder()));
  connect(m_actionBar, SIGNAL(sendProviderAction()),
          SIGNAL(sendOpenProviderDialog()));
}

void ProvidersOrderPage::pushCmd(const QJsonObject &action) {
  AntiquaCRM::ATransmitter *m_sock = new AntiquaCRM::ATransmitter(this);
  connect(m_sock, SIGNAL(disconnected()), m_sock, SLOT(deleteLater()));
  if (m_sock->pushOperation(action))
    m_sock->close();
}

bool ProvidersOrderPage::findCustomer(const QJsonObject &customer) {
  QString fullname = customer.value("c_provider_import").toString();
  m_header->setHeader(fullname);
  if (customer.contains("c_postal_address")) {
    QString address = customer.value("c_postal_address").toString();
    if (customer.contains("c_phone_0")) {
      address.append("\nTel: ");
      address.append(customer.value("c_phone_0").toString());
    }
    m_buyerInfo->setAddress(address);
  }

  AntiquaCRM::ASqlFiles query("query_customer_exists");
  if (query.openTemplate()) {
    QStringList buffer;
    QStringList fields;
    fields << "c_firstname"
           << "c_lastname"
           << "c_postalcode";

    foreach (QString f, fields) {
      QVariant v = customer.value(f);
      if (!v.isNull()) {
        if (f == "c_postalcode")
          buffer << f + " ILIKE '" + v.toString() + "'";
        else
          buffer << f + " ILIKE '" + v.toString() + "%'";
      }
    }
    QString clause("(");
    clause.append(buffer.join(" AND "));
    clause.append(") OR (c_provider_import ILIKE '");
    clause.append(fullname);
    clause.append("')");
    query.setWhereClause(clause);
    QSqlQuery q = m_sql->query(query.getQueryContent());
    if (q.size() > 0) {
      q.next();
      qint64 id = q.value("c_id").toInt();
      QString name = q.value("display_name").toString();
      if (id > 0 && !name.isEmpty()) {
        m_header->setHeader(name, id);
        // qDebug() << Q_FUNC_INFO << name;
        return true;
      }
    }
  }
  return false;
}

AntiquaCRM::ArticleType ProvidersOrderPage::getArticleType(qint64 aid) {
  QString strid = QString::number(aid);
  QSqlQuery q = m_sql->query("SELECT func_get_article_type(" + strid + ");");
  if (q.size() > 0) {
    q.next();
    return static_cast<AntiquaCRM::ArticleType>(q.value(0).toInt());
  }
  return AntiquaCRM::ArticleType::BOOK;
}

const QString ProvidersOrderPage::mediaType(const QJsonValue &val) {
  QString name;
  switch (static_cast<AntiquaCRM::ArticleType>(val.toInt())) {
  case AntiquaCRM::ArticleType::MEDIA: /**< Film & Tonträger */
    name = tr("CD/Vinyl");
    break;

  case AntiquaCRM::ArticleType::PRINTS: /**< Drucke & Stiche */
    name = tr("Prints or Stitches");
    break;

  case AntiquaCRM::ArticleType::OTHER: /**< Diverse */
    name = tr("Diverse");
    break;

  default:
    name = tr("Book");
    break;
  };
  return name;
}

const QString ProvidersOrderPage::convertPrice(double price) const {
  AntiquaCRM::ASettings cfg;
  QString currency = cfg.value("payment/currency", "$").toString();
  QString buffer(QString::number(price, 'd', 2));
  buffer.append(" ");
  buffer.append(currency);
  return buffer;
}

// Must be identical to the "index" call of the tab class!
// grep -hoe '\<[a-z]\+_tab\>' tabs/*/tab*.cpp
// books_tab,customers_tab,orders_tab,providers_tab,views_tab
void ProvidersOrderPage::openOrder(qint64 oid) {
  QJsonObject obj;
  obj.insert("window_operation", "open_order");
  obj.insert("tab", "orders_tab");
  obj.insert("open_order", oid);
  pushCmd(obj);
}

void ProvidersOrderPage::createOrder(const QString &providerId) {
  QStringList list = m_table->getArticleIds();
  if (list.size() < 1) {
    QString txt("<p>" + tr("No available Articles!") + "</p><p>");
    txt.append(tr("Can not create an Order without them!") + "</p>");
    QMessageBox::warning(this, tr("No available Articles!"), txt);
    return;
  }
  QJsonObject obj;
  obj.insert("window_operation", "create_order");
  obj.insert("tab", "orders_tab");
  obj.insert("create_order", providerId);
  obj.insert("article_numbers", list.join(","));
  pushCmd(obj);
}

void ProvidersOrderPage::createOpenCustomer() {
  qint64 c_id = m_header->CustomerId;
  if (c_id < 1)
    return;

  QJsonObject obj;
  obj.insert("window_operation", "open_customer");
  obj.insert("tab", "customers_tab");
  obj.insert("open_customer", c_id);
  pushCmd(obj);
}

void ProvidersOrderPage::openArticle(qint64 aid) {
  QJsonObject obj;
  obj.insert("window_operation", "open_article");
  // mediaType
  switch (getArticleType(aid)) {
  case AntiquaCRM::ArticleType::MEDIA: /**< Film & Tonträger */
    obj.insert("tab", "cdvinyl_tab");
    break;

  case AntiquaCRM::ArticleType::PRINTS: /**< Drucke & Stiche */
    obj.insert("tab", "printsstitches_tab");
    break;

  case AntiquaCRM::ArticleType::OTHER: /**< Various */
    obj.insert("tab", "various_tab");
    break;

  default: /**< Default: Books */
    obj.insert("tab", "books_tab");
    break;
  };
  obj.insert("open_article", aid);
  pushCmd(obj);
}

void ProvidersOrderPage::findArticleIds() {
  int found = 0;
  QStringList queries;
  foreach (QString aid, m_table->getArticleIds()) {
    QSqlQuery q = m_sql->query("SELECT func_get_article_count(" + aid + ");");
    if (q.size() > 0) {
      q.next();
      qint64 id = q.value(0).toInt();
      m_table->setArticleStatus(aid, (id > 0));
      if (id > 0)
        found++;
    } else {
      m_table->setArticleStatus(aid, false);
    }
  }
  m_actionBar->enableCreateButton((found > 0));
}

void ProvidersOrderPage::prepareCreateOrder() {
  QString provider = p_order.value("provider").toString();
  QString orderid = p_order.value("orderid").toString();

  if (provider.isEmpty() || orderid.isEmpty()) {
    qWarning("CreateOrder rejected: missing arguments!");
    return;
  }

  AntiquaCRM::ASqlFiles query("query_order_exists");
  if (query.openTemplate()) {
    QString clause("o_provider_name ILIKE '" + provider);
    clause.append("' AND o_provider_order_id='" + orderid + "'");
    query.setWhereClause(clause);
  }

  QSqlQuery q = m_sql->query(query.getQueryContent());
  if (q.size() > 0) {
    q.next();
    qint64 o_id = q.value("o_id").toInt();
    QString prinfo = q.value("prinfo").toString();
    QString buyer = q.value("buyer").toString();
    QString txt("<p>");
    txt.append(tr("An order for %1 already exists!").arg(prinfo));
    txt.append("</p><p>" + tr("Buyer: %1").arg(buyer) + "</p><p>");
    txt.append(tr("Would you like to open this order?") + "</p>");
    int ret = QMessageBox::question(this, tr("Order already exists!"), txt);
    if (ret == QMessageBox::Yes)
      openOrder(o_id);

    return;
  } else if (!m_sql->lastError().isEmpty()) {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << query.getQueryContent() << m_sql->lastError();
#endif
    return;
  }

  createOrder(orderid);
}

bool ProvidersOrderPage::loadOrderDataset() {
  bool status = false;
  // qDebug() << Q_FUNC_INFO << p_order.count();
  if (p_order.count() == 0)
    return status;

  // Buyer/Customer Info
  QJsonObject customer = p_order.value("customer").toObject();
  status = findCustomer(customer);
  if (!status) {
    qWarning("No Customer found!");
    QStringList txt(tr("No Customer found!"));
    txt.append(tr("Can not create an Order without customer data!"));
    txt.append(
        tr("This can be due to an incorrect import or deleted customer data."));
    txt.append(tr("Alternatively, you must create the customer manually."));
    QMessageBox::warning(this, tr("Broken Orderdata!"), txt.join("\n"));
    return false;
  }

  // Payment Info
  QJsonObject orderInfo = p_order.value("orderinfo").toObject();
  m_orderInfo->setOrderInfo(orderInfo);

  // Comments from buyer
  if (orderInfo.contains("o_delivery_comment")) {
    QString comment = orderInfo.value("o_delivery_comment").toString();
    m_buyerInfo->setBuyerComment(comment.trimmed());
  }

  // buyed Articles
  QJsonArray array = p_order.value("articles").toArray();
  if (array.count() > 0) {
    m_table->setRowCount(array.count());
    // int rows = m_table->rowCount();
    for (int r = 0; r < array.count(); r++) {
      QJsonObject o = array[r].toObject();
      // qDebug() << Q_FUNC_INFO << o;
      m_table->setItem(r, 0, m_table->createItem(o.value("a_provider_id")));
      m_table->setItem(r, 1, m_table->createItem(o.value("a_article_id")));
      m_table->setItem(r, 2, m_table->createItem(o.value("a_count")));
      double price = o.value("a_price").toDouble();
      m_table->setItem(r, 3, m_table->createItem(convertPrice(price)));
      m_table->setItem(r, 4, m_table->createItem(mediaType(o.value("a_type"))));
      m_table->setItem(r, 5, m_table->createItem(o.value("a_title")));
    }
  }

  setEnabled(status);
  return status;
}