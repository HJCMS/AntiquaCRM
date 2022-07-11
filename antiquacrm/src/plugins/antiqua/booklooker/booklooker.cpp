// -*- coding: utf-8 -*-
// vim: set fileencoding=utf-8

#include "booklooker.h"
#include "booklookerconfig.h"
#include "booklookerifacewidget.h"
#include "booklookerproviderwidget.h"
#include "booklookerrequester.h"

#include <QDebug>

void Booklooker::prepareJsonListResponse(const QJsonDocument &doc) {
  if (QJsonValue(doc["status"]).toString() != "OK")
    return;

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
  emit listResponse(jsDoc);
}

bool Booklooker::createInterface(QObject *parent) {
  if (parent) {
    m_handle = new QObject(parent);
    m_handle->setObjectName("booklooker_plugin");
    return true;
  }
  return false;
}

Antiqua::ProviderWidget *Booklooker::providerWidget(const QString &widgetId,
                                                    QWidget *parent) {
  m_blProviderWidget = new BooklookerProviderWidget(widgetId, parent);
  m_blProviderWidget->setObjectName(widgetId);
  return m_blProviderWidget;
}

Antiqua::InterfaceWidget *Booklooker::addWidget(const QString &widgetId,
                                                QWidget *parent) {
  m_blIfaceWidget = new BooklookerIfaceWidget(widgetId, parent);
  m_blIfaceWidget->setObjectName(widgetId);
  m_blIfaceWidget->setProviderName(provider());
  m_blIfaceWidget->setOrderId(widgetId);
  return m_blIfaceWidget;
}

const QString Booklooker::provider() const { return QString(CONFIG_PROVIDER); }

const QString Booklooker::configGroup() const { return QString(CONFIG_GROUP); }

void Booklooker::queryMenueEntries() {
  BooklookerRequester *req = new BooklookerRequester(this);
  req->setObjectName(CONFIG_PROVIDER);
  connect(req, SIGNAL(response(const QJsonDocument &)), this,
          SLOT(prepareJsonListResponse(const QJsonDocument &)));

  req->queryList();
}

void Booklooker::updateArticleCount(int articleId, int count) {
#ifdef ANTIQUA_DEVELOPEMENT
  // https://api.booklooker.de/2.0/article_status?token=REST_API_TOKEN
  qDebug() << Q_FUNC_INFO << "DISABLED" << articleId << count;
  return;
#endif
}
