// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklookeractions.h"
#include "booklookerconfig.h"
#include "booklookeroperations.h"

#include <QDateTime>
#include <QFrame>
#include <QList>
#include <QRadioButton>

BookLookerActions::BookLookerActions(QWidget *parent)
    : AntiquaCRM::ProviderActionDialog{parent} {
  setObjectName("booklooker_actions_dialog");
  setWindowTitle(tr("Update BookLooker order.") + "[*]");

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

  m_selecter = new BookLookerOperations(this);
  layout->insertWidget(col++, m_selecter);
  layout->setStretch(col, 1);

  m_frame->setLayout(layout);
  m_scrollArea->setWidget(m_frame);

  m_network = new AntiquaCRM::ANetworker(
      AntiquaCRM::NetworkQueryType::JSON_QUERY, this);

  connect(m_network, SIGNAL(sendJsonResponse(const QJsonDocument &)),
          SLOT(prepareResponse(const QJsonDocument &)));

  connect(m_network, SIGNAL(finished(QNetworkReply *)),
          SLOT(queryFinished(QNetworkReply *)));

  connect(m_selecter, SIGNAL(sendSelectionModified(bool)),
          SLOT(isChildWindowModified(bool)));

  connect(this, SIGNAL(sendSubmitClicked()), SLOT(prepareOperation()));
}

bool BookLookerActions::initConfiguration() {
  p_config.clear();
  QJsonDocument _document;
  AntiquaCRM::ASqlCore dbsql(this);
  QString _sql("SELECT cfg_jsconfig FROM antiquacrm_configs ");
  _sql.append(" WHERE cfg_group='");
  _sql.append(BOOKLOOKER_CONFIG_POINTER);
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

const QUrl BookLookerActions::apiQuery(const QString &target) {
  QString _path = p_config.value("api_path", "/2.0/").toString();
  _path.append(target);

  QUrl _url;
  _url.setScheme("https");
  _url.setHost(p_config.value("api_host", "api.booklooker.de").toString(),
               QUrl::StrictMode);

  int _p = p_config.value("api_port", 443).toInt();
  if (_p != 443)
    _url.setPort(_p);

  _url.setPath(_path);
  return _url;
}

void BookLookerActions::setTokenCookie(const QString &token) {
  QDateTime _dt = QDateTime::currentDateTime();
  _dt.setTimeSpec(Qt::UTC);
  authenticCookie = QNetworkCookie("token", token.toLocal8Bit());
  authenticCookie.setDomain(p_config.value("api_host").toString());
  authenticCookie.setSecure(true);
  authenticCookie.setExpirationDate(_dt.addSecs((9 * 60)));
  if (!authenticCookie.value().isNull()) {
    qInfo("New Token add (%s)", qPrintable(authenticCookie.value()));
    if (cacheCookie.value().isNull())
      return;

    actionsCookie.setValue(QByteArray());
    QJsonDocument _doc = QJsonDocument::fromJson(cacheCookie.value());
    orderUpdateAction(_doc.object());
  }
}

bool BookLookerActions::isCookieExpired() {
  if (authenticCookie.value().isNull())
    return true;

  QDateTime dt = QDateTime::currentDateTime();
  dt.setTimeSpec(Qt::UTC);
  return (authenticCookie.expirationDate() <= dt);
}

void BookLookerActions::authenticate() {
  // https://api.booklooker.de/2.0/authenticate
  QUrl _url = apiQuery("authenticate");
  QString _key("apiKey=");
  _key.append(p_config.value("api_key").toString());
  actionsCookie = QNetworkCookie("action", "authenticate");
  actionsCookie.setDomain(_url.host());
  actionsCookie.setSecure(true);
  m_network->loginRequest(_url, _key.toLocal8Bit());
}

void BookLookerActions::prepareResponse(const QJsonDocument &document) {
  QJsonObject _obj = document.object();
  if (actionsCookie.value().contains("authenticate")) {
    if (_obj.value("status").toString().toLower() == "ok") {
      QString _token = _obj.value("returnValue").toString();
      setTokenCookie(_token);
    }
    return;
  }

  if (_obj.value("status").toString().toLower() != "ok") {
    QString info = _obj.value("returnValue").toString();
    statusMessage(info);
  }

  QJsonValue returnValue = _obj.value("returnValue");
  if (returnValue.type() == QJsonValue::String) {
    statusMessage(returnValue.toString());
  }

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << _obj;
#endif

  cacheCookie.setValue(QByteArray());
  actionsCookie.setValue(QByteArray());

  statusMessage(tr("Update Successfully."));
}

void BookLookerActions::orderUpdateAction(const QJsonObject &data) {
  if (order_id.isEmpty()) {
    statusMessage(tr("Missing OrderId."));
    return;
  }

  if (isCookieExpired()) {
    QJsonDocument _doc(data);
    cacheCookie = QNetworkCookie("queue", _doc.toJson(QJsonDocument::Compact));
    cacheCookie.setDomain(p_config.value("api_host").toString());
    cacheCookie.setSecure(true);
    authenticate();
    return;
  }

  QJsonObject _query = data.value("query").toObject();
  if (_query.isEmpty()) {
    return;
  }

  // It's no longer a queued request!
  cacheCookie.setValue(QByteArray());

  QUrlQuery q;
  q.addQueryItem("token", QString(authenticCookie.value()));
  q.addQueryItem("orderId", order_id);
  q.addQueryItem("status", _query.value("status").toString());

  QString action = data.value("action").toString();
  QUrl url = apiQuery(action);
  if (!q.isEmpty())
    url.setQuery(q);

  // qDebug() << Q_FUNC_INFO << "DISABLED" << url.toString();
  m_network->putRequest(url, QByteArray());
}

void BookLookerActions::prepareOperation() {
  QString action;
  QList<QRadioButton *> l = findChildren<QRadioButton *>(QString());
  for (int i = 0; i < l.count(); i++) {
    QRadioButton *rb = l.at(i);
    if (rb != nullptr && !rb->objectName().isEmpty()) {
      if (rb->isChecked()) {
        action = rb->objectName();
        break;
      }
    }
  }

  if (action.isEmpty()) {
    statusMessage(tr("We need a Selection for this operations."));
    return;
  }

  QJsonObject query;
  query.insert("orderId", QJsonValue(order_id));
  query.insert("status", QJsonValue(action));

  // Reset
  QJsonObject obj;
  obj.insert("action", QJsonValue("order_status"));
  obj.insert("query", QJsonValue(query));
  orderUpdateAction(obj);
}

void BookLookerActions::queryFinished(QNetworkReply *reply) {
  if (reply->error() != QNetworkReply::NoError) {
    statusMessage(tr("Network request, response with error!"));
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << reply->readAll();
#endif
  }
  statusMessage(tr("Query successfully."));
}

int BookLookerActions::exec(const QJsonObject &data) {
  if (!initConfiguration()) {
    qWarning("Plugin BookLooker missing configuration!");
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

  return AntiquaCRM::ProviderActionDialog::exec();
}
