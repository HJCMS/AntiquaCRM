// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklooker.h"
#include "booklookerconfig.h"
#include "booklookerifacewidget.h"
#include "booklookerrequester.h"

#include <QDebug>

BooklookerRequester *Booklooker::apiRequester() {
  BooklookerRequester *req = new BooklookerRequester(this);
  req->setObjectName(CONFIG_PROVIDER);
  connect(req, SIGNAL(response(const QJsonDocument &)), this,
          SLOT(prepareJsonListResponse(const QJsonDocument &)));
  connect(req, SIGNAL(errorMessage(Antiqua::ErrorStatus, const QString &)), this,
          SIGNAL(s_errorResponse(Antiqua::ErrorStatus, const QString &)));
  return req;
}

void Booklooker::prepareJsonListResponse(const QJsonDocument &doc) {
  if (QJsonValue(doc["status"]).toString() != "OK")
  {
#ifdef ANTIQUA_DEVELOPEMENT
    qDebug() << Q_FUNC_INFO << BooklookerRequester::getResponseErrors(doc);
#endif
    return;
  }

  if (!QJsonValue(doc["returnValue"]).isArray())
    return;

  QJsonArray senderArray;
  QJsonArray array = QJsonValue(doc["returnValue"]).toArray();
  for (int i = 0; i < array.size(); i++) {
    if (array[i].isObject()) {
      QJsonObject convert;
      QJsonObject obj = array[i].toObject();
      qint64 id = obj.value("orderId").toInt();
      convert.insert("id", QJsonValue(QString::number(id)));
      QDateTime d =
          QDateTime::fromString(obj["orderDate"].toString(), DATE_FORMAT);
      convert.insert("datum", QJsonValue(d.toString(ANTIQUA_DATETIME_FORMAT)));
      senderArray.append(convert);
    }
  }
  QJsonObject senderObject;
  senderObject.insert("provider", QJsonValue(CONFIG_PROVIDER));
  senderObject.insert("items", senderArray);
  QJsonDocument jsDoc(senderObject);
  emit s_listResponse(jsDoc);
}

bool Booklooker::createInterface(QObject *parent) {
  if (parent) {
    m_handle = new QObject(parent);
    m_handle->setObjectName("booklooker_plugin");
    return true;
  }
  return false;
}

Antiqua::InterfaceWidget *Booklooker::addWidget(const QString &orderId,
                                                QWidget *parent) {
  m_blIfaceWidget = new BooklookerIfaceWidget(orderId, parent);
  m_blIfaceWidget->setObjectName(orderId);
  m_blIfaceWidget->setProviderName(provider());
  m_blIfaceWidget->setOrderId(orderId);
  return m_blIfaceWidget;
}

const QString Booklooker::provider() const { return QString(CONFIG_PROVIDER); }

const QString Booklooker::configGroup() const { return QString(CONFIG_GROUP); }

void Booklooker::queryMenueEntries() {
  BooklookerRequester *req = apiRequester();
  if (req != nullptr)
    req->queryList();
}

void Booklooker::updateOrderDelivery(const QJsonObject &jso) {
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << "TODO" << jso;
#endif
}

void Booklooker::updateArticleCount(int articleId, int count) {
  // NOTE - Es ist nur ein Löschen möglich!
  if (articleId < 1 || count > 0)
    return;

#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << "TODO DELETE" << articleId << count;
#endif
  return;

  QString orderNo = QString::number(articleId);
  BooklookerRequester *req = apiRequester();
  if (req != nullptr)
    req->queryArticleReset(orderNo);
}

void Booklooker::uploadArticleImage(int articleId, const QString &base64) {
#ifdef ANTIQUA_DEVELOPEMENT
  qDebug() << Q_FUNC_INFO << "TODO" << articleId << base64.length();
#endif
}
