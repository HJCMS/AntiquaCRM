// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "whsoft.h"
#include "whsoftconfig.h"
#include "whsoftjsonquery.h"
#include "whsoftwidget.h"

void WHSoft::prepareJsonListResponse(const QJsonDocument &doc) {
  bool errors = QJsonValue(doc["error"]).toBool();
  if (!errors) {
    QJsonArray senderArray;
    QJsonArray array = QJsonValue(doc["response"]).toArray();
    for (int i = 0; i < array.count(); i++) {
      QJsonObject convert;
      QJsonObject obj = array[i].toObject();
      convert.insert("id", obj["id"]);
      QDateTime d = QDateTime::fromString(obj["datum"].toString(), DATE_FORMAT);
      convert.insert("datum", QJsonValue(d.toString(ANTIQUA_DATETIME_FORMAT)));
      senderArray.append(convert);
    }
    QJsonObject senderObject;
    senderObject.insert("provider", QJsonValue(CONFIG_PROVIDER));
    senderObject.insert("items", senderArray);
    QJsonDocument jsDoc(senderObject);
    emit listResponse(jsDoc);
  }
}

void WHSoft::responseAnswerCheck(const QJsonDocument &doc) {
  // qDebug() << Q_FUNC_INFO << doc;
  bool errors = QJsonValue(doc["error"]).toBool();
  emit s_queryResponse(errors);
}

bool WHSoft::createInterface(QObject *parent) {
  if (parent) {
    m_whsoft = new QObject(parent);
    m_whsoft->setObjectName("whsoft_plugin");
    return true;
  }
  return false;
}

Antiqua::InterfaceWidget *WHSoft::addWidget(const QString &orderId,
                                            QWidget *parent) {
  m_whsoftWidget = new WHSoftWidget(orderId, parent);
  m_whsoftWidget->setObjectName(orderId);
  m_whsoftWidget->setProviderName(provider());
  m_whsoftWidget->setOrderId(orderId);
  return m_whsoftWidget;
}

const QString WHSoft::provider() const { return QString(CONFIG_PROVIDER); }

const QString WHSoft::configGroup() const { return QString(CONFIG_GROUP); }

void WHSoft::queryMenueEntries() {
  WHSoftJSonQuery *mjs = new WHSoftJSonQuery(this);
  mjs->setObjectName(CONFIG_PROVIDER);
  connect(mjs, SIGNAL(listResponsed(const QJsonDocument &)), this,
          SLOT(prepareJsonListResponse(const QJsonDocument &)));

  mjs->queryList();
}

void WHSoft::updateOrderDelivery(const QJsonObject &jso) {
/*
 * w+hbook Erwartet
  {
   "id":"BF-2013249",
   "versender" : "Hermes",
   "sendungsart" : "Paket",
   "sendungsnummer" : "32002348230409824",
   "einlieferungsdatum" : "2019-09-11 11:52:47"
  }
*/
  qDebug() << Q_FUNC_INFO << jso;
  return;

  QJsonDocument doc;
  if (doc.isEmpty()) {
    emit s_queryResponse(false);
    return;
  }

  WHSoftJSonQuery *jq = new WHSoftJSonQuery(this);
  jq->setObjectName("json_update_article_counts");
  connect(jq, SIGNAL(orderResponsed(const QJsonDocument &)), this,
          SLOT(responseAnswerCheck(const QJsonDocument &)));

  jq->customQuery("versandbestaetigung", doc);
}

void WHSoft::updateArticleCount(int articleId, int count) {
  QJsonDocument doc = createUpdateArtcileCount(articleId, count);
  if (doc.isEmpty()) {
    emit s_queryResponse(false);
    return;
  }
  WHSoftJSonQuery *jq = new WHSoftJSonQuery(this);
  jq->setObjectName("json_update_article_counts");
  connect(jq, SIGNAL(orderResponsed(const QJsonDocument &)), this,
          SLOT(responseAnswerCheck(const QJsonDocument &)));

  jq->customQuery("bestand", doc);
}

void WHSoft::uploadArticleImage(int articleId, const QString &base64) {
  QJsonObject jo;
  jo.insert("bestellnr", QJsonValue(QString::number(articleId)));
  jo.insert("bildnummer", QJsonValue(0));
  jo.insert("content", QJsonValue(base64));
  QJsonDocument jdoc(jo);
  if (jdoc.isNull()) {
    emit s_queryResponse(false);
    return;
  }

  WHSoftJSonQuery *jq = new WHSoftJSonQuery(this);
  jq->setObjectName("json_uload_image_data");
  connect(jq, SIGNAL(orderResponsed(const QJsonDocument &)), this,
          SLOT(responseAnswerCheck(const QJsonDocument &)));

  jq->customQuery("bild", jdoc);
}
