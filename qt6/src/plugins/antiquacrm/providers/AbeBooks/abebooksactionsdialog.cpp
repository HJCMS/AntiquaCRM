// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooksactionsdialog.h"
#include "abebooksactionsbar.h"
#include "abebooksconfig.h"
#include "abebooksordermaininfo.h"
#include "abebookssellerstatus.h"

#include <QFrame>
#include <QRadioButton>
#include <QUrl>
#include <QUrlQuery>
#include <QVBoxLayout>

AbeBooksActionsDialog::AbeBooksActionsDialog(QWidget *parent)
    : AntiquaCRM::ProviderActionDialog{parent} {
  setObjectName("abebooks_actions_dialog");
  setWindowTitle(tr("Update AbeBooks order.") + "[*]");
  setMinimumWidth(550);

  QFrame *m_frame = new QFrame(this);
  m_frame->setContentsMargins(5, 0, 5, 0);

  QVBoxLayout *layout = new QVBoxLayout(m_frame);
  layout->setContentsMargins(0, 0, 0, 0);

  int _page = 0;
  m_pages = new QStackedWidget(m_frame);
  layout->addWidget(m_pages);

  m_orderInfo = new AbeBooksOrderMainInfo(m_frame);
  m_pages->insertWidget(_page++, m_orderInfo);

  m_sellerStatus = new AbeBooksSellerStatus(m_pages);
  m_pages->insertWidget(_page++, m_sellerStatus);

  m_actionsBar = new AbeBooksActionsBar(this);
  layout->addWidget(m_actionsBar);

  m_frame->setLayout(layout);
  m_scrollArea->setWidget(m_frame);

  m_network =
      new AntiquaCRM::ANetworker(AntiquaCRM::NetworkQueryType::XML_QUERY, this);

  connect(m_network, SIGNAL(sendXmlResponse(const QDomDocument &)),
          SLOT(prepareResponse(const QDomDocument &)));

  connect(m_network, SIGNAL(finished(QNetworkReply *)),
          SLOT(queryFinished(QNetworkReply *)));

  connect(m_sellerStatus, SIGNAL(sendSelectionModified(bool)),
          SLOT(isChildWindowModified(bool)));

  connect(m_actionsBar, SIGNAL(goPrevious()), SLOT(previousPage()));
  connect(m_actionsBar, SIGNAL(goNext()), SLOT(nextPage()));

  connect(this, SIGNAL(sendSubmitClicked()), SLOT(prepareOperation()));
}

bool AbeBooksActionsDialog::initConfiguration() {
  p_config.clear();
  QJsonDocument _document;
  AntiquaCRM::ASqlCore dbsql(this);
  QString _sql("SELECT cfg_jsconfig FROM antiquacrm_configs ");
  _sql.append(" WHERE cfg_group='");
  _sql.append(ABEBOOKS_CONFIG_POINTER);
  _sql.append("';");
  QSqlQuery _q = dbsql.query(_sql);
  if (_q.size() > 0) {
    _q.next();
    _document = QJsonDocument::fromJson(_q.value(0).toByteArray());
  }

  if (_document.isNull())
    return false;

  QJsonObject _config = _document.object();
  QJsonObject::const_iterator it;
  for (it = _config.begin(); it != _config.end(); ++it) {
    p_config.insert(it.key(), it.value().toVariant());
  }

  return (p_config.size() > 0);
}

const QUrl AbeBooksActionsDialog::apiQuery(const QString &target) {
  Q_UNUSED(target);
  const QString _host("orderupdate.abebooks.com");
  QUrl _url;
  _url.setScheme("https");
  _url.setHost(p_config.value("api_host", _host).toString(), QUrl::StrictMode);
  _url.setPort(p_config.value("api_port", 10003).toInt());
  return _url;
}

QDomDocument AbeBooksActionsDialog::orderUpdateRequest(const QString &attr) {
  QDomDocument _dom;
  const QString _user = p_config.value("api_user").toString();
  const QString _key = p_config.value("api_key").toString();
  if (_user.isEmpty() || _key.isEmpty()) {
    qWarning("AbeBooks - Missing access configurations!");
    return _dom;
  }

  _dom.appendChild(_dom.createProcessingInstruction(
      "xml", "version=\"1.0\" encoding=\"ISO-8859-1\""));

  QDomElement _rootNode = _dom.createElement("orderUpdateRequest");
  _rootNode.setAttribute("version", "1.1");
  _dom.appendChild(_rootNode);

  QDomElement _actionNode = _dom.createElement("action");
  _actionNode.setAttribute("name", attr);
  _rootNode.appendChild(_actionNode);

  QDomElement _usernode = _dom.createElement("username");
  _usernode.appendChild(_dom.createTextNode(_user));
  _actionNode.appendChild(_usernode);

  QDomElement _keynode = _dom.createElement("password");
  _keynode.appendChild(_dom.createTextNode(_key));
  _actionNode.appendChild(_keynode);

  QDomElement _purchaseOrder = _dom.createElement("purchaseOrder");
  _purchaseOrder.setAttribute("id", order_id);
  _rootNode.appendChild(_purchaseOrder);

  return _dom;
}

void AbeBooksActionsDialog::previousPage() {
  int _index = (m_pages->currentIndex() - 1);
  if (_index >= 0)
    m_pages->setCurrentIndex(_index);
}

void AbeBooksActionsDialog::nextPage() {
  int _index = (m_pages->currentIndex() + 1);
  if (_index > 0 && _index < m_pages->count())
    m_pages->setCurrentIndex(_index);
}

void AbeBooksActionsDialog::queryFinished(QNetworkReply *reply) {
  if (reply->error() != QNetworkReply::NoError) {
    statusMessage(tr("Network request, response with error!"));
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << reply->readAll();
#endif
    return;
  }
  statusMessage(tr("Query successfully."));
}

void AbeBooksActionsDialog::prepareResponse(const QDomDocument &xml) {
#ifdef ANTIQUA_DEVELOPEMENT
  QDir dir = AntiquaCRM::ASettings::getUserTempDir();
  QFileInfo info(dir, order_id + ".xml");
  QFile fp(info.filePath());
  if (fp.open(QIODevice::WriteOnly)) {
    QTextStream data(&fp);
    data << xml.toString(1); // Indented: 1
    fp.close();
  }
#endif

  QDomElement _root = xml.documentElement();
  if (_root.tagName() == "orderUpdateResponse") {
    QDomElement _poNode = _root.firstChildElement("purchaseOrder");
    if (_poNode.isNull())
      return;

    QDomElement _buyerNode = _poNode.firstChildElement("buyer");
    if (_buyerNode.isNull())
      return;

    m_orderInfo->setContentData(_buyerNode);

    QDomElement _bpoNode = _poNode.lastChildElement("buyerPurchaseOrder");
    if (_bpoNode.isNull())
      return;

    m_orderInfo->setOrderPurchaseId(_bpoNode.attribute("id", "0"));
  }
#ifdef ANTIQUA_DEVELOPEMENT
  else {
    qDebug() << Q_FUNC_INFO << xml.toString(-1) << Qt::endl;
  }
#endif
}

void AbeBooksActionsDialog::prepareOperation() {
  QString _status = m_sellerStatus->getStatus();
  if (_status.isEmpty()) {
    statusMessage(tr("We need a Selection for this operations."));
    return;
  }

  QDomDocument _dom = orderUpdateRequest("update");
  QDomElement _pnode = _dom.documentElement().lastChildElement("purchaseOrder");
  if (_pnode.isNull())
    return;

  QDomElement _statusNode = _dom.createElement("status");
  _statusNode.appendChild(_dom.createTextNode(_status));
  _pnode.appendChild(_statusNode);

  qDebug() << "TODO" << _dom.toString(-1);
  // m_network->xmlPostRequest(apiQuery(), _dom);
}

int AbeBooksActionsDialog::exec(const QJsonObject &data) {
  if (!initConfiguration()) {
    qWarning("Plugin AbeBooks missing configuration!");
    return QDialog::Rejected;
  }
  // AbeBooks Seller customer id
  seller_id = p_config.value("seller_id", QString()).toString();

  QJsonObject _main = data.value("DATA").toObject();
  QJsonObject _order = _main.value("orderinfo").toObject();
  order_id = _order.value("o_provider_order_id").toString();
  if (order_id.isEmpty()) {
    qWarning("Missing OrderId - Operation rejected!");
    return QDialog::Rejected;
  }

  m_orderInfo->setOrderId(order_id);

  QUrl _servUrl;
  if (seller_id.length() > 2) {
    _servUrl.setScheme("https");
    _servUrl.setHost("www.abebooks.com");
    _servUrl.setPath("/servlet/OrderUpdate");
    QUrlQuery _query;
    _query.addQueryItem("abepoid", order_id);
    _query.addQueryItem("clientid", seller_id);
    _servUrl.setQuery(_query);
  }

  QString _sellerInfo;
  if (_servUrl.isValid()) {
    _sellerInfo = tr("Remote action for <a href='%1'>%2</a> with Order Id: %3")
                      .arg(_servUrl.toString(), // remote url
                           _main.value("buyer").toString(), order_id);
  } else {
    _sellerInfo = tr("Remote action for %1 with Order Id: %2")
                      .arg(_main.value("buyer").toString(), order_id);
  }
  m_sellerStatus->setBuyerInfo(_sellerInfo);

  m_network->xmlPostRequest(apiQuery(), orderUpdateRequest("getOrder"));

  return AntiquaCRM::ProviderActionDialog::exec();
}
