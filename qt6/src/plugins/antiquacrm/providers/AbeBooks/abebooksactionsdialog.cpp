// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooksactionsdialog.h"
#include "abebooksconfig.h"
#include "abebooksordermaininfo.h"

#include <QFrame>
#include <QRadioButton>
#include <QUrl>
#include <QUrlQuery>
#include <QVBoxLayout>

AbeBooksActionsDialog::AbeBooksActionsDialog(QWidget *parent)
    : AntiquaCRM::ProviderActionDialog{parent, false} {
  setObjectName("abebooks_actions_dialog");
  setWindowTitle(tr("Update AbeBooks order.") + "[*]");
  setMinimumWidth(550);

  m_orderInfo = new AbeBooksOrderMainInfo(stackedWidget);
  stackedWidget->addWidget(m_orderInfo);

  m_network =
      new AntiquaCRM::ANetworker(AntiquaCRM::NetworkQueryType::XML_QUERY, this);

  connect(m_network, SIGNAL(sendXmlResponse(const QDomDocument &)),
          SLOT(prepareResponse(const QDomDocument &)));

  connect(m_network, SIGNAL(finished(QNetworkReply *)),
          SLOT(queryFinished(QNetworkReply *)));
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

  if (!p_config.contains("api_user") || !p_config.contains("api_key"))
    statusMessage(tr("Missing access configurations!"));

  return (p_config.size() > 2);
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
  const QString _encoding("version=\"1.0\" encoding=\"ISO-8859-1\"");
  const QString _user = p_config.value("api_user").toString();
  const QString _key = p_config.value("api_key").toString();
  QDomDocument _dom;
  if (_user.isEmpty() || _key.isEmpty()) {
    statusMessage(tr("Access configuration is incomplete!"));
    return _dom;
  }
  _dom.appendChild(_dom.createProcessingInstruction("xml", _encoding));

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

void AbeBooksActionsDialog::queryFinished(QNetworkReply *reply) {
  if (reply->error() != QNetworkReply::NoError) {
    statusMessage(tr("Network request, response with error!"));
#ifdef ANTIQUA_DEVELOPMENT
    qDebug() << Q_FUNC_INFO << Qt::endl << reply->readAll() << Qt::endl;
#endif
  }
}

void AbeBooksActionsDialog::prepareResponse(const QDomDocument &xml) {
#ifdef ANTIQUA_DEVELOPMENT
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
#ifdef ANTIQUA_DEVELOPMENT
  else {
    qDebug() << Q_FUNC_INFO << Qt::endl << xml.toString(-1) << Qt::endl;
  }
#endif
}

int AbeBooksActionsDialog::exec(const QJsonObject &data) {
  if (!initConfiguration()) {
    qWarning("Plugin AbeBooks missing configuration!");
    return QDialog::Rejected;
  }
  QJsonObject _main = data.value("DATA").toObject();
  QJsonObject _order = _main.value("orderinfo").toObject();
  order_id = _order.value("o_provider_order_id").toString();
  if (order_id.isEmpty()) {
    qWarning("Missing OrderId - Operation rejected!");
    return QDialog::Rejected;
  }
  m_orderInfo->setOrderId(order_id);
  // AbeBooks Seller-/Customer id
  seller_id = p_config.value("seller_id", QString()).toString();

  QUrl _url;
  if (seller_id.length() > 2) {
    _url.setScheme("https");
    _url.setHost("www.abebooks.com");
    _url.setPath("/servlet/OrderUpdate");
    QUrlQuery _query;
    _query.addQueryItem("abepoid", order_id);
    _query.addQueryItem("clientid", seller_id);
    _url.setQuery(_query);
    if (_url.isValid())
      m_orderInfo->setOrderLink(_url);
  }

  m_network->xmlPostRequest(apiQuery(), orderUpdateRequest("getOrder"));

  return AntiquaCRM::ProviderActionDialog::exec();
}
