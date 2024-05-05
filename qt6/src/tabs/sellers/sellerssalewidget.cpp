// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "sellerssalewidget.h"
#include "buyerinfo.h"
#include "creditworthiness.h"
#include "popupopenexists.h"
#include "purchaseactionbar.h"
#include "purchaseheader.h"
#include "purchasetable.h"
#include "salesinfo.h"

#include <QLayout>
#include <QMessageBox>

SellersSalesWidget::SellersSalesWidget(const QJsonObject &config,
                                       QWidget *parent)
    : QWidget{parent}, p_order{config} {
  setWindowTitle(p_order.value("orderid").toString());
  setContentsMargins(0, 0, 0, 0);
  setEnabled(false);

  const QIcon _icon = AntiquaCRM::antiquaIcon("system-users");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 2, 0, 2);

  m_header = new PurchaseHeader(this);
  layout->addWidget(m_header);

  m_tab = new QTabWidget(this);
  m_tab->setTabPosition(QTabWidget::South);

  m_buyerInfo = new BuyerInfo(m_tab);
  m_tab->addTab(m_buyerInfo, _icon, tr("Buyer"));

  m_salesInfo = new SalesInfo(m_tab);
  m_tab->addTab(m_salesInfo, _icon, tr("Paymentinfo"));

  layout->addWidget(m_tab);

  m_worthiness = new Creditworthiness(this);
  layout->addWidget(m_worthiness);

  m_table = new PurchaseTable(this);
  layout->addWidget(m_table);

  m_actionBar = new PurchaseActionBar(this);
  layout->addWidget(m_actionBar);

  setLayout(layout);

  connect(m_table, SIGNAL(sendCheckArticles()), SLOT(findArticleIds()));
  connect(m_table, SIGNAL(sendOpenArticle(qint64)), SLOT(openArticle(qint64)));
  connect(m_actionBar, SIGNAL(sendViewCustomer()), SLOT(openCustomer()));
  connect(m_actionBar, SIGNAL(sendCheckArticles()), SLOT(findArticleIds()));
  connect(m_actionBar, SIGNAL(sendCreateOrder()), SLOT(prepareCreateOrder()));
  connect(m_actionBar, SIGNAL(sendProviderAction()),
          SLOT(createProviderActions()));
}

const QString SellersSalesWidget::mediaType(const QJsonValue &object) {
  QString _name;
  switch (static_cast<AntiquaCRM::ArticleType>(object.toInt())) {
  case AntiquaCRM::ArticleType::BOOK: /**< Bücher */
    _name = tr("Book");
    break;

  case AntiquaCRM::ArticleType::MEDIA: /**< Film & Tonträger */
    _name = tr("Media");
    break;

  case AntiquaCRM::ArticleType::PRINTS: /**< Drucke & Stiche */
    _name = tr("Prints");
    break;

  default:
    _name = tr("Diverse");
    break;
  };
  return _name;
}

AntiquaCRM::ArticleType SellersSalesWidget::getArticleType(qint64 aid) {
  const QString _strid = QString::number(aid);
  AntiquaCRM::ASqlCore psql(this);
  QSqlQuery _q = psql.query("SELECT func_get_article_type(" + _strid + ");");
  if (_q.size() > 0) {
    _q.next();
    return static_cast<AntiquaCRM::ArticleType>(_q.value(0).toInt());
  }
  return AntiquaCRM::ArticleType::BOOK;
}

const QString SellersSalesWidget::getPrice(const QJsonValue &price) const {
  return AntiquaCRM::ATaxCalculator::money(price.toDouble());
}

const QString SellersSalesWidget::getTitle(const QJsonValue &title) const {
  QString _str = title.toString();
  _str.replace("\"", "’");
  _str.replace("'", "’");
  return _str.trimmed();
}

bool SellersSalesWidget::findCustomer(const QJsonObject &object) {
  // qDebug() << object;
  QString _fullname = object.value("c_provider_import").toString();
  if (object.contains("c_postal_address")) {
    QString address = object.value("c_postal_address").toString();
    if (object.contains("c_country")) {
      address.append("\n");
      address.append(object.value("c_country").toString());
    }
    if (object.contains("c_phone_0")) {
      address.append("\n\n" + tr("Phone") + ": ");
      address.append(object.value("c_phone_0").toString());
    }
    m_buyerInfo->setAddress(address);
  }

  AntiquaCRM::ASqlFiles _tpl("query_customer_exists");
  if (_tpl.openTemplate()) {
    const QStringList _fields({"c_firstname", "c_lastname", "c_postalcode"});
    QStringList _values;

    QString _postalcode;
    foreach (QString f, _fields) {
      QVariant _v = object.value(f);
      if (_v.isNull())
        continue;

      if (f == "c_postalcode") {
        _values << f + " ILIKE '" + _v.toString() + "'";
        _postalcode = _v.toString().trimmed();
      } else {
        _values << f + " ILIKE '" + _v.toString() + "%'";
      }
    }

    QString _clause("(");
    _clause.append(_values.join(" AND "));
    _clause.append(") OR (c_provider_import ILIKE '");
    _clause.append(_fullname);
    _clause.append("' AND c_postalcode LIKE '");
    _clause.append(_postalcode);
    _clause.append("')");
    _tpl.setWhereClause(_clause);

    AntiquaCRM::ASqlCore psql(this);
    QSqlQuery _q = psql.query(_tpl.getQueryContent());
    if (_q.size() > 0) {
      _q.next();
      qint64 _cid = _q.value("c_id").toInt();
      AntiquaCRM::CustomerTrustLevel _trust =
          static_cast<AntiquaCRM::CustomerTrustLevel>(
              _q.value("c_trusted").toInt());
      QString _name = _q.value("display_name").toString();
      if (_cid > 0 && !_name.isEmpty()) {
        m_header->setHeader(_name, _cid);
        setTrustStatus(_trust);
        return true;
      }
    }
  }
  m_header->setHeader(_fullname);
  return false;
}

void SellersSalesWidget::pushCmd(const QJsonObject &action) {
  AntiquaCRM::ATransmitter *m_sock = new AntiquaCRM::ATransmitter(this);
  connect(m_sock, SIGNAL(disconnected()), m_sock, SLOT(deleteLater()));
  if (m_sock->pushOperation(action))
    m_sock->close();

#ifdef ANTIQUA_DEVELOPMENT
  qDebug() << Q_FUNC_INFO << action;
#endif
}

void SellersSalesWidget::openOrder(qint64 oid) {
  QJsonObject _obj;
  _obj.insert("ACTION", "open_order");
  _obj.insert("TARGET", "orders_tab");
  _obj.insert("VALUE", oid);
  pushCmd(_obj);
}

void SellersSalesWidget::createOrder(const QString &prid) {
  qint64 _cid = m_header->getCustomerId();
  QStringList _list = m_table->getArticleIds();
  if (_list.size() < 1 || _cid < 1) {
    QString txt("<p>" + tr("No available Articles!") + "</p><p>");
    txt.append(tr("Can not create an Order without them!") + "</p>");
    QMessageBox::warning(this, tr("No available Articles!"), txt);
    return;
  }

  QJsonObject _obj;
  _obj.insert("ACTION", "import_order");
  _obj.insert("TARGET", "orders_tab");
  _obj.insert("VALUE", _cid);
  _obj.insert("PRORDER", prid);
  _obj.insert("ARTICLES", _list.join(","));
  pushCmd(_obj);
}

void SellersSalesWidget::openCustomer() {
  qint64 _cid = m_header->getCustomerId();
  if (_cid < 1)
    return;

  QJsonObject _obj;
  _obj.insert("ACTION", "open_customer");
  _obj.insert("TARGET", "customers_tab");
  _obj.insert("VALUE", _cid);
  pushCmd(_obj);
}

void SellersSalesWidget::openArticle(qint64 aid) {
  QJsonObject obj;
  obj.insert("ACTION", "open_article");
  // mediaType
  switch (getArticleType(aid)) {
  case AntiquaCRM::ArticleType::MEDIA: /**< Film & Tonträger */
    obj.insert("TARGET", "cdvinyl_tab");
    break;

  case AntiquaCRM::ArticleType::PRINTS: /**< Drucke & Stiche */
    obj.insert("TARGET", "printsstitches_tab");
    break;

  case AntiquaCRM::ArticleType::OTHER: /**< Various */
    obj.insert("TARGET", "various_tab");
    break;

  default: /**< Default: Books */
    obj.insert("TARGET", "books_tab");
    break;
  };
  obj.insert("VALUE", aid);
  pushCmd(obj);
}

void SellersSalesWidget::createProviderActions() {
  if (!p_order.contains("orderinfo"))
    return;

  QString _provider = p_order.value("provider").toString().toLower();
  QJsonObject _values;
  _values.insert("orderinfo", p_order.value("orderinfo").toObject());

  const QJsonObject _recipient = p_order.value("customer").toObject();
  _values.insert("buyer", _recipient.value("c_provider_import").toString());
  _values.insert("email", _recipient.value("c_email_0").toString());

  QJsonObject _action;
  _action.insert("PROVIDER", _provider);
  _action.insert("DATA", _values);

  emit sendOpenRemoteAction(_action);
}

void SellersSalesWidget::findArticleIds() {
  int _found = 0;
  AntiquaCRM::ASqlCore psql(this);
  foreach (QString i, m_table->getArticleIds()) {
    QSqlQuery _q = psql.query("SELECT func_get_article_count(" + i + ");");
    if (_q.size() > 0) {
      _q.next();
      qint64 _id = _q.value(0).toInt();
      m_table->setArticleStatus(i, (_id > 0));
      if (_id > 0)
        _found++;
    } else {
      m_table->setArticleStatus(i, false);
    }
  }
  m_actionBar->enableCreateButton((_found > 0));
}

void SellersSalesWidget::prepareCreateOrder() {
  const QString _provider = p_order.value("provider").toString();
  const QString _proid = p_order.value("orderid").toString();
  if (_provider.isEmpty() || _proid.isEmpty()) {
    qWarning("CreateOrder rejected: missing arguments!");
    return;
  }

  AntiquaCRM::ASqlFiles _tpl("query_order_exists");
  if (_tpl.openTemplate()) {
    QString clause("o_provider_name ILIKE '" + _provider);
    clause.append("' AND o_provider_order_id='" + _proid + "'");
    clause.append(" AND pr_order_id IS NOT NULL");
    _tpl.setWhereClause(clause);
  }

  AntiquaCRM::ASqlCore psql(this);
  QSqlQuery _query = psql.query(_tpl.getQueryContent());
  if (_query.size() < 1) {
    createOrder(_proid);
    return;
  }

  _query.next();
  qint64 _oid = _query.value("o_id").toInt();
  const QString _prinfo = _query.value("prinfo").toString();
  const QString _buyer = _query.value("buyer").toString();
  QStringList _list; // create PopUp Message
  _list.append(tr("An order for %1 already exists!").arg(_prinfo));
  _list.append(tr("Current Order number: %1").arg(_oid));
  _list.append(tr("Buyer: %1").arg(_buyer));

  PopUpOpenExists popUp(this);
  popUp.setWindowTitle(tr("Order already exists!"));
  popUp.setMessage(_list);
  switch (popUp.exec()) {
  case (QMessageBox::Open):
    openOrder(_oid);
    break;

  case (QMessageBox::Yes):
    createOrder(_proid);
    break;

  default:
    break;
  }
}

void SellersSalesWidget::setTrustStatus(AntiquaCRM::CustomerTrustLevel tl) {
  switch (tl) {
  case (AntiquaCRM::CustomerTrustLevel::WITH_DELAY):
  case (AntiquaCRM::CustomerTrustLevel::PREPAYMENT):
  case (AntiquaCRM::CustomerTrustLevel::NO_DELIVERY):
    m_worthiness->setVisible(true);
    break;

  default:
    m_worthiness->setVisible(false);
    break;
  }
}

bool SellersSalesWidget::init() {
  bool _status = false;
  if (p_order.count() == 0)
    return _status;

  // Buyer/Customer Info
  QJsonObject _customer = p_order.value("customer").toObject();
  _status = findCustomer(_customer);
  if (!_status) {
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
  const QJsonObject _orderinfo = p_order.value("orderinfo").toObject();
  m_salesInfo->setInfo(_orderinfo);

  // Comments from buyer
  if (_orderinfo.contains("o_delivery_comment")) {
    const QString _comment = _orderinfo.value("o_delivery_comment").toString();
    m_buyerInfo->setComment(_comment.trimmed());
  }

  // Buyed Articles
  const QJsonArray _array = p_order.value("articles").toArray();
  if (_array.count() > 0) {
    m_table->setRowCount(_array.count());
    // int rows = m_table->rowCount();
    for (int r = 0; r < _array.count(); r++) {
      const QJsonObject _obj = _array[r].toObject();
      m_table->setItem(r, 0, m_table->createItem(_obj.value("a_provider_id")));
      m_table->setItem(r, 1, m_table->createItem(_obj.value("a_article_id")));
      m_table->setItem(r, 2, m_table->createItem(_obj.value("a_count")));
      m_table->setItem(r, 3, // Price
                       m_table->createItem(getPrice(_obj.value("a_price"))));
      m_table->setItem(r, 4, // Media
                       m_table->createItem(mediaType(_obj.value("a_type"))));
      m_table->setItem(r, 5, // Title
                       m_table->createItem(getTitle(_obj.value("a_title"))));
    }
  }

  setEnabled(_status);
  return _status;
}
