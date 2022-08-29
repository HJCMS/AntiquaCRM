// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "abebooksrequester.h"
#include "abebooksconfig.h"
#include "abebooksdocument.h"
#include "applsettings.h"

#include <QDebug>
#include <QFileInfo>
#include <QSslConfiguration>
#include <QTextCodec>
#include <QTextEncoder>

static const QByteArray applCaBundlePath() {
#ifdef Q_OS_WIN
  // Ist Normalerweise im Paket enthalten!
  QFileInfo info(QDir::current(), "curl-ca-bundle.crt");
  if (info.isReadable()) {
    return info.filePath().toLocal8Bit();
  }
#endif
  return QByteArray();
}

static const QByteArray userAgentString() {
  QLocale locale = QLocale::system();
  QString iso639_1 = locale.bcp47Name();
  QString iso3166 = locale.name();
  QString str("Mozilla/5.0 (compatible; ");
  str.append(ANTIQUACRM_DISPLAYNAME);
  str.append("/");
  str.append(ANTIQUACRM_VERSION);
  str.append(" " + iso639_1 + ", " + iso3166);
  str.append(") AppleWebKit (KHTML, like Gecko)");
  return str.toLocal8Bit();
}

const QByteArray languageRange() {
  // https://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html
  QLocale locale = QLocale::system();
  QString al = locale.bcp47Name();
  al.append(", ");
  al.append(locale.name());
  al.append("; q=0.8, en;q=0.7");
  return al.toLocal8Bit();
}

AbeBooksRequester::AbeBooksRequester(QObject *parent)
    : QNetworkAccessManager{parent} {
  config = new ApplSettings(this);
  m_reply = nullptr;
  connect(this, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(slotFinished(QNetworkReply *)));
}

const QNetworkRequest AbeBooksRequester::initRequest() {
  QUrl url;
  config->beginGroup(CONFIG_GROUP);
  url.setScheme(config->value("api_scheme", "https").toString());
  url.setHost(config->value("api_host", "orderupdate.abebooks.com").toString());
  url.setPort(config->value("api_port", 10003).toInt());
  config->endGroup();

  QNetworkRequest req(url);
  req.setPeerVerifyName(url.host());
  req.setRawHeader("User-Agent", userAgentString());
  req.setRawHeader("Accept-Language", languageRange());
  req.setRawHeader("Accept", "application/xml,text/xml,text/*;q=0.1");
  req.setRawHeader("Cache-Control", "no-cache, private");
  req.setRawHeader("Content-Type", "application/xml;text/xml");
  QSslConfiguration ssl;
  if (!ssl.addCaCertificates(applCaBundlePath(), QSsl::Pem)) {
    QFileInfo info(config->value("ssloptions/ssl_bundle").toString());
    if (info.isReadable()) {
      ssl.addCaCertificates(info.filePath().toLocal8Bit(), QSsl::Pem);
    }
  }
  req.setSslConfiguration(ssl);

  return req;
}

AbeBooksDocument AbeBooksRequester::createDocument() {
  AbeBooksAccess ac;
  config->beginGroup(CONFIG_GROUP);
  ac.user = toISO88591(config->value("api_user").toString());
  ac.key = toISO88591(config->value("api_key").toString());
  config->endGroup();
  if (ac.user.isEmpty() || ac.key.isEmpty())
    qWarning("Invalid Settings to Access 'ABEPO'!");

  return AbeBooksDocument(ac);
}

const QString AbeBooksRequester::toISO88591(const QString &str) {
  QTextCodec *codec = QTextCodec::codecForLocale();
  QTextEncoder encoder(codec);
  return QString(encoder.fromUnicode(str));
}

bool AbeBooksRequester::createRequest(const QDomDocument &document) {
  QByteArray query = document.toByteArray(-1);
  if (query.length() < 100) {
    qWarning("Invalid Post data ...");
    return false;
  }

  QNetworkRequest request = initRequest();
  QByteArray size = QString::number(query.size()).toLocal8Bit();
  request.setRawHeader(QByteArray("Content-Length"), size);

#ifdef ANTIQUA_DEVELOPEMENT
  saveSources(document, "createRequest");
#endif

  m_reply = post(request, query);
  connect(m_reply, SIGNAL(error(QNetworkReply::NetworkError)), this,
          SLOT(slotError(QNetworkReply::NetworkError)));

  connect(m_reply, SIGNAL(sslErrors(QList<QSslError>)), this,
          SLOT(slotSslErrors(QList<QSslError>)));

  connect(m_reply, SIGNAL(readyRead()), this, SLOT(replyReadyRead()));

  return true; // m_reply->isRunning();
}

void AbeBooksRequester::slotError(QNetworkReply::NetworkError error) {
  switch (error) {
  case QNetworkReply::ConnectionRefusedError:
    qWarning("Networker: Connection Refused Error");
    return;

  case QNetworkReply::TimeoutError:
    qWarning("Networker: Timeout Error");
    return;

  case QNetworkReply::HostNotFoundError:
    qWarning("Networker: Host NotFound Error");
    return;

  case QNetworkReply::RemoteHostClosedError:
    qWarning("Networker: RemoteHost Closed Error");
    return;

  case QNetworkReply::OperationCanceledError:
    qWarning("Networker: Operation Canceled Error");
    return;

  case QNetworkReply::InsecureRedirectError:
    qWarning("Networker: Insecure Redirect Error");
    return;

  default:
    qWarning("Networker: Unknown Error:%s", qPrintable(QString::number(error)));
    return;
  }
}

void AbeBooksRequester::slotFinished(QNetworkReply *reply) {
  bool success = true;
  if (reply->error() != QNetworkReply::NoError) {
    slotError(reply->error());
    success = false;
  }
  emit requestFinished(success);
}

void AbeBooksRequester::slotSslErrors(const QList<QSslError> &list) {
  for (int i = 0; i < list.count(); i++) {
    QSslError ssl_error = list.at(i);
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << ssl_error.errorString();
#endif
  }
}

void AbeBooksRequester::replyFinished(QNetworkReply *reply) {
  if (reply->error() == QNetworkReply::NoError) {
    m_reply->deleteLater();
  }
}

void AbeBooksRequester::replyReadyRead() {
  QVector<char> buf;
  QByteArray data;
  qint64 chunk;
  while (m_reply->bytesAvailable() > 0) {
    chunk = m_reply->bytesAvailable();
    if (chunk > 4096) {
      chunk = 4096;
    }
    buf.resize(chunk + 1);
    memset(&buf[0], 0, chunk + 1);
    if (chunk != m_reply->read(&buf[0], chunk)) {
      qWarning("AbeBooksRequester: buffer read error");
    }
    data += &buf[0];
  }
  buf.clear();

  QDomDocument doc("response");
  QString errorMsg = QString();
  int errorLine = 0;
  int errorColumn = 0;
  if (doc.setContent(data, false, &errorMsg, &errorLine, &errorColumn)) {
    emit response(doc);
  } else {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << errorMsg << errorLine << errorColumn;
#else
    qWarning("Returned XML is not well formatted!");
#endif
  }
  data.clear();
}

void AbeBooksRequester::queryList() {
  AbeBooksDocument doc = createDocument();
  doc.createAction("getAllNewOrders");
  if (!createRequest(doc)) {
    qWarning("AbeBooks: getAllNewOrders failed");
  }
}

void AbeBooksRequester::queryOrder(const QString &purchaseId) {
  if (purchaseId.isEmpty()) {
    qWarning("AbeBooks: Missing queryOrder(purchaseId)");
    return;
  }

  AbeBooksDocument doc = createDocument();
  doc.createAction("getOrder");
  QDomElement e = doc.createElement("purchaseOrder");
  QString id(purchaseId);
  e.setAttribute("id", id.trimmed());
  doc.documentElement().appendChild(e);
  if (!createRequest(doc)) {
    qWarning("AbeBooks purchaseOrder failed");
  }
}

void AbeBooksRequester::updateOrderStatus(const QString &purchaseId,
                                          const QString &action) {
  AbeBooksDocument doc = createDocument();
  doc.createAction("update");
  QDomElement pOrder = doc.createElement("purchaseOrder");
  pOrder.setAttribute("id", purchaseId.trimmed());
  doc.documentElement().appendChild(pOrder);
  QDomElement status = doc.createElement("status");
  status.appendChild(doc.createTextNode(action));
  pOrder.appendChild(status);

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << purchaseId << action << doc.toString();
  return;
#endif

  if (!createRequest(doc)) {
    qWarning("AbeBooks purchaseOrder failed");
  }
}
