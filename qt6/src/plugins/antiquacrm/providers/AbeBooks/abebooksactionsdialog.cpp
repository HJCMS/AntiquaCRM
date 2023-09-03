// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooksactionsdialog.h"
#include "abebooksconfig.h"
#include "abebookssellerstatus.h"

#include <QFrame>
#include <QRadioButton>
#include <QVBoxLayout>

AbeBooksActionsDialog::AbeBooksActionsDialog(QWidget *parent)
    : AntiquaCRM::ProviderActionDialog{parent} {
  setObjectName("abebooks_actions_dialog");
  setWindowTitle(tr("Update AbeBooks order."));

  QFrame *m_frame = new QFrame(this);
  m_frame->setFrameStyle(QFrame::NoFrame);
  m_frame->setContentsMargins(5, 0, 5, 0);

  int col = 0;
  QVBoxLayout *layout = new QVBoxLayout(m_frame);
  layout->setContentsMargins(0, 0, 0, 0);

  QLabel *m_lb = new QLabel(this);
  m_lb->setWordWrap(false);
  m_lb->setText(tr("Changes the Provider status of the current order."));
  m_lb->setStyleSheet("QLabel {font-weight:bold;}");
  layout->insertWidget(col++, m_lb);

  QLabel *m_info = new QLabel(this);
  m_info->setWordWrap(true);
  QStringList notes;
  notes << tr("Not all service providers fully support these features.");
  notes << tr(
      "Please make sure to check it regularly on the service provider side.");
  m_info->setText(notes.join("<br>"));
  m_info->setStyleSheet("QLabel {font-style:italic; font-size:smaller;}");
  layout->insertWidget(col++, m_info);

  m_buyerInfo = new QLabel(this);
  layout->insertWidget(col++, m_buyerInfo);

  m_status = new AbeBooksSellerStatus(this);
  layout->insertWidget(col++, m_status);
  layout->setStretch(col, 1);

  m_frame->setLayout(layout);
  m_scrollArea->setWidget(m_frame);

  m_network =
      new AntiquaCRM::ANetworker(AntiquaCRM::NetworkQueryType::XML_QUERY, this);

  connect(m_network, SIGNAL(sendXmlResponse(const QDomDocument &)),
          SLOT(prepareResponse(const QDomDocument &)));

  connect(m_network, SIGNAL(finished(QNetworkReply *)),
          SLOT(queryFinished(QNetworkReply *)));

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

void AbeBooksActionsDialog::queryFinished(QNetworkReply *reply) {
  if (reply->error() != QNetworkReply::NoError) {
    statusMessage(tr("AbeBooks response with errors!"));
    return;
  }
  statusMessage(tr("Update successfully."));
  setWindowModified(false);
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
  qDebug() << Q_FUNC_INFO << xml.toString(-1);
}

void AbeBooksActionsDialog::prepareOperation() {
  QString _status;
  QList<QRadioButton *> l = findChildren<QRadioButton *>(QString());
  for (int i = 0; i < l.count(); i++) {
    QRadioButton *rb = l.at(i);
    if (rb != nullptr && !rb->objectName().isEmpty()) {
      if (rb->isChecked()) {
        _status = rb->objectName();
        break;
      }
    }
  }

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

  qDebug() << Q_FUNC_INFO << _dom.toString(-1);
  // m_network->xmlPostRequest(apiQuery(), _dom);
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

  QString _info = tr("Create remote action for %1 with Order Id: %2")
                      .arg(_main.value("buyer").toString(), order_id);
  m_buyerInfo->setText(_info);

#ifdef ANTIQUA_DEVELOPEMENT
  AntiquaCRM::ASharedCacheFiles _tmpf(AntiquaCRM::ASettings::getUserTempDir());
  QString _data = _tmpf.getTempFile(order_id + ".xml");
  if (_data.length() > 10) {
    QDomDocument _doc;
    QString _errno;
    if (!_doc.setContent(_data, false, &_errno)) {
      qWarning("XML Errors: '%s'.", qPrintable(_errno));
      return QDialog::Rejected;
    }
    prepareResponse(_doc);
    return AntiquaCRM::ProviderActionDialog::exec();
  }
#endif

  m_network->xmlPostRequest(apiQuery(), orderUpdateRequest("getOrder"));

  return AntiquaCRM::ProviderActionDialog::exec();
}
